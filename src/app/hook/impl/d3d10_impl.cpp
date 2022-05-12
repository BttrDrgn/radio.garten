#if KIERO_INCLUDE_D3D10

#include "d3d10_impl.h"
#include <d3d10.h>

typedef long(__stdcall* Present)(IDXGISwapChain*, UINT, UINT);
static Present oPresent = NULL;

long __stdcall hkPresent10(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	static bool init = false;

	if (!init)
	{
		DXGI_SWAP_CHAIN_DESC desc;
		pSwapChain->GetDesc(&desc);

		ID3D10Device* device;
		pSwapChain->GetDevice(__uuidof(ID3D10Device), (void**)&device);

		HWND hwnd = desc.OutputWindow;

		global::hwnd = hwnd;
		audio::init_overlay(hwnd);
		input::init_overlay(hwnd);

		ImGui::CreateContext();
		ImGui_ImplWin32_Init(hwnd);
		ImGui_ImplDX10_Init(device);

		init = true;
	}

	ImGui_ImplDX10_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	menus::update();

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX10_RenderDrawData(ImGui::GetDrawData());

	return oPresent(pSwapChain, SyncInterval, Flags);
}

void impl::d3d10::init()
{
	if (kiero::bind(8, (void**)&oPresent, hkPresent10) != kiero::Status::Success)
	{
		MessageBoxA(nullptr, "Failed to hook DirectX 10!", "Radio.Garten", 0);
	}
}

#endif // KIERO_INCLUDE_D3D10