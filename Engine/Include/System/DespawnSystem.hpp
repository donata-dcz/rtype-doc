/*
** EPITECH PROJECT, 2026
** RType
** File description:
** DespawnSystem
*/

#pragma once

#include "ISystem.hpp"

namespace ecs {

    class DespawnSystem : public ISystem {
    public:
        DespawnSystem(float areaWidth, float areaHeight);

        void update(Registry& registry, float dt) override;

    private:
        float _areaWidth;
        float _areaHeight;
    };

} //* ecs namespace
