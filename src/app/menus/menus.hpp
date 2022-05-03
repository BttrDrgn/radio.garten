#pragma once

class menus
{
public:
	static void prepare();
	static void present();
	static void cleanup();
	static void init();
	static void update();

	static std::string current_country;
private:
	static void main_menu_bar();
	static void actions();
	static void places();
	static void stations();
};