// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "hooks.h"
#include "functions.h"
#pragma warning(disable:4996) // freeopen



/*
01584D6A | 5F                       | pop edi                                                        | starplayer.cpp:1829
01584D6B | 5E                       | pop esi                                                        |
01584D6C | 8B E5                    | mov esp,ebp                                                    |
01584D6E | 5D                       | pop ebp                                                        |
*/



DWORD WINAPI MainThread(LPVOID param) {

	AllocConsole();
	freopen("CONOUT$", "w", stdout);

	
	DWORD baseAddress = (DWORD)GetModuleHandle(NULL);
	printf("Injected!\nBaseAddress: %x", baseAddress);
	Hooks *hooks = new Hooks(baseAddress);


	hooks->InstallHook(0x2D4D6A, 5, (DWORD)sayChatActionHook, SayChatActionReturnJump);

	//FreeLibraryAndExitThread((HMODULE)param, 0);
	return 0;
}


BOOL APIENTRY DllMain(HMODULE hModule,
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
