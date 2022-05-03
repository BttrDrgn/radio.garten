#include "global.hpp"
#include "logger/logger.hpp"
#include "input/input.hpp"
#include "api/api.hpp"
#include "window/window.hpp"
#include "menus/menus.hpp"

//Main app init

void init()
{
	global::window = SDL_CreateWindow("Radio.Garten", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		global::resolution.x, global::resolution.y, 0);

	global::renderer = SDL_CreateRenderer(global::window, 0, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

	menus::init();

	while (!global::shutdown)
	{
		window::update();
		input::update();

		menus::prepare();
		menus::update();
		menus::present();

		if (!api::places_done)
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
ImVec2 global::resolution = {1280, 720};
bool global::always_on_top = false;