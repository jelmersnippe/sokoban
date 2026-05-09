#pragma once

#include "core/point.hpp"
#include "core/utils.hpp"
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
    Scene scene;
    LevelState level_state = LevelState::Finished;

    bool should_exit = false;
    int level_selection_index = 0;

    std::vector<Button> buttons;
};

void change_scene(GameState& state, const Scene& scene);

void LoadLevel(const Level& level, GameState& state);
