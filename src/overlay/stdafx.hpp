#pragma once

//System
#include <vector>

//Deps
#include <httplib.h>

#include <imgui.h>

//For G-API hooks
#include <kiero.h>

//Enable D3D9-11 and MinHook
#define KIERO_INCLUDE_D3D9 1
#define KIERO_INCLUDE_D3D10 1
#define KIERO_INCLUDE_D3D11 1
#define KIERO_INCLUDE_OPENGL 1
#define KIERO_USE_MINHOOK 1

#include <nlohmann/json.hpp>	

#include <bass.h>

namespace nl = nlohmann;

#ifdef _WIN32
#define __FUNCNAME__ __FUNCSIG__
#else
#define __FUNCNAME__ __PRETTY_FUNCTION__
#endif