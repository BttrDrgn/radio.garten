workspace "Radio.Garten"
	location "./build/"
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
		"./src/",
		"./deps/imgui/",
	}

	flags {
		"shadowedvariables",
		"undefinedidentifiers",
	}

	--Windows (Visual Studio)
	filter "action:vs*"
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
			"./deps/SDL2-2.0.22/MSVC/include/",
		}

		--x86
		filter "platforms:Win-x86"	
			architecture "x86"

			syslibdirs {
				"./deps/bass/Win32/c/",
				"./deps/SDL2-2.0.22/MSVC/lib/x86/",
				"./deps/discord/lib/x86/",
			}

			postbuildcommands {
				"copy /y \"..\\deps\\SDL2-2.0.22\\MSVC\\lib\\x86\\SDL2.dll\" \"$(OutDir)\"",
				"copy /y \"..\\deps\\bass\\Win32\\bass.dll\" \"$(OutDir)\"",
				"copy /y \"..\\deps\\discord\\lib\\x86\\discord_game_sdk.dll\" \"$(OutDir)\"",
			}
		--end

		--x64
		filter "platforms:Win-x64"
			architecture "x86_64"

			syslibdirs {
				"./deps/bass/Win32/c/x64/",
				"./deps/SDL2-2.0.22/MSVC/lib/x64/",
				"./deps/discord/lib/x86_64/",
			}

			postbuildcommands {
				"copy /y \"..\\deps\\SDL2-2.0.22\\MSVC\\lib\\x64\\SDL2.dll\" \"$(OutDir)\"",
				"copy /y \"..\\deps\\bass\\Win32\\x64\\bass.dll\" \"$(OutDir)\"",
				"copy /y \"..\\deps\\discord\\lib\\x86_64\\discord_game_sdk.dll\" \"$(OutDir)\"",
			}
		--end
	--end


	--Linux (GCC Make)
	filter "action:gmake2"
		platforms {
			"Deb-x64",
		}

		configurations {
			"Release",
			"Debug",
		}

		buildoptions {
			"`sdl2-config --cflags --libs`",
		}

		--x64
		filter "platforms:Deb-x64"
			architecture "x86_64"
		--end
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

		--Windows (Visual Studio)
		filter "action:vs*"

			dependson {
				"Discord",
				"ImGui",
				"FT2",
			}

			links {
				"imgui",
				"SDL2",
				"SDL2main",
				"bass",
				"Discord",
				"ft2",
				"discord_game_sdk.dll.lib",
			}

			includedirs {
				"./src/app/",
				"./src/utils/",
				"./deps/json/include/",
				"./deps/cpp-httplib/",
				"./deps/imgui/backends/",
				"./deps/imgui/misc/freetype/",
				"./deps/discord/cpp/",
				"./deps/bass/Win32/c/",
				"./deps/freetype-2.12.1/include/",
			}

			files {
				"./src/app/stdafx.hpp",
				"./src/app/stdafx.cpp",
				"./src/app/main.cpp",
				"./src/app/global.hpp",
				"./src/app/api/**",
				"./src/app/audio/**",
				"./src/app/gfx/**",
				"./src/app/input/**",
				"./src/app/menus/**",
				"./src/app/window/**",

				"./src/utils/**",

				"./src/app/resource/**",
				"./src/app/drpc/**",
				"./deps/bass/Win32/c/bass.h",
			}
		--end

		--Linux (GCC Make)
		filter "action:gmake*"

			runpathdirs {
				"./bins/",
			}

			syslibdirs {
				"./deps/bass/Linux/x64",
			}

			links {
				"imgui",
				"SDL2",
				"SDL2main",
				"bass",
			}

			includedirs {
				"./src/app/",
				"./src/utils/",
				"./deps/json/include/",
				"./deps/cpp-httplib/",
				"./deps/imgui/backends/",
				"./deps/bass/Linux/",
			}

			files {
				"./src/app/stdafx.hpp",
				"./src/app/stdafx.cpp",
				"./src/app/main.cpp",
				"./src/app/global.hpp",
				"./src/app/api/**",
				"./src/app/audio/**",
				"./src/app/gfx/**",
				"./src/app/input/**",
				"./src/app/menus/**",
				"./src/app/window/**",

				"./src/utils/**",

				"./deps/bass/Linux/bass.h",
			}
		--end

	group "Dependencies"
	
	project "FT2"
		targetname "ft2"

		language "c++"
		kind "staticlib"

		defines {
			"FT2_BUILD_LIBRARY",
		}
		
		--Windows (Visual Studio)
		filter "action:vs*"
			files {
				--FT_MODULES
				"./deps/freetype-2.12.1/src/base/ftbbox.c",
				"./deps/freetype-2.12.1/src/base/ftbdf.c",
				"./deps/freetype-2.12.1/src/base/ftbitmap.c",
				"./deps/freetype-2.12.1/src/base/ftcid.c",
				"./deps/freetype-2.12.1/src/base/ftfstype.c",
				"./deps/freetype-2.12.1/src/base/ftgasp.c",
				"./deps/freetype-2.12.1/src/base/ftglyph.c",
				"./deps/freetype-2.12.1/src/base/ftgxval.c",
				"./deps/freetype-2.12.1/src/base/ftmm.c",
				"./deps/freetype-2.12.1/src/base/ftotval.c",
				"./deps/freetype-2.12.1/src/base/ftpatent.c",
				"./deps/freetype-2.12.1/src/base/ftpfr.c",
				"./deps/freetype-2.12.1/src/base/ftstroke.c",
				"./deps/freetype-2.12.1/src/base/ftsynth.c",
				"./deps/freetype-2.12.1/src/base/fttype1.c",
				"./deps/freetype-2.12.1/src/base/ftwinfnt.c",
				"./deps/freetype-2.12.1/src/base/ftbase.c",
				"./deps/freetype-2.12.1/src/base/ftdebug.c",
				"./deps/freetype-2.12.1/src/base/ftinit.c",
				"./deps/freetype-2.12.1/src/base/ftsystem.c",
				"./deps/freetype-2.12.1/src/base/ftver.rc",
				
				--root
				"./deps/freetype-2.12.1/src/autofit/autofit.c",
				"./deps/freetype-2.12.1/src/bdf/bdf.c",
				"./deps/freetype-2.12.1/src/cff/cff.c",
				"./deps/freetype-2.12.1/src/dlg/dlgwrap.c",
				"./deps/freetype-2.12.1/src/cache/ftcache.c",
				"./deps/freetype-2.12.1/src/gzip/ftgzip.c",
				"./deps/freetype-2.12.1/src/lzw/ftlzw.c",
				"./deps/freetype-2.12.1/src/pcf/pcf.c",
				"./deps/freetype-2.12.1/src/pfr/pfr.c",
				"./deps/freetype-2.12.1/src/psaux/psaux.c",
				"./deps/freetype-2.12.1/src/pshinter/pshinter.c",
				"./deps/freetype-2.12.1/src/psnames/psmodule.c",
				"./deps/freetype-2.12.1/src/raster/raster.c",
				"./deps/freetype-2.12.1/src/sdf/sdf.c",
				"./deps/freetype-2.12.1/src/sfnt/sfnt.c",
				"./deps/freetype-2.12.1/src/smooth/smooth.c",
				"./deps/freetype-2.12.1/src/svg/svg.c",
				"./deps/freetype-2.12.1/src/truetype/truetype.c",
				"./deps/freetype-2.12.1/src/type1/type1.c",
				"./deps/freetype-2.12.1/src/cid/type1cid.c",
				"./deps/freetype-2.12.1/src/type42/type42.c",
				"./deps/freetype-2.12.1/src/winfonts/winfnt.c",
			}
		--end

		includedirs {
			"./deps/freetype-2.12.1/include/",
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
			"./deps/imgui/*.h",
			"./deps/imgui/*.cpp",
			"./deps/imgui/backends/imgui_impl_sdl.h",
			"./deps/imgui/backends/imgui_impl_sdl.cpp",
			"./deps/imgui/backends/imgui_impl_sdlrenderer.h",
			"./deps/imgui/backends/imgui_impl_sdlrenderer.cpp",

			"./deps/imgui/misc/freetype/imgui_freetype.cpp",
			"./deps/imgui/misc/freetype/imgui_freetype.h",
		}

		includedirs {
			"./deps/imgui/",
			"./deps/imgui/backends/",
			"./deps/imgui/misc/freetype/",
			"./deps/freetype-2.12.1/include/",
		}
	
	--Windows (Visual Studip)
	filter "action:vs*"
		project "Discord"
			targetname "discord"

			language "c++"
			kind "staticlib"

			files {
				"./deps/discord/cpp/**",
			}

			includedirs {
				"./deps/discord/cpp/",
			}
	--end