#pragma once

#include <cstdio>

class logger
{
public:
	static void log(const std::string& type, const std::string& text)
	{
		std::cout << "[ " << type << " ] " << text << std::endl;
	}

	static void log_info(const std::string& text)
	{
		log("INFO", text);
	}

	static void log_error(const std::string& text)
	{
		log("ERROR", text);
	}

	static void log_warning(const std::string& text)
	{
		log("WARNING", text);
	}

	static void log_debug(const std::string& text)
	{
#ifdef DEBUG
		log("DEBUG", text);
#endif
	}

	static std::string va(const char* fmt, ...)
	{
		va_list va;
		va_start(va, fmt);
		char result[512]{};
		std::vsprintf(result, fmt, va);
		return std::string(result);
	}

	static std::string get_toggle(bool input)
	{
		switch (input)
		{
		case true:
			return "On";
			break;

		case false:
			return "Off";
			break;

		default:
			return "???";
			break;
		}
	}

	std::vector<std::string> split(std::string string, const std::string& delimiter)
	{
		std::vector<std::string> out = std::vector<std::string>();
		std::uint32_t pos = 0u;

		while ((pos = string.find(delimiter)) != std::string::npos)
		{
			out.emplace_back(string.substr(0, pos));
			string.erase(0, pos + delimiter.size());
		}

		out.emplace_back(string);

		return out;
	}
};