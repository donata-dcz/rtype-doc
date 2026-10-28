/*
** EPITECH PROJECT, 2026
** RType
** File description:
** DespawnSystem
*/

#include <vector>

#include "System/DespawnSystem.hpp"
#include "ECS/Component.hpp"

namespace ecs {

    namespace {

        bool IsLeaving(const Transform& transform, const Velocity& velocity, const Hitbox& hitbox, float areaWidth, float areaHeight)
        {
            const bool leftOut = transform.x + hitbox.width < 0.0f && velocity.x < 0.0f;
            const bool rightOut = transform.x > areaWidth && velocity.x > 0.0f;
            const bool topOut = transform.y + hitbox.height < 0.0f && velocity.y < 0.0f;
            const bool bottomOut = transform.y > areaHeight && velocity.y > 0.0f;

            return leftOut || rightOut || topOut || bottomOut;
        }

    } //* namespace anonyme

    DespawnSystem::DespawnSystem(float areaWidth, float areaHeight)
        : _areaWidth(areaWidth), _areaHeight(areaHeight)
    {
    }

    void DespawnSystem::update(Registry& registry, float)
    {
        std::vector<Entity> leaving;

        for (Entity entity : registry.view<Transform, Velocity, Hitbox>()) {
            if (IsLeaving(registry.get<Transform>(entity), registry.get<Velocity>(entity), registry.get<Hitbox>(entity), _areaWidth, _areaHeight))
                leaving.push_back(entity);
        }
        for (Entity entity : leaving)
            registry.destroy(entity);
    }

} //* ecs namespace
