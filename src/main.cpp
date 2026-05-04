#include "core/globals.hpp"
#include "game_state.hpp"
#include "level/level_loader.hpp"
#include "raylib.h"
#include "scenes/level_scene.hpp"

#include <algorithm>
#include <vector>

void Update(GameState& state) {
    state.timeSinceLastTick += GetFrameTime();

    while (state.timeSinceLastTick >= TICK_TIME) {
        // TODO: Call updates
        UpdateLevelScene(state);

        state.timeSinceLastTick -= TICK_TIME;
    }
};

void Draw(const GameState& state) {
    BeginDrawing();

    DrawLevelScene(state);

    // TODO: Call draws
    EndDrawing();
}

void HandleInput(GameState& state) {
    // TODO: Call input handling
    HandleLevelSceneInput(state);
}

int main() {
    std::vector<Level> levels = LoadLevels();

    const int index = 0;
    auto it = std::ranges::find_if(levels, [](Level& level) { return level.index == index; });

    GameState state;
    LoadLevel(*it, state);

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
