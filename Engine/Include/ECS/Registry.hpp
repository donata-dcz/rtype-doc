/*
** EPITECH PROJECT, 2026
** RType
** File description:
** Registry
*/

#pragma once

#include <memory>
#include <typeindex>
#include <unordered_map>
#include <unordered_set>

#include "Entity.hpp"
#include "ComponentStorage.hpp"
#include "View.hpp"

namespace ecs {

    class Registry {
    public:
        //^ Gestion des entités
        Entity create();
        void destroy(Entity entity);
        bool isAlive(Entity entity) const;

        //^ Gestion des composants
        template<typename T>
        void add(Entity entity, T component) {
            getStorage<T>().add(entity, std::move(component));
        }

        template<typename T>
        T& get(Entity entity) {
            return getStorage<T>().get(entity);
        }

        //^ read only acces
        template<typename T>
        const T& get(Entity entity) const {
            return static_cast<const ComponentStorage<T>&>(
                *storages.at(std::type_index(typeid(T)))
            ).get(entity);
        }

        template<typename T>
        bool has(Entity entity) const {
            auto it = storages.find(std::type_index(typeid(T)));

            if (it == storages.end())
                return false;

            return static_cast<const ComponentStorage<T>*>(
                it->second.get()
            )->has(entity);
        }

        template<typename T>
        void remove(Entity entity) {
            getStorage<T>().remove(entity);
        }

        template<typename... Components>
        View<Components...> view() const;

    private:
        //^ Entités
        std::uint32_t nextEntityId = 0;
        std::unordered_set<Entity> entities;

        //^ Composants
        std::unordered_map<
            std::type_index,
            std::unique_ptr<IComponentStorage>
        > storages;

        template<typename T>
        ComponentStorage<T>& getStorage();
    };

} //* ecs namespace

#include "Registry.tpp"
