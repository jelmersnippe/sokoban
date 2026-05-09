#include "utils.hpp"

#include "raylib.h"

bool point_in_rect(const Vector2& point, const Rectangle& rect) {
    return point.x > rect.x && point.x < rect.x + rect.width && point.y > rect.y && point.y < rect.y + rect.height;
}

void draw_button(const Button& button) {
    const Color button_background = point_in_rect(GetMousePosition(), button.rect) ? DARKGRAY : LIGHTGRAY;

    DrawRectangleRec(button.rect, button_background);
    DrawRectangleLinesEx(button.rect, 5, BLACK);

    int text_width = MeasureText(button.text.c_str(), button.font_size);
    DrawText(button.text.c_str(), button.rect.x + (button.rect.width / 2) - (text_width / 2),
             button.rect.y + (button.rect.height / 2) - (button.font_size / 2), button.font_size, RED);
}
