// Infector.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Syringe.h"

int main()
{

	// Inject our main dll
	if (Syringe::Inject("starbound.exe", "..\\debug\\payload.dll"))
	{
		printf("DLL Successfully injected. Press any key to exit");
	}

	std::cin.get();
	return 0;
}

