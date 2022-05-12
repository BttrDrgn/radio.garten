#if KIERO_INCLUDE_OPENGL

#include "../utils/logger/logger.hpp"
#include "opengl3_impl.h"

typedef bool(__stdcall* twglSwapBuffers) (_In_ HDC hDc);
twglSwapBuffers owglSwapBuffers;

BOOL __stdcall hkWglSwapBuffers(_In_ HDC hDc)
{
	static bool init = false;

	if (!init)
	{
		HWND hwnd = WindowFromDC(hDc);

		global::hwnd = hwnd;
		audio::init_overlay(hwnd);
		input::init_overlay(hwnd);

		menus::init();

		ImGui_ImplWin32_Init(hwnd);
		ImGui_ImplOpenGL3_Init();

		init = true;
	}

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	menus::update();

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return owglSwapBuffers(hDc);
}

void impl::opengl3::init()
{
	if (kiero::bind(336, (void**)&owglSwapBuffers, hkWglSwapBuffers) != kiero::Status::Success)
	{
		MessageBoxA(nullptr, "Failed to hook OpenGL!", "Radio.Garten", 0);
	}
}

#endif // KIERO_INCLUDE_OPENGL