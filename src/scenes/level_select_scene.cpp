#include "core/globals.hpp"
#include "core/utils.hpp"
#include "game_state.hpp"
#include "level/level_loader.hpp"
#include "raylib.h"
#include "scenes/scenes.hpp"
#include <string>

const int FONT_SIZE = 30;
const Vector2 level_image_size = Vector2{.x = SCREEN_WIDTH * 0.6, .y = SCREEN_HEIGHT * 0.6};
const float ui_height = (SCREEN_WIDTH / 2) + (level_image_size.y / 2);

const Rectangle level_display_box = Rectangle{.x = (SCREEN_WIDTH / 2) - (level_image_size.x / 2),
                                              .y = (SCREEN_HEIGHT / 2) - (level_image_size.y / 2),
                                              .width = level_image_size.x,
                                              .height = level_image_size.y};

const int LEVEL_DISPLAY_BOX_PADDING = 30;

// TODO: Create container wrapper so outer click area is always same size and can be boxed (RectangleLines)
Rectangle get_destination_rect(const Texture2D texture, const Vector2 size) {
    auto texW = static_cast<float>(texture.width);
    auto texH = static_cast<float>(texture.height);

    float boxW = size.x;
    float boxH = size.y;

    // Aspect ratios
    float texAspect = texW / texH;
    float boxAspect = boxW / boxH;

    // Final size (preserving aspect ratio)
    float drawW = 0;
    float drawH = 0;

    if (texAspect > boxAspect) {
        // Texture is wider than the box → fit width
        drawW = boxW;
        drawH = boxW / texAspect;
    } else {
        // Texture is taller → fit height
        drawH = boxH;
        drawW = boxH * texAspect;
    }

    // Optional: center inside the box
    float posX = (boxW - drawW) / 2.0f;
    float posY = (boxH - drawH) / 2.0f;

    return Rectangle{.x = posX + LEVEL_DISPLAY_BOX_PADDING / 2,
                     .y = posY + LEVEL_DISPLAY_BOX_PADDING / 2,
                     .width = drawW,
                     .height = drawH};
}

void InitLevelSelectScene(GameState& state) {
    state.buttons.push_back(
        Button{.rect = Rectangle{.x = SCREEN_WIDTH / 2 - 150, .y = ui_height, .width = 50, .height = FONT_SIZE},
               .text = "<",
               .font_size = FONT_SIZE,
               .on_click = [](GameState& state) {
                   if (state.level_selection_index <= 0) return;
                   state.level_selection_index--;
               }});

    state.buttons.push_back(
        Button{.rect = Rectangle{.x = SCREEN_WIDTH / 2 + 100, .y = ui_height, .width = 50, .height = FONT_SIZE},
               .text = ">",
               .font_size = FONT_SIZE,
               .on_click = [](GameState& state) {
                   if (state.level_selection_index >= levels.size() - 1) return;
                   state.level_selection_index++;
               }});
}

void UpdateLevelSelectScene(GameState& state) {}

void DrawLevelSelectScene(const GameState& state) {
    ClearBackground(WHITE);

    if (state.level_selection_index >= levels.size()) return;

    // TODO: Reuse level drawing with sprites
    const Level level = levels[state.level_selection_index];

    const Rectangle dest_rect =
        get_destination_rect(level.texture, {.x = level_image_size.x - LEVEL_DISPLAY_BOX_PADDING,
                                             .y = level_image_size.y - LEVEL_DISPLAY_BOX_PADDING});
    const Vector2 position = {.x = static_cast<float>((SCREEN_WIDTH - level_image_size.x) / 2.0 + dest_rect.x),
                              .y = static_cast<float>((SCREEN_HEIGHT - level_image_size.y) / 2.0 + dest_rect.y)};

    DrawRectangleLines(level_display_box.x, level_display_box.y, level_display_box.width, level_display_box.height,
                       BLACK);
    DrawTexturePro(level.texture,
                   Rectangle{.x = 0,
                             .y = 0,
                             .width = static_cast<float>(level.texture.width),
                             .height = static_cast<float>(level.texture.height)},
                   Rectangle{.x = position.x, .y = position.y, .width = dest_rect.width, .height = dest_rect.height},
                   Vector2{.x = 0, .y = 0}, 0.0f, WHITE);

    const std::string level_name = "Level " + std::to_string(level.index + 1);

    int text_width = MeasureText(level_name.c_str(), FONT_SIZE);
    DrawText(level_name.c_str(), (SCREEN_WIDTH / 2.0) - (text_width / 2), ui_height, FONT_SIZE, BLACK);
}

void HandleLevelSelectSceneInput(GameState& state) {
    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { return; }

    if (state.level_selection_index >= levels.size()) return;

    if (!point_in_rect(GetMousePosition(), level_display_box)) return;

    const Level level = levels[state.level_selection_index];
    LoadLevel(level, state);

    change_scene(state, Scene::Level);
}
