#include "stdafx.h"
#include "Starbound.h"

typedef int func(void *p, int a2);
func* SayChatAction = (func*)0x00CAA3C0;

void Starbound::SayChatActionHook(void *func, int a2)
{
	SayChatAction(func, a2);
}

Starbound::Starbound()
{
}


Starbound::~Starbound()
{
}
