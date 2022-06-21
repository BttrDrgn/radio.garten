#pragma once

#include <ini_rw.h>

class settings
{
public:
	static void init();
	static void update();
	static bool get_boolean(const char* bool_text);
	
private:
	static std::string config_file;
};
