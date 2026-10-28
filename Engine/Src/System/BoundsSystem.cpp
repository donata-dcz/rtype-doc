/*
** EPITECH PROJECT, 2026
** RType
** File description:
** BoundsSystem
*/

#include <algorithm>

#include "System/BoundsSystem.hpp"
#include "ECS/Component.hpp"

namespace ecs {

    BoundsSystem::BoundsSystem(float areaWidth, float areaHeight) : _areaWidth(areaWidth), _areaHeight(areaHeight)
    {
    }

    void BoundsSystem::update(Registry& registry, float)
    {
        for (Entity entity : registry.view<Transform, Hitbox, Bounded>()) {
            Transform& transform = registry.get<Transform>(entity);
            const Hitbox& hitbox = registry.get<Hitbox>(entity);

            const float maxX = std::max(_areaWidth - hitbox.width, 0.0f);
            const float maxY = std::max(_areaHeight - hitbox.height, 0.0f);

            transform.x = std::clamp(transform.x, 0.0f, maxX);
            transform.y = std::clamp(transform.y, 0.0f, maxY);
        }
    }

} //* ecs namespace
