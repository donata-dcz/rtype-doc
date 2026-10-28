/*
** EPITECH PROJECT, 2026
** r-type
** File description:
** Components of the R-Type entities
*/

#pragma once

#include <cstdint>

#include "ECS/Entity.hpp"
#include "Input.hpp"
#include "Snapshot.hpp"
#include "Weapon.hpp"

namespace rtype {

    constexpr int StartingLives = 3;

    //^ ship
    struct Player {
        std::uint8_t id = 0;
        InputState input;
        Gun gun;
        int lives = StartingLives;
        float invincibility = 0.0f;
        std::uint32_t score = 0;
    };

    //^ bydos waves
    struct Enemy {
        float baseY = 0.0f;
        float age = 0.0f;
        float reload = 0.0f;
    };

    //^ missile player
    struct PlayerMissile {
        ecs::Entity shooter{};
    };

    //^ missile bydos
    struct EnemyMissile {};

    //^ network
    struct Networked {
        EntityType type = EntityType::Bydos;
    };

} //* namespace rtype
