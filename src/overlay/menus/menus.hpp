#pragma once

class menus
{
public:
	static void update();

	static bool show_all_stations;
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
};