#include "raylib.h"

bool mouse_in_rect(const Rectangle& rect) {
    const Vector2 mouse_pos = GetMousePosition();

    return mouse_pos.x > rect.x && mouse_pos.x < rect.x + rect.width && mouse_pos.y > rect.y &&
           mouse_pos.y < rect.y + rect.height;
}
