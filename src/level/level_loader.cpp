#include "raylib.h"

#include "level_loader.hpp"
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <unordered_map>
#include <vector>

struct ColorHash {
    std::size_t operator()(const Color& c) const { return (c.r << 24) | (c.g << 16) | (c.b << 8) | c.a; }
};

struct ColorEqual {
    bool operator()(const Color& a, const Color& b) const {
        return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
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
    {Color{255, 255, 255, 255}, TileType::Floor},     {Color{0, 0, 0, 255}, TileType::Wall},
    {Color{34, 177, 76, 255}, TileType::Player},      {Color{185, 122, 87, 255}, TileType::Box},
    {Color{237, 28, 36, 255}, TileType::Destination}, {Color{66, 44, 31, 255}, TileType::BoxOnDestination},
};

Level parse_level_file(const std::string& filename) {
    Image image = LoadImage(filename.c_str());

    Level level{};
    level.size = {.width = image.width, .height = image.height};
    level.layout = std::vector(image.height, std::vector(image.width, FloorType::None));

    std::vector<std::vector<FloorType>> layout;

    Color* colors = LoadImageColors(image);

    for (int y = 0; y < image.height; y++) {
        for (int x = 0; x < image.width; x++) {
            // Colors is a straight array so we have to convert to row-oriented index
            const int i = y * image.width + x;
            const Color& color = colors[i];

            const auto it = color_map.find(color);

            if (it == color_map.end()) { throw "Invalid color found"; }

            switch (it->second) {
                case TileType::Floor:
                    break;
                case TileType::Wall:
                    level.layout[y][x] = FloorType::Solid;
                    break;
                case TileType::Player:
                    level.player = {.x = x, .y = y};
                    break;
                case TileType::Destination:
                    level.layout[y][x] = FloorType::Destination;
                    break;
                case TileType::BoxOnDestination:
                    level.layout[y][x] = FloorType::Destination;
                    level.boxes.push_back({.x = x, .y = y});
                    break;
                case TileType::Box:
                    level.boxes.push_back({.x = x, .y = y});
                    break;
            }
        }
    }

    UnloadImageColors(colors);
    UnloadImage(image);

    return level;
}

std::vector<Level> LoadLevels() {
    std::vector<Level> levels;
    const std::filesystem::path target_path{"res/levels"};

    try {
        for (std::filesystem::directory_entry const& dir_entry : std::filesystem::directory_iterator{target_path}) {
            if (!std::filesystem::is_regular_file(dir_entry.path())) { continue; }

            auto path = dir_entry.path();
            levels.push_back(parse_level_file(path.string()));
        }
    } catch (std::filesystem::filesystem_error const& ex) {
        std::cout << "Error occured during file operation!\n" << ex.what() << std::endl;
    }

    return levels;
}
