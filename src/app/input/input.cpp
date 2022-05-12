#include "global.hpp"

#include "logger/logger.hpp"

#include "input.hpp"

#ifdef OVERLAY

WNDPROC o_wndproc;
bool toggle_once = false;

LRESULT __stdcall wndproc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui::IsKeyPressed(192, false) && !toggle_once)
	{
		global::hide = !global::hide;
		toggle_once = true;
	}
	else if (!ImGui::IsKeyPressed(192, false) && toggle_once) toggle_once = false;

	//Should prevent some applications from getting input
	if (!global::hide && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
	{
		return true;
	}

	return CallWindowProc(o_wndproc, hWnd, uMsg, wParam, lParam);
}

void input::init_overlay(HWND hwnd)
{
	if (global::hwnd != hwnd)
	{
		logger::log("INPUT_ERR", "Global hwnd is not the same!");
		return;
	}

	o_wndproc = (WNDPROC)SetWindowLongPtr(global::hwnd, GWLP_WNDPROC, (LONG_PTR)wndproc);
}
#endif

void input::update()
{
#ifndef OVERLAY
	SDL_Event evt;
	while (SDL_PollEvent(&evt))
	{
		switch (evt.type)
		{
		case SDL_QUIT:
			global::shutdown = true;
			break;
		}

		ImGui_ImplSDL2_ProcessEvent(&evt);
	}
#endif
}

#ifndef OVERLAY
SDL_HitTestResult input::hit_test_callback(SDL_Window* window, const SDL_Point* p, void* data)
{
	SDL_HitTestResult report = SDL_HITTEST_NORMAL;

	//Menu bar is 20 pixels tall
	//current buttons are 344 pixels wide
	if (p->y <= 20 && p->x > 344)
	{
		//Are we on header but not over a button?
		//Moveable window
		report = SDL_HITTEST_DRAGGABLE;
	}

	return report;
}
#endif