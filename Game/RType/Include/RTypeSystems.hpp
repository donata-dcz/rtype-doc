/*
** EPITECH PROJECT, 2026
** r-type
** File description:
** R-Type rules as ECS systems, run in order by the Game at every tick
*/

#pragma once

#include <cstdint>
#include <random>

#include "System/ISystem.hpp"

//~ System specific to RType game
namespace rtype {

    //& Input system
    class PlayerControlSystem : public ecs::ISystem {
    public:
        void update(ecs::Registry &registry, float dt) override;
    };

    //& Enenmy wave system
    class EnemyWaveSystem : public ecs::ISystem {
    public:
        void update(ecs::Registry &registry, float dt) override;
    };

    //& fire system
    class PlayerFireSystem : public ecs::ISystem {
    public:
        void update(ecs::Registry &registry, float dt) override;
    };

    //& enemy fire system
    class EnemyFireSystem : public ecs::ISystem {
    public:
        void update(ecs::Registry &registry, float dt) override;
    };

    //& spawn bydos system
    class EnemySpawnSystem : public ecs::ISystem {
    public:
        explicit EnemySpawnSystem(std::uint32_t seed);

        void update(ecs::Registry &registry, float dt) override;

    private:
        std::mt19937 _random;
        float _timer = 0.0f;
    };

    //& colisions system 
    class CollisionSystem : public ecs::ISystem {
    public:
        void update(ecs::Registry &registry, float dt) override;

    private:
        static void ShootEnemies(ecs::Registry &registry);
        static void HitPlayers(ecs::Registry &registry);
    };

} //* namespace rtype
