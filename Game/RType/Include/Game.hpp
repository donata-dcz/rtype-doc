/*
** EPITECH PROJECT, 2026
** r-type
** File description:
** The full match server run and client too in offline mode
*/

#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <optional>
#include <vector>

#include "Input.hpp"
#include "Snapshot.hpp"
#include "ECS/Registry.hpp"
#include "System/ISystem.hpp"

namespace rtype {

    //^ play area stats
    constexpr float WorldWidth = 800.0f;
    constexpr float WorldHeight = 600.0f;

    //^ player visual
    constexpr float PlayerSpeed = 250.0f;
    constexpr float PlayerWidth = 99.0f;
    constexpr float PlayerHeight = 51.0f;

    //^ game stats
    constexpr std::uint32_t PointsPerEnemy = 100;
    constexpr float InvincibilityDuration = 2.0f;

    class Game {
    public:
        explicit Game(std::uint32_t seed);

        //^ false if id taken or not beetween 0 and -1
        bool AddPlayer(std::uint8_t id);
        void RemovePlayer(std::uint8_t id);

        //^last actions received aplied at every ticks
        void SetInput(std::uint8_t id, InputState input);

        void Tick(float deltaTime);
        Snapshot BuildSnapshot() const;

    private:
        void AddPlayers(Snapshot &snapshot) const;
        void AddEntities(Snapshot &snapshot) const;

        //^ registry
        ecs::Registry _registry;

        //^ systems
        std::vector<std::unique_ptr<ecs::ISystem>> _systems;

        //^ players ids
        std::array<std::optional<ecs::Entity>, MaxPlayers> _ships;
        std::uint32_t _tick = 0;
    };

} //* namespace rtype
