#pragma once

#include <MinHook.h>

#include "menus/menus.hpp"
#include "audio/audio.hpp"

#include "win32_impl.h"

#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_dx9.h>
#include <backends/imgui_impl_dx10.h>
#include <backends/imgui_impl_dx11.h>
#include <backends/imgui_impl_opengl3.h>

namespace impl
{
	void showExampleWindow(const char* comment);
}