/*
** EPITECH PROJECT, 2026
** r-type
** File description:
** R-Type rules as ECS systems, run in order by the Game at every tick
*/

#include <algorithm>
#include <cmath>
#include <optional>

#include "Game.hpp"
#include "RTypeSystems.hpp"
#include "ECS/Component.hpp"
#include "RTypeComponents.hpp"
#include "Entities.hpp"
#include "Enemies.hpp"

namespace rtype {

    namespace {

        //& colisions
        bool Touch(const ecs::Registry &registry, ecs::Entity first, ecs::Entity second)
        {
            const ecs::Transform &a = registry.get<ecs::Transform>(first);
            const ecs::Hitbox &aBox = registry.get<ecs::Hitbox>(first);
            const ecs::Transform &b = registry.get<ecs::Transform>(second);
            const ecs::Hitbox &bBox = registry.get<ecs::Hitbox>(second);

            return a.x < b.x + bBox.width && b.x < a.x + aBox.width && a.y < b.y + bBox.height && b.y < a.y + aBox.height;
        }

        template <typename Entities>
        std::optional<ecs::Entity> FindTouching(const ecs::Registry &registry, ecs::Entity target, const Entities &candidates)
        {
            for (ecs::Entity candidate : candidates) {
                if (registry.isAlive(candidate) && Touch(registry, target, candidate))
                    return candidate;
            }
            return std::nullopt;
        }

        //& score system
        void GiveScore(ecs::Registry &registry, ecs::Entity shooter)
        {
            if (registry.isAlive(shooter))
                registry.get<Player>(shooter).score += PointsPerEnemy;
        }

        //& loosing lisfe system
        void LoseLife(ecs::Registry &registry, ecs::Entity ship)
        {
            Player &player = registry.get<Player>(ship);

            player.lives--;
            player.invincibility = InvincibilityDuration;
            registry.get<ecs::Transform>(ship) = StartPosition(player.id);
        }

    } //* namespace anonyme

    //& control system
    void PlayerControlSystem::update(ecs::Registry &registry, float dt)
    {
        for (ecs::Entity ship : registry.view<Player, ecs::Velocity>()) {
            Player &player = registry.get<Player>(ship);
            ecs::Velocity &velocity = registry.get<ecs::Velocity>(ship);

            velocity = {};
            if (player.lives <= 0)
                continue;
            player.invincibility = std::max(player.invincibility - dt, 0.0f);
            if (player.input.IsActive(Action::MoveUp))
                velocity.y -= PlayerSpeed;
            if (player.input.IsActive(Action::MoveDown))
                velocity.y += PlayerSpeed;
            if (player.input.IsActive(Action::MoveLeft))
                velocity.x -= PlayerSpeed;
            if (player.input.IsActive(Action::MoveRight))
                velocity.x += PlayerSpeed;
        }
    }

    //& wave system
    void EnemyWaveSystem::update(ecs::Registry &registry, float dt)
    {
        for (ecs::Entity bydos : registry.view<Enemy, ecs::Transform>()) {
            Enemy &enemy = registry.get<Enemy>(bydos);

            enemy.age += dt;
            registry.get<ecs::Transform>(bydos).y = enemy.baseY + EnemyWaveAmplitude * std::sin(enemy.age * EnemyWaveSpeed);
        }
    }

    //&  Player Fire system
    void PlayerFireSystem::update(ecs::Registry &registry, float dt)
    {
        for (ecs::Entity ship : registry.view<Player, ecs::Transform>()) {
            Player &player = registry.get<Player>(ship);

            if (player.lives <= 0 || !player.gun.Update(player.input.IsActive(Action::Shoot), dt))
                continue;
            const ecs::Transform nose = registry.get<ecs::Transform>(ship);

            CreatePlayerMissile(registry, nose.x + PlayerWidth, nose.y + (PlayerHeight - MissileHeight) / 2.0f, ship);
        }
    }

    //& Enemy shoot system
    void EnemyFireSystem::update(ecs::Registry &registry, float dt)
    {
        for (ecs::Entity bydos : registry.view<Enemy, ecs::Transform>()) {
            Enemy &enemy = registry.get<Enemy>(bydos);

            enemy.reload -= dt;
            if (enemy.reload > 0.0f)
                continue;
            enemy.reload = EnemyFireInterval;
            const ecs::Transform mouth = registry.get<ecs::Transform>(bydos);

            // missile leaves from the front of the bydos at mid heigth
            CreateEnemyMissile(registry, mouth.x - MissileWidth, mouth.y + (EnemyHeight - MissileHeight) / 2.0f);
        }
    }

    //& Enemy spawn system
    EnemySpawnSystem::EnemySpawnSystem(std::uint32_t seed) : _random(seed)
    {
    }

    //& Enemy spawn update system
    void EnemySpawnSystem::update(ecs::Registry &registry, float dt)
    {
        const float minY = EnemyWaveAmplitude;
        const float maxY = std::max(WorldHeight - EnemyHeight - EnemyWaveAmplitude, minY);

        _timer += dt;
        while (_timer >= EnemySpawnInterval) {
            CreateBydos(registry, std::uniform_real_distribution<float>(minY, maxY)(_random));
            _timer -= EnemySpawnInterval;
        }
    }

    //& collision system
    void CollisionSystem::update(ecs::Registry &registry, float)
    {
        ShootEnemies(registry);
        HitPlayers(registry);
    }

    void CollisionSystem::ShootEnemies(ecs::Registry &registry)
    {
        const auto missiles = registry.view<PlayerMissile, ecs::Transform, ecs::Hitbox>();

        for (ecs::Entity bydos : registry.view<Enemy, ecs::Transform, ecs::Hitbox>()) {
            const std::optional<ecs::Entity> missile = FindTouching(registry, bydos, missiles);

            if (!missile)
                continue;
            GiveScore(registry, registry.get<PlayerMissile>(*missile).shooter);
            registry.destroy(*missile);
            registry.destroy(bydos);
        }
    }

    void CollisionSystem::HitPlayers(ecs::Registry &registry)
    {
        const auto enemies = registry.view<Enemy, ecs::Transform, ecs::Hitbox>();
        const auto enemyMissiles = registry.view<EnemyMissile, ecs::Transform, ecs::Hitbox>();

        for (ecs::Entity ship : registry.view<Player, ecs::Transform, ecs::Hitbox>()) {
            const Player &player = registry.get<Player>(ship);

            if (player.lives <= 0 || player.invincibility > 0.0f)
                continue;
            const bool rammed = FindTouching(registry, ship, enemies).has_value();
            const std::optional<ecs::Entity> missile = FindTouching(registry, ship, enemyMissiles);

            if (missile)
                registry.destroy(*missile);
            if (rammed || missile)
                LoseLife(registry, ship);
        }
    }

} //* namespace rtype
