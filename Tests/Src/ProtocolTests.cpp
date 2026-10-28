/*
** EPITECH PROJECT, 2026
** r-type
** File description:
** Tests for Protocol
*/

#include <gtest/gtest.h>
#include "Protocol.hpp"
#include "Game.hpp"
#include "Network/Messages.hpp"

TEST(Protocol, VersionIsOne)
{
    EXPECT_EQ(Common::Network::ProtocolVersion, 1);
}

TEST(Protocol, WorldStateSnapshotBroadcast)
{
    rtype::Game game(42);
    game.AddPlayer(0);
    game.AddPlayer(1);
    game.Tick(0.1f);
    rtype::Snapshot snapshot = game.BuildSnapshot();
    EXPECT_EQ(snapshot.players.size(), 2u);

    std::vector<std::uint8_t> encoded = rtype::Network::EncodeSnapshot(snapshot);
    Common::Network::BinaryReader reader(encoded);
    std::optional<rtype::Snapshot> decoded = rtype::Network::DecodeSnapshot(reader);
    ASSERT_TRUE(decoded.has_value());
    EXPECT_EQ(decoded->tick, snapshot.tick);
    EXPECT_EQ(decoded->players.size(), snapshot.players.size());
    EXPECT_EQ(decoded->entities.size(), snapshot.entities.size());
}
