/*
** EPITECH PROJECT, 2026
** r-type
** File description:
** Ready made R-Type entities
*/

#include "Entities.hpp"
#include "RTypeComponents.hpp"
#include "Game.hpp"
#include "Enemies.hpp"

namespace rtype {

    //& starting pos
    ecs::Transform StartPosition(std::uint8_t playerId)
    {
        return {60.0f, 90.0f + static_cast<float>(playerId) * 120.0f, 0.0f};
    }

    //& create ship
    ecs::Entity CreateShip(ecs::Registry &registry, std::uint8_t playerId)
    {
        const ecs::Entity ship = registry.create();

        registry.add<ecs::Transform>(ship, StartPosition(playerId));
        registry.add<ecs::Velocity>(ship, {});
        registry.add<ecs::Hitbox>(ship, {PlayerWidth, PlayerHeight});
        registry.add<ecs::Bounded>(ship, {});
        registry.add<Player>(ship, {playerId, {}, {}, StartingLives, 0.0f, 0});
        return ship;
    }

    //& create bydos
    ecs::Entity CreateBydos(ecs::Registry &registry, float y)
    {
        const ecs::Entity bydos = registry.create();

        registry.add<ecs::Transform>(bydos, {WorldWidth, y, 0.0f});
        registry.add<ecs::Velocity>(bydos, {-EnemySpeed, 0.0f});
        registry.add<ecs::Hitbox>(bydos, {EnemyWidth, EnemyHeight});
        registry.add<Enemy>(bydos, {y, 0.0f, EnemyFirstShotDelay});
        registry.add<Networked>(bydos, {EntityType::Bydos});
        return bydos;
    }

    //& create missiles
    ecs::Entity CreatePlayerMissile(ecs::Registry &registry, float x, float y, ecs::Entity shooter)
    {
        const ecs::Entity missile = registry.create();

        registry.add<ecs::Transform>(missile, {x, y, 0.0f});
        registry.add<ecs::Velocity>(missile, {MissileSpeed, 0.0f});
        registry.add<ecs::Hitbox>(missile, {MissileWidth, MissileHeight});
        registry.add<PlayerMissile>(missile, {shooter});
        registry.add<Networked>(missile, {EntityType::PlayerMissile});
        return missile;
    }

    //& create enemy missile
    ecs::Entity CreateEnemyMissile(ecs::Registry &registry, float x, float y)
    {
        const ecs::Entity missile = registry.create();

        registry.add<ecs::Transform>(missile, {x, y, 0.0f});
        registry.add<ecs::Velocity>(missile, {-EnemyMissileSpeed, 0.0f});
        registry.add<ecs::Hitbox>(missile, {MissileWidth, MissileHeight});
        registry.add<EnemyMissile>(missile, {});
        registry.add<Networked>(missile, {EntityType::EnemyMissile});
        return missile;
    }

} //* namespace rtype
