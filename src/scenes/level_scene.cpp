#include "core/point.hpp"

#include "core/globals.hpp"
#include "game_state.hpp"
#include "level/level_loader.hpp"
#include "raylib.h"
#include "scenes/level_scene.hpp"
#include <algorithm>
#include <vector>

enum class OccupiedType {
    None,
    Wall,
    Box
};

constexpr int TILE_SIZE = 50;

constexpr Color WALL_COLOR = {.r = 90, .g = 90, .b = 90, .a = 255};
constexpr Color FLOOR_COLOR = {.r = 170, .g = 150, .b = 100, .a = 255};
constexpr Color BOX_COLOR = {.r = 185, .g = 115, .b = 40, .a = 255};
constexpr Color BOX_ON_DESTINATION_COLOR = {.r = 85, .g = 55, .b = 20, .a = 255};
constexpr Color DESTINATION_COLOR = RED;
constexpr Color PLAYER_COLOR = GREEN;

std::vector<std::vector<OccupiedType>> GetOccupiedGrid(const Level& level, const std::vector<Point>& boxes) {
    std::vector<std::vector<OccupiedType>> occupiedGrid(
        level.size.height, std::vector<OccupiedType>(level.size.width, OccupiedType::None));

    for (size_t y = 0; y < level.layout.size(); y++) {
        const std::vector<FloorType> row = level.layout[y];

        for (size_t x = 0; x < row.size(); x++) {
            if (row[x] == FloorType::Solid) { occupiedGrid[y][x] = OccupiedType::Wall; }
        }
    }

    for (Point position : boxes) {
        occupiedGrid[position.y][position.x] = OccupiedType::Box;
    }

    return occupiedGrid;
}

void UpdateLevelScene(GameState& state) {
    if (state.level_state == LevelState::Finished) return;

    // All boxes are on a destination
    if (std::ranges::all_of(state.boxPositions, [&state](Point& box) {
            return std::ranges::any_of(state.destinations, [&box](const Point& destination) {
                return box.x == destination.x && box.y == destination.y;
            });
        })) {
        state.level_state = LevelState::Finished;
        return;
    }

    std::vector<std::vector<OccupiedType>> occupiedGrid = GetOccupiedGrid(state.levelConfiguration, state.boxPositions);

    Point desiredPosition = {.x = state.playerPosition.x + state.desiredMove.x,
                             .y = state.playerPosition.y + state.desiredMove.y};

    switch (occupiedGrid[desiredPosition.y][desiredPosition.x]) {
        case OccupiedType::None:
            state.playerPosition = desiredPosition;
            break;
        case OccupiedType::Box: {
            Point boxPushPosition = {.x = desiredPosition.x + state.desiredMove.x,
                                     .y = desiredPosition.y + state.desiredMove.y};

            if (occupiedGrid[boxPushPosition.y][boxPushPosition.x] == OccupiedType::None) {
                auto box = std::ranges::find_if(state.boxPositions, [desiredPosition](const Point& box) {
                    return box.x == desiredPosition.x && box.y == desiredPosition.y;
                });
                box->x = boxPushPosition.x;
                box->y = boxPushPosition.y;
                state.playerPosition = desiredPosition;
            }
            break;
        }
        case OccupiedType::Wall:
            break;
    }

    state.desiredMove = {.x = 0, .y = 0};
}

void DrawLevelScene(const GameState& state) {
    ClearBackground(FLOOR_COLOR);

    if (state.level_state == LevelState::Finished) {
        DrawText("You did it!", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 20, BLACK);
        return;
    }

    for (size_t y = 0; y < state.levelConfiguration.layout.size(); y++) {
        const std::vector<FloorType> row = state.levelConfiguration.layout[y];

        for (size_t x = 0; x < row.size(); x++) {
            switch (row[x]) {
                case FloorType::Solid:
                    DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, WALL_COLOR);
                    break;
                case FloorType::Destination:
                    DrawCircle((x * TILE_SIZE) + (TILE_SIZE / 2), (y * TILE_SIZE) + (TILE_SIZE / 2), TILE_SIZE / 5.0,
                               DESTINATION_COLOR);
                    break;
                default:
                    break;
            }
        }
    }

    for (Point position : state.boxPositions) {
        Color boxColor = BOX_COLOR;
        if (state.levelConfiguration.layout[position.y][position.x] == FloorType::Destination) {
            boxColor = BOX_ON_DESTINATION_COLOR;
        }
        DrawRectangle(position.x * TILE_SIZE, position.y * TILE_SIZE, TILE_SIZE, TILE_SIZE, boxColor);
    }

    DrawRectangle(state.playerPosition.x * TILE_SIZE, state.playerPosition.y * TILE_SIZE, TILE_SIZE, TILE_SIZE,
                  PLAYER_COLOR);
}

void HandleLevelSceneInput(GameState& state) {
    if (IsKeyPressed(KEY_UP)) {
        state.desiredMove = {.x = 0, .y = -1};
    } else if (IsKeyPressed(KEY_DOWN)) {
        state.desiredMove = {.x = 0, .y = 1};
    } else if (IsKeyPressed(KEY_RIGHT)) {
        state.desiredMove = {.x = 1, .y = 0};
    } else if (IsKeyPressed(KEY_LEFT)) {
        state.desiredMove = {.x = -1, .y = 0};
    }

    if (IsKeyPressed(KEY_R)) { LoadLevel(state.levelConfiguration, state); };
}
