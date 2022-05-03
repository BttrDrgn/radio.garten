#include "global.hpp"
#include "logger/logger.hpp"
#include "menus.hpp"
#include "api/api.hpp"

void menus::init()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForSDLRenderer(global::window, global::renderer);
	ImGui_ImplSDLRenderer_Init(global::renderer);
}

void menus::update()
{
	ImGuiStyle& style = ImGui::GetStyle();
	ImGuiIO& io = ImGui::GetIO();

	ImGui::SetNextWindowPos({0, 0});
	ImGui::SetNextWindowSize(global::resolution);
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove;
	if(ImGui::Begin("Radio.Garten", nullptr, flags))
	{
		menus::main_menu_bar();
		ImGui::End();
	}
}

void menus::prepare()
{
	ImGui_ImplSDLRenderer_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
}

void menus::present()
{
	ImGui::Render();
	SDL_SetRenderDrawColor(global::renderer, 0, 0, 0, 255);
	SDL_RenderClear(global::renderer);
	ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
	SDL_RenderPresent(global::renderer);
}

void menus::cleanup()
{
	ImGui_ImplSDLRenderer_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_DestroyRenderer(global::renderer);
	SDL_DestroyWindow(global::window);
	SDL_Quit();
}

void menus::main_menu_bar()
{
	if (ImGui::BeginMainMenuBar())
	{
		menus::actions();
		menus::places();
		menus::stations();
		ImGui::EndMainMenuBar();
	}
}

void menus::actions()
{
	if (ImGui::BeginMenu("Actions"))
	{
		if (ImGui::Button("Refresh"))
		{
			api::get_places();
		}
		ImGui::EndMenu();
	}
}

void menus::places()
{
	if (ImGui::BeginMenu("Places"))
	{
		
		if (ImGui::BeginCombo("Country", &menus::current_country[0]))
		{
			if (api::places_done)
			{
				if (ImGui::Button("Reset"))
				{
					menus::current_country = "N/A";
					ImGui::CloseCurrentPopup();
				}

				ImGui::NewLine();

				for (auto c : api::all_countries)
				{
					if (ImGui::Button(&c[0]))
					{
						menus::current_country = c;
						ImGui::CloseCurrentPopup();
					}
				}
			}
			ImGui::EndCombo();
		}


		if (ImGui::BeginMenu("Locations"))
		{
			if (api::places_done)
			{
				for (auto place : api::place)
				{
					std::string country = place.country;
					if (std::strcmp(&menus::current_country[0], "N/A"))
					{
						if (!std::strcmp(&place.country[0], &menus::current_country[0]))
						{
							if (ImGui::Button(&logger::va("%s", &place.city[0])[0]))
							{
								SDL_SetClipboardText(&place.id[0]);
							}
						}
					}
					else
					{
						ImGui::Button(&logger::va("[%s] %s", &country[0], &place.city[0])[0]);
					}
				}
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenu();
	}
}

void menus::stations()
{
	if (ImGui::BeginMenu("Stations"))
	{

		ImGui::EndMenu();
	}
}

std::string menus::current_country = "N/A";