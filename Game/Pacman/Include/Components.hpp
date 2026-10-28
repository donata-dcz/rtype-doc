/*
** EPITECH PROJECT, 2026
** RType
** File description:
** Components
*/

#pragma once
#include "PacmanTypes.hpp"

struct GridPosition
{
    int x = 0;
    int y = 0;
    bool operator==(const GridPosition &other) const = default;
};

struct Facing
{
    Direction direction = Direction::RIGHT;
};

struct PacmanTag {};
struct GhostTag {};

inline GridPosition Next(GridPosition pos, Direction dir)
{
    switch (dir) {
        case Direction::UP: pos.y--; break;
        case Direction::DOWN: pos.y++; break;
        case Direction::LEFT: pos.x--; break;
        case Direction::RIGHT: pos.x++; break;
    }
    return pos;
}
