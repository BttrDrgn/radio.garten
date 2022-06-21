--May not compile at the moment

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
		"../src/",
		"../deps/imgui/",
	}

	flags {
		"shadowedvariables",
		"undefinedidentifiers",
	}

	platforms {
		"Lin-x64",
	}

	configurations {
		"Release",
		"Debug",
	}

	buildoptions {
		"`sdl2-config --cflags --libs`",
	}

	--x64
	filter "platforms:Lin-x64"
		architecture "x86_64"
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
		kind "consoleapp"
		warnings "off"

		pchheader "stdafx.hpp"
		pchsource "src/app/stdafx.cpp"
		forceincludes "stdafx.hpp"

		defines {
			"IMGUI_USER_CONFIG=\"menus/config.h\""
		}

		dependson {
			"ImGui",
			"FT2",
			"SDL2",
			"SDL2main"
		}

	
		runpathdirs {
			"../bins/",
		}

		syslibdirs {
			"../deps/bass/Linux/x64",
		}

		links {
			"imgui",
			"SDL2",
			"SDL2main",
			"bass",
		}

		includedirs {
			"../src/app/",
			"../src/utils/",
			"../deps/json/include/",
			"../deps/cpp-httplib/",
			"../deps/imgui/backends/",
			"../deps/SDL/include/",
			"../deps/bass/Linux/",
		}

		files {
			"../src/app/stdafx.hpp",
			"../src/app/stdafx.cpp",
			"../src/app/main.cpp",
			"../src/app/global.hpp",
			"../src/app/api/**",
			"../src/app/audio/**",
			"../src/app/gfx/**",
			"../src/app/input/**",
			"../src/app/menus/**",
			"../src/app/window/**",

			"../src/utils/**",

			"../deps/bass/Linux/bass.h",
		}

	group "Dependencies"

	project "FT2"
		targetname "ft2"

		language "c++"
		kind "staticlib"

		defines {
			"FT2_BUILD_LIBRARY",
		}

		--WILL NOT COMPILE, NEEDS FILES

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
			"SDL2",
			"ft2",
		}
		
		files {
			"../deps/imgui/*.h",
			"../deps/imgui/*.cpp",
			"../deps/imgui/backends/imgui_impl_sdl.h",
			"../deps/imgui/backends/imgui_impl_sdl.cpp",
			"../deps/imgui/backends/imgui_impl_sdlrenderer.h",
			"../deps/imgui/backends/imgui_impl_sdlrenderer.cpp",

			"../deps/imgui/misc/freetype/imgui_freetype.cpp",
			"../deps/imgui/misc/freetype/imgui_freetype.h",
		}

		includedirs {
			"../deps/imgui/",
			"../deps/imgui/backends/",
			"../deps/imgui/misc/freetype/",
			"../deps/SDL/include/",
			"../deps/freetype-2.12.1/include/",
		}