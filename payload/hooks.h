#pragma once
class Hooks
{
public:
	//void SayChatAction();
	Hooks(DWORD baseAdd);
	void InstallHook(DWORD offset, int hookLength, DWORD funct, DWORD &jumpBackAddress);
	~Hooks();



private:
	DWORD baseAdd;
};

