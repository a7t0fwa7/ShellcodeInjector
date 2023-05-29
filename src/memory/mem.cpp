#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>
#include "mem.h"

// big thanks to guidedhacking.com and king rake for teaching me how to reverse engineer/hack in early days ;)


DWORD mem::GetProcessId(const wchar_t* process_name)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	DWORD pid = 0;

	if (hSnapshot != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 procentry;
		procentry.dwSize = sizeof(procentry); // this is retarded lol

		if (Process32First(hSnapshot, &procentry))
		{
			do
			{
				if (!_wcsicmp(process_name, procentry.szExeFile))
				{
					pid = procentry.th32ProcessID;
					break;
				}
			} while (Process32Next(hSnapshot, &procentry));
		}
	}

	CloseHandle(hSnapshot);
	return pid;
}



//******************************** THE CODE BELOW IS NOT REQUIRED, I AM JUST GETTING AND OUTPUTING INFO ABOUT PROCESS YOU ARE TRYING TO EXECUTE SHELLCODE ON ********************************\\

uintptr_t mem::GetModuleBase(DWORD process_id, const wchar_t* process_name)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, process_id);
	uintptr_t modulebase = 0;

	if (hSnapshot != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 modentry;
		modentry.dwSize = sizeof(modentry); // this is retarded 2

		if (Module32First(hSnapshot, &modentry))
		{
			do
			{
				if (!_wcsicmp(process_name, modentry.szModule))
				{
					modulebase = (uintptr_t)modentry.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnapshot, &modentry));
		}
	}

	CloseHandle(hSnapshot);
	return modulebase;
}