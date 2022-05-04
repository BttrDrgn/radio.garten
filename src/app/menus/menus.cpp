#include "global.hpp"
#include "logger/logger.hpp"
#include "menus.hpp"
#include "api/api.hpp"
#include "audio/audio.hpp"
#include "gfx/gfx.hpp"

void menus::init()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForSDLRenderer(global::window, global::renderer);
	ImGui_ImplSDLRenderer_Init(global::renderer);
}

void menus::update()
{
	if (menus::show_snow)
	{
		if (menus::snow.empty())
		{
			menus::enumerate_snow();
		}

		menus::render_snow();
	}

	ImGui::SetNextWindowPos({0, 0});
	ImGui::SetNextWindowSize(global::resolution);
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoBringToFrontOnFocus| ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground;
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
	SDL_SetRenderDrawColor(global::renderer, 68, 43, 134, 255);
	SDL_RenderClear(global::renderer);
}

void menus::present()
{
	ImGui::Render();
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
		if (ImGui::Button("Refresh Places"))
		{
			api::get_places();
		}

		ImGui::NewLine();

		if(ImGui::Button("Toggle Snow"))
		{
			menus::show_snow = !menus::show_snow;
		}

		ImGui::NewLine();

		if (ImGui::Button("Minimize"))
		{
			SDL_MinimizeWindow(global::window);
		}

		if (ImGui::Button("Exit"))
		{
			global::shutdown = true;
		}
		ImGui::EndMenu();
	}
}

void menus::places()
{
	if (ImGui::BeginMenu("Places"))
	{
		if (std::strlen(menus::search_buffer) == 0)
		{
			menus::filtering = false;
		}

		if (ImGui::Button("Reset Filter"))
		{
			menus::filtering = false;
			memset(menus::search_buffer, 0, sizeof(menus::search_buffer));
		}

		if (ImGui::InputText("Search", menus::search_buffer, sizeof(search_buffer)))
		{
			menus::filtering = true;
			api::filter_place(std::string(menus::search_buffer));
		}

		if (ImGui::BeginMenu("Locations"))
		{
			if (api::places_done)
			{
				if (!menus::filtering)
				{
					if (api::place.empty())
					{
						ImGui::Text("Places are empty!\nYou might need to refresh\n\nPlease click Actions -> Refresh Places.");
					}
					else
					{
						if (!menus::show_all_stations)
						{
							ImGui::Text("There are %i places, hidden by default for performance.\nClick the button to show all stations", api::place.size());
							if (ImGui::Button("Show All"))
							{
								menus::show_all_stations = true;
							}
						}
						else if (menus::show_all_stations)
						{
							if (ImGui::Button("Hide All"))
							{
								menus::show_all_stations = false;
							}

							ImGui::NewLine();

							for (auto place : api::place)
							{
								if (ImGui::Button(&logger::va("[%s] %s", &place.country[0], &place.city[0])[0]))
								{
									api::get_details(place.id);
									ImGui::CloseCurrentPopup();
								}
							}
						}
					}
				}
				else if (menus::filtering)
				{
					if (api::filtered_place.empty())
					{
						ImGui::Text("Places are empty!\nYou might need to refresh\n\nPlease click Actions -> Refresh Places.");
					}
					{
						for (auto place : api::filtered_place)
						{
							if (ImGui::Button(&logger::va("[%s] %s", &place.country[0], &place.city[0])[0]))
							{
								api::get_details(place.id);
								ImGui::CloseCurrentPopup();
							}
						}
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
		if (api::station.empty())
		{
			ImGui::Text("Stations are empty!\nYou might need to select a place.");
		}
		else
		{
			for (station_t station : api::station)
			{
				if (ImGui::Button(&logger::va("%s", &station.title[0])[0]))
				{
					audio::play(station.id);
				}
			}
		}
		ImGui::EndMenu();
	}
}

void menus::render_snow()
{
	ImGuiIO& io = ImGui::GetIO();

	for (std::int32_t i = 0; i < menus::snow.size(); i++)
	{
		if (menus::snow[i].pos.y > global::resolution.y)
		{
			menus::snow[i].pos.y = 0;
		}
		else
		{
			menus::snow[i].pos.y += (std::int32_t)std::ceil(2.0f * io.DeltaTime);
		}

		gfx::draw_circle(menus::snow[i].pos, 1.0f, { 255, 255, 255, 255 });
	}
}

void menus::enumerate_snow()
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<std::int32_t> x_pos(0, global::resolution.x);
	std::uniform_int_distribution<std::int32_t> y_pos(0, global::resolution.y);

	for (std::int32_t i = 0; i < menus::max_points; i++)
	{
		snow_t snow;
		snow.pos = SDL_Point{ x_pos(mt), y_pos(mt) };
		menus::snow.emplace_back(snow);
	}
}

std::vector<snow_t> menus::snow;
std::int32_t menus::max_points = 255;
bool menus::show_all_stations = false;
bool menus::show_snow = false;
bool menus::filtering = false;
char menus::search_buffer[64];