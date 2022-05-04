#include "global.hpp"
#include "logger/logger.hpp"
#include "input/input.hpp"
#include "api/api.hpp"
#include "window/window.hpp"
#include "menus/menus.hpp"
#include "audio/audio.hpp"

//Main app init

void init()
{
	global::window = SDL_CreateWindow("Radio.Garten", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		global::resolution.x, global::resolution.y, SDL_WINDOW_BORDERLESS);

	global::renderer = SDL_CreateRenderer(global::window, 0, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

	audio::init();
	menus::init();

	while (!global::shutdown)
	{
		std::uint64_t s = SDL_GetPerformanceCounter();
		std::uint32_t start = SDL_GetTicks();

		window::update();
		input::update();

		menus::prepare();
		menus::update();
		menus::present();

		if (global::framelimit > (SDL_GetTicks() - start))
		{
			SDL_Delay(global::framelimit - (SDL_GetTicks() - start));
		}

		global::framerate = 1.0f / ((SDL_GetPerformanceCounter() - s) / (float)SDL_GetPerformanceFrequency());

		if (!api::places_done || !api::detail_done || !api::stations_done)
		{
			//Prevent shutdown while enumerating data as it causes a hard crash in the http lib
			global::shutdown = false;
		}
	}

	menus::cleanup();
}

int main(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_TIMER | SDL_INIT_VIDEO) != 0)
	{
		LOG_ERROR("%s", SDL_GetError());
	}

	init();

	return 0;
}

bool global::shutdown = false;
SDL_Window* global::window = 0;
SDL_Renderer* global::renderer = 0;
ImVec2 global::resolution = {800, 600};
bool global::always_on_top = false;
float global::framerate = 0.0f;
std::uint32_t global::framelimit = global::get_framelimit(60);
