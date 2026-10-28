/*
** EPITECH PROJECT, 2026
** RType
** File description:
** PacmanTests
*/

#include <gtest/gtest.h>
#include <iterator>
#include <vector>
#include <ECS/Registry.hpp>
#include <Engine/EventBus.hpp>
#include "Components.hpp"
#include "Events.hpp"
#include "PacmanGame.hpp"
#include "Systems.hpp"

static Grid MakeRoom(int width, int height)
{
    Grid grid(height, std::vector<BoxType>(width, BoxType::EMPTY));

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (x == 0 || y == 0 || x == width - 1 || y == height - 1)
                grid[y][x] = BoxType::CUBE;
        }
    }
    return grid;
}

static ecs::Entity SpawnPacman(ecs::Registry &registry, GridPosition pos, Direction dir)
{
    const ecs::Entity pacman = registry.create();

    registry.add(pacman, PacmanTag{});
    registry.add(pacman, pos);
    registry.add(pacman, Facing{dir});
    return pacman;
}

static ecs::Entity SpawnGhost(ecs::Registry &registry, GridPosition pos)
{
    const ecs::Entity ghost = registry.create();

    registry.add(ghost, GhostTag{});
    registry.add(ghost, pos);
    registry.add(ghost, Facing{Direction::UP});
    return ghost;
}

TEST(PacmanMovement, MovesAndPublishesEvent)
{
    ecs::Registry registry;
    Engine::EventBus bus;
    const Grid grid = MakeRoom(5, 3);
    PacmanMovementSystem system(grid, bus);
    const ecs::Entity pacman = SpawnPacman(registry, {1, 1}, Direction::RIGHT);
    int moves = 0;
    GridPosition lastPos;

    bus.Subscribe<PacmanMoved>([&](const PacmanMoved &event) {
        moves++;
        lastPos = event.position;
    });
    system.update(registry, 0.0f);

    EXPECT_EQ(registry.get<GridPosition>(pacman), (GridPosition{2, 1}));
    EXPECT_EQ(moves, 1);
    EXPECT_EQ(lastPos, (GridPosition{2, 1}));
}

TEST(PacmanMovement, StopsAtWall)
{
    ecs::Registry registry;
    Engine::EventBus bus;
    const Grid grid = MakeRoom(3, 3);
    PacmanMovementSystem system(grid, bus);
    const ecs::Entity pacman = SpawnPacman(registry, {1, 1}, Direction::RIGHT);
    int moves = 0;

    bus.Subscribe<PacmanMoved>([&](const PacmanMoved &) { moves++; });
    system.update(registry, 0.0f);

    EXPECT_EQ(registry.get<GridPosition>(pacman), (GridPosition{1, 1}));
    EXPECT_EQ(moves, 0);
}

TEST(GhostAI, NeverEntersAWall)
{
    ecs::Registry registry;
    const Grid grid = MakeRoom(7, 7);
    GhostAISystem system(grid);
    const ecs::Entity ghost = SpawnGhost(registry, {3, 3});

    for (int i = 0; i < 500; i++) {
        system.update(registry, 0.0f);
        const auto &pos = registry.get<GridPosition>(ghost);
        EXPECT_FALSE(IsWall(grid[pos.y][pos.x]));
    }
}

TEST(GhostAI, EnclosedGhostStaysPut)
{
    ecs::Registry registry;
    const Grid grid = MakeRoom(3, 3);
    GhostAISystem system(grid);
    const ecs::Entity ghost = SpawnGhost(registry, {1, 1});

    for (int i = 0; i < 20; i++)
        system.update(registry, 0.0f);

    EXPECT_EQ(registry.get<GridPosition>(ghost), (GridPosition{1, 1}));
}

TEST(Collision, PublishesCaughtWhenGhostOnPacman)
{
    ecs::Registry registry;
    Engine::EventBus bus;
    CollisionSystem system(bus);
    int caught = 0;

    bus.Subscribe<PacmanCaught>([&](const PacmanCaught &) { caught++; });
    SpawnPacman(registry, {2, 2}, Direction::RIGHT);
    SpawnGhost(registry, {2, 2});
    system.update(registry, 0.0f);

    EXPECT_EQ(caught, 1);
}

TEST(Collision, NothingWhenGhostElsewhere)
{
    ecs::Registry registry;
    Engine::EventBus bus;
    CollisionSystem system(bus);
    int caught = 0;

    bus.Subscribe<PacmanCaught>([&](const PacmanCaught &) { caught++; });
    SpawnPacman(registry, {2, 2}, Direction::RIGHT);
    SpawnGhost(registry, {3, 2});
    system.update(registry, 0.0f);

    EXPECT_EQ(caught, 0);
}

TEST(PacmanGameInit, SpawnsOnePacmanAndFourGhosts)
{
    PacmanGame game;

    game.Init();
    auto &registry = game.GetRegistry();
    const auto pacmans = registry.view<PacmanTag, GridPosition>();
    const auto ghosts = registry.view<GhostTag, GridPosition>();

    EXPECT_EQ(std::distance(pacmans.begin(), pacmans.end()), 1);
    EXPECT_EQ(std::distance(ghosts.begin(), ghosts.end()), 4);
    EXPECT_EQ(game.GetScore(), 0);
    EXPECT_FALSE(game.IsGameOver());
}