#include "global.hpp"

#include "input.hpp"

void input::update()
{
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
}