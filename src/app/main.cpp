#include <logger/logger.hpp>

#include "global.hpp"
#include "input/input.hpp"
#include "api/api.hpp"

//Main app init

void init()
{
	global::window = SDL_CreateWindow("Radio.Garten", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, global::resolution.x, global::resolution.y, 0);
	global::renderer = SDL_CreateRenderer(global::window, 0, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForSDLRenderer(global::window, global::renderer);
	ImGui_ImplSDLRenderer_Init(global::renderer);

	api::get_places();

	while (!global::shutdown)
	{
		input::update();

		ImGui_ImplSDLRenderer_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		//Menu render
		ImGui::Begin("Actions", nullptr, 0);
		if (ImGui::Button("Refresh"))
		{
			api::get_places();
		}
		ImGui::End();

		ImGui::Begin("Place ID's", nullptr, 0);

		if (api::places_done)
		{
			for (auto i = 0; i < api::place.size(); i++)
			{
				std::string country = api::place[i].country;
				std::string city = api::place[i].city;
				std::string id = api::place[i].id;
				std::string title = logger::va("[%s] %s : %s", country.c_str(), city.c_str(), id.c_str());
				ImGui::Button(title.c_str());
			}
		}

		ImGui::End();
		//end

		ImGui::Render();
		SDL_SetRenderDrawColor(global::renderer, 0, 0, 0, 255);
		SDL_RenderClear(global::renderer);
		ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
		SDL_RenderPresent(global::renderer);
	}

	//cleanup
	ImGui_ImplSDLRenderer_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_DestroyRenderer(global::renderer);
	SDL_DestroyWindow(global::window);
	SDL_Quit();
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