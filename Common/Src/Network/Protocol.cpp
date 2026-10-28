/*
** EPITECH PROJECT, 2026
** r-type
** File description:
** Header shared by every packet of the RType protocol
*/

#include "Protocol.hpp"

namespace Common::Network {

    namespace {

        bool IsKnownType(std::uint8_t type)
        {
            switch (static_cast<MessageType>(type)) {
                case MessageType::Connect:
                case MessageType::Input:
                case MessageType::Ping:
                case MessageType::Disconnect:
                case MessageType::Accept:
                case MessageType::Reject:
                case MessageType::Snapshot:
                case MessageType::Event:
                case MessageType::Pong:
                    return true;
            }
            return false;
        }

    } //* namespace anonyme

    std::vector<std::uint8_t> BuildPacket(MessageType type, std::uint32_t sequence, std::span<const std::uint8_t> payload)
    {
        BinaryWriter writer;

        writer.WriteU16(Magic);
        writer.WriteU8(ProtocolVersion);
        writer.WriteU8(static_cast<std::uint8_t>(type));
        writer.WriteU32(sequence);
        writer.WriteU16(static_cast<std::uint16_t>(payload.size()));
        writer.WriteBytes(payload);
        return writer.GetBuffer();
    }

    std::optional<Header> ReadHeader(BinaryReader &reader)
    {
        std::uint16_t magic = 0;
        std::uint8_t version = 0;
        std::uint8_t type = 0;
        std::uint32_t sequence = 0;
        std::uint16_t payloadSize = 0;

        if (reader.GetRemaining() > MaxPacketSize)
            return std::nullopt;
        if (!reader.ReadU16(magic) || !reader.ReadU8(version) || !reader.ReadU8(type) ||
            !reader.ReadU32(sequence) || !reader.ReadU16(payloadSize))
            return std::nullopt;
        if (magic != Magic || version != ProtocolVersion || !IsKnownType(type))
            return std::nullopt;
        if (payloadSize != reader.GetRemaining())
            return std::nullopt;
        return Header{static_cast<MessageType>(type), sequence, payloadSize};
    }

} //* namespace Common::Network
