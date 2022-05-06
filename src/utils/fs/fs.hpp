#pragma once

#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201703L) || __cplusplus >= 201703L)
#define CXX 17
#elif ((defined(_MSVC_LANG) && _MSVC_LANG >= 201402L) || __cplusplus >= 201402L)
#define CXX 14
#endif

#if CXX == 14
#include <experimental/filesystem>
#elif CXX >= 17
#include <filesystem>
#endif

class fs
{
public:
	static bool exists(const std::string& path)
	{
#if CXX == 14
		return std::experimental::filesystem::exists(path);
#elif CXX >= 17
		return std::filesystem::exists(path);
#endif
	}
};