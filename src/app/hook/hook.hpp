#pragma once

#include <psapi.h>

#include "global.hpp"

class hook
{
public:
	static std::vector<process_t> processes;
	static bool load(process_t proc);
	static void get_procs();
	static bool auto_refresh;
	static std::vector<std::string> auto_hook;

	static void SummonChyron(const char* title, const char* artist, const char* album)
	{
		switch (global::game)
		{
		case game_t::NFSU2:
			char* v3; // esi
			int v4; // eax

			v3 = "Chyron_FE.fng";

			if (*(int*)(0x008654A4) != 3)
			{
				v3 = "Chyron_IG.fng";
			}

			if (!((bool(*)(char*))0x0052CF60)(v3))
			{
				((void(*)(int, char*, int, int, int))0x00555F50)(14, v3, 0, 0, 0);
			}

			v4 = ((int(*)(char*))0x0051BD10)(v3);
			if (v4)
			{
				reinterpret_cast<void(__thiscall*)(int, const char*, const char*, const char*)>(0x004AC880)(v4, title, artist, album);
			}
			break;
		}
	}

	template <typename T> static void jump(std::uint32_t address, T function)
	{
		*(std::uint8_t*)(address) = 0xE9;
		*(std::uint32_t*)(address + 1) = (std::uint32_t(function) - address - 5);
	};

	template <typename T> static void retn_value(std::uint32_t address, T value)
	{
		*(std::uint8_t*)(address) = 0xB8;
		*(std::uint32_t*)(address + 1) = std::uint32_t(value);
		*(std::uint8_t*)(address + 5) = 0xC3;
	}

	static void retn(std::uint32_t address)
	{
		*(std::uint8_t*)(address) = 0xC3;
	};

private:
	static int CALLBACK get_window(HWND hWnd, LPARAM lparam);
	static std::initializer_list<std::string> blacklist;
	static std::initializer_list<std::string> dlls;
	static std::vector<process_t> injected_apps;
};