
#include <iostream>
#include <Windows.h>

typedef __declspec(dllimport) char* __stdcall GetAddressOfData(DWORD pid, const char* data, size_t len);

int main()
{
	HMODULE mylib = LoadLibrary(L"C:\\Users\\Kamar\\Desktop\\WinApiLabs\\Debug\\MyLibrary.dll");
	
	GetAddressOfData* func = (GetAddressOfData*)GetProcAddress(mylib, "GetAddressOfData");

	char* mystring = (char*)malloc(sizeof(char) * 255);
	mystring[0] = 'H';
	mystring[1] = 'i';
	mystring[2] = ' ';
	mystring[3] = 't';
	mystring[4] = 'h';
	mystring[5] = 'e';
	mystring[6] = 'r';
	mystring[7] = 'e';
	mystring[8] = '\0';

	char* strAddress = func(GetCurrentProcessId(), mystring, sizeof(mystring));
	strcpy(strAddress, "Changed :)");
	std::cout << mystring;
	free(mystring);
	
	FreeLibrary(mylib);
}
