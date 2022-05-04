#pragma once

class input
{
public:
	static void update();
	static SDL_HitTestResult hit_test_callback(SDL_Window* win, const SDL_Point* area, void* data);
};