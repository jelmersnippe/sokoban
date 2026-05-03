#include "core/point.hpp"
#include "level/level_loader.hpp"
#include "raylib.h"

#include <algorithm>
#include <vector>

const int TARGET_FPS = 60;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int TICKS_PER_SECOND = 60;
const float TICK_TIME = 1.0 / TICKS_PER_SECOND;

const int TILE_SIZE = 50;

const Color WALL_COLOR = {.r = 90, .g = 90, .b = 90, .a = 255};
const Color FLOOR_COLOR = {.r = 170, .g = 150, .b = 100, .a = 255};
const Color BOX_COLOR = {.r = 185, .g = 115, .b = 40, .a = 255};
const Color BOX_ON_DESTINATION_COLOR = {.r = 85, .g = 55, .b = 20, .a = 255};
const Color DESTINATION_COLOR = RED;
const Color PLAYER_COLOR = GREEN;

std::vector<Point> GetDestinations(const Level& level) {
    std::vector<Point> destinations;

    for (int y = 0; y < level.layout.size(); y++) {
        const std::vector<FloorType> row = level.layout[y];

        for (int x = 0; x < row.size(); x++) {
            if (row[x] == FloorType::Destination) { destinations.push_back({.x = x, .y = y}); };
        }
    }

    return destinations;
};

const Level level1 = {
    .index = 0,
    .size = {.width = 8, .height = 9},
    .layout =
        {
            {FloorType::None, FloorType::None, FloorType::Solid, FloorType::Solid, FloorType::Solid, FloorType::Solid,
             FloorType::Solid, FloorType::None},
            {FloorType::Solid, FloorType::Solid, FloorType::Solid, FloorType::None, FloorType::None, FloorType::None,
             FloorType::Solid, FloorType::None},
            {FloorType::Solid, FloorType::Destination, FloorType::None, FloorType::None, FloorType::None,
             FloorType::None, FloorType::Solid, FloorType::None},
            {FloorType::Solid, FloorType::Solid, FloorType::Solid, FloorType::None, FloorType::None,
             FloorType::Destination, FloorType::Solid, FloorType::None},
            {FloorType::Solid, FloorType::Destination, FloorType::Solid, FloorType::Solid, FloorType::None,
             FloorType::None, FloorType::Solid, FloorType::None},
            {FloorType::Solid, FloorType::None, FloorType::Solid, FloorType::None, FloorType::Destination,
             FloorType::None, FloorType::Solid, FloorType::Solid},
            {FloorType::Solid, FloorType::None, FloorType::None, FloorType::Destination, FloorType::None,
             FloorType::None, FloorType::Destination, FloorType::Solid},
            {FloorType::Solid, FloorType::None, FloorType::None, FloorType::None, FloorType::Destination,
             FloorType::None, FloorType::None, FloorType::Solid},
            {FloorType::Solid, FloorType::Solid, FloorType::Solid, FloorType::Solid, FloorType::Solid, FloorType::Solid,
             FloorType::Solid, FloorType::Solid},
        },
    .boxes =
        {
            {.x = 3, .y = 2},
            {.x = 4, .y = 3},
            {.x = 4, .y = 4},
            {.x = 1, .y = 6},
            {.x = 3, .y = 6},
            {.x = 4, .y = 6},
            {.x = 5, .y = 6},
        },
    .player = {.x = 2, .y = 2},
};

struct GameState {
    float timeSinceLastTick = 0;
    Level levelConfiguration;
    std::vector<Point> destinations;
    std::vector<Point> boxPositions;
    Point playerPosition;

    Point desiredMove = {.x = 0, .y = 0};
};

enum class OccupiedType {
    None,
    Wall,
    Box
};

std::vector<std::vector<OccupiedType>> GetOccupiedGrid(const Level& level, const std::vector<Point>& boxes) {
    std::vector<std::vector<OccupiedType>> occupiedGrid(
        level.size.height, std::vector<OccupiedType>(level.size.width, OccupiedType::None));

    for (int y = 0; y < level.layout.size(); y++) {
        const std::vector<FloorType> row = level.layout[y];

        for (int x = 0; x < row.size(); x++) {
            if (row[x] == FloorType::Solid) { occupiedGrid[y][x] = OccupiedType::Wall; }
        }
    }

    for (Point position : boxes) {
        occupiedGrid[position.y][position.x] = OccupiedType::Box;
    }

    return occupiedGrid;
}

void Update(GameState& state) {
    // All boxes are on a destination
    if (std::ranges::all_of(state.boxPositions, [&state](Point& box) {
            return std::ranges::any_of(state.destinations, [&box](Point& destination) {
                return box.x == destination.x && box.y == destination.y;
            });
        })) {
        DrawText("You did it!", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 20, BLACK);
        return;
    }

    state.timeSinceLastTick += GetFrameTime();

    if (state.timeSinceLastTick >= TICK_TIME) {
        std::vector<std::vector<OccupiedType>> occupiedGrid =
            GetOccupiedGrid(state.levelConfiguration, state.boxPositions);
        state.timeSinceLastTick -= TICK_TIME;

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
};

void Draw(const GameState& state) {
    BeginDrawing();

    ClearBackground(FLOOR_COLOR);

    for (int y = 0; y < state.levelConfiguration.layout.size(); y++) {
        const std::vector<FloorType> row = state.levelConfiguration.layout[y];

        for (int x = 0; x < row.size(); x++) {
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

    EndDrawing();
}

void LoadLevel(const Level& level, GameState& state) {
    state.levelConfiguration = level;
    state.boxPositions = level.boxes;
    state.destinations = GetDestinations(level);
    state.playerPosition = level.player;
    state.desiredMove = {.x = 0, .y = 0};
    state.timeSinceLastTick = 0;
};

void HandleInput(GameState& state) {
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

int main() {
    std::vector<Level> levels = LoadLevels();

    GameState state;
    LoadLevel(level1, state);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sokoban");

    SetTargetFPS(TARGET_FPS);

    while (!WindowShouldClose()) {
        HandleInput(state);

        Update(state);

        Draw(state);
    }

    CloseWindow();

    return 0;
}
