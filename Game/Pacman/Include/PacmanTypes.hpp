/*
** EPITECH PROJECT, 2026
** RType
** File description:
** PacmanTypes
*/

#pragma once
#include <vector>

enum class BoxType
{
    EMPTY,
    CUBE,
    CELLP,
    PACGUM,
    SUPERFOOD
};

enum class Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

using Grid = std::vector<std::vector<BoxType>>;

inline bool IsWall(BoxType type)
{
    return type == BoxType::CUBE || type == BoxType::CELLP;
}
