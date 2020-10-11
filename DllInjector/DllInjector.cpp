
#include <iostream>
#include <Windows.h>



BOOL WINAPI Inject(DWORD processID, PCWSTR sourceDLL)
{
    BOOL success = false;
    HANDLE targetProcess = NULL, createdThread = NULL;
    PWSTR pszLibFileRemote = NULL;
    __try
    {
        std::cout << "Process ID: " << processID << std::endl;
        targetProcess = OpenProcess(
            PROCESS_QUERY_INFORMATION | PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION | PROCESS_VM_WRITE,
            FALSE, processID);
        if (targetProcess == NULL)
        {
            std::cout << "ERROR: " << GetLastError();
            MessageBox(NULL, L"Unable to open process.", L"Error", MB_OK);
            __leave;
        }

        int cch = 1 + lstrlenW(sourceDLL);
        int cb = cch * sizeof(wchar_t);

        pszLibFileRemote = (PWSTR)VirtualAllocEx(targetProcess, NULL, cb, MEM_COMMIT, PAGE_READWRITE);
        if (pszLibFileRemote == NULL)
        {
            MessageBox(NULL, L"Could not allocate dll pathname in target process.", L"Error", MB_OK);
            __leave;
        }

        if (!WriteProcessMemory(targetProcess, pszLibFileRemote, (PVOID)sourceDLL, cb, NULL))
        {
            MessageBox(NULL, L"Could not write dll pathname in target process.", L"Error", MB_OK);
            __leave;
        }

        PTHREAD_START_ROUTINE pfnThreadRtn = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(L"Kernel32"), "LoadLibraryW");

        if (pfnThreadRtn == NULL)
        {
            MessageBox(NULL, L"Error finding LoadLibraryW address.", L"Error", MB_OK);
            __leave;
        }

        createdThread = CreateRemoteThread(targetProcess, NULL, 0, pfnThreadRtn, pszLibFileRemote, 0, NULL);
        if (createdThread == NULL)
        {
            __leave;
        }

        WaitForSingleObject(createdThread, INFINITE);
        success = true;

    }
    __finally { 
        if (pszLibFileRemote != NULL)
            VirtualFreeEx(targetProcess, pszLibFileRemote, 0, MEM_RELEASE);
        if (createdThread != NULL)
            CloseHandle(createdThread);
        if (targetProcess != NULL)
            CloseHandle(targetProcess);
    }

    return success;
}


int main(int argc, char* argv[])
{
    PCWSTR srcDll = L"C:\\Users\\Kamar\\Desktop\\WinApiLabs\\Debug\\MyLibrary.dll";
    DWORD processID = 5900;
    bool injected = Inject(processID, srcDll);
    if (injected)
    {
        std::cout << "Injection successful" << std::endl;
        
    }
    system("PAUSE");
    return 0;
}