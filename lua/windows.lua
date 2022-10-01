workspace "Radio.Garten"
	location "../build/"
	targetdir "%{wks.location}/bin/%{cfg.buildcfg}-%{cfg.platform}/"
	objdir "%{wks.location}/obj/%{prj.name}/%{cfg.buildcfg}-%{cfg.platform}/"
	buildlog "%{wks.location}/obj/%{cfg.platform}/%{cfg.buildcfg}-%{prj.name}.log"

	largeaddressaware "on"
	editandcontinue "off"
	staticruntime "on"

	systemversion "latest"
	characterset "unicode"
	warnings "extra"

	includedirs {
		"../deps/imgui/",
	}

	flags {
		"shadowedvariables",
		"undefinedidentifiers",
	}

	flags {
		"multiprocessorcompile",
	}

	platforms {
		"Win-x86",
		"Win-x64",
	}

	configurations {
		"Release",
		"Debug",
	}

	defines {
		"_SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING",
	}
	
	includedirs {
		"../deps/SDL2/include/",
	}

	--x86
	filter "platforms:Win-x86"	
		architecture "x86"

		syslibdirs {
			"../deps/bass/Win32/c/",
			"../deps/discord/lib/x86/",
			"../deps/SDL/lib/x86/",
		}

		postbuildcommands {
			"IF NOT EXIST \"$(OutDir)\"x86 mkdir \"$(OutDir)x86\\",

			"IF NOT EXIST \"$(OutDir)\"x86\\bass.dll copy /y \"..\\deps\\bass\\Win32\\bass.dll\" \"$(OutDir)\"x86\\",
			"IF NOT EXIST \"$(OutDir)\"x86\\discord_game_sdk.dll copy /y \"..\\deps\\discord\\lib\\x86\\discord_game_sdk.dll\" \"$(OutDir)\"\\x86\\",
			"IF NOT EXIST \"$(OutDir)\"x86\\SDL2.dll copy /y \"..\\deps\\SDL\\lib\\x86\\SDL2.dll\" \"$(OutDir)\"\\x86\\",
		}
	--end

	--x64
	filter "platforms:Win-x64"
		architecture "x86_64"

		syslibdirs {
			"../deps/bass/Win32/c/x64/",
			"../deps/discord/lib/x86_64/",
			"../deps/SDL/lib/x64/",
		}

		postbuildcommands {
			"IF NOT EXIST \"$(OutDir)\"x86 mkdir \"$(OutDir)x86\\",
			"IF NOT EXIST \"$(OutDir)\"x86_64 mkdir \"$(OutDir)x86_64\\",

			"IF NOT EXIST \"$(OutDir)\"x86\\bass.dll copy /y \"..\\deps\\bass\\Win32\\bass.dll\" \"$(OutDir)\"x86\\",
			"IF NOT EXIST \"$(OutDir)\"x86\\discord_game_sdk.dll copy /y \"..\\deps\\discord\\lib\\x86\\discord_game_sdk.dll\" \"$(OutDir)\"\\x86\\",
			"IF NOT EXIST \"$(OutDir)\"x86\\SDL2.dll copy /y \"..\\deps\\SDL\\lib\\x86\\SDL2.dll\" \"$(OutDir)\"\\x86\\",

			"IF NOT EXIST \"$(OutDir)\"x86_64\\bass.dll copy /y \"..\\deps\\bass\\Win32\\x64\\bass.dll\" \"$(OutDir)\"\\x86_64\\",
			"IF NOT EXIST \"$(OutDir)\"x86_64\\discord_game_sdk.dll copy /y \"..\\deps\\discord\\lib\\x86_64\\discord_game_sdk.dll\" \"$(OutDir)\"x86_64\\",
			"IF NOT EXIST \"$(OutDir)\"x86_64\\SDL2.dll copy /y \"..\\deps\\SDL\\lib\\x64\\SDL2.dll\" \"$(OutDir)\"\\x86_64\\",
		}
	--end

	filter "Release"
		defines "NDEBUG"
		optimize "full"
		runtime "release"
		symbols "off"

	filter "Debug"
		defines {
			"DEBUG",

			--FT2 uses _DEBUG rather than just DEBUG
			"_DEBUG",
			"FT_DEBUG_LEVEL_ERROR",
			"FT_DEBUG_LEVEL_TRACE",
		}
		optimize "debug"
		runtime "debug"
		symbols "on"

	project "App"
		targetname "radio.garten"
		language "c++"
		cppdialect "c++17"
		kind "windowedapp"
		warnings "off"

		pchheader "stdafx.hpp"
		pchsource "../src/app/stdafx.cpp"
		forceincludes "stdafx.hpp"

		defines {
			"IMGUI_USER_CONFIG=\"menus/config.h\""
		}

		dependson {
			"Discord",
			"ImGui",
			"FT2",
			"ini_rw",
		}

		links {
			"delayimp",
			"imgui",
			"SDL2",
			"bass",
			"Discord",
			"ft2",
			"discord_game_sdk.dll.lib",
			"ini_rw",
		}

		includedirs {
			"../src/app/",
			"../src/utils/",
			"../deps/json/include/",
			"../deps/cpp-httplib/",
			"../deps/imgui/backends/",
			"../deps/imgui/misc/freetype/",
			"../deps/SDL/include/",
			"../deps/discord/cpp/",
			"../deps/bass/Win32/c/",
			"../deps/freetype-2.12.1/include/",
			"../deps/ini_rw/src/",
		}

		files {
			"../src/app/stdafx.hpp",
			"../src/app/stdafx.cpp",
			"../src/app/main.cpp",
			"../src/app/global.hpp",
			"../src/app/global.cpp",
			"../src/app/api/**",
			"../src/app/hook/hook.*",
			"../src/app/audio/**",
			"../src/app/gfx/**",
			"../src/app/input/**",
			"../src/app/menus/**",
			"../src/app/window/**",
			"../src/app/settings/**",

			"../src/utils/fs/**",
			"../src/utils/logger/**",

			"../src/app/resource/**",
			"../src/app/drpc/**",
			"../deps/bass/Win32/c/bass.h",
		}

		linkoptions{
			"/DELAYLOAD:bass.dll",
			"/DELAYLOAD:discord_game_sdk.dll",
			"/DELAYLOAD:SDL2.dll",
		}

	project "Overlay"
		targetname "%{cfg.architecture}/overlay.radio.garten.%{cfg.architecture}"
		language "c++"
		cppdialect "c++17"
		kind "sharedlib"
		warnings "off"

		defines "OVERLAY"

		pchheader "stdafx.hpp"
		pchsource "../src/app/stdafx.cpp"
		forceincludes "stdafx.hpp"

		defines {
			"IMGUI_USER_CONFIG=\"../app/menus/config.h\""
		}

		dependson {
			"SDL2",
			"Discord",
			"ImGui",
			"FT2",
			"ini_rw",
			"MinHook",
		}

		links {
			"d3d9",
			"d3d10",
			"d3d11",

			"SDL2",
			"imgui",
			"ft2",
			"bass",
			"Discord",
			"discord_game_sdk.dll.lib",
			"ini_rw",
			"MinHook",
		}

		includedirs {
			"../src/app/",
			"../src/utils/",
			"../deps/json/include/",
			"../deps/cpp-httplib/",
			"../deps/imgui/backends/",
			"../deps/imgui/misc/freetype/",
			"../deps/discord/cpp/",
			"../deps/bass/Win32/c/",
			"../deps/ini_rw/src/",
			"../deps/kiero/",
			"../deps/minhook/include/",
			"../deps/freetype-2.12.1/include/",
			"../deps/SDL/include/",
		}

		files {
			"../src/app/stdafx.**",
			"../src/app/main.cpp",
			"../src/app/global.**",
			"../src/app/hook/impl/**",
			"../src/app/api/**",
			"../src/app/menus/**",
			"../src/app/input/**",
			"../src/app/audio/**",
			"../src/app/settings/**",
			"../src/app/drpc/**",

			"../src/utils/fs/**",
			"../src/utils/logger/**",

			"../deps/bass/Win32/c/bass.h",

			"../deps/kiero/*.h",
			"../deps/kiero/*.cpp",
		}

	project "Helper"
		architecture "x86"
		targetname "x86/helper"
		language "c++"
		cppdialect "c++17"
		kind "windowedapp"
		warnings "off"

		defines {
			"OVERLAY",
			"HELPER",
		}

		pchheader "stdafx.hpp"
		pchsource "../src/helper/stdafx.cpp"
		forceincludes "stdafx.hpp"

		filter "platforms:Win-x64"
			includedirs {
				"../src/helper/",
				"../src/utils/",
			}

		filter "platforms:Win-x64"
			files {
				"../src/helper/stdafx.**",
				"../src/helper/main.**",
			}

	group "Dependencies"

	project "FT2"
		targetname "ft2"

		language "c++"
		kind "staticlib"

		defines {
			"FT2_BUILD_LIBRARY",
		}
	
		files {
			--FT_MODULES
			"../deps/freetype-2.12.1/src/base/ftbbox.c",
			"../deps/freetype-2.12.1/src/base/ftbdf.c",
			"../deps/freetype-2.12.1/src/base/ftbitmap.c",
			"../deps/freetype-2.12.1/src/base/ftcid.c",
			"../deps/freetype-2.12.1/src/base/ftfstype.c",
			"../deps/freetype-2.12.1/src/base/ftgasp.c",
			"../deps/freetype-2.12.1/src/base/ftglyph.c",
			"../deps/freetype-2.12.1/src/base/ftgxval.c",
			"../deps/freetype-2.12.1/src/base/ftmm.c",
			"../deps/freetype-2.12.1/src/base/ftotval.c",
			"../deps/freetype-2.12.1/src/base/ftpatent.c",
			"../deps/freetype-2.12.1/src/base/ftpfr.c",
			"../deps/freetype-2.12.1/src/base/ftstroke.c",
			"../deps/freetype-2.12.1/src/base/ftsynth.c",
			"../deps/freetype-2.12.1/src/base/fttype1.c",
			"../deps/freetype-2.12.1/src/base/ftwinfnt.c",
			"../deps/freetype-2.12.1/src/base/ftbase.c",
			"../deps/freetype-2.12.1/src/base/ftdebug.c",
			"../deps/freetype-2.12.1/src/base/ftinit.c",
			"../deps/freetype-2.12.1/src/base/ftsystem.c",
			"../deps/freetype-2.12.1/src/base/ftver.rc",
			
			--root
			"../deps/freetype-2.12.1/src/autofit/autofit.c",
			"../deps/freetype-2.12.1/src/bdf/bdf.c",
			"../deps/freetype-2.12.1/src/cff/cff.c",
			"../deps/freetype-2.12.1/src/dlg/dlgwrap.c",
			"../deps/freetype-2.12.1/src/cache/ftcache.c",
			"../deps/freetype-2.12.1/src/gzip/ftgzip.c",
			"../deps/freetype-2.12.1/src/lzw/ftlzw.c",
			"../deps/freetype-2.12.1/src/pcf/pcf.c",
			"../deps/freetype-2.12.1/src/pfr/pfr.c",
			"../deps/freetype-2.12.1/src/psaux/psaux.c",
			"../deps/freetype-2.12.1/src/pshinter/pshinter.c",
			"../deps/freetype-2.12.1/src/psnames/psmodule.c",
			"../deps/freetype-2.12.1/src/raster/raster.c",
			"../deps/freetype-2.12.1/src/sdf/sdf.c",
			"../deps/freetype-2.12.1/src/sfnt/sfnt.c",
			"../deps/freetype-2.12.1/src/smooth/smooth.c",
			"../deps/freetype-2.12.1/src/svg/svg.c",
			"../deps/freetype-2.12.1/src/truetype/truetype.c",
			"../deps/freetype-2.12.1/src/type1/type1.c",
			"../deps/freetype-2.12.1/src/cid/type1cid.c",
			"../deps/freetype-2.12.1/src/type42/type42.c",
			"../deps/freetype-2.12.1/src/winfonts/winfnt.c",
		}

		includedirs {
			"../deps/freetype-2.12.1/include/",
		}

	project "ImGui"
		targetname "imgui"

		language "c++"
		kind "staticlib"

		dependson {
			"FT2",
		}

		defines {
			"IMGUI_USER_CONFIG=\"../src/app/menus/config.h\""
		}
	
		links {
			"d3d9",
			"d3d10",
			"d3d11",

			"SDL2",
			"ft2",
		}
		
		files {
			"../deps/imgui/*.h",
			"../deps/imgui/*.cpp",

			"../deps/imgui/backends/imgui_impl_sdl.*",
			"../deps/imgui/backends/imgui_impl_win32.*",

			"../deps/imgui/backends/imgui_impl_sdlrenderer.*",
			"../deps/imgui/backends/imgui_impl_dx9.*",
			"../deps/imgui/backends/imgui_impl_dx10.*",
			"../deps/imgui/backends/imgui_impl_dx11.*",
			"../deps/imgui/backends/imgui_impl_opengl3.*",

			"../deps/imgui/misc/freetype/imgui_freetype.*",
		}

		includedirs {
			"../src/",
			"../deps/imgui/",
			"../deps/imgui/backends/",
			"../deps/imgui/misc/freetype/",
			"../deps/SDL/include/",
			"../deps/freetype-2.12.1/include/",
		}
	
	project "Discord"
		targetname "discord"

		language "c++"
		kind "staticlib"

		files {
			"../deps/discord/cpp/**",
		}

		includedirs {
			"../deps/discord/cpp/",
		}

	project "ini_rw"
		language "c"
		kind "staticlib"

		files {
			"../deps/ini_rw/src/**",
		}

		includedirs {
			"../deps/ini_rw/src/",
		}

	project "MinHook"
		targetname "MinHook"

		language "c++"
		kind "staticlib"

		files {
			"../deps/minhook/src/**",
		}

		includedirs {
			"../deps/minhook/include/",
		}