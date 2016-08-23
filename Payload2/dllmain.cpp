// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <easyhook.h>
#include <string>
#include <iostream>
#include <Windows.h>
#include "Hooks.h"
#include "Starbound.h"

DWORD gFreqOffset = 0;

//BOOL WINAPI myBeepHook(DWORD dwFreq, DWORD dwDuration)
//{
//	std::cout << "\n    BeepHook: ****All your beeps belong to us!\n\n";
//	return Beep(dwFreq + gFreqOffset, dwDuration);
//}

void myHook(int num) {
	std::cout << "Hooked!!!\n";
}

// EasyHook will be looking for this export to support DLL injection. If not found then 
// DLL injection will fail.
extern "C" void __declspec(dllexport) __stdcall NativeInjectionEntryPoint(REMOTE_ENTRY_INFO* inRemoteInfo);

void __stdcall NativeInjectionEntryPoint(REMOTE_ENTRY_INFO* inRemoteInfo)
{
	if (inRemoteInfo->UserDataSize == sizeof(DWORD))
	{
		gFreqOffset = *reinterpret_cast<DWORD *>(inRemoteInfo->UserData);
		std::cout << "Adjusting Beep frequency by: " << gFreqOffset << "\n";
	}

	Hooks::InstallHook((void*)0x00CAA3C0, Starbound::SayChatActionHook);

	return;
}