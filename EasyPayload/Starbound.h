#pragma once
class Starbound
{
public:


	//static void* SayChatAction = (void*)0x00CAA3C0;
	static void SayChatActionHook(void *p, int a2);
	Starbound();
	~Starbound();
};

