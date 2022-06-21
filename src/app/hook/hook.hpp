#pragma once

#include <psapi.h>

#include "global.hpp"

class hook
{
public:
	static std::vector<process_t> processes;
	static bool load(process_t proc);
	static void get_procs();
	static bool auto_refresh;
	static std::vector<std::string> auto_hook;

private:
	static int CALLBACK get_window(HWND hWnd, LPARAM lparam);
	static std::initializer_list<std::string> blacklist;
	static std::initializer_list<std::string> dlls;
	static std::vector<process_t> injected_apps;
};