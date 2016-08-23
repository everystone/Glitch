#pragma once
#include <Windows.h>
#include <iostream>

DWORD SayChatAction = 0x13e08a0;
DWORD SayChatActionReturnJump;
std::string const message;

void __declspec(naked) sayChatActionHook() {
	__asm {
		//pushfd
		//pushad

		push edx
		mov edx, dword ptr ss : [ebp + 8]
		mov message, edx
		pop edx
	}

	//std::cout << "Message: " << message << std::endl;
	printf("Message: %s\n", message);

	// Original opcodes
	__asm {
		//popfd
		//popad

		pop edi
		pop esi
		mov esp, ebp
		pop ebp
		jmp[SayChatActionReturnJump]
	}
}