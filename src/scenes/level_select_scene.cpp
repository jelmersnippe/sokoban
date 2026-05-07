#include "core/globals.hpp"
#include "core/utils.hpp"
#include "game_state.hpp"
#include "level/level_loader.hpp"
#include "raylib.h"
#include <string>

const int FONT_SIZE = 30;

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

    return Rectangle{.x = posX, .y = posY, .width = drawW, .height = drawH};
}

void UpdateLevelSelectScene(GameState& state) {}

void DrawLevelSelectScene(const GameState& state) {
    ClearBackground(WHITE);

    if (state.level_selection_index >= levels.size()) return;

    const Level level = levels[state.level_selection_index];

    const Vector2 level_image_size = Vector2{.x = SCREEN_WIDTH * 0.6, .y = SCREEN_HEIGHT * 0.6};
    const Rectangle dest_rect = get_destination_rect(level.texture, level_image_size);
    const Vector2 position = {.x = static_cast<float>((SCREEN_WIDTH - level_image_size.x) / 2.0 + dest_rect.x),
                              .y = static_cast<float>((SCREEN_HEIGHT - level_image_size.y) / 2.0 + dest_rect.y)};

    DrawTexturePro(level.texture,
                   Rectangle{.x = 0,
                             .y = 0,
                             .width = static_cast<float>(level.texture.width),
                             .height = static_cast<float>(level.texture.height)},
                   Rectangle{.x = position.x, .y = position.y, .width = dest_rect.width, .height = dest_rect.height},
                   Vector2{.x = 0, .y = 0}, 0.0f, WHITE);

    // TODO: HOok up functionality
    draw_button(Rectangle{.x = SCREEN_WIDTH / 2 - 150,
                          .y = position.y + level_image_size.y + (FONT_SIZE / 2),
                          .width = 50,
                          .height = FONT_SIZE},
                "<", FONT_SIZE);

    const std::string level_name = "Level " + std::to_string(level.index + 1);

    int text_width = MeasureText(level_name.c_str(), FONT_SIZE);
    DrawText(level_name.c_str(), (SCREEN_WIDTH / 2.0) - (text_width / 2),
             position.y + level_image_size.y + FONT_SIZE - (FONT_SIZE / 2), FONT_SIZE, BLACK);

    // TODO: HOok up functionality
    draw_button(Rectangle{.x = SCREEN_WIDTH / 2 + 100,
                          .y = position.y + level_image_size.y + (FONT_SIZE / 2),
                          .width = 50,
                          .height = FONT_SIZE},
                ">", FONT_SIZE);
}

void HandleLevelSelectSceneInput(GameState& state) {
    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { return; }

    if (state.level_selection_index >= levels.size()) return;

    const Level level = levels[state.level_selection_index];

    const Rectangle dest_rect =
        get_destination_rect(level.texture, Vector2{.x = SCREEN_WIDTH * 0.6, .y = SCREEN_HEIGHT * 0.6});

    if (!mouse_in_rect(dest_rect)) return;

    LoadLevel(level, state);

    state.scene = Scene::Level;
}
