#pragma once

#ifndef HELPER
#include <SDL.h>
#endif

#include <fstream>
#include <sstream>
#include <filesystem>

class fs
{
public:
	//Copies important files to the pref directory
	static void init()
	{
		if (fs::exists("fonts"))
		{
			fs::move(fs::get_cur_dir().append("fonts"), fs::get_pref_dir().append("fonts"));
			fs::del("fonts");
		}
	}

	static bool exists(const std::string& path)
	{
		return std::filesystem::exists(path);
	}

	static std::string get_cur_dir()
	{
		return std::filesystem::current_path().string() + "\\";
	}

	static std::string get_pref_dir()
	{
#ifndef HELPER
		return std::string(SDL_GetPrefPath("BttrDrgn", "radio.garten"));
#endif
		return "";
	}

	static void write(const std::string& path, const std::string& contents, const bool append)
	{
		std::ofstream stream(path, std::ios::binary | std::ofstream::out | (append ? std::ofstream::app : 0));

		if (stream.is_open())
		{
			stream.write(contents.data(), static_cast<std::streamsize>(contents.size()));
			stream.close();
		}
	}

	static std::string read(const std::string& path)
	{
		std::ifstream in(path);
		std::ostringstream out;
		out << in.rdbuf();
		return out.str();
	}

	static void del(const std::string& path)
	{
		std::filesystem::remove(path);
	}

	static void move(const std::string& path, const std::string& new_path, bool create_root = true)
	{
		if (create_root)
		{
			std::filesystem::create_directory(new_path);
		}

		for (std::filesystem::path p : std::filesystem::directory_iterator(path))
		{
			std::filesystem::path dest = new_path / p.filename();

			if (std::filesystem::is_directory(p))
			{
				std::filesystem::create_directory(dest);
				move(p.string().c_str(), dest.string().c_str(), false);
			}
			else
			{
				std::filesystem::rename(p, dest);
			}
		}
	}
};