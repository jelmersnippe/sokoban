#include "test_scene.hpp"
#include "raylib.h";

const Texture2D* tilemap_texture = nullptr;
void UpdateTestScene(GameState& state) {
    if (tilemap_texture != nullptr) return;

    const Image tilemap = LoadImage("res/sprites/tileset.png");
    const Texture2D tilemap_text = LoadTextureFromImage(tilemap);
    tilemap_texture = &tilemap_text;
    UnloadImage(tilemap);
}

void DrawTestScene(const GameState& state) {
    ClearBackground(WHITE);

    if (tilemap_texture == nullptr) return;

    DrawTexture(*tilemap_texture, 50, 50, WHITE);
}

void HandleTestSceneInput(GameState& state) {}
