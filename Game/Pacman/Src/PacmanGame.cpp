/*
** EPITECH PROJECT, 2026
** RType
** File description:
** PacmanGame
*/

#include <PacmanGame.hpp>
#include <cstdlib>
#include <string>
#include <ECS/Component.hpp>
#include "Events.hpp"
#include "Systems.hpp"

PacmanGame::PacmanGame()
{
    _mapWidth = 40;
    _mapHeight = 30;
    _isGameOver = false;
    _score = 0;
    _moveTimer = 0.0f;
    _systems.push_back(std::make_unique<GhostAISystem>(_pacmanMap));
    _systems.push_back(std::make_unique<CollisionSystem>(_bus));
    _systems.push_back(std::make_unique<PacmanMovementSystem>(_pacmanMap, _bus));
    _systems.push_back(std::make_unique<CollisionSystem>(_bus));
    _bus.Subscribe<PacmanMoved>([this](const PacmanMoved &event) { OnPacmanMoved(event.position); });
    _bus.Subscribe<PacmanCaught>([this](const PacmanCaught &) { _isGameOver = true; });
}

void PacmanGame::Init()
{
    LoadMap();
    GenerateFood();
    GenerateWall();
    SpawnPacman();
    SpawnGhosts();
}

void PacmanGame::LoadMap() {
    _pacmanMap = Grid(_mapHeight, std::vector<BoxType>(_mapWidth, BoxType::EMPTY));
    for (int y = 0; y < _mapHeight; y++) {
        for (int x = 0; x < _mapWidth; x++) {
            if (y == 0 || y == _mapHeight - 1)
                _pacmanMap[y][x] = BoxType::CELLP;
            if (x == 0 || x == _mapWidth - 1)
                _pacmanMap[y][x] = BoxType::CUBE;
        }
    }
}

void PacmanGame::GenerateFood() {
    for (std::size_t i = 0; i < _pacmanMap.size(); i++) {
        for (std::size_t j = 0; j < _pacmanMap[i].size(); j++) {
            if (_pacmanMap[i][j] == BoxType::EMPTY)
                _pacmanMap[i][j] = BoxType::PACGUM;
            if (_pacmanMap[i][j] == BoxType::PACGUM && rand() % 50 == 0)
                _pacmanMap[i][j] = BoxType::SUPERFOOD;
        }
    }
}

void PacmanGame::GenerateWall() {
    for (std::size_t i = 0; i < _pacmanMap.size(); i++) {
        for (std::size_t j = 0; j < _pacmanMap[i].size(); j++) {
            if (_pacmanMap[i][j] == BoxType::PACGUM && rand() % 10 == 0)
                _pacmanMap[i][j] = BoxType::CUBE;
        }
    }
}

void PacmanGame::SpawnPacman() {
    const GridPosition start{_mapWidth / 2, _mapHeight / 2};

    _pacmanMap[start.y][start.x] = BoxType::EMPTY;
    _pacman = _registry.create();
    _registry.add(_pacman, PacmanTag{});
    _registry.add(_pacman, start);
    _registry.add(_pacman, Facing{Direction::RIGHT});
    _registry.add(_pacman, ecs::Sprite{"pacman"});
}

void PacmanGame::SpawnGhosts() {
    for (int i = 0; i < GhostCount; i++) {
        const GridPosition start{_mapWidth / 2 + i, _mapHeight / 2 - 3};

        if (IsWall(_pacmanMap[start.y][start.x]))
            _pacmanMap[start.y][start.x] = BoxType::EMPTY;
        const ecs::Entity ghost = _registry.create();
        _registry.add(ghost, GhostTag{});
        _registry.add(ghost, start);
        _registry.add(ghost, Facing{Direction::UP});
        _registry.add(ghost, ecs::Sprite{"ghost-" + std::to_string(i)});
    }
}

void PacmanGame::SetDirection(Direction dir) {
    if (!_registry.isAlive(_pacman))
        return;
    _registry.get<Facing>(_pacman).direction = dir;
}

void PacmanGame::OnPacmanMoved(const GridPosition &pos) {
    BoxType &cell = _pacmanMap[pos.y][pos.x];

    if (cell == BoxType::PACGUM)
        _score += 10;
    else if (cell == BoxType::SUPERFOOD)
        _score += 50;
    else
        return;
    cell = BoxType::EMPTY;
}

void PacmanGame::Update(float deltaTime) {
    if (_isGameOver)
        return;
    _moveTimer += deltaTime;
    if (_moveTimer < StepInterval)
        return;
    _moveTimer = 0.0f;
    for (auto &system : _systems) {
        system->update(_registry, StepInterval);
        if (_isGameOver)
            return;
    }
}
