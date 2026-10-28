/*
** EPITECH PROJECT, 2026
** RType
** File description:
** Systems
*/

#include "Systems.hpp"
#include <cstdlib>
#include <vector>
#include "Components.hpp"
#include "Events.hpp"

PacmanMovementSystem::PacmanMovementSystem(const Grid &grid, Engine::EventBus &bus) : _grid(grid), _bus(bus) {}

void PacmanMovementSystem::update(ecs::Registry &registry, float /*dt*/)
{
    for (ecs::Entity entity : registry.view<PacmanTag, GridPosition, Facing>()) {
        auto &pos = registry.get<GridPosition>(entity);
        const GridPosition target = Next(pos, registry.get<Facing>(entity).direction);

        if (IsWall(_grid[target.y][target.x]))
            continue;
        pos = target;
        _bus.Publish(PacmanMoved{pos});
    }
}

GhostAISystem::GhostAISystem(const Grid &grid) : _grid(grid) {}

static Direction SelectGhostDirection(const Grid &grid, const GridPosition &pos, Direction current)
{
    constexpr Direction all[] = {Direction::UP, Direction::DOWN, Direction::LEFT, Direction::RIGHT};
    std::vector<Direction> validDirs;
    bool canContinue = false;

    for (Direction dir : all) {
        const GridPosition next = Next(pos, dir);
        if (IsWall(grid[next.y][next.x]))
            continue;
        validDirs.push_back(dir);
        if (dir == current)
            canContinue = true;
    }
    if (validDirs.empty())
        return current;
    if (!canContinue || (rand() % 4 == 0))
        return validDirs[rand() % validDirs.size()];
    return current;
}

void GhostAISystem::update(ecs::Registry &registry, float /*dt*/)
{
    for (ecs::Entity entity : registry.view<GhostTag, GridPosition, Facing>()) {
        auto &pos = registry.get<GridPosition>(entity);
        auto &facing = registry.get<Facing>(entity);

        facing.direction = SelectGhostDirection(_grid, pos, facing.direction);
        const GridPosition target = Next(pos, facing.direction);
        if (!IsWall(_grid[target.y][target.x]))
            pos = target;
    }
}

CollisionSystem::CollisionSystem(Engine::EventBus &bus) : _bus(bus) {}

void CollisionSystem::update(ecs::Registry &registry, float /*dt*/)
{
    for (ecs::Entity pacman : registry.view<PacmanTag, GridPosition>()) {
        const GridPosition pacmanPos = registry.get<GridPosition>(pacman);

        for (ecs::Entity ghost : registry.view<GhostTag, GridPosition>()) {
            if (registry.get<GridPosition>(ghost) == pacmanPos) {
                _bus.Publish(PacmanCaught{});
                return;
            }
        }
    }
}
