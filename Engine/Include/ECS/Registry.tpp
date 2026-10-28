/*
** EPITECH PROJECT, 2026
** RType
** File description:
** Registry
*/

#pragma once

namespace ecs {

    template<typename T>
    ComponentStorage<T>& Registry::getStorage()
    {
        std::type_index type = typeid(T);

        auto it = storages.find(type);
        if (it == storages.end()) {
            auto storage = std::make_unique<ComponentStorage<T>>();
            auto* ptr = storage.get();
            storages.emplace(type, std::move(storage));
            return *ptr;
        }
        return static_cast<ComponentStorage<T>&>(*it->second);
    }

    template<typename... Components>
    View<Components...> Registry::view() const
    {
        std::vector<Entity> sameEntities;

        for (Entity entity : entities) {
            if ((has<Components>(entity) && ...))
                sameEntities.push_back(entity);
        }
        return View<Components ...>(std::move(sameEntities));
    }

} //* ecs namespace
