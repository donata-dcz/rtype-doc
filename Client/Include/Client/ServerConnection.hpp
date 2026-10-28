/*
** EPITECH PROJECT, 2026
** r-type
** File description:
** R-Type protocol spoken by client
*/

#pragma once

#include <cstdint>
#include <span>
#include <string>

#include "Client/NetworkClient.hpp"
#include "Input.hpp"
#include "Messages.hpp"
#include "Protocol.hpp"
#include "Snapshot.hpp"

namespace Client {

    enum class ConnectionStatus {
        Connecting,
        Connected,
        Rejected,
        Unreachable,
        Lost,
    };

    class ServerConnection {
    public:
        //^ sends first CONNECT
        ServerConnection(const std::string &host, std::uint16_t port, std::string name);

        void Update(float deltaTime);

        //^ when connected input is the one saying "alive"
        void SendInput(rtype::InputState input);

        ConnectionStatus GetStatus() const { return _status; }
        std::uint8_t GetPlayerId() const { return _playerId; }
        rtype::Network::RejectReason GetRejectReason() const { return _rejectReason; }

        const std::optional<rtype::Snapshot> &GetSnapshot() const { return _snapshot; }

    private:
        void SendConnect();
        void HandlePacket(std::span<const std::uint8_t> packet);
        void HandleConnectAnswer(Common::Network::MessageType type, Common::Network::BinaryReader &reader);
        void HandleSnapshot(Common::Network::BinaryReader &reader);


        NetworkClient _network;
        std::string _name;

        ConnectionStatus _status = ConnectionStatus::Connecting;
        std::uint8_t _playerId = 0;
        rtype::Network::RejectReason _rejectReason = rtype::Network::RejectReason::ServerFull;
        std::optional<rtype::Snapshot> _snapshot;

        std::uint32_t _sequence = 0;
        std::uint32_t _frame = 0;

        float _retryTimer = 0.0f;
        float _connectingTime = 0.0f;
        float _silentTime = 0.0f;
    };

} //* Namespace Client
