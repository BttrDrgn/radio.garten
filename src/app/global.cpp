#include "global.hpp"

bool global::shutdown = false;
bool global::sys_init = false;
game_t global::game = game_t::NFSU2;
bool global::hide = true;
HWND global::hwnd;
kiero::RenderType::Enum global::renderer;