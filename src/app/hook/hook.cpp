#include "hook.hpp"
#include "logger/logger.hpp"

void hook::load(std::uint32_t pid)
{
	wchar_t* path = L"overlay.radio.garten.x86.dll";

	logger::log_info("Hook in progress");

	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (!handle)
	{
		logger::log("HOOK_ERR", "Failed to open target");
		return;
	}

	LPVOID alloc = VirtualAllocEx(handle, 0, lstrlenW(path), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (!alloc)
	{
		logger::log("HOOK_ERR", "Failed to allocate memory in target");
		return;
	}

	if (!WriteProcessMemory(handle, alloc, path, lstrlenW(path), 0))
	{
		logger::log("HOOK_ERR", "Failed to write memory in target");
		return;
	}

	DWORD thread_id;
	LPVOID loadlib = (LPVOID)GetProcAddress(LoadLibraryW(L"kernel32.dll"), "LoadLibraryW");
	if (!CreateRemoteThread(handle, 0, 0, (LPTHREAD_START_ROUTINE)loadlib, alloc, 0, &thread_id))
	{
		logger::log("HOOK_ERR", "Failed to create remote thread");
		return;
	}

	logger::log_info("Hook finished!");

}

int CALLBACK get_window(HWND hWnd, LPARAM lparam)
{

	int length = GetWindowTextLengthA(hWnd);
	char* buffer = new char[length + 1];
	GetWindowTextA(hWnd, buffer, length + 1);
	std::string window_title(buffer);

	if(!IsWow64Process)

	for (std::string look_up : hook::blacklist)
	{
		if (window_title.find(look_up) != std::string::npos)
		{
			return 0;
		}
	}

	if (IsWindowVisible(hWnd) && length != 0)
	{
		std::string arch;
		std::uint32_t proc_id;

		//Get proc_id
		GetWindowThreadProcessId(hWnd, (LPDWORD)&proc_id);

		//Get handle for arch detection
		if (HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, 0, proc_id))
		{
			int target = 0;
			if (IsWow64Process(handle, &target))
			{
				if (!target)
				{
					arch = "x86_64";
				}
				else
				{
					arch = "x86";
				}
			}
			else
			{
				arch = "???";
				logger::log_error(logger::va("Unable to determine arch! (0x%x)", GetLastError()));
			}
		}
		else if(!handle)
		{
			arch = "???";
			logger::log_error("Unable to open proc for arch detection!");
		}

		hook::processes.emplace_back(process_t{ window_title, arch, proc_id});
	}

	return 1;
}

void hook::get_procs()
{
	hook::processes = {};

	EnumWindows(get_window, 0);
}

std::vector<process_t> hook::processes;
std::vector<std::string> hook::blacklist
{
	"Groove Music",
	"Calculator",
	"Settings",
	"Movies & TV",
	"Google Chrome",
};