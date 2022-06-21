#include "global.hpp"
#include "logger/logger.hpp"
#include "input/input.hpp"
#include "menus/menus.hpp"
#include "audio/audio.hpp"
#include "settings/settings.hpp"
#include "fs/fs.hpp"
#include "hook/hook.hpp"

#include "hook/impl/d3d9_impl.h"
#include "hook/impl/d3d10_impl.h"
#include "hook/impl/d3d11_impl.h"
#include "hook/impl/opengl3_impl.h"

game_t game = game_t::NFSU2;

void sys_init_()
{
	global::sys_init = true;

	//Mute the in-game music
	switch (global::game)
	{
	case game_t::NFSU2:
		*(float*)(0x0083AA30) = 0.0f; //FE
		*(float*)(0x0083AA34) = 0.0f; //IG
		break;
	}
}

__declspec(naked) void sys_init()
{
	__asm
	{
		call sys_init_;
		push 0x0057EDA8;
		retn;
	}
}

static void(* sub_00537980_)(int a2, char* a3, int a4);
void sub_00537980(int a2, char* a3, int a4)
{
	bool found = false;
	for (const char* package : audio::mute_detection)
	{
		if (!strcmp(package, a3))
		{
			found = true;
			break;
		}
	}

	//Will be reworked to better match the game eventually
	if (found)
	{
		audio::pause();
	}
	else
	{
		audio::play();
	}

	return sub_00537980_(a2, a3, a4);
}

//Overlay init
void init()
{
	MH_Initialize();

	switch (game)
	{
	case game_t::NFSU2:
		hook::retn(0x004AC950);	//Kill SummonChryon for custom impl

		*(std::uint8_t*)(0x00534535) = 0xEB; //Prevent save from loading audio values

		//Disable sliders in menu
		hook::jump(0x004B6EDA, 0x004B6F92);

		//Disable sliders in-game menu
		hook::jump(0x004C347B, 0x004C3533);

		//Wait for sys init stub
		hook::jump(0x0057EDA3, sys_init);

		MH_CreateHook((void*)0x00537980, sub_00537980, (void**)&sub_00537980_);
		break;
	}

	settings::init();

	if (kiero::init(kiero::RenderType::Auto) == kiero::Status::Success)
	{
		switch (kiero::getRenderType())
		{
#if KIERO_INCLUDE_D3D9
		case kiero::RenderType::D3D9:
			impl::d3d9::init();
			break;
#endif

#if KIERO_INCLUDE_D3D10
		case kiero::RenderType::D3D10:
			impl::d3d10::init();
			break;
#endif

#if KIERO_INCLUDE_D3D11
		case kiero::RenderType::D3D11:
			impl::d3d11::init();
			break;
#endif

#if KIERO_INCLUDE_OPENGL
		case kiero::RenderType::OpenGL:
			impl::opengl3::init();
			break;
#endif

		case kiero::RenderType::None:
			FreeLibraryAndExitThread(global::self, 0);
			break;
		}
	}

	MH_EnableHook(MH_ALL_HOOKS);
}

LONG WINAPI CustomUnhandledExceptionFilter(LPEXCEPTION_POINTERS ExceptionInfo)
{
	// step 1: write minidump
	static LPEXCEPTION_POINTERS exceptionData;

	exceptionData = ExceptionInfo;

	char error[1024] = { 0 };
	char filename[MAX_PATH];
	__time64_t time;
	tm* ltime;

	_time64(&time);
	ltime = _localtime64(&time);
	strftime(filename, std::size(filename) - 1, "ecm-%Y%m%d%H%M%S.dmp", ltime);
	_snprintf(error, std::size(error) - 1, "A minidump has been written to %s.", filename);

	HANDLE hFile = CreateFileA(filename, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		MINIDUMP_EXCEPTION_INFORMATION ex;
		memset(&ex, 0, sizeof(ex));
		ex.ThreadId = GetCurrentThreadId();
		ex.ExceptionPointers = exceptionData;
		ex.ClientPointers = FALSE;

#if defined(DEBUG)
		MINIDUMP_TYPE mdType = (MINIDUMP_TYPE)(MiniDumpWithProcessThreadData | MiniDumpWithUnloadedModules | MiniDumpWithThreadInfo | MiniDumpWithFullMemory);
#else
		MINIDUMP_TYPE mdType = MiniDumpNormal;
#endif

		MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, mdType, &ex, NULL, NULL);
		MessageBoxA(NULL, error, "ECM", MB_OK | MB_ICONERROR);

		CloseHandle(hFile);
	}

	return EXCEPTION_EXECUTE_HANDLER;
}

DWORD WINAPI OnAttachImpl(LPVOID lpParameter)
{
	std::ios_base::sync_with_stdio(false);

	AllocConsole();
	SetConsoleTitleA("ECM Debug Console");


	std::freopen("CONOUT$", "w", stdout);
	std::freopen("CONIN$", "r", stdin);

	//For WHATEVER REASON, the mod fails to load properly without the console allocated
	//so we hide it here instead...
#ifdef NDEBUG
	ShowWindow(GetConsoleWindow(), 0);
#endif


	int found = -1;
	for (int i = 0; i < global::game_bins.size(); ++i)
	{
		if (fs::exists(global::game_bins[i]))
		{
			found = i;
			break;
		}
	}

	if (found == -1)
	{
		global::game = game_t::UNIVERSAL;
		logger::log_info("No game found! Switching to universal mode.");
	}
	else
	{
		global::game = (game_t)found;
		logger::log_info(logger::va("Game = %i", global::game));
	}

	init();
	return 0;
}

DWORD WINAPI OnAttach(LPVOID lpParameter)
{
	__try
	{
		return OnAttachImpl(lpParameter);
	}
	__except (CustomUnhandledExceptionFilter(GetExceptionInformation()))
	{
		FreeLibraryAndExitThread((HMODULE)lpParameter, 0xDECEA5ED);
	}

	return 0;
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		global::self = hModule;
		DisableThreadLibraryCalls(global::self);
		CreateThread(nullptr, 0, OnAttach, global::self, 0, nullptr);
		return true;
	}

	return false;
}
