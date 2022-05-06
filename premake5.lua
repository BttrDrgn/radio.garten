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
		defines "DEBUG"
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
		
		--Windows (Visual Studio)
		filter "action:vs*"

			dependson {
				"Discord",
				"ImGui",
			}

			links {
				"imgui",
				"SDL2",
				"SDL2main",
				"bass",
				"Discord",
				"discord_game_sdk.dll.lib",
			}

			includedirs {
				"./src/app/",
				"./src/utils/",
				"./deps/json/include/",
				"./deps/cpp-httplib/",
				"./deps/imgui/backends/",
				"./deps/discord/cpp/",
				"./deps/bass/Win32/c/",
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
	
	project "ImGui"
		targetname "imgui"

		language "c++"
		kind "staticlib"

		links {
			"SDL2",
		}
		
		files {
			"./deps/imgui/*.h",
			"./deps/imgui/*.cpp",
			"./deps/imgui/backends/imgui_impl_sdl.h",
			"./deps/imgui/backends/imgui_impl_sdl.cpp",
			"./deps/imgui/backends/imgui_impl_sdlrenderer.h",
			"./deps/imgui/backends/imgui_impl_sdlrenderer.cpp",
		}

		includedirs {
			"./deps/imgui/",
			"./deps/imgui/backends/",
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