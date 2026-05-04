#include "game_state.hpp"

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

void LoadLevel(const Level& level, GameState& state) {
    state.levelConfiguration = level;
    state.boxPositions = level.boxes;
    state.destinations = GetDestinations(level);
    state.playerPosition = level.player;
    state.desiredMove = {.x = 0, .y = 0};
    state.timeSinceLastTick = 0;
};
