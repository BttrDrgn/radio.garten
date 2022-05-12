#if KIERO_INCLUDE_OPENGL

#include "../utils/logger/logger.hpp"
#include "opengl3_impl.h"

typedef bool(__stdcall* twglSwapBuffers) (_In_ HDC hDc);
twglSwapBuffers owglSwapBuffers;

BOOL __stdcall hwglSwapBuffers(_In_ HDC hDc)
{
	static bool init = false;

	if (!init)
	{
		HWND hwnd = WindowFromDC(hDc);

		impl::win32::init(hwnd);

		global::hwnd = hwnd;
		audio::init_overlay(hwnd);
		input::init_overlay(hwnd);

		ImGui::CreateContext();
		ImGui_ImplWin32_Init(hwnd);
		ImGui_ImplOpenGL3_Init();

		init = true;
	}

	if (!global::hide)
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		menus::update();

		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	return owglSwapBuffers(hDc);
}

void impl::opengl3::init()
{
	void* ptr = GetProcAddress(GetModuleHandleA("opengl32.dll"), "wglSwapBuffers");
	MH_Initialize();
	MH_CreateHook(ptr, hwglSwapBuffers, reinterpret_cast<void**>(&owglSwapBuffers));
	MH_EnableHook(ptr);
}

#endif // KIERO_INCLUDE_OPENGL