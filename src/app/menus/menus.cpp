#include "global.hpp"
#include "logger/logger.hpp"
#include "fs/fs.hpp"
#include "menus.hpp"
#include "audio/audio.hpp"
#include "hook/hook.hpp"
#include "settings/settings.hpp"
#include "audio/player.hpp"

#include <shellapi.h>

void menus::init()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui::GetIO().IniFilename = nullptr;

	menus::build_font(ImGui::GetIO());
}

void menus::prepare()
{
	switch (global::renderer)
	{
	case kiero::RenderType::D3D9:
		ImGui_ImplDX9_NewFrame();
		break;

	case kiero::RenderType::D3D10:
		ImGui_ImplDX10_NewFrame();
		break;

	case kiero::RenderType::D3D11:
		ImGui_ImplDX11_NewFrame();
		break;

	case kiero::RenderType::OpenGL:
		ImGui_ImplOpenGL3_NewFrame();
		break;
	}

	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void menus::present()
{
	ImGui::EndFrame();
	ImGui::Render();

	switch (global::renderer)
	{
	case kiero::RenderType::D3D9:
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		break;

	case kiero::RenderType::D3D10:
		ImGui_ImplDX10_RenderDrawData(ImGui::GetDrawData());
		break;

	case kiero::RenderType::D3D11:
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		break;

	case kiero::RenderType::OpenGL:
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		break;
	}
}

void menus::update()
{
	static bool once = false;

	if (GetAsyncKeyState(VK_F12) < 0 && !once)
	{
		global::hide = !global::hide;
		once = true;
	}
	else if (GetAsyncKeyState(VK_F12) == 0 && once)
	{
		once = false;
	}

	ImGui::GetIO().MouseDrawCursor = !global::hide;

	if (!global::hide)
	{
		ImGui::SetNextWindowPos({ 0, 0 });
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground;
		if (ImGui::Begin("ECM", nullptr, flags))
		{
			menus::main_menu_bar();
			ImGui::End();
		}
	}
}

void menus::main_menu_bar()
{
	if (ImGui::BeginMainMenuBar())
	{
		menus::actions();
		menus::playlist();

		ImGui::Text(logger::va("Listening: %s on %s", audio::currently_playing.title.c_str(), "Custom Playlist").c_str());

		ImGui::EndMainMenuBar();
	}
}

void menus::actions()
{
	if (ImGui::BeginMenu("Actions"))
	{
		ImGui::Text("Audio Controls");
		ImGui::Text("Volume: ");

		ImGui::SameLine();
		ImGui::PushItemWidth(50.0f);

		if (ImGui::SliderInt("##Volume", &audio::volume, 0, 100))
		{
			audio::set_volume(audio::volume);
		}

		if (ImGui::Button("Skip"))
		{
			audio::stop();
			audio::play_next_song();
		}

		ImGui::EndMenu();
	}
}

void menus::playlist()
{
	if (ImGui::BeginMenu("Playlist"))
	{
		for (int i = 0; i < audio::playlist_files.size(); ++i)
		{
			std::string song = audio::playlist_files[i];
			logger::rem_path_info(song, audio::playlist_dir);
			if (ImGui::Button(logger::va("%s", song.c_str()).c_str()))
			{
				audio::stop();
				audio::play_file(audio::playlist_files[i]);
			}
		}

		ImGui::EndMenu();
	}
}

void menus::build_font(ImGuiIO& io)
{
	std::string font = "ecm/fonts/NotoSans-Regular.ttf";
	std::string font_jp = "ecm/fonts/NotoSansJP-Regular.ttf";
	std::string emoji = "ecm/fonts/NotoEmoji-Regular.ttf";

	if (fs::exists(font))
	{
		io.Fonts->AddFontFromFileTTF(&font[0], 18.0f);

		static ImFontConfig cfg;
		static ImWchar emoji_ranges[] = { 0x1, 0x1FFFF, 0 };

		if (fs::exists(emoji))
		{
			cfg.MergeMode = true;
			cfg.OversampleH = cfg.OversampleV = 1;
			//cfg.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_LoadColor;	//Noto doesnt have color
			io.Fonts->AddFontFromFileTTF(&emoji[0], 12.0f, &cfg, emoji_ranges);
		}

		if (fs::exists(font_jp))
		{
			ImFontConfig cfg;
			cfg.OversampleH = cfg.OversampleV = 1;
			cfg.MergeMode = true;
			io.Fonts->AddFontFromFileTTF(&font_jp[0], 18.0f, &cfg, io.Fonts->GetGlyphRangesJapanese());
		}
	}
}
