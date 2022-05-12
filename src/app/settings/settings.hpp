#pragma once

#include <ini_rw.h>

#include "api/api.hpp"

class settings
{
public:
	static void init();
	static void update();
	static void favorites_update();
	static bool get_boolean(const char* bool_text);
	static void add_favorite(station_t station);
	static void remove_favorite(station_t station);
	
private:
	static std::string config_file;
	static std::string favorites_file;
	static ini_t* config;
};