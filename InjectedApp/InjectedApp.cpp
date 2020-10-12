
#include <iostream>
#include <Windows.h>

typedef void __stdcall PrntStr(const char* str);

int main()
{
    std::cout << "Hello World!\n";
    HMODULE module = 0;
    while (module == 0)
    {
        module = GetModuleHandle(L"MyLibrary.dll");
        Sleep(30);
    }

    PrntStr* proc = (PrntStr*)GetProcAddress(module, "PrintString");
    proc("Injected ;)");

    return 0;
}