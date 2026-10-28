/*
** EPITECH PROJECT, 2026
** r-type
** File description:
** Header shared by every packet of the RType protocol
*/

#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <vector>

#include "Binary.hpp"

namespace Common::Network {

    // RT in ascii datagram strat with other = not Rtype packet
    constexpr std::uint16_t Magic = 0x5254;
    constexpr std::uint8_t ProtocolVersion = 1;
    constexpr std::uint16_t DefaultPort = 4242;

    // magic : 2 + version : 1 + type : 1 + sequence : 4 + payload size : 2
    constexpr std::size_t HeaderSize = 10;

    // client to server under 0x80 server to client from 0x80
    enum class MessageType : std::uint8_t {
        Connect = 0x01,
        Input = 0x02,
        Ping = 0x03,
        Disconnect = 0x04,
        Accept = 0x81,
        Reject = 0x82,
        Snapshot = 0x83,
        Event = 0x84,
        Pong = 0x85,
    };

    struct Header {
        MessageType type;
        // grows by one for every packet that is sent
        std::uint32_t sequence;
        std::uint16_t payloadSize;
    };

    // build the entire datagram followed by the 10 octets payload
    std::vector<std::uint8_t> BuildPacket(MessageType type, std::uint32_t sequence, std::span<const std::uint8_t> payload);

    // reads checks the header returns nothing when datagram must be dropped: too big, wrong magic, version, unknown type, payload size
    // On success reader is on the first byte of the payload
    std::optional<Header> ReadHeader(BinaryReader &reader);

} //* namespace comNet
