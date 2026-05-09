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
        PlaySound(get_sound("celebrate"));
        state.level_state = LevelState::Finished;

        if (state.level_configuration.index < levels.size() - 1) {
            state.buttons.push_back(
                Button{.rect = {.x = SCREEN_WIDTH / 2 - 100, .y = SCREEN_HEIGHT / 2 + 100, .width = 200, .height = 50},
                       .text = "Next level",
                       .font_size = 20,
                       .on_click = [](GameState& state) {
                           change_scene(state, Scene::Level);
                           LoadLevel(levels[state.level_configuration.index + 1], state);
                       }});
        }
        return;
    }

    std::vector<std::vector<OccupiedType>> occupiedGrid =
        GetOccupiedGrid(state.level_configuration, state.boxPositions);

    Point desiredPosition = {.x = state.playerPosition.x + state.desiredMove.x,
                             .y = state.playerPosition.y + state.desiredMove.y};

    switch (occupiedGrid[desiredPosition.y][desiredPosition.x]) {
        case OccupiedType::None:
            PlaySound(get_sound("walk"));
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

                PlaySound(get_sound("box_push"));

                if (state.level_configuration.layout[boxPushPosition.y][boxPushPosition.y] == FloorType::Destination) {
                    PlaySound(get_sound("box_connect"));
                }
            } else {
                PlaySound(get_sound("fail"));
            }
            break;
        }
        case OccupiedType::Wall:
            PlaySound(get_sound("fail"));
            break;
    }

    state.desiredMove = {.x = 0, .y = 0};
}

void DrawLevelScene(const GameState& state) {
    ClearBackground(WHITE);

    if (state.level_state == LevelState::Finished) {
        const int width = MeasureText("You did it!", 20);
        DrawText("You did it!", SCREEN_WIDTH / 2 - width / 2, SCREEN_HEIGHT / 2, 20, BLACK);
        return;
    }

    const int offset_x = (SCREEN_WIDTH / 2) - (state.level_configuration.size.width * TILE_SIZE / 2);
    const int offset_y = (SCREEN_HEIGHT / 2) - (state.level_configuration.size.height * TILE_SIZE / 2);
    const Vector2 offset = {.x = static_cast<float>(offset_x), .y = static_cast<float>(offset_y)};

    for (size_t y = 0; y < state.level_configuration.layout.size(); y++) {
        const std::vector<FloorType> row = state.level_configuration.layout[y];

        for (size_t x = 0; x < row.size(); x++) {
            Texture2D sprite;
            switch (row[x]) {
                case FloorType::Solid: {
                    // Tile above is also a wall
                    if (y != state.level_configuration.layout.size() - 1 &&
                        state.level_configuration.layout[y + 1][x] == FloorType::Solid) {
                        sprite = get_sprite("wall_2");
                        break;
                    }

                    sprite = get_sprite("wall");
                    break;
                }
                case FloorType::Destination: {
                    sprite = get_sprite("destination");
                    break;
                }
                default:
                    // Tile above is also a wall
                    if (x != row.size() - 1 && row[x + 1] == FloorType::Solid) {
                        sprite = get_sprite("floor_shadow");
                        break;
                    }
                    sprite = get_sprite("floor");
                    break;
            }

            DrawTexturePro(sprite,
                           Rectangle{.x = 0,
                                     .y = 0,
                                     .width = static_cast<float>(sprite.width),
                                     .height = static_cast<float>(sprite.height)},
                           Rectangle{.x = static_cast<float>(x * TILE_SIZE) + offset.x,
                                     .y = static_cast<float>(y * TILE_SIZE) + offset.y,
                                     .width = TILE_SIZE,
                                     .height = TILE_SIZE},
                           Vector2{.x = 0, .y = 0}, 0, WHITE);
        }
    }

    for (Point position : state.boxPositions) {
        Texture2D sprite = get_sprite("box");
        if (state.level_configuration.layout[position.y][position.x] == FloorType::Destination) {
            sprite = get_sprite("box_on_destination");
        }

        DrawTexturePro(
            sprite,
            Rectangle{
                .x = 0, .y = 0, .width = static_cast<float>(sprite.width), .height = static_cast<float>(sprite.height)},
            Rectangle{.x = static_cast<float>(position.x * TILE_SIZE) + offset.x,
                      .y = static_cast<float>(position.y * TILE_SIZE) + offset.y,
                      .width = TILE_SIZE,
                      .height = TILE_SIZE},
            Vector2{.x = 0, .y = 0}, 0, WHITE);
    }

    const Texture2D sprite = get_sprite("player");
    DrawTexturePro(
        sprite,
        Rectangle{
            .x = 0, .y = 0, .width = static_cast<float>(sprite.width), .height = static_cast<float>(sprite.height)},
        Rectangle{.x = static_cast<float>(state.playerPosition.x * TILE_SIZE) + offset.x,
                  .y = static_cast<float>(state.playerPosition.y * TILE_SIZE) + offset.y,
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

    if (IsKeyPressed(KEY_R)) {
        PlaySound(get_sound("fail"));
        LoadLevel(state.level_configuration, state);
    };
}
