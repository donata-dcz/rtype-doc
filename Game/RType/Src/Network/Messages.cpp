/*
** EPITECH PROJECT, 2026
** r-type
** File description:
** Content of each message of the R-Type protocol, and how it is turned into bytes
*/

#include <algorithm>

#include "Messages.hpp"

namespace rtype::Network {

    namespace {

        //^ 5 actions use bits 0 to 4 other bit = packet corupted
        constexpr std::uint8_t KnownActionBits = 0b11111;
        constexpr std::uint8_t AliveFlag = 0b01;
        constexpr std::uint8_t InvincibleFlag = 0b10;

        //^ id 4 + type 1 + x 2 + y 2
        constexpr std::size_t EntitySize = 9;

        bool IsKnownEntityType(std::uint8_t type)
        {
            return type >= static_cast<std::uint8_t>(EntityType::Ship) && type <= static_cast<std::uint8_t>(EntityType::EnemyMissile);
        }

    } //* namespace anonyme

    std::vector<std::uint8_t> EncodeConnect(const ConnectMessage &message)
    {
        BinaryWriter writer;
        const std::size_t length = std::min(message.name.size(), MaxNameLength);

        writer.WriteU8(static_cast<std::uint8_t>(length));
        for (std::size_t i = 0; i < length; i++)
            writer.WriteU8(static_cast<std::uint8_t>(message.name[i]));
        return writer.GetBuffer();
    }

    std::optional<ConnectMessage> DecodeConnect(BinaryReader &reader)
    {
        std::uint8_t length = 0;
        ConnectMessage message;

        if (!reader.ReadU8(length) || length > MaxNameLength || length != reader.GetRemaining())
            return std::nullopt;
        for (std::uint8_t i = 0; i < length; i++) {
            std::uint8_t character = 0;

            reader.ReadU8(character);
            // printable ASCCI only
            if (character < 0x20 || character > 0x7E)
                return std::nullopt;
            message.name.push_back(static_cast<char>(character));
        }
        return message;
    }

    std::vector<std::uint8_t> EncodeAccept(const AcceptMessage &message)
    {
        BinaryWriter writer;

        writer.WriteU8(message.playerId);
        return writer.GetBuffer();
    }

    std::optional<AcceptMessage> DecodeAccept(BinaryReader &reader)
    {
        std::uint8_t playerId = 0;

        if (!reader.ReadU8(playerId) || playerId >= MaxPlayers || reader.GetRemaining() != 0)
            return std::nullopt;
        return AcceptMessage{playerId};
    }

    std::vector<std::uint8_t> EncodeReject(const RejectMessage &message)
    {
        BinaryWriter writer;

        writer.WriteU8(static_cast<std::uint8_t>(message.reason));
        return writer.GetBuffer();
    }

    std::optional<RejectMessage> DecodeReject(BinaryReader &reader)
    {
        std::uint8_t reason = 0;

        if (!reader.ReadU8(reason) || reader.GetRemaining() != 0)
            return std::nullopt;
        if (reason != static_cast<std::uint8_t>(RejectReason::ServerFull) &&
            reason != static_cast<std::uint8_t>(RejectReason::WrongVersion))
            return std::nullopt;
        return RejectMessage{static_cast<RejectReason>(reason)};
    }

    std::vector<std::uint8_t> EncodeInput(const InputMessage &message)
    {
        BinaryWriter writer;

        writer.WriteU32(message.frame);
        writer.WriteU8(message.input.GetBits());
        return writer.GetBuffer();
    }

    std::optional<InputMessage> DecodeInput(BinaryReader &reader)
    {
        std::uint32_t frame = 0;
        std::uint8_t bits = 0;

        if (!reader.ReadU32(frame) || !reader.ReadU8(bits) || reader.GetRemaining() != 0)
            return std::nullopt;
        if ((bits & ~KnownActionBits) != 0)
            return std::nullopt;
        return InputMessage{frame, InputState::FromBits(bits)};
    }

    std::vector<std::uint8_t> EncodeSnapshot(const Snapshot &snapshot)
    {
        BinaryWriter writer;
        const std::size_t playerCount = std::min(snapshot.players.size(), MaxPlayers);
        const std::size_t entityCount = std::min(snapshot.entities.size(), MaxSnapshotEntities);

        writer.WriteU32(snapshot.tick);
        writer.WriteU8(static_cast<std::uint8_t>(playerCount));
        for (std::size_t i = 0; i < playerCount; i++) {
            const SnapshotPlayer &player = snapshot.players[i];
            const std::uint8_t flags = static_cast<std::uint8_t>((player.alive ? AliveFlag : 0) | (player.invincible ? InvincibleFlag : 0));

            writer.WriteU8(player.id);
            writer.WriteU8(flags);
            writer.WriteU8(player.lives);
            writer.WriteU32(player.score);
        }
        writer.WriteU16(static_cast<std::uint16_t>(entityCount));
        for (std::size_t i = 0; i < entityCount; i++) {
            const SnapshotEntity &entity = snapshot.entities[i];

            writer.WriteU32(entity.id);
            writer.WriteU8(static_cast<std::uint8_t>(entity.type));
            writer.WriteI16(entity.x);
            writer.WriteI16(entity.y);
        }
        return writer.GetBuffer();
    }

    std::optional<Snapshot> DecodeSnapshot(BinaryReader &reader)
    {
        Snapshot snapshot{};
        std::uint8_t playerCount = 0;
        std::uint16_t entityCount = 0;

        if (!reader.ReadU32(snapshot.tick) || !reader.ReadU8(playerCount) || playerCount > MaxPlayers)
            return std::nullopt;
        for (std::uint8_t i = 0; i < playerCount; i++) {
            SnapshotPlayer player{};
            std::uint8_t flags = 0;

            if (!reader.ReadU8(player.id) || !reader.ReadU8(flags) || !reader.ReadU8(player.lives) ||
                !reader.ReadU32(player.score))
                return std::nullopt;
            if (player.id >= MaxPlayers || (flags & ~(AliveFlag | InvincibleFlag)) != 0)
                return std::nullopt;
            player.alive = (flags & AliveFlag) != 0;
            player.invincible = (flags & InvincibleFlag) != 0;
            snapshot.players.push_back(player);
        }
        if (!reader.ReadU16(entityCount) || entityCount > MaxSnapshotEntities)
            return std::nullopt;

        //* check size before alowing to avoid double alocation and memeory poorly handled
        if (static_cast<std::size_t>(entityCount) * EntitySize != reader.GetRemaining())
            return std::nullopt;
        snapshot.entities.reserve(entityCount);
        for (std::uint16_t i = 0; i < entityCount; i++) {
            SnapshotEntity entity{};
            std::uint8_t type = 0;

            reader.ReadU32(entity.id);
            reader.ReadU8(type);
            reader.ReadI16(entity.x);
            reader.ReadI16(entity.y);
            if (!IsKnownEntityType(type))
                return std::nullopt;
            entity.type = static_cast<EntityType>(type);
            snapshot.entities.push_back(entity);
        }
        return snapshot;
    }

} //* namespace rtype::Network
