#pragma once

#include <ini_rw.h>

class settings
{
public:
	static void init();
	static void update();
	static bool get_boolean(const char* bool_text);
	
private:
	static const char* config_file;
	static const char* favorites_file;
	static ini_t* config;
};