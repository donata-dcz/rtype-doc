/*
** EPITECH PROJECT, 2026
** r-type
** File description:
** draw snapshot of the match what the server says is happening
*/

#pragma once

#include <array>
#include <cstdint>
#include <optional>

#include <Client/ResourceManager.hpp>
#include <Client/ShipAnimation.hpp>
#include "Game.hpp"
#include "Snapshot.hpp"

namespace Client {

    //& no game rule client only draw
    class SnapshotRenderer {
    public:
        explicit SnapshotRenderer(ResourceManager &resources);

        //^ called beetween Begin() and EndDrawing()
        void Draw(const rtype::Snapshot &snapshot, std::uint8_t localPlayer, float deltaTime);

    private:
        void DrawShip(const rtype::Snapshot &snapshot, const rtype::SnapshotEntity &ship, float deltaTime);
        void DrawHud(const rtype::Snapshot &snapshot, std::uint8_t localPlayer) const;

        ResourceManager &_resources;
        std::array<ShipAnimation, rtype::MaxPlayers> _animations;

        //^ heigh of ship moove to have tilting effect
        std::array<std::optional<float>, rtype::MaxPlayers> _lastY;
    };

} //* namespace Client
