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
		global::renderer = kiero::RenderType::Enum::D3D10;

		DXGI_SWAP_CHAIN_DESC desc;
		pSwapChain->GetDesc(&desc);

		ID3D10Device* device;
		pSwapChain->GetDevice(__uuidof(ID3D10Device), (void**)&device);

		HWND hwnd = desc.OutputWindow;

		global::hwnd = hwnd;
		audio::init_overlay();
		input::init_overlay();

		menus::init();

		ImGui_ImplWin32_Init(hwnd);
		ImGui_ImplDX10_Init(device);

		init = true;
	}

	menus::prepare();
	menus::update();
	menus::present();

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