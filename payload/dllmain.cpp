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
	
	*(BYTE*)toHook - 0xE9; // 0xE8 = call hook? 0xE9 = jmp hook?
	*(DWORD*)((DWORD)toHook + 1) = relativeAdress;
	
	DWORD temp;
	VirtualProtect(toHook, len, curProtection, &temp);
	return true;
}

void MakeJMP(BYTE *pAddress, DWORD dwJumpTo, DWORD dwLen)
{
	DWORD dwOldProtect, dwBkup, dwRelAddr;

	// give the paged memory read/write permissions

	VirtualProtect(pAddress, dwLen, PAGE_EXECUTE_READWRITE, &dwOldProtect);

	// calculate the distance between our address and our target location
	// and subtract the 5bytes, which is the size of the jmp
	// (0xE9 0xAA 0xBB 0xCC 0xDD) = 5 bytes

	dwRelAddr = (DWORD)(dwJumpTo - (DWORD)pAddress) - 5;

	// overwrite the byte at pAddress with the jmp opcode (0xE9)

	*pAddress = 0xE9;

	// overwrite the next 4 bytes (which is the size of a DWORD)
	// with the dwRelAddr

	*((DWORD *)(pAddress + 0x1)) = dwRelAddr;

	// overwrite the remaining bytes with the NOP opcode (0x90)
	// NOP opcode = No OPeration

	for (DWORD x = 0x5; x < dwLen; x++) *(pAddress + x) = 0x90;

	// restore the paged memory permissions saved in dwOldProtect

	VirtualProtect(pAddress, dwLen, dwOldProtect, &dwBkup);

	return;

}

DWORD jumpBackAddress;
DWORD Number;
void __declspec(naked) ourFun() {
	__asm {
		push edx

		mov edx, dword ptr[ebp - 8]
		mov Number, edx
		pop edx
	}
	std::cout << "read number: " << Number << std::endl;
	__asm {
		mov eax, dword ptr[ebp - 8]
		add eax, 1
		jmp [jumpBackAddress]

	}
}


DWORD WINAPI MainThread(LPVOID param) {
	
	int hookLength = 5;
	DWORD hookAddress = 0x00CAA3C0;

	jumpBackAddress = hookAddress + hookLength;

	MakeJMP((BYTE *)hookAddress, (DWORD)ourFun, hookLength);


	//Hook((void*)hookAddress, ourFun, hookLength);

	//FreeLibraryAndExitThread((HMODULE)param, 0);
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
