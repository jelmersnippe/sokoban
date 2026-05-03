#pragma once

#include "core/point.hpp"
#include <vector>

struct Size {
    int width = 0;
    int height = 0;
};

enum class FloorType {
    None,
    Solid,
    Destination,
};

struct Level {
    int index = 0;
    Size size = {.width = 0, .height = 0};
    std::vector<std::vector<FloorType>> layout = {{}};
    std::vector<Point> boxes;
    Point player = {.x = 0, .y = 0};
};

std::vector<Level> LoadLevels();
