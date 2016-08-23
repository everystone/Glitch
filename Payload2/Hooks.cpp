#include "stdafx.h"
#include "Hooks.h"


void Hooks::SayMessage() {
	
}

void Hooks::InstallHook(void *Address, void *HookFunction) {
	// Perform hooking
	HOOK_TRACE_INFO hHook = { NULL }; // keep track of our hook

	//std::cout << "\n";
	//std::cout << "Win32 Beep found at address: " << GetProcAddress(GetModuleHandle(TEXT("kernel32")), "Beep") << "\n";

	// Install the hook
	NTSTATUS result = LhInstallHook(
		Address,
		HookFunction,
		NULL,
		&hHook);
	if (FAILED(result))
	{
		//std::wstring s(RtlGetLastErrorString());
		//std::wcout << "Failed to install hook: ";
		//std::wcout << s;
	}
	else
	{
		//std::cout << "Hook installed successfully.";
	}

	// If the threadId in the ACL is set to 0,
	// then internally EasyHook uses GetCurrentThreadId()
	ULONG ACLEntries[1] = { 0 };

	// Disable the hook for the provided threadIds, enable for all others
	LhSetExclusiveACL(ACLEntries, 1, &hHook);
}

Hooks::Hooks()
{
}


Hooks::~Hooks()
{
}
