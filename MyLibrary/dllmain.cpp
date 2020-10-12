// dllmain.cpp : Определяет точку входа для приложения DLL.

#include "pch.h"
#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}





__declspec(dllexport) char* __stdcall GetAddressOfData(DWORD pid, const char* data, size_t len)
{
    HANDLE process = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, pid);
    

    if (process)
    {
        SYSTEM_INFO si;
        GetSystemInfo(&si);
        
        MEMORY_BASIC_INFORMATION info;
        std::vector<char> chunk;
        char* p = 0;
        while (p < si.lpMaximumApplicationAddress)
        {
            if (VirtualQueryEx(process, p, &info, sizeof(info)) == sizeof(info))
            {
                p = (char*)info.BaseAddress;
                chunk.resize(info.RegionSize);
                SIZE_T bytesRead;
                if (ReadProcessMemory(process, p, &chunk[0], info.RegionSize, &bytesRead))
                {
                    for (size_t i = 0; i < (bytesRead - len); ++i)
                    {
                        if (memcmp(data, &chunk[i], len) == 0)
                        {
                            char* result = (char*)p + i;
                            return result;
                        }
                    }
                }
                p += info.RegionSize;
            }
        }
    }
    return 0;
}


__declspec(dllexport) void __stdcall PrintString(char* str)
{
    std::cout << str;
}
