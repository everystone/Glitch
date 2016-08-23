#pragma once
class Hack
{
public:
	static void Init(DWORD base);
	static void MakeJMP(BYTE * pAddress, DWORD dwJumpTo, DWORD dwLen);
	static void InstallHook(DWORD offset, int hookLength, DWORD funct, DWORD &jumpBackAddress);
private:
	static DWORD baseAddress;
};

