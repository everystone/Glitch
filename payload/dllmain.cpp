// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"


bool Hook(void *toHook, void *ourFun, int len) {
	if (len < 5) {
		return false;
	}

	DWORD curProtection;
	VirtualProtect(toHook, len, PAGE_EXECUTE_READWRITE, &curProtection);

	memset(toHook, 0x90, len);
	DWORD relativeAdress = ((DWORD)ourFun - (DWORD)toHook) - 5;
	
	*(BYTE*)toHook - 0xE9;
	*(DWORD*)((DWORD)toHook + 1) = relativeAdress;
	
	DWORD temp;
	VirtualProtect(toHook, len, curProtection, &temp);
	return true;
}

DWORD jumpBackAddress;
void __declspec(naked) ourFun() {
	__asm {
		push edx

		pop edx

		push ebp
		mov ebp, esp
		push -1
		jmp [jumpBackAddress]

	}
}


DWORD WINAPI MainThread(LPVOID param) {
	
	int hookLength = 5;
	DWORD hookAddress = 0x00CAA3C0;
	jumpBackAddress = hookAddress + hookLength;

	Hook((void*)hookAddress, ourFun, hookLength);

	while (true) {
		Sleep(200);
	}
	FreeLibraryAndExitThread((HMODULE)param, 0);
	return 0;
}


BOOL APIENTRY DllMain( HMODULE hModule,
					   DWORD  ul_reason_for_call,
					   LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(0, 0, MainThread, hModule, 0, 0);
		//MessageBoxA(0, "Injected", "asd", 0);
	case DLL_THREAD_ATTACH:

	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
