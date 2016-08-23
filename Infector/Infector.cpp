// Infector.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Syringe.h"
#include <io.h>

int main()
{

	



	// Inject our main dll
	if (!Syringe::Inject("starbound.exe", "..\\debug\\payload.dll"))
	{
		printf("Not loaded.."); // If injection is not sucsessfull 
	}
	else
	{
		printf("Loaded!"); //  If injection is sucsessfull 
	}

	std::cin.get();
    return 0;
}

