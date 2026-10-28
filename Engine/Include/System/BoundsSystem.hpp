/*
** EPITECH PROJECT, 2026
** RType
** File description:
** BoundsSystem
*/

#pragma once

#include "ISystem.hpp"

namespace ecs {

    //& bound player screen
    class BoundsSystem : public ISystem {
    public:
        BoundsSystem(float areaWidth, float areaHeight);

        void update(Registry& registry, float dt) override;

    private:
        float _areaWidth;
        float _areaHeight;
    };

} //* ecs namespace
