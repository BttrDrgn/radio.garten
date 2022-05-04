#pragma once

#include <iostream>

#ifdef DEBUG
#define LOG(__FMT__, ...) std::printf("[ INFO ][" __FUNCTION__ "]: " __FMT__ "\n", __VA_ARGS__)
#define LOG_DEBUG(__FMT__, ...) std::printf("[ DEBUG ][" __FUNCTION__ "]: " __FMT__ "\n", __VA_ARGS__)
#define LOG_WARNING(__FMT__, ...) std::printf("[ WARNING ][" __FUNCTION__ "]: " __FMT__ "\n", __VA_ARGS__)
#define LOG_ERROR(__FMT__, ...) std::printf("[ ERROR ][" __FUNCTION__ "]: " __FMT__ "\n", __VA_ARGS__)
#else
#define LOG(__FMT__, ...) std::printf("[ INFO ]: " __FMT__ "\n", __VA_ARGS__)
#define LOG_DEBUG(__FMT__, ...)
#define LOG_WARNING(__FMT__, ...) std::printf("[ WARNING ]: " __FMT__ "\n", __VA_ARGS__)
#define LOG_ERROR(__FMT__, ...) std::printf("[ ERROR ]: " __FMT__ "\n", __VA_ARGS__)
#endif

class logger
{
public:
	static std::string va(const char* fmt, ...)
	{
		char* va = ::va_list();
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