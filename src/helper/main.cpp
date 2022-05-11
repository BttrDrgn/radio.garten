#include "../app/global.hpp"
#include "../utils/logger/logger.hpp"
#include "../utils/fs/fs.hpp"

process_t proc;

std::vector<std::string> args;
std::vector<std::string> dlls
{
	"bass.dll",
	"discord_game_sdk.dll",
	"overlay.radio.garten.x86.dll",
};

void load()
{
	//A smaller version of hook::load
	for (const std::string& dll : dlls)
	{
		DWORD thread_id;
		std::string dll_path = fs::get_cur_dir().append(logger::va("\\%s\\%s", &proc.arch[0], &dll[0]));

		HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, proc.pid);
		LPVOID alloc = VirtualAllocEx(handle, 0, dll_path.length(), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
		WriteProcessMemory(handle, alloc, (LPVOID)&dll_path[0], dll_path.length(), 0);
		LPVOID loadlib = (LPVOID)GetProcAddress(LoadLibraryA("kernel32.dll"), "LoadLibraryA");
		CreateRemoteThread(handle, 0, 0, (LPTHREAD_START_ROUTINE)loadlib, alloc, 0, &thread_id);
	}

	BringWindowToTop(proc.hwnd);
	SetForegroundWindow(proc.hwnd);
	SetFocus(proc.hwnd);
}

int __stdcall WinMain(HINSTANCE instance, HINSTANCE prev_instance, char* cmd_line, int cmd_show)
{
	for (int i = 0; i < __argc; i++)
	{
		args.emplace_back(__argv[i]);
	}

	for (int i = 0; i < args.size(); i++)
	{
		if (args[i].find("--pid") != std::string::npos)
		{
			int value = std::stoi(__argv[i + 1]);
			proc.pid = (std::uint32_t)value;
		}
		else if (args[i].find("--arch") != std::string::npos)
		{
			std::string value = __argv[i + 1];
			proc.arch = value;
		}
		else if (args[i].find("--hwnd") != std::string::npos)
		{
			HWND value = reinterpret_cast<HWND>(std::stoul(__argv[i + 1], nullptr, 0));
			proc.hwnd = value;
		}
	}

	if (!proc.pid || proc.arch.size() == 0 || proc.hwnd == 0)
	{
		MessageBoxA(0, "Not enough arguments!", "Helper", 0);
	}

	load();
}