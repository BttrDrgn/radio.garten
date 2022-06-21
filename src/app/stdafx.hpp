#pragma once

//System
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <thread>
#include <unordered_map>
#include <regex>

#include <Windows.h>
#include <DbgHelp.h>

//Deps
#include <imgui_freetype.h>

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

#include <bass.h>


#define __fartcall __fastcall
