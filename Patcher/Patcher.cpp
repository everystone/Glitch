// Patcher.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Memory.h"
using namespace std;

//Define all Here, its easier.
char BytesToPatch[] = "\x90\x90"; //What we are replacing with, for example \x90\ = NOP.
char ProcessName[] = "target.exe"; //Processname
char BytePattern[] = "\x8B\x45\xF8\x83\xC0\x01"; //Our Pattern
char ByteMask[] = "xxxxxx"; //Our Mask A string of 'x' (match), '!' (not-match), or '?' (wildcard).
int Position = 0; //0 means first.
int NoOfBytes = 2;

//Our Main Function -- should be called from injected dll?
void InitiatePatch()
{
	DWORD Bytes = Memory::FindPattern(ProcessName, BytePattern, ByteMask);
	Bytes += Position;
	std::cout << "Found bytes at: " << Bytes;
	Memory::WriteToMemory(Bytes, BytesToPatch, NoOfBytes);

}

int main()
{
	//InitiatePatch();
	return 0;
}

