#include "core/utils.hpp"
#include "game_state.hpp"
#include "raylib.h"
#include <algorithm>
#include <string>

static std::vector<Level> levels;
static std::vector<Rectangle> level_positions;

const Vector2 LEVEL_IMAGE_SIZE = Vector2{.x = 100.0, .y = 50.0};

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

void UpdateLevelSelectScene(GameState& state) {
    if (!levels.empty()) return;

    levels = LoadLevels();
    std::ranges::sort(levels, std::ranges::less{}, &Level::index);

    int i = 0;
    for (const Level& level : levels) {

        auto dest_rect = get_destination_rect(level.texture, LEVEL_IMAGE_SIZE);
        auto dest_rect_with_pos = Rectangle{.x = 50 + (200 * i) + dest_rect.x,
                                            .y = 50 + dest_rect.y,
                                            .width = dest_rect.width,
                                            .height = dest_rect.height};
        level_positions.push_back(dest_rect_with_pos);

        i++;
    }
}

void DrawLevelSelectScene(const GameState& state) {
    ClearBackground(WHITE);

    int i = 0;
    // TODO: Parse this AI poop
    for (const Level& level : levels) {
        auto dest_rect = level_positions[i];

        DrawTexturePro(level.texture,
                       Rectangle{.x = 0,
                                 .y = 0,
                                 .width = static_cast<float>(level.texture.width),
                                 .height = static_cast<float>(level.texture.height)},
                       dest_rect, Vector2{.x = 0, .y = 0}, 0.0f, WHITE);
        DrawText(("Level " + std::to_string(i + 1)).c_str(), dest_rect.x, dest_rect.y + dest_rect.height + 10, 14,
                 BLACK);
        i++;
    }
}

void HandleLevelSelectSceneInput(GameState& state) {
    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { return; }

    int i = 0;
    for (const Rectangle& rect : level_positions) {
        if (!mouse_in_rect(rect)) {
            i++;
            continue;
        };

        const auto it = std::ranges::find_if(levels, [i](const Level& level) { return level.index == i; });

        LoadLevel(*it, state);

        state.scene = Scene::Level;

        break;
    }
}
