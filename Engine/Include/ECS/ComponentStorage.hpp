/*
** EPITECH PROJECT, 2026
** RType
** File description:
** ComponentStorage
*/

#pragma once

#include <unordered_map>
#include "IComponentStorage.hpp"

namespace ecs {

    template<typename T>
    class ComponentStorage : public IComponentStorage {
    public:
        void add(Entity entity, T component) {
            components.insert_or_assign(entity, std::move(component));
        }

        T& get(Entity entity) {
            return components.at(entity);
        }

        const T& get(Entity entity) const {
            return components.at(entity);
        }

        bool has(Entity entity) const {
            return components.contains(entity);
        }

        void remove(Entity entity) override {
            components.erase(entity);
        }

    private:
        std::unordered_map<Entity, T> components;
    };

} //* ecs namespace
