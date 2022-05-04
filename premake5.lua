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

	buildoptions {
		"/Zm200",
		"/utf-8",
		"/std:c++17",
		"/bigobj",
	}

	flags {
		"noincrementallink",
		"no64bitchecks",
		"shadowedvariables",
		"undefinedidentifiers",
		"multiprocessorcompile",
	}

	defines {
		"NOMINMAX",
		"WIN32_LEAN_AND_MEAN",
		"_CRT_SECURE_NO_WARNINGS",
		"_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS",
	}

	if string.contains(_ACTION, "vs") then
		printf("Building for Windows in Visual Studio..");

		platforms {
			"Win-x86",
			"Win-x64",
		}

		configurations {
			"Release",
			"Debug",
		}

		filter "platforms:Win-x86"	
			architecture "x86"
			syslibdirs {
				".\\deps\\bass\\c\\",
			}
			files {
				".\\src\\app\\resource\\**"
			}
		filter ""


		filter "platforms:Win-x64"
			architecture "x86_64"
			syslibdirs {
				".\\deps\\bass\\c\\x64\\",
			}
			files {
				".\\src\\app\\resource\\**"
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

		nuget {
			"sdl2.nuget:2.0.20",
			"sdl2.nuget.redist:2.0.20"
		}
		
		dependson {
			"ImGui",
		}

		links {
			"imgui",
			"bass",
		}
		
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

		includedirs {
			".\\src\\app\\",
			".\\src\\utils\\",
			".\\deps\\json\\include\\",
			".\\deps\\cpp-httplib\\",
			".\\deps\\imgui\\backends\\",
			".\\deps\\bass\\c\\",
		}

	group "Dependencies"
	
	project "ImGui"
		targetname "imgui"

		language "c++"
		kind "staticlib"
		
		nuget {
			"sdl2.nuget:2.0.20",
			"sdl2.nuget.redist:2.0.20"
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