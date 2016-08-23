#include "stdafx.h"
#include "Hack.h"


DWORD Hack::baseAddress = 0x00;

void Hack::Init(DWORD base)
{
	baseAddress = base;
}

void Hack::MakeJMP(BYTE *pAddress, DWORD dwJumpTo, DWORD dwLen)
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

void Hack::InstallHook(DWORD offset, int hookLength, DWORD hookFunction, DWORD &jumpBackAddress)
{
	if (baseAddress == 0x00) {
		printf("Base Address is zero, call Hack::Init() before trying to install hooks.");
		return;
	}

	// base: 12b0000
	// = 0x01584D6A - 0x12b0000 = 0x2D4D6A
	//DWORD hookAddress = 0x01584D6A; //0x00B74D4B; -- Base + 0x4D4B ?
	// to find relative address: address - baseaddress
	DWORD hookAddress = Hack::baseAddress + offset;

	jumpBackAddress = hookAddress + hookLength;
	MakeJMP((BYTE *)hookAddress, (DWORD)hookFunction, hookLength);
}