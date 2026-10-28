/*
** EPITECH PROJECT, 2026
** r-type
** File description:
** content of each message of the R-Type protocol
*/

#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "Input.hpp"
#include "Binary.hpp"
#include "Snapshot.hpp"

namespace rtype::Network {

    using Common::Network::BinaryReader;
    using Common::Network::BinaryWriter;
    constexpr std::size_t MaxNameLength = 16;

    enum class RejectReason : std::uint8_t {
        ServerFull = 1,
        WrongVersion = 2,
    };

    struct ConnectMessage {
        std::string name;
    };

    struct AcceptMessage {
        std::uint8_t playerId;
    };

    struct RejectMessage {
        RejectReason reason;
    };

    struct InputMessage {
        //^ frame number of client grow up
        std::uint32_t frame;
        InputState input;
    };

    //^ encode functions return payload only decode functions read payload return nothing when field out of range
    std::vector<std::uint8_t> EncodeConnect(const ConnectMessage &message);
    std::optional<ConnectMessage> DecodeConnect(BinaryReader &reader);

    std::vector<std::uint8_t> EncodeAccept(const AcceptMessage &message);
    std::optional<AcceptMessage> DecodeAccept(BinaryReader &reader);

    std::vector<std::uint8_t> EncodeReject(const RejectMessage &message);
    std::optional<RejectMessage> DecodeReject(BinaryReader &reader);

    std::vector<std::uint8_t> EncodeInput(const InputMessage &message);
    std::optional<InputMessage> DecodeInput(BinaryReader &reader);

    std::vector<std::uint8_t> EncodeSnapshot(const Snapshot &snapshot);
    std::optional<Snapshot> DecodeSnapshot(BinaryReader &reader);

} //* namespace rtype::Network
