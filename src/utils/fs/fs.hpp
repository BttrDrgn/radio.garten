#pragma once

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
};