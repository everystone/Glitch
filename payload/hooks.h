#pragma once
#include <Windows.h>
#include <iostream>



/*
01584D6A | 5F                       | pop edi                                                        | starplayer.cpp:1829
01584D6B | 5E                       | pop esi                                                        |
01584D6C | 8B E5                    | mov esp,ebp                                                    |
01584D6E | 5D                       | pop ebp                                                        |
*/

DWORD SayChatActionReturnAddress;
std::string message; //const works
void __declspec(naked) SayChatActionHook() {
	__asm {
		push edx
		mov edx, dword ptr ss : [ebp + 8]
		mov message, edx
		pop edx
	}

	printf("Message: %s\n", message);
	//sayChatAction("Hacked");

	// Original opcodes
	__asm {
		pop edi
		pop esi
		mov esp, ebp
		pop ebp
		jmp[SayChatActionReturnAddress]
	}
}


// Calling functions in asm:
/*
push eax
push dword ptr ss:[ebp+8]
push edi
call dword ptr [SayChatAction]
*/
DWORD SayChatAction = 0x13e08a0; // offset: 1308A0
// void(*sayChatAction)(const std::string) = (void(*)(const std::string))0x13e08a0;
void __declspec(naked) sayChat() {
	__asm {
		//push eax
		push dword ptr ss : [message]
		//push edi
		call dword ptr[SayChatAction]
	}
}
