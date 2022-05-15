#pragma once

#ifdef _M_AMD64
std::int64_t ImGui_ImplWin32_WndProcHandler(::HWND, std::uint32_t, std::uint64_t, std::int64_t);
#else
long ImGui_ImplWin32_WndProcHandler(::HWND, std::uint32_t, std::uint32_t, long);
#endif

class input
{
public:
	static void update();
#ifndef OVERLAY
	static SDL_HitTestResult hit_test_callback(SDL_Window* win, const SDL_Point* area, void* data);
#else
	static void init_overlay();
#endif
};