#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <tchar.h>
#include <stdio.h>
#include <psapi.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>

using namespace std;

int main() {
    // The DLL path we want to inject and the target process id.
    const char* dllpath = "C:\\Users\\Kamar\\Desktop\\WinApiLabs\\Debug\\MyLibrary.dll";
    int processID = 13540;

    printf("#### Starting ####\n");

    // Open target process handle    
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
    if (hProcess == NULL) {
        printf("[!] Unable to find the target process id: %d\n", processID);
        return 1;
    }
    printf("[+] Open target process handle\n");

    // Getting targt memory address for the dll path
    LPVOID dllpathMemoryAddr = VirtualAllocEx(hProcess, NULL, strlen(dllpath), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (dllpathMemoryAddr == NULL) {
        printf("[!] Unable to get memory address of target process for the dllpath");
        return 1;
    }
    printf("[+] Allocate the memory address to store the dllpath\n");

    // Writing the dll path to the target memory address
    BOOL succeedWrite = WriteProcessMemory(hProcess, dllpathMemoryAddr, dllpath, strlen(dllpath), NULL);
    if (!succeedWrite) {
        printf("[!] Unable to write to the memory address of target process the dllpath\n");
        return 1;
    }
    printf("[+] Writed the dllpath to memory\n");

    // Getting LoadLibreryA address
    FARPROC loadLibAddr = GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "LoadLibraryA");
    if (loadLibAddr == NULL) {
        printf("[!] Unable to get the memory address of LoadLibraryA function\n");
        return 1;
    }
    printf("[+] Allocate the memory address to LoadLibraryA function\n");

    // Create remote thread on the remote process to load the dll
    HANDLE rThread = CreateRemoteThread(hProcess, NULL, strlen(dllpath), (LPTHREAD_START_ROUTINE)loadLibAddr, dllpathMemoryAddr, NULL, NULL);
    if (rThread == NULL) {
        printf("[!] Unable to create thread to execute the LoadLibraryA function\n the error: %u\n", GetLastError());
        return 1;
    }
    printf("#### DLL INJECTED ####\n");

    
    Sleep(1000);
    
    
    system("pause");

    return 0;
}






