#pragma once

class global
{
public:
	static bool shutdown;
	static SDL_Window* window;
	static SDL_Renderer* renderer;
	static ImVec2 resolution;
	static bool always_on_top;
	static float framerate;
	static std::uint32_t framelimit;

	static std::uint32_t get_framelimit(std::uint32_t fl)
	{
		return 1000 / fl;
	}

	static float get_timestep()
	{
		return (framelimit / framerate);
	}
};