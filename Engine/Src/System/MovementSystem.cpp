/*
** EPITECH PROJECT, 2026
** RType
** File description:
** MovementSystem
*/

#include "System/MovementSystem.hpp"
#include "ECS/Component.hpp"

namespace ecs {

    void MovementSystem::update(Registry& registry, float dt)
    {
        for (Entity entity : registry.view<Transform, Velocity>()) {
            Transform& transform = registry.get<Transform>(entity);
            const Velocity& velocity = registry.get<Velocity>(entity);

            transform.x += velocity.x * dt;
            transform.y += velocity.y * dt;
        }
    }

} //* ecs namespace
