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

const Rectangle test_rectangle = Rectangle{.x = static_cast<float>(BUTTON_POS.x),
                                           .y = static_cast<float>(BUTTON_POS.y + (BUTTON_HEIGHT + BUTTON_GAP) * 2),
                                           .width = BUTTON_WIDTH,
                                           .height = BUTTON_HEIGHT};

const Rectangle quit_rectangle = Rectangle{.x = static_cast<float>(BUTTON_POS.x),
                                           .y = static_cast<float>(BUTTON_POS.y + (BUTTON_HEIGHT + BUTTON_GAP) * 3),
                                           .width = BUTTON_WIDTH,
                                           .height = BUTTON_HEIGHT};

void InitMenuScene(GameState& state) {
    state.buttons.push_back(Button{.rect = level_select_rectangle,
                                   .text = "Level Select",
                                   .font_size = FONT_SIZE,
                                   .on_click = [](GameState& state) { change_scene(state, Scene::LevelSelect); }});
    state.buttons.push_back(
        Button{.rect = game_rectangle, .text = "Game", .font_size = FONT_SIZE, .on_click = [](GameState& state) {
                   change_scene(state, Scene::Level);
               }});
    state.buttons.push_back(
        Button{.rect = test_rectangle, .text = "Test", .font_size = FONT_SIZE, .on_click = [](GameState& state) {
                   change_scene(state, Scene::Test);
               }});
    state.buttons.push_back(
        Button{.rect = quit_rectangle, .text = "Quit", .font_size = FONT_SIZE, .on_click = [](GameState& state) {
                   state.should_exit = true;
               }});
}

void UpdateMenuScene(GameState& state) {}

void DrawMenuScene(const GameState& state) {
    ClearBackground(WHITE);
}

void HandleMenuSceneInput(GameState& state) {}
