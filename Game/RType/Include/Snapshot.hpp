/*
** EPITECH PROJECT, 2026
** r-type
** File description:
** What a client needs to draw 1 moment of the match
*/

#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

namespace rtype {

    constexpr std::size_t MaxPlayers = 4;

    enum class EntityType : std::uint8_t {
        Ship = 1,
        PlayerMissile = 2,
        Bydos = 3,
        EnemyMissile = 4,
    };

    struct SnapshotPlayer {
        std::uint8_t id;

        //^ alive and invincible share one flag byte bit 0 / bit 1
        bool alive;
        bool invincible;
        std::uint8_t lives;
        std::uint32_t score;
    };

    struct SnapshotEntity {
        std::uint32_t id;
        EntityType type;

        //^ rounded wold position
        std::int16_t x;
        std::int16_t y;
    };

    //^ content of snapshot message sent 30 times/sec
    struct Snapshot {
        std::uint32_t tick;
        std::vector<SnapshotPlayer> players;
        std::vector<SnapshotEntity> entities;
    };

    //^ snapshot safe datagram size 128 x 9 bytes
    constexpr std::size_t MaxSnapshotEntities = 128;

} //* namespace rtype
