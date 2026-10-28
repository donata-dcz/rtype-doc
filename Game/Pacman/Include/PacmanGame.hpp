/*
** EPITECH PROJECT, 2026
** RType
** File description:
** PacmanGame
*/

#pragma once
#include <memory>
#include <vector>
#include <ECS/Registry.hpp>
#include <Engine/EventBus.hpp>
#include <System/ISystem.hpp>

#include "Components.hpp"
#include "PacmanTypes.hpp"

class PacmanGame
{
    private:
        int _mapWidth;
        int _mapHeight;
        bool _isGameOver;
        int _score;
        float _moveTimer;
        constexpr static float StepInterval = 0.15f;
        constexpr static int GhostCount = 4;
        Grid _pacmanMap;
        ecs::Registry _registry;
        Engine::EventBus _bus;
        ecs::Entity _pacman{};
        std::vector<std::unique_ptr<ecs::ISystem>> _systems;

        void LoadMap();
        void GenerateFood();
        void GenerateWall();
        void SpawnPacman();
        void SpawnGhosts();
        void OnPacmanMoved(const GridPosition &pos);

    public:
        PacmanGame();
        ~PacmanGame() = default;
        PacmanGame(const PacmanGame &) = delete;
        PacmanGame &operator=(const PacmanGame &) = delete;
        void Init();
        void Update(float deltaTime);
        void SetDirection(Direction dir);
        [[nodiscard]] const Grid &GetMap() const { return _pacmanMap; }
        [[nodiscard]] ecs::Registry &GetRegistry() { return _registry; }
        [[nodiscard]] bool IsGameOver() const { return _isGameOver; }
        [[nodiscard]] int GetScore() const { return _score; }
};
