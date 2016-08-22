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

