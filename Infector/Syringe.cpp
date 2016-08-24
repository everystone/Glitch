#include "stdafx.h"
#include "Syringe.h"
#pragma warning(disable:4996)

bool Syringe::Inject(char *targetExe, char * dll)
{
	HANDLE Proc;
	HMODULE hLib;
	LPVOID RemoteString, LoadLibAddy;


	// The name of the process you want to inject
	DWORD pId = Syringe::GetTargetThreadIdFromProcName(targetExe);

	if (pId == 0) {
		printf("Target: ERROR (Process not found): %s..\n", targetExe);
		return false;
	}
	printf("Target: %s (%d)\n", targetExe, pId);

	// Get the dll's full path name 
	char dllPath[MAX_PATH] = { 0 };
	GetFullPathName(dll, MAX_PATH, dllPath, NULL);

	// check existence
	if (!fileExists(dllPath)) {
		printf("Dll: ERROR (File not found):  %s..\n", dllPath);
		return false;
	}


	printf("Dll: %s\n", dllPath);

	Proc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pId);
	if (!Proc)
	{
		int errorCode = GetLastError();
		printf("OpenProcess() failed with error code: %d ", errorCode);
		switch (errorCode) {
		case 0x2:
			printf("(ERROR_FILE_NOT_FOUND)\n");
			break;
		case 0x3:
			printf("(ERROR_PATH_NOT_FOUND)\n");
			break;
		case 0x5:
			printf("(ERROR_ACCESS_DENIED)\n");
			break;
		}
		// https://msdn.microsoft.com/en-us/library/windows/desktop/ms681382(v=vs.85).aspx

		return false;
	}
	LoadLibAddy = (LPVOID)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");
	// Allocate space in the process for our DLL
	RemoteString = (LPVOID)VirtualAllocEx(Proc, NULL, strlen(dllPath), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	// Write the string name of our DLL in the memory allocated 
	WriteProcessMemory(Proc, (LPVOID)RemoteString, dllPath, strlen(dllPath), NULL);
	CreateRemoteThread(Proc, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibAddy, (LPVOID)RemoteString, NULL, NULL);
	CloseHandle(Proc);
	return true;
}

DWORD Syringe::GetTargetThreadIdFromProcName(const char * ProcName)
{
	PROCESSENTRY32 pe;
	HANDLE thSnapShot;
	BOOL retval, ProcFound = false;

	thSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (thSnapShot == INVALID_HANDLE_VALUE)
	{
		printf("Error: Unable to create toolhelp snapshot!");
		return false;
	}
	pe.dwSize = sizeof(PROCESSENTRY32);
	retval = Process32First(thSnapShot, &pe);
	while (retval)
	{
		//std::cout << pe.szExeFile << std::endl;
		if (!strcmp(pe.szExeFile, ProcName))
		{
			return pe.th32ProcessID;
		}
		retval = Process32Next(thSnapShot, &pe);
	}
	return 0;
}

bool Syringe::fileExists(const std::string& name)
{

	if (FILE *file = fopen(name.c_str(), "r")) {
		fclose(file);
		return true;
	}
	else {
		return false;
	}

}
