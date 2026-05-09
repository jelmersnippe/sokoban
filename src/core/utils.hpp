#pragma once

#include "raylib.h"
#include <string>

//Define the macro for assert
#define ASSERT(condition, message) \
do { \
assert(condition && #message); \
} while (0)

class GameState;

enum class ButtonState {
    None,
    Hover,
    Active
};

struct Button {
    Rectangle rect{};
    std::string text = "Placeholder";
    int font_size = 14;
    ButtonState state = ButtonState::None;
    void (*on_click)(GameState&){};
};

bool point_in_rect(const Vector2& point, const Rectangle& rect);
void draw_button(const Button& button);
