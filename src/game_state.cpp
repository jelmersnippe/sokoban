#include "game_state.hpp"
#include "scenes/level_select_scene.hpp"
#include "scenes/menu_scene.hpp"

std::vector<Point> GetDestinations(const Level& level) {
    std::vector<Point> destinations;

    for (size_t y = 0; y < level.layout.size(); y++) {
        const std::vector<FloorType> row = level.layout[y];

        for (size_t x = 0; x < row.size(); x++) {
            if (row[x] == FloorType::Destination) {
                destinations.push_back({.x = static_cast<int>(x), .y = static_cast<int>(y)});
            };
        }
    }

    return destinations;
};

void change_scene(GameState& state, const Scene& scene) {
    state.scene = scene;

    // TODO: Scene stack -> move scene stuff into Scene struct instead of manual disposing in game state
    state.buttons.clear();

    switch (scene) {
        case Scene::Menu:
            InitMenuScene(state);
            break;
        case Scene::LevelSelect:
            InitLevelSelectScene(state);
            break;
        case Scene::Level:
        case Scene::Test:
            break;
    }
}

void LoadLevel(const Level& level, GameState& state) {
    state.level_state = LevelState::Active;
    state.level_configuration = level;
    state.boxPositions = level.boxes;
    state.destinations = GetDestinations(level);
    state.playerPosition = level.player;
    state.desiredMove = {.x = 0, .y = 0};
    state.timeSinceLastTick = 0;
};
