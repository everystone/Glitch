// Injector.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
int main()
{
	DWORD processId;
	//std::wcout << "Enter the target process Id: ";
	//std::cin >> processId;

	//std::wstring windowName;
	//std::getline(std::wcin, windowName);

	std::wstring windowName = L"Starbound";
	HWND windowHandle = FindWindowW(NULL, windowName.c_str());

	GetWindowThreadProcessId(windowHandle, &processId);

	DWORD freqOffset = 0;
	//std::cout << "Enter a frequency offset in hertz (e.g. 800): ";
	//std::cin >> freqOffset;

	WCHAR* dllToInject = L"..\\Debug\\Payload2.dll";
	wprintf(L"Attempting to inject: %s\n\n", dllToInject);

	// Inject dllToInject into the target process Id, passing 
	// freqOffset as the pass through data.
	NTSTATUS nt = RhInjectLibrary(
		processId,   // The process to inject into
		0,           // ThreadId to wake up upon injection
		EASYHOOK_INJECT_DEFAULT,
		dllToInject, // 32-bit
		NULL,		 // 64-bit not provided
		&freqOffset, // data to send to injected DLL entry point
		sizeof(DWORD)// size of data to send
	);

	if (nt != 0)
	{
		printf("RhInjectLibrary failed with error code = %d\n", nt);
		PWCHAR err = RtlGetLastErrorString();
		std::wcout << err << "\n";
	}
	else
	{
		std::cout << "Library injected successfully.\n";
	}

	std::wcout << "Press Enter to exit";
	std::wstring input;
	std::getline(std::wcin, input);
	return 0;
}

