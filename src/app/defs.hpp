#pragma once

enum class GameFlowState : std::uint8_t
{
	None,
	LoadingFrontend,
	UnloadingFrontend,
	InFrontend,
	LoadingRegion,
	LoadingTrack,
	Racing,
	UnloadingTrack,
	UnloadingRegion,
	ExitDemoDisc,
};

static const auto& game_state = *reinterpret_cast<GameFlowState*>(0x008654A4);
