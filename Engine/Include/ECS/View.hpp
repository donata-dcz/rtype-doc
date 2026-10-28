/*
** EPITECH PROJECT, 2026
** RType
** File description:
** View
*/

#pragma once

#include <vector>

#include "Entity.hpp"

namespace ecs {

    template<typename... Components>
    class View {
    public:
        explicit View(std::vector<Entity> entities)
            : entities(std::move(entities))
        {
        }

        auto begin() const {
            return entities.begin();
        }

        auto end() const {
            return entities.end();
        }

    private:
        std::vector<Entity> entities;
    };

} //* ecs namespace
