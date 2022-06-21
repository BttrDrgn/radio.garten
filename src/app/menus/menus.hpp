#pragma once

class menus
{
public:
	static void update();

	static void init();
	static void prepare();
	static void present();

	static std::string currently_playing;

private:
	static void build_font(ImGuiIO& io);

	static void main_menu_bar();
	static void actions();
	static void playlist();
};