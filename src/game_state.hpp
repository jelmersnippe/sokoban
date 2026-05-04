#pragma once

#include "core/point.hpp"
#include "level/level_loader.hpp"
#include "scenes/scenes.hpp"
#include <vector>

enum class LevelState {
    Active,
    Finished
};

struct GameState {
    float timeSinceLastTick = 0;
    Level levelConfiguration;
    std::vector<Point> boxPositions;
    std::vector<Point> destinations;
    Point playerPosition;

    Point desiredMove = {.x = 0, .y = 0};
    Scene scene = Scene::Menu;
    LevelState level_state = LevelState::Active;
};

void LoadLevel(const Level& level, GameState& state);
