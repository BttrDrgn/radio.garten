workspace "Radio.Garten"
	location ".\\build\\"
	targetdir "%{wks.location}\\bin\\%{cfg.buildcfg}-%{cfg.platform}\\"
	objdir "%{wks.location}\\obj\\%{prj.name}\\%{cfg.buildcfg}-%{cfg.platform}\\"
	buildlog "%{wks.location}\\obj\\%{cfg.platform}\\%{cfg.buildcfg}-%{prj.name}.log"

	largeaddressaware "on"
	editandcontinue "off"
	staticruntime "on"

	systemversion "latest"
	characterset "unicode"
	warnings "extra"

	includedirs {
		".\\src\\",
		".\\deps\\imgui\\",
	}

	flags {
		"shadowedvariables",
		"undefinedidentifiers",
	}

	if string.contains(_ACTION, "vs") then
		printf("Building for Windows in Visual Studio..");

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
			".\\deps\\SDL2-2.0.22\\MSVC\\include\\",
		}

		filter "platforms:Win-x86"	
			architecture "x86"

			syslibdirs {
				".\\deps\\bass\\c\\",
				".\\deps\\SDL2-2.0.22\\MSVC\\lib\\x86\\",
				".\\deps\\discord\\lib\\x86\\",
			}

			postbuildcommands {
				"copy /y \"..\\deps\\SDL2-2.0.22\\MSVC\\lib\\x86\\SDL2.dll\" \"$(OutDir)\"",
				"copy /y \"..\\deps\\bass\\bass.dll\" \"$(OutDir)\"",
				"copy /y \"..\\deps\\discord\\lib\\x86\\discord_game_sdk.dll\" \"$(OutDir)\"",
			}
		filter ""


		filter "platforms:Win-x64"
			architecture "x86_64"

			syslibdirs {
				".\\deps\\bass\\c\\x64\\",
				".\\deps\\SDL2-2.0.22\\MSVC\\lib\\x64\\",
				".\\deps\\discord\\lib\\x86_64\\",
			}

			postbuildcommands {
				"copy /y \"..\\deps\\SDL2-2.0.22\\MSVC\\lib\\x64\\SDL2.dll\" \"$(OutDir)\"",
				"copy /y \"..\\deps\\bass\\x64\\bass.dll\" \"$(OutDir)\"",
				"copy /y \"..\\deps\\discord\\lib\\x86_64\\discord_game_sdk.dll\" \"$(OutDir)\"",
			}
		filter ""
	end

	filter "Release"
		defines "NDEBUG"
		optimize "full"
		runtime "release"
		symbols "off"
	filter ""

	filter "Debug"
		defines "DEBUG"
		optimize "debug"
		runtime "debug"
		symbols "on"
	filter ""

	project "App"
		targetname "radio.garten"
		language "c++"
		kind "consoleapp"
		warnings "off"

		pchheader "stdafx.hpp"
		pchsource "src/app/stdafx.cpp"
		forceincludes "stdafx.hpp"
		
		dependson {
			"ImGui",
		}

		if string.contains(_ACTION, "vs") then
			dependson {
				"Discord",
			}
		end

		links {
			"imgui",
			"bass",
			"SDL2",
			"SDL2main",
		}
		
		if string.contains(_ACTION, "vs") then
			links {
				"Discord",
				"discord_game_sdk.dll.lib",
			}
		end

		files {
			".\\src\\app\\stdafx.hpp",
			".\\src\\app\\stdafx.cpp",
			".\\src\\app\\main.cpp",
			".\\src\\app\\global.hpp",
			".\\src\\app\\api\\**",
			".\\src\\app\\audio\\**",
			".\\src\\app\\gfx\\**",
			".\\src\\app\\input\\**",
			".\\src\\app\\menus\\**",
			".\\src\\app\\window\\**",

			".\\src\\utils\\**",

			".\\deps\\bass\\c\\bass.h",
		}

		if string.contains(_ACTION, "vs") then
			files {
				".\\src\\app\\resource\\**",
				".\\src\\app\\drpc\\**",
			}
		end

		includedirs {
			".\\src\\app\\",
			".\\src\\utils\\",
			".\\deps\\json\\include\\",
			".\\deps\\cpp-httplib\\",
			".\\deps\\imgui\\backends\\",
			".\\deps\\bass\\c\\",
		}

		if string.contains(_ACTION, "vs") then
			includedirs {
				".\\deps\\discord\\cpp\\",
			}
		end


	group "Dependencies"
	
	project "ImGui"
		targetname "imgui"

		language "c++"
		kind "staticlib"

		links {
			"SDL2",
		}
		
		files {
			".\\deps\\imgui\\*.h",
			".\\deps\\imgui\\*.cpp",
			".\\deps\\imgui\\backends\\imgui_impl_sdl.h",
			".\\deps\\imgui\\backends\\imgui_impl_sdl.cpp",
			".\\deps\\imgui\\backends\\imgui_impl_sdlrenderer.h",
			".\\deps\\imgui\\backends\\imgui_impl_sdlrenderer.cpp",
		}

		includedirs {
			".\\deps\\imgui\\",
			".\\deps\\imgui\\backends\\",
		}
	
	if string.contains(_ACTION, "vs") then
		project "Discord"
			targetname "discord"

			language "c++"
			kind "staticlib"

			files {
				".\\deps\\discord\\cpp\\**",
			}

			includedirs {
				".\\deps\\discord\\cpp\\",
			}
	end