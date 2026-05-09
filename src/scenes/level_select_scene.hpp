#pragma once

#include "game_state.hpp"

void InitLevelSelectScene(GameState& state);

void HandleLevelSelectSceneInput(GameState& state);

void DrawLevelSelectScene(const GameState& state);

void UpdateLevelSelectScene(GameState& state);
