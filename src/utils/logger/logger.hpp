#pragma once

#include <cstdio>

class logger
{
public:
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

	//https://stackoverflow.com/questions/10178700/c-strip-non-ascii-characters-from-string
	static bool check_invalid(char c)
	{
		return !(c >= 0 && c < 128);
	}

	static void strip_unicode(std::string str)
	{
		str.erase(remove_if(str.begin(), str.end(), check_invalid), str.end());
	}
};