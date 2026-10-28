/*
** EPITECH PROJECT, 2026
** RType
** File description:
** MovementSystem
*/

#pragma once

#include "ISystem.hpp"

namespace ecs {

    class MovementSystem : public ISystem {
    public:
        void update(Registry& registry, float dt) override;
    };

} //* ecs namespace
