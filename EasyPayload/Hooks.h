#pragma once
class Hooks
{
public:
	Hooks();
	~Hooks();
	
	static void InstallHook(void *Address, void *HookFunction);
	static void SayMessage();
};

