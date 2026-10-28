#include <raylib.h>
#include <Engine/GameLoop.hpp>
#include <ECS/Component.hpp>
#include <filesystem>
#include <string>
#include <unordered_map>
#include "Components.hpp"
#include "PacmanGame.hpp"

using Textures = std::unordered_map<std::string, Texture2D>;

static Textures LoadGameTextures()
{
    const std::filesystem::path assetsPath = "Assets/Pacman";
    const std::unordered_map<std::string, std::string> files = {
        {"wall", "Cube.bmp"},
        {"pacgum", "Pacgum.bmp"},
        {"superPacgum", "SuperPacgum.bmp"},
        {"pacman", "Pacman.bmp"},
        {"ghost-0", "Ghost0.bmp"},
        {"ghost-1", "Ghost1.bmp"},
        {"ghost-2", "Ghost2.bmp"},
        {"ghost-3", "Ghost3.bmp"}
    };
    Textures textures;
    for (const auto &[id, file] : files)
        textures[id] = LoadTexture((assetsPath / file).string().c_str());
    return textures;
}

static void UnloadGameTextures(const Textures &textures)
{
    for (const auto &[id, texture] : textures)
        UnloadTexture(texture);
}

static void ProcessInput(PacmanGame &game)
{
    if (IsKeyDown(KEY_UP))
        game.SetDirection(Direction::UP);
    if (IsKeyDown(KEY_DOWN))
        game.SetDirection(Direction::DOWN);
    if (IsKeyDown(KEY_LEFT))
        game.SetDirection(Direction::LEFT);
    if (IsKeyDown(KEY_RIGHT))
        game.SetDirection(Direction::RIGHT);
}

static Rectangle CellRect(int x, int y)
{
    constexpr float cellSize = 18.0f;

    return {
        static_cast<float>(x) * cellSize + 40.0f,
        static_cast<float>(y) * cellSize + 20.0f,
        cellSize,
        cellSize
    };
}

static void DrawSprite(const Texture2D &texture, Rectangle dest)
{
    DrawTexturePro(texture, {0, 0, (float)texture.width, (float)texture.height}, dest, {0, 0}, 0.0f, WHITE);
}

static void DrawTile(BoxType type, Rectangle dest, const Textures &tex)
{
    switch (type) {
        case BoxType::CUBE:
        case BoxType::CELLP:
            DrawSprite(tex.at("wall"), dest);
            break;
        case BoxType::PACGUM:
            DrawSprite(tex.at("pacgum"), dest);
            break;
        case BoxType::SUPERFOOD:
            DrawSprite(tex.at("superPacgum"), dest);
            break;
        case BoxType::EMPTY:
            break;
    }
}

static void RenderGame(PacmanGame &game, const Textures &tex)
{
    BeginDrawing();
    ClearBackground(BLACK);

    const auto &map = game.GetMap();

    for (std::size_t y = 0; y < map.size(); y++) {
        for (std::size_t x = 0; x < map[y].size(); x++)
            DrawTile(map[y][x], CellRect(static_cast<int>(x), static_cast<int>(y)), tex);
    }
    auto &registry = game.GetRegistry();
    for (ecs::Entity entity : registry.view<GridPosition, ecs::Sprite>()) {
        const auto &pos = registry.get<GridPosition>(entity);
        DrawSprite(tex.at(registry.get<ecs::Sprite>(entity).textureId), CellRect(pos.x, pos.y));
    }
    DrawText(TextFormat("SCORE: %d", game.GetScore()), 20, 560, 20, WHITE);
    if (game.IsGameOver())
        DrawText("GAME OVER", 320, 280, 30, RED);
    EndDrawing();
}

static void RunLoop(PacmanGame &game, const Textures &textures)
{
    Engine::GameLoop loop(60.0);

    loop.SetTickCallback([&game](double dt) {
        game.Update(static_cast<float>(dt));
    });
    while (!WindowShouldClose()) {
        ProcessInput(game);
        loop.Step();
        RenderGame(game, textures);
    }
}

int main()
{
    InitWindow(800, 600, "Pacman made by R-Type engine");
    SetTargetFPS(60);
    PacmanGame game;
    game.Init();
    Textures textures = LoadGameTextures();
    RunLoop(game, textures);
    UnloadGameTextures(textures);
    CloseWindow();
    return 0;
}
