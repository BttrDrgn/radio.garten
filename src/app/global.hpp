#pragma once

struct vec2
{
	int x, y;
};

class global
{
public:
	static bool shutdown;

	static void msg_box(std::string title, std::string message)
	{
#ifdef OVERLAY
		MessageBoxA(global::hwnd, &message[0], &title[0], 0);
#else
		SDL_ShowSimpleMessageBox(0, &title[0], &message[0], global::window);
#endif
	}

#ifndef OVERLAY
	static SDL_Window* window;
	static SDL_Renderer* renderer;
	static SDL_Surface* surface;
	static ImVec2 resolution;
	static bool always_on_top;
	static bool use_hardware;
	static std::float_t framerate;
	static std::uint32_t  desired_framerate;
	static std::uint32_t framelimit;

	static std::uint64_t counter;
	static std::uint32_t start;

	static void tick_start()
	{
		global::counter = SDL_GetPerformanceCounter();
		global::start = SDL_GetTicks();
	}

	static void tick_end()
	{
		if (global::framelimit > (SDL_GetTicks() - global::start))
		{
			SDL_Delay(global::framelimit - (SDL_GetTicks() - global::start));
		}

		global::framerate = 1.0f / ((SDL_GetPerformanceCounter() - global::counter) / (std::float_t)SDL_GetPerformanceFrequency());
	}

	static std::float_t get_timestep()
	{
		return (global::desired_framerate / global::framerate);
	}
#else
	static HWND hwnd;
#endif
};