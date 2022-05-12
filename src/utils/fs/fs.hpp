#pragma once

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
		std::ofstream ofs(path);
		ofs << contents;
		ofs.close();
	}
};