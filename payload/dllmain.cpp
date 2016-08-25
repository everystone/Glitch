// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "hooks.h"
#include "Hack.h"
#pragma warning(disable:4996) // freeopen

DWORD WINAPI MainThread(LPVOID param) {

	AllocConsole();
	freopen("CONOUT$", "w", stdout);

	DWORD baseAddress = (DWORD)GetModuleHandle(NULL);
	printf("Injected!\nBaseAddress: %x\n\n", baseAddress);
	Hack::Init(baseAddress);

	 //Lua.execute("C:\\temp\\hello.lua");
	//message = "Injected";
	//sayChat();
	// Instlal Starbound hooks

	 //Hack::InstallHook(0x2D4D6A, 5, (DWORD)SayChatActionHook, SayChatActionReturnAddress);
	Hack::InstallHook(0x1250E, 6, (DWORD)targetPrintNumberHook, target_print_jumpback); // base: 40000

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
		MessageBoxA(0, "Injected", "asaaa", 0);
		CreateThread(0, 0, MainThread, hModule, 0, 0);
	case DLL_THREAD_ATTACH:

	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
