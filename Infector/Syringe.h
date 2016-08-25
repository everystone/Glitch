#pragma once
#include "stdafx.h"
#include <iostream>
#include <tlhelp32.h> 

class Syringe
{
public:

	static bool Syringe::Inject(char *targetExe, char *dllName);

private:
	static DWORD GetTargetThreadIdFromProcName(const char * ProcName);
	static bool fileExists(const std::string& name);
	static void printError(char *method, int errorCode);
};

