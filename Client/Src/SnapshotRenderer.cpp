/*
** EPITECH PROJECT, 2026
** r-type
** File description:
** draw snapshot of the match what the server says is happening
*/

#include <algorithm>

#include <raylib.h>

#include <Client/SnapshotRenderer.hpp>

#include "Weapon.hpp"
#include "Enemies.hpp"

namespace Client {

    namespace {

        //^ ship is 33x17 cell of the spritesheet one row by player color one column by tilt
        constexpr float ShipCellWidth = 33.0f;
        constexpr float ShipCellHeight = 17.0f;

        const rtype::SnapshotPlayer *FindPlayer(const rtype::Snapshot &snapshot, std::uint32_t id)
        {
            for (const rtype::SnapshotPlayer &player : snapshot.players) {
                if (player.id == id)
                    return &player;
            }
            return nullptr;
        }

    } //* namespace anonyme

    SnapshotRenderer::SnapshotRenderer(ResourceManager &resources) : _resources(resources)
    {
    }

    void SnapshotRenderer::Draw(const rtype::Snapshot &snapshot, std::uint8_t localPlayer, float deltaTime)
    {
        const float screenWidth = static_cast<float>(GetScreenWidth());
        const float screenHeight = static_cast<float>(GetScreenHeight());

        //^ wold centered and proportionous no matter screen size
        const float scale = std::min(screenWidth / rtype::WorldWidth, screenHeight / rtype::WorldHeight);

        Camera2D camera = {};
        camera.offset = {(screenWidth - rtype::WorldWidth * scale) / 2.0f, (screenHeight - rtype::WorldHeight * scale) / 2.0f};
        camera.zoom = scale;
        BeginMode2D(camera);
        for (const rtype::SnapshotEntity &entity : snapshot.entities) {
            const float x = static_cast<float>(entity.x);
            const float y = static_cast<float>(entity.y);

            switch (entity.type) {
                case rtype::EntityType::Ship:
                    DrawShip(snapshot, entity, deltaTime);
                    break;
                case rtype::EntityType::PlayerMissile:
                    DrawRectangleV({x, y - rtype::MissileHeight / 2.0f}, {rtype::MissileWidth, rtype::MissileHeight}, YELLOW);
                    break;
                case rtype::EntityType::Bydos:
                    DrawRectangleV({x, y}, {rtype::EnemyWidth, rtype::EnemyHeight}, RED);
                    break;
                case rtype::EntityType::EnemyMissile:
                    DrawRectangleV({x, y - rtype::MissileHeight / 2.0f}, {rtype::MissileWidth, rtype::MissileHeight}, ORANGE);
                    break;
            }
        }
        EndMode2D();
        DrawHud(snapshot, localPlayer);
    }

    void SnapshotRenderer::DrawShip(const rtype::Snapshot &snapshot, const rtype::SnapshotEntity &ship, float deltaTime)
    {
        //^ ship use played id so it also pick the color
        const std::size_t id = ship.id;

        if (id >= rtype::MaxPlayers)
            return;
        const float x = static_cast<float>(ship.x);
        const float y = static_cast<float>(ship.y);

        _animations[id].Update(_lastY[id] ? y - *_lastY[id] : 0.0f, deltaTime);
        _lastY[id] = y;
        const rtype::SnapshotPlayer *player = FindPlayer(snapshot, ship.id);

        //^ ship blinks when invincible
        if (player && player->invincible && static_cast<int>(GetTime() * 10.0) % 2 == 0)
            return;

        const Rectangle frame = {ShipCellWidth * static_cast<float>(_animations[id].GetColumn()), ShipCellHeight * static_cast<float>(id), ShipCellWidth, ShipCellHeight};

        DrawTexturePro(_resources.GetTexture("Sprites/PlayerShips.gif"), frame, {x, y, rtype::PlayerWidth, rtype::PlayerHeight}, {0.0f, 0.0f}, 0.0f, WHITE);
        DrawText(TextFormat("P%d", static_cast<int>(id) + 1), static_cast<int>(x), static_cast<int>(y) - 16, 14, LIGHTGRAY);
    }

    void SnapshotRenderer::DrawHud(const rtype::Snapshot &snapshot, std::uint8_t localPlayer) const
    {
        int line = 0;

        for (const rtype::SnapshotPlayer &player : snapshot.players) {
            DrawText(TextFormat("P%d   SCORE %u   LIVES %d", static_cast<int>(player.id) + 1, static_cast<unsigned>(player.score), static_cast<int>(player.lives)), 20, 20 + line * 24, 20, player.id == localPlayer ? WHITE : GRAY);
            line++;
        }

        const rtype::SnapshotPlayer *me = FindPlayer(snapshot, localPlayer);
        if (me && !me->alive)
            DrawText("GAME OVER", GetScreenWidth() / 2 - 120, GetScreenHeight() / 2 - 20, 40, RED);
    }

} //* namespace Client
