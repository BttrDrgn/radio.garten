#pragma once

#include <fstream>
#include <sstream>
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
		return std::filesystem::current_path().string() + "\\";
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

	static void del(const std::string& path, bool folder = false)
	{
		if (!fs::exists(path)) return;

		switch (folder)
		{
		case false:
			std::filesystem::remove(path);
			break;
		case true:
			std::filesystem::remove_all(path);
			break;
		}
	}

	static void move(const std::string& path, const std::string& new_path, bool create_root = true)
	{
		if (create_root)
		{
			std::filesystem::create_directory(new_path);
		}

		for (const std::filesystem::path& p : std::filesystem::directory_iterator(path))
		{
			std::filesystem::path dest = new_path / p.filename();

			if (fs::exists(path))
			{
				if (std::filesystem::is_directory(p))
				{
					std::filesystem::create_directory(dest);
					fs::move(&p.string()[0], &dest.string()[0], false);
				}
				else
				{
					std::filesystem::rename(p, dest);
				}
			}
		}
	}

	static std::vector<std::string> get_all_files(const std::string& path, const std::initializer_list<std::string>& exts)
	{
		std::vector<std::string> retn;

		if (std::filesystem::exists(path) && std::filesystem::is_directory(path))
		{
			for (const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(path))
			{
				if (std::filesystem::is_regular_file(entry))
				{
					//Check if we got this extension
					bool found = false;
					for (const std::string& ext : exts)
					{
						if (entry.path().extension().compare(ext))
						{
							found = true;
							break;
						}
					}

					//Oh no we didn't :(
					if (!found)
					{
						continue;
					}
					else if (found)	//Oh wait we did ;)
					{
						retn.emplace_back(entry.path().string());
					}
				}
			}
		}

		return retn;
	}
};