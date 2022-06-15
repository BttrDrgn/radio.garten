#pragma once

struct process_t
{
	std::string title, arch, exe;
	std::uint32_t pid;
	HWND hwnd;
};

struct vec2
{
	int x, y;
};

enum game_t : int
{
	NFSU = 1,
	NFSU2,
	NFSC = 4,
	NFSPS,
	NFSUC,
};

class global
{
public:
	static bool shutdown;

	static void msg_box(std::string title, std::string message)
	{
		MessageBoxA(global::hwnd, &message[0], &title[0], 0);
	}

	static bool sys_init;
	static game_t game;
	static bool hide;
	static HWND hwnd;
	static kiero::RenderType::Enum renderer;
};