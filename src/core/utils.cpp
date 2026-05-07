#include "raylib.h"

bool mouse_in_rect(const Rectangle& rect) {
    const Vector2 mouse_pos = GetMousePosition();

    return mouse_pos.x > rect.x && mouse_pos.x < rect.x + rect.width && mouse_pos.y > rect.y &&
           mouse_pos.y < rect.y + rect.height;
}

void draw_button(const Rectangle& rect, const char* text, const int font_size) {
    const Color button_background = mouse_in_rect(rect) ? DARKGRAY : LIGHTGRAY;

    DrawRectangleRec(rect, button_background);
    DrawRectangleLinesEx(rect, 5, BLACK);

    int text_width = MeasureText(text, font_size);
    DrawText(text, rect.x + (rect.width / 2) - (text_width / 2), rect.y + (rect.height / 2) - (font_size / 2),
             font_size, RED);
}
