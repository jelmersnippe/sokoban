#include "raylib.h"

#include "level_loader.hpp"
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <ios>
#include <iostream>
#include <unordered_map>
#include <vector>

struct ColorHash {
    std::size_t operator()(const Color& c) const {
        return (c.r << 24) | (c.g << 16) | (c.b << 8) | c.a;
    }
};

struct ColorEqual {
    bool operator()(const Color& a, const Color& b) const {
        return a.r == b.r &&
               a.g == b.g &&
               a.b == b.b &&
               a.a == b.a;
    }
};

enum class TileType {
    Floor,
    Wall,
    Player,
    Box,
    Destination,
    BoxOnDestination
};

static const std::unordered_map<Color, TileType, ColorHash, ColorEqual> color_map = {
    {Color{255,255,255,255}, TileType::Floor},
    {Color{0,0,0,255}, TileType::Wall},
    {Color{34,177,76,255}, TileType::Player},
    {Color{185,122,87,255}, TileType::Box},
    {Color{237,28,36,255}, TileType::Destination},
    {Color{66,44,31,255}, TileType::BoxOnDestination},
};

Level parse_level_file(const std::string& filename) {
    Image image = LoadImage(filename.c_str());
    const int image_size = image.width * image.height;

    Color* colors = LoadImageColors(image);

    for (int i = 0; i < image_size; i++) {
        Color color = colors[i];

       const FloorType type = color_map.find(color);
    }

    UnloadImageColors(colors);
    UnloadImage(image); // Once image has been converted to texture and uploaded to VRAM, it can be unloaded from RAM
}

std::vector<Level> LoadLevels() {
    const std::filesystem::path target_path{"res/levels"};

    try {
        for (std::filesystem::directory_entry const& dir_entry : std::filesystem::directory_iterator{target_path}) {
            if (!std::filesystem::is_regular_file(dir_entry.path())) { continue; }

            auto path = dir_entry.path();
            parse_level_file(path.string());
        }
    } catch (std::filesystem::filesystem_error const& ex) {
        std::cout << "Error occured during file operation!\n" << ex.what() << std::endl;
    }

    return {};
}
