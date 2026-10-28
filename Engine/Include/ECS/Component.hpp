/*
** EPITECH PROJECT, 2026
** RType
** File description:
** Component
*/

#pragma once

#include <string>

namespace ecs {

    struct Transform {
        float x = 0.0f;
        float y = 0.0f;
        float rotation = 0.0f;
    };

    struct Velocity {
        float x = 0.0f;
        float y = 0.0f;
    };

    struct Health {
        float current = 100.0f;
        float max = 100.0f;
    };

    struct Weapon {
        float dammage = 100.0f;
        float durability = 100.0f;
    };

    struct Sprite {
        std::string textureId;
    };

    struct Hitbox {
        float width = 0.0f;
        float height = 0.0f;
    };

    struct Bounded {};

} //* ecs namespace

