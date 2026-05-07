#include "asset_manager.hpp"
#include "raylib.h"

#include <cstring>
#include <filesystem>
#include <iostream>
#include <string>
#include <unordered_map>

void load_sprites() {
    const std::filesystem::path target_path{"res/sprites"};

    try {
        for (std::filesystem::directory_entry const& dir_entry : std::filesystem::directory_iterator{target_path}) {
            if (!std::filesystem::is_regular_file(dir_entry.path())) { continue; }

            const std::string sprite_name = strtok(dir_entry.path().filename().string().data(), ".");

            const Image image = LoadImage(dir_entry.path().string().data());
            const Texture2D texture = LoadTextureFromImage(image);
            UnloadImage(image);

            sprites.insert(std::make_pair(sprite_name, texture));
        }
    } catch (std::filesystem::filesystem_error const& ex) {
        std::cout << "Error occured during file operation!\n" << ex.what() << std::endl;
    }
}

Texture2D get_sprite(const std::string& sprite_name) {
    return sprites.find(sprite_name)->second;
}
