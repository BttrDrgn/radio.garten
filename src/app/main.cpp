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

	audio::enumerate_playlist(audio::playlist_dir);
	audio::shuffle();
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
void init_overlay()
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

	audio::playlist_dir = fs::get_self_path() + audio::playlist_dir;

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

bool __stdcall DllMain(::HMODULE hmod, ::DWORD reason, ::LPVOID)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		std::ios_base::sync_with_stdio(false);

		AllocConsole();
		SetConsoleTitleA("ECM Debug Console");


		std::freopen("CONOUT$", "w", stdout);
		std::freopen("CONIN$", "r", stdin);
		logger::log_info("Attached!");

		DisableThreadLibraryCalls(hmod);
		global::self = hmod;

		//For WHATEVER REASON, the mod fails to load properly without the console allocated
		//so we hide it here instead...
#ifdef NDEBUG
		ShowWindow(GetConsoleWindow(), 0);
#endif

		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)init_overlay, 0, 0, 0);
		return true;
	}
	return true;
}
