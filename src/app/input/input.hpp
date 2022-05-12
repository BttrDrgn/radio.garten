#pragma once

class input
{
public:
	static void update();
#ifndef OVERLAY
	static SDL_HitTestResult hit_test_callback(SDL_Window* win, const SDL_Point* area, void* data);
#else
	static void init_overlay(HWND hwnd);
#endif
};