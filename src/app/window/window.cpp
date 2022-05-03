#include "global.hpp"

#include "window.hpp"

void window::update()
{
	if (global::always_on_top) SDL_SetWindowAlwaysOnTop(global::window, SDL_TRUE);
	else SDL_SetWindowAlwaysOnTop(global::window, SDL_FALSE);
}