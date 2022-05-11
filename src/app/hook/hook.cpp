#include "hook.hpp"
#include "logger/logger.hpp"
#include "fs/fs.hpp"

void hook::load(process_t proc)
{

#ifdef _M_AMD64
	if (proc.arch == "x86")
	{
		ShellExecuteA(0, "open", "x86\\helper.exe", &logger::va("--pid %i --arch %s --hwnd %u", proc.pid, &proc.arch[0], proc.hwnd)[0], 0, 1);
		return;
	}
#endif

	hook::dlls.emplace_back(logger::va("overlay.radio.garten.%s.dll", &proc.arch[0]));

	for (const std::string& dll : hook::dlls)
	{
		std::string dll_path = fs::get_cur_dir().append(logger::va("\\%s\\%s", &proc.arch[0], &dll[0]));
		logger::log_debug(logger::va("Loading %s", &dll_path[0]));

		HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, proc.pid);
		if (!handle)
		{
			logger::log("HOOK_ERR", "Failed to open target");
			return;
		}

		LPVOID alloc = VirtualAllocEx(handle, 0, dll_path.length(), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		if (!alloc)
		{
			logger::log("HOOK_ERR", "Failed to allocate memory in target");
			return;
		}

		if (!WriteProcessMemory(handle, alloc, (LPVOID)&dll_path[0], dll_path.length(), 0))
		{
			logger::log("HOOK_ERR", "Failed to write memory in target");
			return;
		}

		DWORD thread_id;
		LPVOID loadlib = (LPVOID)GetProcAddress(LoadLibraryA("kernel32.dll"), "LoadLibraryA");
		if (!CreateRemoteThread(handle, 0, 0, (LPTHREAD_START_ROUTINE)loadlib, alloc, 0, &thread_id))
		{
			logger::log("HOOK_ERR", "Failed to create remote thread");
			return;
		}
	}

	//Not sure one does the trick but they all sound nice
	BringWindowToTop(proc.hwnd);
	SetForegroundWindow(proc.hwnd);
	SetFocus(proc.hwnd);

	hook::dlls.pop_back();
}

int CALLBACK hook::get_window(HWND hWnd, LPARAM lparam)
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

		hook::processes.emplace_back(process_t{ window_title, arch, proc_id, hWnd});
	}

	return 1;
}

void hook::get_procs()
{
	hook::processes = {};

	EnumWindows(hook::get_window, 0);
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

std::vector<std::string> hook::dlls
{
	"bass.dll",
	"discord_game_sdk.dll",
};