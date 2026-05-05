#include "core/globals.hpp"
#include "core/point.hpp"
#include "core/utils.hpp"

#include "game_state.hpp"
#include "raylib.h"

const int BUTTON_HEIGHT = 40;
const int BUTTON_WIDTH = 200;
const int BUTTON_GAP = 50;
const int FONT_SIZE = 20;

const Point BUTTON_POS = {.x = SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2, .y = SCREEN_HEIGHT / 2 - BUTTON_HEIGHT / 2};
const Rectangle level_select_rectangle = Rectangle{.x = static_cast<float>(BUTTON_POS.x),
                                                   .y = static_cast<float>(BUTTON_POS.y),
                                                   .width = BUTTON_WIDTH,
                                                   .height = BUTTON_HEIGHT};

const Rectangle game_rectangle = Rectangle{.x = static_cast<float>(BUTTON_POS.x),
                                           .y = static_cast<float>(BUTTON_POS.y + BUTTON_HEIGHT + BUTTON_GAP),
                                           .width = BUTTON_WIDTH,
                                           .height = BUTTON_HEIGHT};

const Rectangle quit_rectangle = Rectangle{.x = static_cast<float>(BUTTON_POS.x),
                                           .y = static_cast<float>(BUTTON_POS.y + (BUTTON_HEIGHT + BUTTON_GAP) * 2),
                                           .width = BUTTON_WIDTH,
                                           .height = BUTTON_HEIGHT};

void draw_button(const Rectangle& rect, const char* text) {
    const Color button_background = mouse_in_rect(rect) ? DARKGRAY : LIGHTGRAY;

    DrawRectangleRec(rect, button_background);
    DrawRectangleLinesEx(rect, 5, BLACK);

    int text_width = MeasureText(text, FONT_SIZE);
    DrawText(text, rect.x + (rect.width / 2) - (text_width / 2), rect.y + (rect.height / 2) - (FONT_SIZE / 2),
             FONT_SIZE, RED);
}

void UpdateMenuScene(GameState& state) {}

void DrawMenuScene(const GameState& state) {
    ClearBackground(WHITE);

    draw_button(level_select_rectangle, "Level Select");
    draw_button(game_rectangle, "Game");
    draw_button(quit_rectangle, "Quit");
}

void HandleMenuSceneInput(GameState& state) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (mouse_in_rect(level_select_rectangle)) {
            state.scene = Scene::LevelSelect;
        } else if (mouse_in_rect(game_rectangle)) {
            state.scene = Scene::Level;
        } else if (mouse_in_rect(quit_rectangle)) {
            state.should_exit = true;
        }
    }
}
