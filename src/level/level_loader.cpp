#include "raylib.h"

#include "level_loader.hpp"
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <ios>
#include <iostream>
#include <vector>

namespace fs = std::filesystem;

void parse_level_file(const std::string& filename) {
    Image image = LoadImage(filename.c_str()); // Loaded in CPU memory (RAM)
    const int image_size = image.width * image.height;

    Color* colors = LoadImageColors(image);

    for (int i = 0; i < image_size; i++) {
        const Color color = colors[i];

        std::cout << "Found color: (" << color.r << ", " << color.g << ", " << color.b << ", " << color.a << ")"
                  << std::endl;
    }

    UnloadImageColors(colors);
    UnloadImage(image); // Once image has been converted to texture and uploaded to VRAM, it can be unloaded from RAM
}

std::vector<Level> LoadLevels() {
    const fs::path target_path{"./levels"};

    try {
        for (std::filesystem::directory_entry const& dir_entry : fs::directory_iterator{target_path}) {
            if (!fs::is_regular_file(dir_entry.path())) { continue; }

            auto file = dir_entry.path();
            auto filename = file.filename();
            std::cout << filename << std::endl;
            std::cout << filename.root_path() << std::endl;
            parse_level_file(filename);
        }
    } catch (fs::filesystem_error const& ex) {
        std::cout << "Error occured during file operation!\n" << ex.what() << std::endl;
    }

    return {};
}
