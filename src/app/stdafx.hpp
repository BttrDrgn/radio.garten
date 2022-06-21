#pragma once

//System
#include <vector>

//Deps
//This HAS to be called before SDL2 or else it throws errors for Linux building
#include <httplib.h>

#include <imgui_freetype.h>

#ifndef OVERLAY
#include <SDL.h>
#include <SDL_syswm.h>

#include <backends/imgui_impl_sdl.h>
#include <backends/imgui_impl_sdlrenderer.h>
#else
#include <MinHook.h>
#include <kiero.h>

#define KIERO_INCLUDE_D3D9 1
#define KIERO_INCLUDE_D3D10 1
#define KIERO_INCLUDE_D3D11 1
#define KIERO_INCLUDE_OPENGL 1
#define KIERO_USE_MINHOOK 1

#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_dx9.h>
#include <backends/imgui_impl_dx10.h>
#include <backends/imgui_impl_dx11.h>
#include <backends/imgui_impl_opengl3.h>
#endif

#include <nlohmann/json.hpp>	

#include <bass.h>

namespace nl = nlohmann;