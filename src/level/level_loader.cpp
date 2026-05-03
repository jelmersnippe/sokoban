#include "level_loader.hpp"
#include <filesystem>
#include <format>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

std::vector<Level> LoadLevels() {
    const fs::path target_path{"./levels"};

    try {
        for (std::filesystem::directory_entry const& dir_entry : fs::directory_iterator{target_path}) {
            if (fs::is_regular_file(dir_entry.path())) {
                std::cout << dir_entry.path().filename().string() << std::endl;
            }
        }
    } catch (fs::filesystem_error const& ex) {
        std::cout << "Error occured during file operation!\n" << ex.what() << std::endl;
    }

    return {};
}
