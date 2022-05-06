#pragma once

//System
#include <vector>

//Deps
//This HAS to be called before SDL2 or else it throws errors for Linux building
#include <httplib.h>

#include <SDL.h>
#include <SDL_syswm.h>

#include <imgui_freetype.h>

#include <backends/imgui_impl_sdl.h>
#include <backends/imgui_impl_sdlrenderer.h>

#include <nlohmann/json.hpp>	

#include <bass.h>

namespace nl = nlohmann;

#ifdef _WIN32
#define __FUNCNAME__ __FUNCSIG__
#else
#define __FUNCNAME__ __PRETTY_FUNCTION__
#endif