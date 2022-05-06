#pragma once

//Could maybe do some fancy stuff with this later
struct snow_t
{
	SDL_Point pos;
};

class menus
{
public:
	static void prepare();
	static void present();
	static void cleanup();
	static void init();
	static void update();

	static std::vector<snow_t> snow;
	static std::int32_t max_points;

	static bool show_all_stations;
	static bool show_snow;
	static bool show_drpc;

	static char search_buffer[64];

	static std::string currently_playing;

private:
	static void build_font(ImGuiIO& io);

	static void main_menu_bar();
	static void actions();
	static void places();
	static void stations();
	static void favorites();
	static bool filtering;

	static void render_snow();
	static void enumerate_snow();
};