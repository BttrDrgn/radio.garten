#if KIERO_INCLUDE_D3D9

#include "d3d9_impl.h"
#include <d3d9.h>

typedef long(__stdcall* Reset)(LPDIRECT3DDEVICE9, D3DPRESENT_PARAMETERS*);
static Reset oReset = NULL;

typedef long(__stdcall* EndScene)(LPDIRECT3DDEVICE9);
static EndScene oEndScene = NULL;

long __stdcall hkReset(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	long result = oReset(pDevice, pPresentationParameters);
	ImGui_ImplDX9_CreateDeviceObjects();

	return result;
}

long __stdcall hkEndScene(LPDIRECT3DDEVICE9 pDevice)
{
	static bool init = false;

	if (!init)
	{
		global::renderer = kiero::RenderType::Enum::D3D9;

		D3DDEVICE_CREATION_PARAMETERS params;
		pDevice->GetCreationParameters(&params);

		HWND hwnd = params.hFocusWindow;

		global::hwnd = hwnd;
		audio::init();
		input::init_overlay();

		menus::init();

		ImGui_ImplWin32_Init(hwnd);
		ImGui_ImplDX9_Init(pDevice);

		init = true;
	}

	menus::prepare();
	menus::update();
	menus::present();

	return oEndScene(pDevice);
}

void impl::d3d9::init()
{
	if (kiero::bind(16, (void**)&oReset, hkReset) != kiero::Status::Success || kiero::bind(42, (void**)&oEndScene, hkEndScene) != kiero::Status::Success)
	{
		MessageBoxA(nullptr, "Failed to hook DirectX 9!", "ECM", 0);
	}
}

#endif // KIERO_INCLUDE_D3D9