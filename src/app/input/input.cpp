#include "global.hpp"

#include "logger/logger.hpp"

#include "input.hpp"

WNDPROC o_wndproc;
bool toggle_once = false;
std::unordered_map<input::callback_type, std::vector<input::callback>> input::callbacks_;

LRESULT __stdcall wndproc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);

	switch (uMsg)
	{

	case WM_SYSKEYDOWN:
		if (wParam == VK_RETURN)
		{
			if ((HIWORD(lParam) & KF_ALTDOWN))
			{
				return 1;
			}
		}
		break;

	case WM_KEYDOWN:
		for (const auto callback : input::callbacks_[input::callback_type::on_key_down])
		{
			callback(wParam);
		}
		break;

	case WM_KEYUP:
		for (const auto callback : input::callbacks_[input::callback_type::on_key_up])
		{
			callback(wParam);
		}
		break;

	case WM_SYSCOMMAND:
		if ((wParam & 0xFF00) == SC_KEYMENU)
		{
			return 1;
		}
	}


	return CallWindowProcA(o_wndproc, hWnd, uMsg, wParam, lParam);
}

void input::init_overlay()
{
	o_wndproc = (WNDPROC)SetWindowLongW(global::hwnd, GWLP_WNDPROC, (LONG_PTR)wndproc);

	static bool once = false;

	input::on(input::callback_type::on_key_down, [](auto key) -> input::result_type
	{
		if (key == 122 && !once)
		{
			global::hide = !global::hide;
			once = true;
		}

		return input::result_type::cont;
	});

	input::on(input::callback_type::on_key_up, [](auto key) -> input::result_type
	{
		if (key == 122 && once)
		{
			once = false;
		}

		return input::result_type::cont;
	});
}


void input::on(input::callback_type type, input::callback callback)
{
	input::callbacks_[type].emplace_back(callback);
}

bool input::is_key_down(std::uint32_t key)
{
	return ImGui::IsKeyPressed(key, false);
}
