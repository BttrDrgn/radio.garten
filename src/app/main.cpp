#include "global.hpp"
#include "logger/logger.hpp"
#include "input/input.hpp"
#include "api/api.hpp"
#include "window/window.hpp"
#include "menus/menus.hpp"
#include "audio/audio.hpp"
#include "settings/settings.hpp"
#include "hook/hook.hpp"

//Main app init

void init()
{
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
	hook::get_procs();

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
#ifdef NDEBUG
	ShowWindow(GetConsoleWindow(), 0);
#endif
#endif

	if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_TIMER | SDL_INIT_VIDEO) != 0)
	{
		logger::log_error(logger::va("%s", SDL_GetError()));
		global::shutdown = true;
	}

	init();

	return 0;
}