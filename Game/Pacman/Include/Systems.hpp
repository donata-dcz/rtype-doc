/*
** EPITECH PROJECT, 2026
** RType
** File description:
** Systems
*/

#pragma once
#include <System/ISystem.hpp>
#include <Engine/EventBus.hpp>
#include "PacmanTypes.hpp"

class PacmanMovementSystem : public ecs::ISystem
{
    public:
        PacmanMovementSystem(const Grid &grid, Engine::EventBus &bus);
        void update(ecs::Registry &registry, float dt) override;

    private:
        const Grid &_grid;
        Engine::EventBus &_bus;
};

class GhostAISystem : public ecs::ISystem
{
    public:
        explicit GhostAISystem(const Grid &grid);
        void update(ecs::Registry &registry, float dt) override;

    private:
        const Grid &_grid;
};

class CollisionSystem : public ecs::ISystem
{
    public:
        explicit CollisionSystem(Engine::EventBus &bus);
        void update(ecs::Registry &registry, float dt) override;

    private:
        Engine::EventBus &_bus;
};
