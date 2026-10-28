/*
** EPITECH PROJECT, 2026
** RType
** File description:
** Registry
*/

#include "ECS/Registry.hpp"

namespace ecs {

    Entity Registry::create()
    {
        Entity entity{nextEntityId++};
        entities.emplace(entity);
        return entity;
    }

    void Registry::destroy(Entity entity)
    {
        auto it = entities.find(entity);
        if (it != entities.end()) {
            for (auto& [type, storage] : storages) {
                storage->remove(entity);
            }
            entities.erase(it);
        }
    }

    bool Registry::isAlive(Entity entity) const
    {
        auto it = entities.find(entity);
        if (it != entities.end())
            return true;
        return false;
    }

} //* ecs namespace
