#pragma once

#include "raylib.h"

#include <cstring>
#include <string>
#include <unordered_map>

inline std::unordered_map<std::string, Texture2D> sprites;

void load_sprites();
void unload_sprites();

Texture2D get_sprite(const std::string& sprite_name);
