#include "global.hpp"
#include "logger/logger.hpp"
#include "input/input.hpp"
#include "api/api.hpp"
#include "menus/menus.hpp"
#include "audio/audio.hpp"
#include "settings/settings.hpp"
#include "fs/fs.hpp"

#ifndef OVERLAY
#include "window/window.hpp"
#include "hook/hook.hpp"
#else
#include "hook/impl/d3d9_impl.h"
#include "hook/impl/d3d10_impl.h"
#include "hook/impl/d3d11_impl.h"
#include "hook/impl/opengl3_impl.h"
#endif

//Main app init
#ifndef OVERLAY
void init_app()
{
#ifdef WIN32
#ifdef _M_AMD64
	SetDllDirectoryA("x86_64");
#else
	SetDllDirectoryA("x86");
#endif
#endif

	fs::init();
	settings::init();

	global::desired_framerate = 60;
	global::framelimit = 1000 / global::desired_framerate;

	global::window = SDL_CreateWindow("Radio.Garten", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		global::resolution.x, global::resolution.y, SDL_WINDOW_BORDERLESS);

	//Use hardware
	if (!global::use_hardware)
	{
		global::surface = SDL_GetWindowSurface(global::window);
		global::renderer = SDL_CreateSoftwareRenderer(global::surface);
	}
	else if (global::use_hardware)
	{
		global::renderer = SDL_CreateRenderer(global::window, 0, SDL_RENDERER_ACCELERATED);
	}

	if (SDL_SetWindowHitTest(global::window, input::hit_test_callback, 0) != 0)
	{
		logger::log_error(logger::va("Failed to init hit test! %s", SDL_GetError()));
		global::shutdown = true;
	}

	audio::init();
	menus::init();

	while (!global::shutdown)
	{
		global::tick_start();

		window::update();
		input::update();

		menus::prepare();
		menus::update();
		menus::present();

		if (!api::places_done || !api::detail_done || !api::stations_done)
		{
			//Prevent shutdown while enumerating data as it causes a hard crash in the http lib
			global::shutdown = false;
		}

		global::tick_end();
	}

	menus::cleanup();
}

int main(int argc, char* argv[])
{
#ifdef _WIN32
#ifdef DEBUG
	AllocConsole();
	SetConsoleTitleA("Radio.Garten Debug Console");

	std::freopen("CONOUT$", "w", stdout);
	std::freopen("CONIN$", "r", stdin);
#endif
#endif

	if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_TIMER | SDL_INIT_VIDEO) != 0)
	{
		logger::log_error(logger::va("%s", SDL_GetError()));
		global::shutdown = true;
	}

	init_app();

	return 0;
}

#else
HMODULE self;

//Overlay init
void init_overlay()
{
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
			FreeLibraryAndExitThread(self, 0);
			break;
		}
	}
}

bool __stdcall DllMain(::HMODULE hmod, ::DWORD reason, ::LPVOID)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
#ifdef DEBUG
		AllocConsole();
		SetConsoleTitleA("Radio.Garten Debug Console");

		std::freopen("CONOUT$", "w", stdout);
		std::freopen("CONIN$", "r", stdin);
		logger::log_info("Attached!");
#endif
		DisableThreadLibraryCalls(hmod);
		self = hmod;
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)init_overlay, 0, 0, 0);
	}
	return true;
}
#endif