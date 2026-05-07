#include "core/point.hpp"

#include "core/asset_manager.hpp"
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
    ClearBackground(WHITE);

    if (state.level_state == LevelState::Finished) {
        DrawText("You did it!", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 20, BLACK);
        return;
    }

    // TODO: Center level
    for (size_t y = 0; y < state.levelConfiguration.layout.size(); y++) {
        const std::vector<FloorType> row = state.levelConfiguration.layout[y];

        for (size_t x = 0; x < row.size(); x++) {
            Texture2D sprite;
            switch (row[x]) {
                case FloorType::Solid: {
                    sprite = get_sprite("wall");
                    break;
                }
                case FloorType::Destination: {
                    sprite = get_sprite("destination");
                    break;
                }
                default:
                    sprite = get_sprite("floor");
                    break;
            }

            DrawTexturePro(sprite,
                           Rectangle{.x = 0,
                                     .y = 0,
                                     .width = static_cast<float>(sprite.width),
                                     .height = static_cast<float>(sprite.height)},
                           Rectangle{.x = static_cast<float>(x * TILE_SIZE),
                                     .y = static_cast<float>(y * TILE_SIZE),
                                     .width = TILE_SIZE,
                                     .height = TILE_SIZE},
                           Vector2{.x = 0, .y = 0}, 0, WHITE);
        }
    }

    for (Point position : state.boxPositions) {
        Texture2D sprite = get_sprite("box");
        if (state.levelConfiguration.layout[position.y][position.x] == FloorType::Destination) {
            sprite = get_sprite("box_on_destination");
        }

        DrawTexturePro(
            sprite,
            Rectangle{
                .x = 0, .y = 0, .width = static_cast<float>(sprite.width), .height = static_cast<float>(sprite.height)},
            Rectangle{.x = static_cast<float>(position.x * TILE_SIZE),
                      .y = static_cast<float>(position.y * TILE_SIZE),
                      .width = TILE_SIZE,
                      .height = TILE_SIZE},
            Vector2{.x = 0, .y = 0}, 0, WHITE);
    }

    const Texture2D sprite = get_sprite("player");
    DrawTexturePro(
        sprite,
        Rectangle{
            .x = 0, .y = 0, .width = static_cast<float>(sprite.width), .height = static_cast<float>(sprite.height)},
        Rectangle{.x = static_cast<float>(state.playerPosition.x * TILE_SIZE),
                  .y = static_cast<float>(state.playerPosition.y * TILE_SIZE),
                  .width = TILE_SIZE,
                  .height = TILE_SIZE},
        Vector2{.x = 0, .y = 0}, 0, WHITE);
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
