#include "raylib.h"

#include <cstring>
#include <string>
#include <unordered_map>

static std::unordered_map<std::string, Texture2D> sprites;

void load_sprites();

Texture2D get_sprite(const std::string& sprite_name);
