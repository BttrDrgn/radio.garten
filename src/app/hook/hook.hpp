#pragma once

#include "global.hpp"

class hook
{
public:
	static std::vector<process_t> processes;
	static void load(process_t proc);
	static void get_procs();

private:
	static int CALLBACK get_window(HWND hWnd, LPARAM lparam);
	static std::vector<std::string> blacklist;
	static std::vector<std::string> dlls;
};