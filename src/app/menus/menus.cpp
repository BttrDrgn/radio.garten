#include "global.hpp"
#include "logger/logger.hpp"
#include "fs/fs.hpp"
#include "menus.hpp"
#include "api/api.hpp"
#include "audio/audio.hpp"
#include "hook/hook.hpp"
#include "settings/settings.hpp"

#ifndef OVERLAY
#include "gfx/gfx.hpp"
#endif

#ifdef _WIN32
#include "drpc/drpc.hpp"
#include <shellapi.h>
#endif

void menus::init()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui::GetIO().IniFilename = nullptr;

	menus::build_font(ImGui::GetIO());

#ifndef OVERLAY
	ImGui_ImplSDL2_InitForSDLRenderer(global::window, global::renderer);
	ImGui_ImplSDLRenderer_Init(global::renderer);
#endif
}

void menus::prepare()
{
#ifndef OVERLAY
	ImGui_ImplSDLRenderer_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
	SDL_SetRenderDrawColor(global::renderer, 68, 43, 134, 255);
	SDL_RenderClear(global::renderer);
#else
	switch (global::renderer)
	{
	case kiero::RenderType::D3D9:
		ImGui_ImplDX9_NewFrame();
		break;

	case kiero::RenderType::D3D10:
		ImGui_ImplDX10_NewFrame();
		break;

	case kiero::RenderType::D3D11:
		ImGui_ImplDX11_NewFrame();
		break;

	case kiero::RenderType::OpenGL:
		ImGui_ImplOpenGL3_NewFrame();
		break;
	}

	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
#endif
}

void menus::present()
{
#ifndef OVERLAY
	ImGui::Render();
	ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());

	if (!global::use_hardware)
	{
		SDL_UpdateWindowSurface(global::window);

	}
	else if (global::use_hardware)
	{
		SDL_RenderPresent(global::renderer);
	}
#else
	ImGui::EndFrame();
	ImGui::Render();

	switch (global::renderer)
	{
	case kiero::RenderType::D3D9:
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		break;

	case kiero::RenderType::D3D10:
		ImGui_ImplDX10_RenderDrawData(ImGui::GetDrawData());
		break;

	case kiero::RenderType::D3D11:
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		break;

	case kiero::RenderType::OpenGL:
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		break;
	}
#endif
}

void menus::cleanup()
{
#ifndef OVERLAY
	ImGui_ImplSDLRenderer_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_DestroyRenderer(global::renderer);
	SDL_DestroyWindow(global::window);
	SDL_Quit();
#else

#endif
}

void menus::update()
{

#ifndef OVERLAY
	if (menus::show_snow)
	{
		if (menus::snow.empty())
		{
			menus::enumerate_snow();
		}
		menus::render_snow();
	}
	else if (!menus::show_snow)
	{
		if (!menus::snow.empty())
		{
			menus::snow.clear();
		}
	}

	static float count = 0;
	count += 1.0f * global::get_timestep();
	if (count > 100.0f)
	{
		count = 0.0f;
		if (hook::auto_refresh)
		{
			hook::get_procs();
		}
	}
#endif

	settings::favorites_update();

#ifdef OVERLAY
	if (!global::hide)
	{
#endif
		ImGui::SetNextWindowPos({ 0, 0 });
#ifndef OVERLAY
		ImGui::SetNextWindowSize(global::resolution);
#endif
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground;
		if (ImGui::Begin("Radio.Garten", nullptr, flags))
		{
			menus::main_menu_bar();
			ImGui::End();
		}
#ifdef OVERLAY
	}
#endif
}

void menus::main_menu_bar()
{
	if (ImGui::BeginMainMenuBar())
	{
		menus::actions();
		menus::places();
		menus::stations();
		menus::favorites();
#ifndef OVERLAY
		menus::overlay();

		std::string listening = logger::va("Listening: %s on %s", &audio::currently_playing.title[0], &audio::currently_playing.station.title[0]);

		//If text offscreen
		if ((ImGui::GetCursorPosX() + ImGui::CalcTextSize(&listening[0]).x) > global::resolution.x)
		{
			//This is probably a terrible way to do this but it works TM
			static bool mode = false;
			static float counter = 0;

			if (counter > 500.0f)
			{
				counter = 0;
				mode = !mode;
			}

			switch (mode)
			{
			case true:
				ImGui::Text("Listening: %s", &audio::currently_playing.station.title[0]);
				break;
			case false:
				ImGui::Text("Listening: %s", &audio::currently_playing.title[0]);
				break;
			}

			counter += 1 * global::get_timestep();
		}
		else
		{
			ImGui::Text(&listening[0]);
		}
#else
		std::string listening = logger::va("Listening: %s on %s", &audio::currently_playing.title[0], &audio::currently_playing.station.title[0]);
		ImGui::Text(&listening[0]);
#endif

		ImGui::EndMainMenuBar();
	}
}

void menus::actions()
{
	if (ImGui::BeginMenu("Actions"))
	{
		if (ImGui::Button("Refresh Places"))
		{
			memset(menus::place_search_buffer, 0, sizeof(menus::place_search_buffer));
			api::get_places();
		}

		ImGui::SameLine();

		if (audio::currently_playing.station.title != "N/A")
		{
			if (ImGui::Button("Share Station"))
			{
				std::string url = logger::va("%s%s", HOST_NAME, &api::get_shareable_url(audio::currently_playing.station.id)[0]);

				ImGuiIO& io = ImGui::GetIO();
				io.SetClipboardTextFn(io.ClipboardUserData, &url[0]);
			}
		}

		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("Copies the link to the\ncurrently playing station");
			ImGui::EndTooltip();
		}

		ImGui::NewLine();
		
		ImGui::Text("Audio Controls");
		ImGui::Text("Volume: ");

		ImGui::SameLine();
		ImGui::PushItemWidth(50.0f);

		if (ImGui::SliderInt("##Volume", &audio::volume, 0, 100))
		{
			audio::set_volume(audio::volume);
		}

		if (!audio::paused)
		{
			if (audio::currently_playing.url != "")
			{
				ImGui::SameLine();
				if (ImGui::Button("||"))
				{
					audio::paused = true;
					audio::stop();
				}

				if (ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();
					ImGui::Text("Pause");
					ImGui::EndTooltip();
				}
			}
		}
		else if (audio::paused)
		{
			ImGui::SameLine();
			if (ImGui::Button(">"))
			{
				if (audio::currently_playing.url != "")
				{
					audio::paused = false;
					audio::play(audio::currently_playing.url);
				}
			}

			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Text("Play");
				ImGui::EndTooltip();
			}
		}

		if (audio::currently_playing.url != "")
		{
			ImGui::SameLine();

			if (ImGui::Button("[]"))
			{
				audio::paused = false;
				audio::stop(true);
			}

			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Text("Stop");
				ImGui::EndTooltip();
			}
		}

		ImGui::NewLine();

#ifdef _WIN32
		if (ImGui::Button(&logger::va("Toggle Discord [%s]", &logger::get_toggle(menus::show_drpc)[0])[0]))
		{
			menus::show_drpc = !menus::show_drpc;
			switch (menus::show_drpc)
			{
			case true:
				drpc::init();
				break;
			case false:
				drpc::deinit();
				break;
			}
		}

		if (!menus::show_drpc)
		{
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Text("Discord now playing status\n\nShows: time since enabled, song title,\nstation name, and region of station");
				ImGui::EndTooltip();
			}
		}

#ifndef OVERLAY
		if (ImGui::Button(&logger::va("Toggle Snow [%s]", &logger::get_toggle(menus::show_snow)[0])[0]))
		{
			menus::show_snow = !menus::show_snow;
		}

		if (!menus::show_snow)
		{
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Text("Fun snow rains down\nfrom the top of the screen!");
				ImGui::EndTooltip();
			}
		}
#endif
#endif


		ImGui::NewLine();


		if (ImGui::Button("Minimize"))
		{
#ifndef OVERLAY
			SDL_MinimizeWindow(global::window);
#else
			global::hide = true;
#endif
		}

#ifndef OVERLAY
		if (ImGui::Button("Exit"))
		{
			global::shutdown = true;
		}
#endif
		ImGui::EndMenu();
	}
}

void menus::places()
{
	if (ImGui::BeginMenu("Places"))
	{
		if (std::strlen(menus::place_search_buffer) == 0)
		{
			menus::filtering = false;
		}

		if (ImGui::Button("Reset Filter"))
		{
			menus::filtering = false;
			menus::current_country = "N/A";
			memset(menus::place_search_buffer, 0, sizeof(menus::place_search_buffer));
		}

		if (ImGui::BeginCombo("Country", &menus::current_country[0]))
		{
			if (api::places_done)
			{
				if (ImGui::Button("Reset ##country"))
				{
					menus::current_country = "N/A";
				}

				ImGui::NewLine();

				for (const std::string& c : api::all_countries)
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

		if (ImGui::InputText("Search", menus::place_search_buffer, sizeof(place_search_buffer)))
		{
			menus::filtering = true;
			api::filter_place(std::string(menus::place_search_buffer));
		}

		if (ImGui::BeginMenu("Locations"))
		{
			std::string performance_text = logger::va("There are %i places, hidden by default for performance.\nClick the button to show all stations", api::places.size());
			ImVec2 size = { ImGui::CalcTextSize(&performance_text[0]).x, 500};

			if ((!menus::show_all_stations && !menus::filtering) || menus::filtering && api::filtered_places.empty())
			{
				size.y = 75;
			}

			if (ImGui::BeginChild(ImGui::GetID("locations_frame"), size))
			{
				if (api::places_done)
				{
					if (!menus::filtering)
					{
						if (api::places.empty())
						{
							ImGui::Text("Places are empty!\nYou might need to refresh\n\nPlease click Actions -> Refresh Places.");
						}
						else
						{
							if (!menus::show_all_stations)
							{
								ImGui::Text(&performance_text[0]);
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

								for (const place_t& place : api::places)
								{
									if (!menus::current_country.compare("N/A"))
									{
										if (ImGui::Button(&logger::va("[%s] %s", &place.country[0], &place.city[0])[0]))
										{
											api::get_details(place);
											ImGui::CloseCurrentPopup();
										}
									}
									else if (!menus::current_country.compare(place.country))
									{
										if (ImGui::Button(&logger::va("%s", &place.city[0])[0]))
										{
											api::get_details(place);
											ImGui::CloseCurrentPopup();
										}
									}
								}
							}
						}
					}
					else if (menus::filtering)
					{
						if (api::filtered_places.empty())
						{
							ImGui::Text("No results found with the search term\n%s", menus::place_search_buffer);
							ImGui::Text("Tip: Search filters country, city, and place ID; case sensitive");
						}
						else 
						{
							for (const place_t& place : api::filtered_places)
							{
								if (!menus::current_country.compare("N/A"))
								{
									if (ImGui::Button(&logger::va("[%s] %s", &place.country[0], &place.city[0])[0]))
									{
										api::get_details(place);
										ImGui::CloseCurrentPopup();
									}
								}
								else if (!menus::current_country.compare(place.country))
								{
									if (ImGui::Button(&logger::va("%s", &place.city[0])[0]))
									{
										api::get_details(place);
										ImGui::CloseCurrentPopup();
									}
								}
							}
						}
					}
				}
				ImGui::EndChild();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenu();
	}
}

void menus::stations()
{
	static char search_text[64];

	if (ImGui::BeginMenu("Stations"))
	{
		ImGui::InputText("##station_search", menus::station_search_buffer, sizeof(menus::station_search_buffer));
		ImGui::SameLine();
		if (ImGui::Button("Search") && std::strlen(menus::station_search_buffer) > 0)
		{
			api::search_stations(menus::station_search_buffer);
			memset(search_text, 0, sizeof(search_text));
			strcpy(search_text, menus::station_search_buffer);
		}

		if (!api::station_search_results.empty())
		{
			ImGui::SameLine();

			if (ImGui::Button("Clear"))
			{
				api::station_search_results.clear();
				memset(menus::station_search_buffer, 0, sizeof(menus::station_search_buffer));
			}

			ImGui::Text("Search results for %s", search_text);

			for (const station_t& station : api::station_search_results)
			{
				if (ImGui::Button(&logger::va("%s##search", &station.title[0])[0]))
				{
					audio::currently_playing.station.title = station.title;
					audio::currently_playing.station.id = station.id;
					audio::currently_playing.region.city = station.place.city;
					audio::currently_playing.region.country = station.place.country;
					audio::play(station.id);
				}

				if (ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();
					ImGui::Text("%s, %s", &station.place.city[0], &station.place.country[0]);
					ImGui::EndTooltip();
				}


				bool has = false;
				for (const station_t& favorite : api::favorite_stations)
				{
					if (favorite.id == station.id)
					{
						has = true;
						break;
					}
				}

				if (!has)
				{
					ImGui::SameLine();
					if (ImGui::Button(&logger::va("%s##search_%s", &menus::fav_star[0], &station.id[0])[0]))
					{
						api::favorite_stations.emplace_back(station);
						settings::add_favorite(station);
					}
				}
			}
		}

		ImGui::NewLine();

		ImGui::Text("Place Stations:");
		if (api::stations.empty())
		{
			ImGui::Text("Stations are empty!\nYou might need to select a place or try\nsearching all global stations above.");
		}
		else
		{
			for (const station_t& station : api::stations)
			{
				if (ImGui::Button(&logger::va("%s", &station.title[0])[0]))
				{
					audio::currently_playing.station.title = station.title;
					audio::currently_playing.station.id = station.id;
					audio::currently_playing.region.city = station.place.city;
					audio::currently_playing.region.country = station.place.country;
					audio::play(station.id);
				}

				bool has = false;
				for (const station_t& favorite : api::favorite_stations)
				{
					if (favorite.id == station.id)
					{
						has = true;
						break;
					}
				}

				if (!has)
				{
					ImGui::SameLine();
					if (ImGui::Button(&logger::va("%s##%s", &menus::fav_star[0], &station.id[0])[0]))
					{
						api::favorite_stations.emplace_back(station);
						settings::add_favorite(station);
					}
				}
			}
		}
		ImGui::EndMenu();
	}
}

void menus::favorites()
{
	if (ImGui::BeginMenu("Favorites"))
	{
		if (api::favorite_stations.empty())
		{
			ImGui::Text("Favorites are empty!");
			ImGui::Text("Click the [%s] button next to a station!", &menus::fav_star[0]);
		}
		else
		{
			for (const station_t& station : api::favorite_stations)
			{
				if (ImGui::Button(&logger::va("%s##favorite", &station.title[0])[0]))
				{
					audio::currently_playing.station.title = station.title;
					audio::currently_playing.station.id = station.id;
					audio::currently_playing.region.city = station.place.city;
					audio::currently_playing.region.country = station.place.country;
					audio::play(station.id);
				}

				ImGui::SameLine();
				
				if (ImGui::Button(&logger::va("%s##%s", &menus::fav_minus[0],  & station.id[0])[0]))
				{
					for (int i = 0; i < api::favorite_stations.size(); ++i)
					{
						if (api::favorite_stations[i].id == station.id)
						{
							settings::remove_favorite(station);
							api::favorite_stations.erase(api::favorite_stations.begin() + i);
							break;
						}
					}
				}
			}
		}
		ImGui::EndMenu();
	}
}

#ifndef OVERLAY
void menus::overlay()
{
	if (ImGui::BeginMenu("Overlay"))
	{
		ImGui::Text("What is this (?)");
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text(
				"The overlay can be injected into any\n"
				"DirectX (9, 10, 11) or OpenGL3 application\n"
				"and will provide a nearly identical\n"
				"experience as the normal application\n\n"
				"USE AT YOUR OWN RISK\n"
				"DO NOT USE ON ONLINE GAMES"
			);
			ImGui::EndTooltip();
		}

		if (ImGui::Button("Refresh"))
		{
			hook::get_procs();
		}

		ImGui::SameLine();

		if (ImGui::Button(&logger::va("Auto Refresh [%s]", &logger::get_toggle(hook::auto_refresh)[0])[0]))
		{
			hook::auto_refresh = !hook::auto_refresh;
		}
		
		ImGui::NewLine();

		for (const process_t& proc : hook::processes)
		{
			if (ImGui::Button(&logger::va("%s", &proc.title[0], &proc.arch[0])[0]))
			{
				logger::log("HOOK", logger::va("Loading overlay into %s [%u]", &proc.title[0], proc.pid));
				if (hook::load(proc))
				{
					audio::paused = true;
					audio::stop();
				}
			}
			
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Text("[%u] %s", proc.pid, &proc.exe[0]);
				ImGui::EndTooltip();
			}
		}

		ImGui::EndMenu();
	}
}

void menus::render_snow()
{
	std::int32_t gravity = (std::int32_t)std::ceil(global::get_timestep() * 1.0f);
	for (std::int32_t i = 0; i < menus::snow.size(); ++i)
	{
		if (menus::snow[i].y > global::resolution.y)
		{
			menus::snow[i].y = 0;
		}
		else
		{
			menus::snow[i].y += gravity;
		}

		gfx::draw_circle(menus::snow[i], 1.0f, { 255, 255, 255, 255 });
	}
}

void menus::enumerate_snow()
{
	static std::random_device rd;
	static std::mt19937 mt(rd());
	static std::uniform_int_distribution<std::int32_t> x_pos(0, global::resolution.x);
	static std::uniform_int_distribution<std::int32_t> y_pos(0, global::resolution.y);

	for (std::int32_t i = 0; i < menus::max_points; ++i)
	{
		menus::snow.emplace_back(vec2{ x_pos(mt), y_pos(mt) });
	}
}
#endif

void menus::build_font(ImGuiIO& io)
{
	std::string font = fs::get_pref_dir().append("fonts/NotoSans-Regular.ttf");
	std::string font_jp = fs::get_pref_dir().append("fonts/NotoSansJP-Regular.ttf");
	std::string emoji = fs::get_pref_dir().append("fonts/NotoEmoji-Regular.ttf");

	if (fs::exists(font))
	{
		io.Fonts->AddFontFromFileTTF(&font[0], 18.0f);

		static ImFontConfig cfg;
		static ImWchar emoji_ranges[] = { 0x1, 0x1FFFF, 0 };

		if (fs::exists(emoji))
		{
			cfg.MergeMode = true;
			cfg.OversampleH = cfg.OversampleV = 1;
			//cfg.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_LoadColor;	//Noto doesnt have color
			io.Fonts->AddFontFromFileTTF(&emoji[0], 12.0f, &cfg, emoji_ranges);
			menus::fav_star = "\u2795";
			menus::fav_minus = "\u2796";
		}

		if (fs::exists(font_jp))
		{
			ImFontConfig cfg;
			cfg.OversampleH = cfg.OversampleV = 1;
			cfg.MergeMode = true;
			io.Fonts->AddFontFromFileTTF(&font_jp[0], 18.0f, &cfg, io.Fonts->GetGlyphRangesJapanese());
		}
	}
}

std::string menus::fav_star = "*";
std::string menus::fav_minus = "-";

#ifndef OVERLAY
std::vector<vec2> menus::snow;
std::int32_t menus::max_points = 255;
bool menus::show_snow;
#endif

bool menus::show_all_stations = false;
bool menus::show_drpc;

bool menus::filtering = false;
char menus::place_search_buffer[64];
char menus::station_search_buffer[64];
std::string menus::current_country = "N/A";