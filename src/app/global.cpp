#include "global.hpp"

bool global::shutdown = false;
HMODULE global::self;
bool global::sys_init = false;
game_t global::game = game_t::UNIVERSAL;
bool global::hide = true;
HWND global::hwnd;
kiero::RenderType::Enum global::renderer;

std::vector<std::string> global::game_bins
{
	"SPEED.exe",	//NFSU
	"SPEED2.exe",	//NFSU2
	"NFSC.exe",		//NFSC
};
