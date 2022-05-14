#if KIERO_INCLUDE_D3D11

#include "d3d11_impl.h"
#include <d3d11.h>

typedef long(__stdcall* Present)(IDXGISwapChain*, UINT, UINT);
static Present oPresent = NULL;

long __stdcall hkPresent11(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	static bool init = false;

	if (!init)
	{
		global::renderer = kiero::RenderType::Enum::D3D11;

		DXGI_SWAP_CHAIN_DESC desc;
		pSwapChain->GetDesc(&desc);

		ID3D11Device* device;
		pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&device);

		ID3D11DeviceContext* context;
		device->GetImmediateContext(&context);

		HWND hwnd = desc.OutputWindow;

		global::hwnd = hwnd;
		audio::init_overlay(hwnd);
		input::init_overlay(hwnd);

		menus::init();

		ImGui_ImplWin32_Init(hwnd);
		ImGui_ImplDX11_Init(device, context);

		init = true;
	}

	menus::prepare();
	menus::update();
	menus::present();

	return oPresent(pSwapChain, SyncInterval, Flags);
}

void impl::d3d11::init()
{
	if (kiero::bind(8, (void**)&oPresent, hkPresent11) != kiero::Status::Success)
	{
		MessageBoxA(nullptr, "Failed to hook DirectX 11!", "Radio.Garten", 0);
	}
}

#endif // KIERO_INCLUDE_D3D11