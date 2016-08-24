// Infector.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Syringe.h"

int main(int argc, char *argv[])
{
	if (argc != 3) {
		printf("Usage: %s <%s> <%s>", argv[0], "process.exe", "path-to-payload.dll");
		Sleep(3000);
		return 0;
	}
	// Inject our main dll
	/*if (Syringe::Inject("starbound.exe", "..\\debug\\payload.dll"))*/
	if (Syringe::Inject(argv[1], argv[2]))
	{
		printf("DLL Successfully injected. Exiting..");
		Sleep(1000);
	}
	else {
		printf("DLL Injection failed, press return to exit.");
		std::cin.get();
	}

	
	return 0;
}

