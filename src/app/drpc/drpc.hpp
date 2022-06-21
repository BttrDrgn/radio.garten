#pragma once

#include <chrono>
using namespace std::literals;

#include <discord.h>

class drpc final
{
public:
	static void init();
	static void deinit();

	static void update();

	static void change_status(const std::string& text);
	static void change_details(const std::string& text);
	static void change_large_image(const char* image, const char* text);
	static void change_small_image(const std::string& image, const std::string& text);

	static bool initalized;
};
