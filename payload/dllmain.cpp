// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

#pragma warning(disable:4996) // freeopen

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


/*
00B74D4B | 50                       | push eax                                                       |
00B74D4C | FF 75 08                 | push dword ptr ss:[ebp+8]                                      | [ebp+8]:"hei"
00B74D4F | 57                       | push edi                                                       |
*/
// search pattern, entire block: 50 FF 75 08 57


/*
01584D6A | 5F                       | pop edi                                                        | starplayer.cpp:1829
01584D6B | 5E                       | pop esi                                                        |
01584D6C | 8B E5                    | mov esp,ebp                                                    |
01584D6E | 5D                       | pop ebp                                                        |
*/
DWORD jumpBackAddress;
DWORD SayChatAction = 0x13e08a0;


std::string const message;
DWORD oldEax;
void __declspec(naked) ourFun() {
	__asm {
		//push eax // printf call modifies this, so we must save it and restore before call to SayChat
		//mov oldEax, eax

		//pushfd
		//pushad

		push edx
		mov edx, dword ptr ss : [ebp + 8]
		mov message, edx
		pop edx
	}

	//std::cout << "Message: " << message << std::endl;
	printf("Message: %s\n", message);

	//MessageBoxA(0, message, "chat", 0);

	// Original opcodes
	__asm {
		//popfd
		//popad

		pop edi
		pop esi
		mov esp, ebp
		pop ebp
		jmp[jumpBackAddress]

	}
}


DWORD WINAPI MainThread(LPVOID param) {

	int hookLength = 5;
	// base: 12b0000
	// = 0x01584D6A - 0x12b0000 = 0x2D4D6A
	DWORD hookAddress = 0x01584D6A; //0x00B74D4B; -- Base + 0x4D4B ?
									// to find relative address: address - baseaddress

	jumpBackAddress = hookAddress + hookLength;
	MakeJMP((BYTE *)hookAddress, (DWORD)ourFun, hookLength);
	AllocConsole();

	freopen("CONOUT$", "w", stdout);

	//SetStdOutToNewConsole();
	DWORD baseAddress = (DWORD)GetModuleHandle(NULL);
	printf("Injected!\nBaseAddress: %x", baseAddress);


	//Hook((void*)hookAddress, ourFun, hookLength);

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
