#include "global.hpp"

#include "input.hpp"

void input::update()
{
	ImGuiIO& io = ImGui::GetIO();
	SDL_Event evt;
	while (SDL_PollEvent(&evt))
	{
		ImGui_ImplSDL2_ProcessEvent(&evt);

		switch (evt.type)
		{
		case SDL_QUIT:
			global::shutdown = true;
			break;
		}
	}
}