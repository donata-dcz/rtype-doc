/*
** EPITECH PROJECT, 2026
** r-type
** File description:
** The full match server run and client too in offline mode
*/

#include <algorithm>
#include <cmath>
#include <utility>
#include <vector>

#include "Entities.hpp"
#include "Game.hpp"
#include "System/BoundsSystem.hpp"
#include "System/DespawnSystem.hpp"
#include "System/MovementSystem.hpp"
#include "RTypeComponents.hpp"
#include "RTypeSystems.hpp"

namespace rtype {

    namespace {

        std::int16_t ToPixel(float value)
        {
            return static_cast<std::int16_t>(std::lround(std::clamp(value, -32768.0f, 32767.0f)));
        }

        //& ids
        std::uint32_t NetworkId(ecs::Entity entity)
        {
            return static_cast<std::uint32_t>(entity) + MaxPlayers;
        }

        bool DrawnBefore(const SnapshotEntity &first, const SnapshotEntity &second)
        {
            const int firstOrder = first.type == EntityType::Bydos ? 0 : 1;
            const int secondOrder = second.type == EntityType::Bydos ? 0 : 1;

            return std::pair(firstOrder, first.id) < std::pair(secondOrder, second.id);
        }

        SnapshotEntity ToSnapshotEntity(const ecs::Registry &registry, ecs::Entity entity)
        {
            const EntityType type = registry.get<Networked>(entity).type;
            const ecs::Transform &transform = registry.get<ecs::Transform>(entity);
            const float y = type == EntityType::Bydos ? transform.y : transform.y + MissileHeight / 2.0f;

            return {NetworkId(entity), type, ToPixel(transform.x), ToPixel(y)};
        }

    } //* namespace anonyme

    Game::Game(std::uint32_t seed)
    {
        _systems.push_back(std::make_unique<PlayerControlSystem>());
        _systems.push_back(std::make_unique<ecs::MovementSystem>());
        _systems.push_back(std::make_unique<ecs::BoundsSystem>(WorldWidth, WorldHeight));
        _systems.push_back(std::make_unique<EnemyWaveSystem>());
        _systems.push_back(std::make_unique<PlayerFireSystem>());
        _systems.push_back(std::make_unique<EnemyFireSystem>());
        _systems.push_back(std::make_unique<EnemySpawnSystem>(seed));
        _systems.push_back(std::make_unique<ecs::DespawnSystem>(WorldWidth, WorldHeight));
        _systems.push_back(std::make_unique<CollisionSystem>());
    }

    bool Game::AddPlayer(std::uint8_t id)
    {
        if (id >= MaxPlayers || _ships[id])
            return false;
        _ships[id] = CreateShip(_registry, id);
        return true;
    }

    void Game::RemovePlayer(std::uint8_t id)
    {
        if (id >= MaxPlayers || !_ships[id])
            return;
        _registry.destroy(*_ships[id]);
        _ships[id].reset();
    }

    void Game::SetInput(std::uint8_t id, InputState input)
    {
        if (id < MaxPlayers && _ships[id])
            _registry.get<Player>(*_ships[id]).input = input;
    }

    void Game::Tick(float deltaTime)
    {
        _tick++;
        for (const auto &system : _systems)
            system->update(_registry, deltaTime);
    }

    Snapshot Game::BuildSnapshot() const
    {
        Snapshot snapshot{_tick, {}, {}};

        AddPlayers(snapshot);
        AddEntities(snapshot);
        if (snapshot.entities.size() > MaxSnapshotEntities)
            snapshot.entities.resize(MaxSnapshotEntities);
        return snapshot;
    }

    void Game::AddPlayers(Snapshot &snapshot) const
    {
        for (const std::optional<ecs::Entity> &ship : _ships) {
            if (!ship)
                continue;
            const Player &player = _registry.get<Player>(*ship);
            const ecs::Transform &transform = _registry.get<ecs::Transform>(*ship);
            const bool alive = player.lives > 0;

            snapshot.players.push_back({player.id, alive, alive && player.invincibility > 0.0f, static_cast<std::uint8_t>(player.lives), player.score});
            if (alive)
                snapshot.entities.push_back({player.id, EntityType::Ship, ToPixel(transform.x), ToPixel(transform.y)});
        }
    }

    void Game::AddEntities(Snapshot &snapshot) const
    {
        std::vector<SnapshotEntity> entities;

        for (ecs::Entity entity : _registry.view<Networked, ecs::Transform>())
            entities.push_back(ToSnapshotEntity(_registry, entity));
        std::sort(entities.begin(), entities.end(), DrawnBefore);
        snapshot.entities.insert(snapshot.entities.end(), entities.begin(), entities.end());
    }

} //* namespace rtype
