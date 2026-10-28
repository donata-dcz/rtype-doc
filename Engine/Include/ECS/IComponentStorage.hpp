/*
** EPITECH PROJECT, 2026
** RType
** File description:
** IComponentStorage
*/

#pragma once

#include "Entity.hpp"

namespace ecs {

    class IComponentStorage {
    public:
        virtual ~IComponentStorage() = default;

        virtual void remove(Entity entity) = 0;
    };

} //* ecs namespace
