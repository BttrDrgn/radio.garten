#include "global.hpp"
#include "logger/logger.hpp"
#include "api/api.hpp"
#include "menus/menus.hpp"
#include "audio/audio.hpp"
#include "settings/settings.hpp"

//Overlay G-API implementations
#include "hook/impl/d3d9_impl.h"
#include "hook/impl/d3d10_impl.h"
#include "hook/impl/d3d11_impl.h"
#include "hook/impl/opengl3_impl.h"

//Main app init

void init()
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
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)init, 0, 0, 0);
	}
	return true;
}