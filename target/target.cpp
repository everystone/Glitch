// target.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


void printNUmber(int num) {
	std::cout << "Number: " << num << std::endl;
}

int main()
{
	int num = 0;
	while (true) {
		printNUmber(++num);
		Sleep(1000);
	}
}

/*
00CD250E                   | 8B 45 F8                 | mov eax,dword ptr ss:[ebp-8]                       | target.cpp:15
00CD2511                   | 83 C0 01                 | add eax,1                                          |
*/