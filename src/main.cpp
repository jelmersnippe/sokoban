#include "core/globals.hpp"
#include "game_state.hpp"
#include "raylib.h"
#include "scenes/level_scene.hpp"
#include "scenes/level_select_scene.hpp"
#include "scenes/menu_scene.hpp"
#include "scenes/scenes.hpp"

void Update(GameState& state) {
    state.timeSinceLastTick += GetFrameTime();

    while (state.timeSinceLastTick >= TICK_TIME) {
        switch (state.scene) {
            case Scene::LevelSelect:
                UpdateLevelSelectScene(state);
                break;
            case Scene::Menu:
                UpdateMenuScene(state);
                break;
            case Scene::Level:
                UpdateLevelScene(state);
                break;
        }

        state.timeSinceLastTick -= TICK_TIME;
    }
};

void Draw(const GameState& state) {
    BeginDrawing();

    switch (state.scene) {
        case Scene::LevelSelect:
            DrawLevelSelectScene(state);
            break;
        case Scene::Menu:
            DrawMenuScene(state);
            break;
        case Scene::Level:
            DrawLevelScene(state);
            break;
    }

    EndDrawing();
}

void HandleInput(GameState& state) {
    if (IsKeyPressed(KEY_ESCAPE)) {
        state.scene = Scene::Menu;
        return;
    }

    switch (state.scene) {
        case Scene::LevelSelect:
            HandleLevelSelectSceneInput(state);
            break;
        case Scene::Menu:
            HandleMenuSceneInput(state);
            break;
        case Scene::Level:
            HandleLevelSceneInput(state);
            break;
    }
}

int main() {
    GameState state;
    state.scene = Scene::LevelSelect;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sokoban");
    SetExitKey(KEY_NULL);

    SetTargetFPS(TARGET_FPS);

    while (!state.should_exit && !WindowShouldClose()) {
        HandleInput(state);

        Update(state);

        Draw(state);
    }

    CloseWindow();

    return 0;
}
