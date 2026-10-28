/*
** EPITECH PROJECT, 2026
** r-type
** File description:
** bydos how they apear and how they moove shared by client and server
*/

#pragma once

#include <cstdint>
#include <random>

#include "ECS/Registry.hpp"
#include "ECS/Component.hpp"

namespace rtype {

    //^ enemy visual
    constexpr float EnemyWidth = 48.0f;
    constexpr float EnemyHeight = 36.0f;

    //^ enemy stats
    constexpr float EnemySpeed = 150.0f;
    constexpr float EnemyWaveAmplitude = 40.0f;
    constexpr float EnemyWaveSpeed = 3.0f;
    constexpr float EnemySpawnInterval = 0.8f;

    //^ shooting stats
    constexpr float EnemyFireInterval = 2.5f;
    constexpr float EnemyFirstShotDelay = 0.8f;

} //* namespace rtype
