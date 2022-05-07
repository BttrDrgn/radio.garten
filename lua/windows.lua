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
		}

		postbuildcommands {
			"copy /y \"..\\deps\\bass\\Win32\\bass.dll\" \"$(OutDir)\"",
			"copy /y \"..\\deps\\discord\\lib\\x86\\discord_game_sdk.dll\" \"$(OutDir)\"",
		}
	--end

	--x64
	filter "platforms:Win-x64"
		architecture "x86_64"

		syslibdirs {
			"../deps/bass/Win32/c/x64/",
			"../deps/discord/lib/x86_64/",
		}

		postbuildcommands {
			"copy /y \"..\\deps\\bass\\Win32\\x64\\bass.dll\" \"$(OutDir)\"",
			"copy /y \"..\\deps\\discord\\lib\\x86_64\\discord_game_sdk.dll\" \"$(OutDir)\"",
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
		kind "consoleapp"
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
			"SDL2",
			"SDL2main",
			"ini_rw",
		}

		links {
			"imgui",
			"SDL2",
			"SDL2main",
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
			"../src/app/api/**",
			"../src/app/audio/**",
			"../src/app/gfx/**",
			"../src/app/input/**",
			"../src/app/menus/**",
			"../src/app/window/**",
			"../src/app/settings/**",

			"../src/utils/**",

			"../src/app/resource/**",
			"../src/app/drpc/**",
			"../deps/bass/Win32/c/bass.h",
		}

	group "Dependencies"
	
	project "SDL2main"
		language "c"
		kind "staticlib"

		files {
			"../deps/SDL/src/main/windows/SDL_windows_main.c"
		}

		includedirs {
			"../deps/SDL/include/",
		}

	project "SDL2"
		language "c"
		kind "staticlib"

		defines {
			"HAVE_LIBC=1",
		}

		links {
			"setupapi.lib",
			"winmm.lib",
			"imm32.lib",
			"version.lib",
		}

		files {
			"../deps/SDL/include/**",

			"../deps/SDL/src/audio/directsound/SDL_directsound.h",
			"../deps/SDL/src/audio/disk/SDL_diskaudio.h",
			"../deps/SDL/src/audio/dummy/SDL_dummyaudio.h",
			"../deps/SDL/src/audio/SDL_audio_c.h",
			"../deps/SDL/src/audio/SDL_audiodev_c.h",
			"../deps/SDL/src/audio/SDL_sysaudio.h",
			"../deps/SDL/src/audio/SDL_wave.h",
			"../deps/SDL/src/audio/wasapi/SDL_wasapi.h",
			"../deps/SDL/src/audio/winmm/SDL_winmm.h",
			"../deps/SDL/src/core/windows/SDL_directx.h",
			"../deps/SDL/src/core/windows/SDL_hid.h",
			"../deps/SDL/src/core/windows/SDL_windows.h",
			"../deps/SDL/src/core/windows/SDL_xinput.h",
			"../deps/SDL/src/dynapi/SDL_dynapi.h",
			"../deps/SDL/src/dynapi/SDL_dynapi_overrides.h",
			"../deps/SDL/src/dynapi/SDL_dynapi_procs.h",
			"../deps/SDL/src/events/blank_cursor.h",
			"../deps/SDL/src/events/default_cursor.h",
			"../deps/SDL/src/events/scancodes_windows.h",
			"../deps/SDL/src/events/SDL_clipboardevents_c.h",
			"../deps/SDL/src/events/SDL_displayevents_c.h",
			"../deps/SDL/src/events/SDL_dropevents_c.h",
			"../deps/SDL/src/events/SDL_events_c.h",
			"../deps/SDL/src/events/SDL_gesture_c.h",
			"../deps/SDL/src/events/SDL_keyboard_c.h",
			"../deps/SDL/src/events/SDL_mouse_c.h",
			"../deps/SDL/src/events/SDL_sysevents.h",
			"../deps/SDL/src/events/SDL_touch_c.h",
			"../deps/SDL/src/events/SDL_windowevents_c.h",
			"../deps/SDL/src/haptic/SDL_haptic_c.h",
			"../deps/SDL/src/haptic/SDL_syshaptic.h",
			"../deps/SDL/src/haptic/windows/SDL_dinputhaptic_c.h",
			"../deps/SDL/src/haptic/windows/SDL_windowshaptic_c.h",
			"../deps/SDL/src/haptic/windows/SDL_xinputhaptic_c.h",
			"../deps/SDL/src/hidapi/hidapi/hidapi.h",
			"../deps/SDL/src/hidapi/SDL_hidapi_c.h",
			"../deps/SDL/src/joystick/controller_type.h",
			"../deps/SDL/src/joystick/hidapi/SDL_hidapijoystick_c.h",
			"../deps/SDL/src/joystick/hidapi/SDL_hidapi_rumble.h",
			"../deps/SDL/src/joystick/SDL_gamecontrollerdb.h",
			"../deps/SDL/src/joystick/SDL_joystick_c.h",
			"../deps/SDL/src/joystick/SDL_sysjoystick.h",
			"../deps/SDL/src/joystick/usb_ids.h",
			"../deps/SDL/src/joystick/virtual/SDL_virtualjoystick_c.h",
			"../deps/SDL/src/joystick/windows/SDL_dinputjoystick_c.h",
			"../deps/SDL/src/joystick/windows/SDL_rawinputjoystick_c.h",
			"../deps/SDL/src/joystick/windows/SDL_windowsjoystick_c.h",
			"../deps/SDL/src/joystick/windows/SDL_xinputjoystick_c.h",
			"../deps/SDL/src/libm/math_libm.h",
			"../deps/SDL/src/libm/math_private.h",
			"../deps/SDL/src/locale/SDL_syslocale.h",
			"../deps/SDL/src/misc/SDL_sysurl.h",
			"../deps/SDL/src/power/SDL_syspower.h",
			"../deps/SDL/src/render/direct3d11/SDL_shaders_d3d11.h",
			"../deps/SDL/src/render/direct3d/SDL_shaders_d3d.h",
			"../deps/SDL/src/render/opengles2/SDL_gles2funcs.h",
			"../deps/SDL/src/render/opengles2/SDL_shaders_gles2.h",
			"../deps/SDL/src/render/opengl/SDL_glfuncs.h",
			"../deps/SDL/src/render/opengl/SDL_shaders_gl.h",
			"../deps/SDL/src/render/SDL_d3dmath.h",
			"../deps/SDL/src/render/SDL_sysrender.h",
			"../deps/SDL/src/render/SDL_yuv_sw_c.h",
			"../deps/SDL/src/render/software/SDL_blendfillrect.h",
			"../deps/SDL/src/render/software/SDL_blendline.h",
			"../deps/SDL/src/render/software/SDL_blendpoint.h",
			"../deps/SDL/src/render/software/SDL_draw.h",
			"../deps/SDL/src/render/software/SDL_drawline.h",
			"../deps/SDL/src/render/software/SDL_drawpoint.h",
			"../deps/SDL/src/render/software/SDL_render_sw_c.h",
			"../deps/SDL/src/render/software/SDL_rotate.h",
			"../deps/SDL/src/render/software/SDL_triangle.h",
			"../deps/SDL/src/SDL_assert_c.h",
			"../deps/SDL/src/SDL_dataqueue.h",
			"../deps/SDL/src/SDL_error_c.h",
			"../deps/SDL/src/SDL_hints_c.h",
			"../deps/SDL/src/SDL_internal.h",
			"../deps/SDL/src/SDL_list.h",
			"../deps/SDL/src/SDL_log_c.h",
			"../deps/SDL/src/sensor/dummy/SDL_dummysensor.h",
			"../deps/SDL/src/sensor/SDL_sensor_c.h",
			"../deps/SDL/src/sensor/SDL_syssensor.h",
			"../deps/SDL/src/sensor/windows/SDL_windowssensor.h",
			"../deps/SDL/src/thread/SDL_systhread.h",
			"../deps/SDL/src/thread/SDL_thread_c.h",
			"../deps/SDL/src/thread/generic/SDL_syscond_c.h",
			"../deps/SDL/src/thread/windows/SDL_sysmutex_c.h",
			"../deps/SDL/src/thread/windows/SDL_systhread_c.h",
			"../deps/SDL/src/timer/SDL_timer_c.h",
			"../deps/SDL/src/video/dummy/SDL_nullevents_c.h",
			"../deps/SDL/src/video/dummy/SDL_nullframebuffer_c.h",
			"../deps/SDL/src/video/dummy/SDL_nullvideo.h",
			"../deps/SDL/src/video/khronos/vulkan/vk_icd.h",
			"../deps/SDL/src/video/khronos/vulkan/vk_layer.h",
			"../deps/SDL/src/video/khronos/vulkan/vk_platform.h",
			"../deps/SDL/src/video/khronos/vulkan/vk_sdk_platform.h",
			"../deps/SDL/src/video/khronos/vulkan/vulkan.h",
			"../deps/SDL/src/video/khronos/vulkan/vulkan.hpp",
			"../deps/SDL/src/video/khronos/vulkan/vulkan_android.h",
			"../deps/SDL/src/video/khronos/vulkan/vulkan_beta.h",
			"../deps/SDL/src/video/khronos/vulkan/vulkan_core.h",
			"../deps/SDL/src/video/khronos/vulkan/vulkan_directfb.h",
			"../deps/SDL/src/video/khronos/vulkan/vulkan_fuchsia.h",
			"../deps/SDL/src/video/khronos/vulkan/vulkan_ggp.h",
			"../deps/SDL/src/video/khronos/vulkan/vulkan_ios.h",
			"../deps/SDL/src/video/khronos/vulkan/vulkan_macos.h",
			"../deps/SDL/src/video/khronos/vulkan/vulkan_metal.h",
			"../deps/SDL/src/video/khronos/vulkan/vulkan_vi.h",
			"../deps/SDL/src/video/khronos/vulkan/vulkan_wayland.h",
			"../deps/SDL/src/video/khronos/vulkan/vulkan_win32.h",
			"../deps/SDL/src/video/khronos/vulkan/vulkan_xcb.h",
			"../deps/SDL/src/video/khronos/vulkan/vulkan_xlib.h",
			"../deps/SDL/src/video/khronos/vulkan/vulkan_xlib_xrandr.h",
			"../deps/SDL/src/video/SDL_blit.h",
			"../deps/SDL/src/video/SDL_blit_auto.h",
			"../deps/SDL/src/video/SDL_blit_copy.h",
			"../deps/SDL/src/video/SDL_blit_slow.h",
			"../deps/SDL/src/video/SDL_egl_c.h",
			"../deps/SDL/src/video/SDL_pixels_c.h",
			"../deps/SDL/src/video/SDL_rect_c.h",
			"../deps/SDL/src/video/SDL_RLEaccel_c.h",
			"../deps/SDL/src/video/SDL_shape_internals.h",
			"../deps/SDL/src/video/SDL_sysvideo.h",
			"../deps/SDL/src/video/SDL_vulkan_internal.h",
			"../deps/SDL/src/video/SDL_yuv_c.h",
			"../deps/SDL/src/video/windows/SDL_msctf.h",
			"../deps/SDL/src/video/windows/SDL_vkeys.h",
			"../deps/SDL/src/video/windows/SDL_windowsclipboard.h",
			"../deps/SDL/src/video/windows/SDL_windowsevents.h",
			"../deps/SDL/src/video/windows/SDL_windowsframebuffer.h",
			"../deps/SDL/src/video/windows/SDL_windowskeyboard.h",
			"../deps/SDL/src/video/windows/SDL_windowsmessagebox.h",
			"../deps/SDL/src/video/windows/SDL_windowsmodes.h",
			"../deps/SDL/src/video/windows/SDL_windowsmouse.h",
			"../deps/SDL/src/video/windows/SDL_windowsopengl.h",
			"../deps/SDL/src/video/windows/SDL_windowsopengles.h",
			"../deps/SDL/src/video/windows/SDL_windowsshape.h",
			"../deps/SDL/src/video/windows/SDL_windowstaskdialog.h",
			"../deps/SDL/src/video/windows/SDL_windowsvideo.h",
			"../deps/SDL/src/video/windows/SDL_windowsvulkan.h",
			"../deps/SDL/src/video/windows/SDL_windowswindow.h",
			"../deps/SDL/src/video/windows/wmmsg.h",
			"../deps/SDL/src/video/yuv2rgb/yuv_rgb.h",
			"../deps/SDL/src/video/yuv2rgb/yuv_rgb_sse_func.h",
			"../deps/SDL/src/video/yuv2rgb/yuv_rgb_std_func.h",

			"../deps/SDL/src/atomic/SDL_atomic.c",
			"../deps/SDL/src/atomic/SDL_spinlock.c",
			"../deps/SDL/src/audio/directsound/SDL_directsound.c",
			"../deps/SDL/src/audio/disk/SDL_diskaudio.c",
			"../deps/SDL/src/audio/dummy/SDL_dummyaudio.c",
			"../deps/SDL/src/audio/SDL_audio.c",
			"../deps/SDL/src/audio/SDL_audiocvt.c",
			"../deps/SDL/src/audio/SDL_audiodev.c",
			"../deps/SDL/src/audio/SDL_audiotypecvt.c",
			"../deps/SDL/src/audio/SDL_mixer.c",
			"../deps/SDL/src/audio/SDL_wave.c",
			"../deps/SDL/src/audio/winmm/SDL_winmm.c",
			"../deps/SDL/src/audio/wasapi/SDL_wasapi.c",
			"../deps/SDL/src/audio/wasapi/SDL_wasapi_win32.c",
			"../deps/SDL/src/core/windows/SDL_hid.c",
			"../deps/SDL/src/core/windows/SDL_windows.c",
			"../deps/SDL/src/core/windows/SDL_xinput.c",
			"../deps/SDL/src/cpuinfo/SDL_cpuinfo.c",
			"../deps/SDL/src/dynapi/SDL_dynapi.c",
			"../deps/SDL/src/events/SDL_clipboardevents.c",
			"../deps/SDL/src/events/SDL_displayevents.c",
			"../deps/SDL/src/events/SDL_dropevents.c",
			"../deps/SDL/src/events/SDL_events.c",
			"../deps/SDL/src/events/SDL_gesture.c",
			"../deps/SDL/src/events/SDL_keyboard.c",
			"../deps/SDL/src/events/SDL_mouse.c",
			"../deps/SDL/src/events/SDL_quit.c",
			"../deps/SDL/src/events/SDL_touch.c",
			"../deps/SDL/src/events/SDL_windowevents.c",
			"../deps/SDL/src/file/SDL_rwops.c",
			"../deps/SDL/src/filesystem/windows/SDL_sysfilesystem.c",
			"../deps/SDL/src/haptic/dummy/SDL_syshaptic.c",
			"../deps/SDL/src/haptic/SDL_haptic.c",
			"../deps/SDL/src/haptic/windows/SDL_dinputhaptic.c",
			"../deps/SDL/src/haptic/windows/SDL_windowshaptic.c",
			"../deps/SDL/src/haptic/windows/SDL_xinputhaptic.c",
			"../deps/SDL/src/hidapi/SDL_hidapi.c",
			"../deps/SDL/src/joystick/dummy/SDL_sysjoystick.c",
			"../deps/SDL/src/joystick/hidapi/SDL_hidapijoystick.c",
			"../deps/SDL/src/joystick/hidapi/SDL_hidapi_gamecube.c",
			"../deps/SDL/src/joystick/hidapi/SDL_hidapi_luna.c",
			"../deps/SDL/src/joystick/hidapi/SDL_hidapi_ps4.c",
			"../deps/SDL/src/joystick/hidapi/SDL_hidapi_ps5.c",
			"../deps/SDL/src/joystick/hidapi/SDL_hidapi_rumble.c",
			"../deps/SDL/src/joystick/hidapi/SDL_hidapi_stadia.c",
			"../deps/SDL/src/joystick/hidapi/SDL_hidapi_switch.c",
			"../deps/SDL/src/joystick/hidapi/SDL_hidapi_xbox360.c",
			"../deps/SDL/src/joystick/hidapi/SDL_hidapi_xbox360w.c",
			"../deps/SDL/src/joystick/hidapi/SDL_hidapi_xboxone.c",
			"../deps/SDL/src/joystick/SDL_gamecontroller.c",
			"../deps/SDL/src/joystick/SDL_joystick.c",
			"../deps/SDL/src/joystick/virtual/SDL_virtualjoystick.c",
			"../deps/SDL/src/joystick/windows/SDL_dinputjoystick.c",
			"../deps/SDL/src/joystick/windows/SDL_rawinputjoystick.c",
			"../deps/SDL/src/joystick/windows/SDL_windowsjoystick.c",
			"../deps/SDL/src/joystick/windows/SDL_windows_gaming_input.c",
			"../deps/SDL/src/joystick/windows/SDL_xinputjoystick.c",
			"../deps/SDL/src/libm/e_atan2.c",
			"../deps/SDL/src/libm/e_exp.c",
			"../deps/SDL/src/libm/e_fmod.c",
			"../deps/SDL/src/libm/e_log.c",
			"../deps/SDL/src/libm/e_log10.c",
			"../deps/SDL/src/libm/e_pow.c",
			"../deps/SDL/src/libm/e_rem_pio2.c",
			"../deps/SDL/src/libm/e_sqrt.c",
			"../deps/SDL/src/libm/k_cos.c",
			"../deps/SDL/src/libm/k_rem_pio2.c",
			"../deps/SDL/src/libm/k_sin.c",
			"../deps/SDL/src/libm/k_tan.c",
			"../deps/SDL/src/libm/s_atan.c",
			"../deps/SDL/src/libm/s_copysign.c",
			"../deps/SDL/src/libm/s_cos.c",
			"../deps/SDL/src/libm/s_fabs.c",
			"../deps/SDL/src/libm/s_floor.c",
			"../deps/SDL/src/libm/s_scalbn.c",
			"../deps/SDL/src/libm/s_sin.c",
			"../deps/SDL/src/libm/s_tan.c",
			"../deps/SDL/src/loadso/windows/SDL_sysloadso.c",
			"../deps/SDL/src/locale/SDL_locale.c",
			"../deps/SDL/src/locale/windows/SDL_syslocale.c",
			"../deps/SDL/src/misc/SDL_url.c",
			"../deps/SDL/src/misc/windows/SDL_sysurl.c",
			"../deps/SDL/src/power/SDL_power.c",
			"../deps/SDL/src/power/windows/SDL_syspower.c",
			"../deps/SDL/src/render/direct3d11/SDL_shaders_d3d11.c",
			"../deps/SDL/src/render/direct3d/SDL_render_d3d.c",
			"../deps/SDL/src/render/direct3d11/SDL_render_d3d11.c",
			"../deps/SDL/src/render/direct3d/SDL_shaders_d3d.c",
			"../deps/SDL/src/render/opengl/SDL_render_gl.c",
			"../deps/SDL/src/render/opengl/SDL_shaders_gl.c",
			"../deps/SDL/src/render/opengles2/SDL_render_gles2.c",
			"../deps/SDL/src/render/opengles2/SDL_shaders_gles2.c",
			"../deps/SDL/src/render/SDL_d3dmath.c",
			"../deps/SDL/src/render/SDL_render.c",
			"../deps/SDL/src/render/SDL_yuv_sw.c",
			"../deps/SDL/src/render/software/SDL_blendfillrect.c",
			"../deps/SDL/src/render/software/SDL_blendline.c",
			"../deps/SDL/src/render/software/SDL_blendpoint.c",
			"../deps/SDL/src/render/software/SDL_drawline.c",
			"../deps/SDL/src/render/software/SDL_drawpoint.c",
			"../deps/SDL/src/render/software/SDL_render_sw.c",
			"../deps/SDL/src/render/software/SDL_rotate.c",
			"../deps/SDL/src/render/software/SDL_triangle.c",
			"../deps/SDL/src/SDL.c",
			"../deps/SDL/src/SDL_assert.c",
			"../deps/SDL/src/SDL_dataqueue.c",
			"../deps/SDL/src/SDL_list.c",
			"../deps/SDL/src/SDL_error.c",
			"../deps/SDL/src/SDL_hints.c",
			"../deps/SDL/src/SDL_log.c",
			"../deps/SDL/src/sensor/dummy/SDL_dummysensor.c",
			"../deps/SDL/src/sensor/SDL_sensor.c",
			"../deps/SDL/src/sensor/windows/SDL_windowssensor.c",
			"../deps/SDL/src/stdlib/SDL_crc32.c",
			"../deps/SDL/src/stdlib/SDL_getenv.c",
			"../deps/SDL/src/stdlib/SDL_iconv.c",
			"../deps/SDL/src/stdlib/SDL_malloc.c",
			"../deps/SDL/src/stdlib/SDL_qsort.c",
			"../deps/SDL/src/stdlib/SDL_stdlib.c",
			"../deps/SDL/src/stdlib/SDL_string.c",
			"../deps/SDL/src/stdlib/SDL_strtokr.c",
			"../deps/SDL/src/thread/generic/SDL_syscond.c",
			"../deps/SDL/src/thread/SDL_thread.c",
			"../deps/SDL/src/thread/windows/SDL_syscond_cv.c",
			"../deps/SDL/src/thread/windows/SDL_sysmutex.c",
			"../deps/SDL/src/thread/windows/SDL_syssem.c",
			"../deps/SDL/src/thread/windows/SDL_systhread.c",
			"../deps/SDL/src/thread/windows/SDL_systls.c",
			"../deps/SDL/src/timer/SDL_timer.c",
			"../deps/SDL/src/timer/windows/SDL_systimer.c",
			"../deps/SDL/src/video/dummy/SDL_nullevents.c",
			"../deps/SDL/src/video/dummy/SDL_nullframebuffer.c",
			"../deps/SDL/src/video/dummy/SDL_nullvideo.c",
			"../deps/SDL/src/video/SDL_blit.c",
			"../deps/SDL/src/video/SDL_blit_0.c",
			"../deps/SDL/src/video/SDL_blit_1.c",
			"../deps/SDL/src/video/SDL_blit_A.c",
			"../deps/SDL/src/video/SDL_blit_auto.c",
			"../deps/SDL/src/video/SDL_blit_copy.c",
			"../deps/SDL/src/video/SDL_blit_N.c",
			"../deps/SDL/src/video/SDL_blit_slow.c",
			"../deps/SDL/src/video/SDL_bmp.c",
			"../deps/SDL/src/video/SDL_clipboard.c",
			"../deps/SDL/src/video/SDL_egl.c",
			"../deps/SDL/src/video/SDL_fillrect.c",
			"../deps/SDL/src/video/SDL_pixels.c",
			"../deps/SDL/src/video/SDL_rect.c",
			"../deps/SDL/src/video/SDL_RLEaccel.c",
			"../deps/SDL/src/video/SDL_shape.c",
			"../deps/SDL/src/video/SDL_stretch.c",
			"../deps/SDL/src/video/SDL_surface.c",
			"../deps/SDL/src/video/SDL_video.c",
			"../deps/SDL/src/video/SDL_vulkan_utils.c",
			"../deps/SDL/src/video/SDL_yuv.c",
			"../deps/SDL/src/video/windows/SDL_windowsclipboard.c",
			"../deps/SDL/src/video/windows/SDL_windowsevents.c",
			"../deps/SDL/src/video/windows/SDL_windowsframebuffer.c",
			"../deps/SDL/src/video/windows/SDL_windowskeyboard.c",
			"../deps/SDL/src/video/windows/SDL_windowsmessagebox.c",
			"../deps/SDL/src/video/windows/SDL_windowsmodes.c",
			"../deps/SDL/src/video/windows/SDL_windowsmouse.c",
			"../deps/SDL/src/video/windows/SDL_windowsopengl.c",
			"../deps/SDL/src/video/windows/SDL_windowsopengles.c",
			"../deps/SDL/src/video/windows/SDL_windowsshape.c",
			"../deps/SDL/src/video/windows/SDL_windowsvideo.c",
			"../deps/SDL/src/video/windows/SDL_windowsvulkan.c",
			"../deps/SDL/src/video/windows/SDL_windowswindow.c",
			"../deps/SDL/src/video/yuv2rgb/yuv_rgb.c",
		}

		includedirs {
			"../deps/SDL/include/",
		}

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