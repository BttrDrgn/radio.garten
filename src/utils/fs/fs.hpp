#pragma once

#ifndef HELPER
#include <SDL.h>
#endif

#include <fstream>
#include <filesystem>

class fs
{
public:
	static bool exists(const std::string& path)
	{
		return std::filesystem::exists(path);
	}

	static std::string get_cur_dir()
	{
		return std::filesystem::current_path().string();
	}

	static void write(const std::string& path, const std::string& contents)
	{
#ifndef HELPER
		std::string full_path = logger::va("%s/%s", SDL_GetPrefPath("BttrDrgn", "radio.garten"), path);
#else
		std::string full_path = path;
#endif
		std::ofstream ofs(full_path);
		ofs << contents;
		ofs.close();
	}
};