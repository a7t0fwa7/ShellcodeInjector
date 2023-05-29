#pragma once
#include <Windows.h>
#include <iostream>

namespace mem
{
	DWORD GetProcessId(const wchar_t* process_name);
	uintptr_t GetModuleBase(DWORD process_id, const wchar_t* process_name);

}