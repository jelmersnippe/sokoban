#include "core/asset_manager.hpp"
#include "core/globals.hpp"
#include "core/utils.hpp"
#include "game_state.hpp"
#include "raylib.h"
#include "scenes/level_scene.hpp"
#include "scenes/level_select_scene.hpp"
#include "scenes/menu_scene.hpp"
#include "scenes/scenes.hpp"
#include "scenes/test_scene.hpp"

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
            case Scene::Test:
                UpdateTestScene(state);
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
        case Scene::Test:
            DrawTestScene(state);
            break;
    }

    for (const Button& button : state.buttons) {
        draw_button(button);
    }

    EndDrawing();
}

void HandleInput(GameState& state) {
    if (IsKeyPressed(KEY_ESCAPE)) {
        change_scene(state, Scene::Menu);
        return;
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        const Vector2 mouse_pos = GetMousePosition();
        for (const Button& button : state.buttons) {
            if (!point_in_rect(mouse_pos, button.rect)) continue;

            button.on_click(state);
            // Button can change Scene, which then updates in the same tick.
            // Meaning you could get a click-through (IsMouseButtonPressed is still true) on the new scene.
            // This happens when you go to level select, because the level ui is behind the button.
            return;
        }
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
        case Scene::Test:
            HandleLevelSceneInput(state);
            break;
    }
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sokoban");
    SetTargetFPS(TARGET_FPS);
    SetExitKey(KEY_NULL);

    load_levels();
    load_sprites();

    GameState state;
    change_scene(state, Scene::Menu);

    while (!state.should_exit && !WindowShouldClose()) {
        HandleInput(state);

        Update(state);

        Draw(state);
    }

    CloseWindow();

    return 0;
}
