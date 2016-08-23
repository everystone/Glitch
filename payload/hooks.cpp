#pragma once
#include "stdafx.h"
#include "hooks.h"
#include "util.h"


Hooks::Hooks(DWORD baseAdd)
{
	this->baseAdd = baseAdd;
}

void Hooks::InstallHook(DWORD offset, int hookLength, DWORD funct, DWORD &jumpBackAddress)
{

	// base: 12b0000
	// = 0x01584D6A - 0x12b0000 = 0x2D4D6A
	//DWORD hookAddress = 0x01584D6A; //0x00B74D4B; -- Base + 0x4D4B ?
	// to find relative address: address - baseaddress
	DWORD hookAddress = this->baseAdd + offset;

	jumpBackAddress = hookAddress + hookLength;
	util::MakeJMP((BYTE *)hookAddress, (DWORD)funct, hookLength);
}






Hooks::~Hooks()
{
}
