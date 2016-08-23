#pragma once
#include <Windows.h>
#include <iostream>


/*
01584D6A | 5F                       | pop edi                                                        | starplayer.cpp:1829
01584D6B | 5E                       | pop esi                                                        |
01584D6C | 8B E5                    | mov esp,ebp                                                    |
01584D6E | 5D                       | pop ebp                                                        |
*/

DWORD SayChatAction = 0x13e08a0; // offset: 1308A0

//void(*sayChatAction)(const std::string) = (void(*)(const std::string))0x13e08a0;
DWORD SayChatActionReturnJump;
std::string message; //const works


// Calling functions in asm:
/*
push eax
push dword ptr ss:[ebp+8]
push edi
call dword ptr [SayChatAction]
*/

void __declspec(naked) sayChat() {
	__asm {
		//push eax
		push dword ptr ss : [message]
		//push edi
		call dword ptr[SayChatAction]
	}
}

void __declspec(naked) sayChatActionHook() {
	__asm {
		push edx
		mov edx, dword ptr ss : [ebp + 8]
		mov message, edx
		pop edx
	}

	//std::cout << "Message: " << message << std::endl;
	printf("Message: %s\n", message);
	//sayChatAction("Hacked");

	// Original opcodes
	__asm {
		pop edi
		pop esi
		mov esp, ebp
		pop ebp
		jmp[SayChatActionReturnJump]
	}
}