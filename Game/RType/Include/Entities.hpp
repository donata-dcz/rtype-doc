/*
** EPITECH PROJECT, 2026
** r-type
** File description:
** Ready made R-Type entities
*/

#pragma once

#include <cstdint>

#include "ECS/Component.hpp"
#include "ECS/Registry.hpp"

namespace rtype {

    //^ ship position
    ecs::Transform StartPosition(std::uint8_t playerId);

    //^ ship
    ecs::Entity CreateShip(ecs::Registry &registry, std::uint8_t playerId);

    //^ bydos
    ecs::Entity CreateBydos(ecs::Registry &registry, float y);

    //^ missiles
    ecs::Entity CreatePlayerMissile(ecs::Registry &registry, float x, float y, ecs::Entity shooter);
    ecs::Entity CreateEnemyMissile(ecs::Registry &registry, float x, float y);

} //* namespace rtype
