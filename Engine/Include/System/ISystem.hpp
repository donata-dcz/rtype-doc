/*
** EPITECH PROJECT, 2026
** RType
** File description:
** ISystem
*/

#pragma once

#include "ECS/Registry.hpp"

namespace ecs {

    class ISystem {
    public:
        virtual ~ISystem() = default;

        virtual void update(
            Registry& registry,
            float dt
        ) = 0;
    };

} //* ecs namespace
