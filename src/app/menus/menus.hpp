#pragma once

//Could maybe do some fancy stuff with this later

class menus
{
public:
	static void update();

	static void init();
	static void prepare();
	static void present();
	static void cleanup();

#ifndef OVERLAY
	static std::vector<vec2> snow;
	static std::int32_t max_points;
	static bool show_snow;

	static void render_snow();
	static void enumerate_snow();
#endif

	static bool show_all_stations;
	static bool show_drpc;

	static char place_search_buffer[64];
	static char station_search_buffer[64];
	static std::string current_country;

	static std::string currently_playing;

private:
	static void build_font(ImGuiIO& io);
	static std::string fav_star;
	static std::string fav_minus;

	static void main_menu_bar();
	static void actions();
	static void places();
	static void stations();
	static void favorites();
	static bool filtering;

#ifndef OVERLAY
	static void overlay();
#endif
};