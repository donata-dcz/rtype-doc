/*
** EPITECH PROJECT, 2026
** r-type
** File description:
** R-Type protocol spoken by client
*/

#include <utility>

#include "Client/ServerConnection.hpp"
#include "Network/Binary.hpp"
#include "Network/Protocol.hpp"

namespace {

    constexpr float ConnectRetryDelay = 0.5f;
    constexpr float ConnectTimeout = 5.0f;
    constexpr float LostTimeout = 3.0f;

}

namespace Client {

    ServerConnection::ServerConnection(const std::string &host, std::uint16_t port, std::string name) : _network(host, port), _name(std::move(name))
    {
        SendConnect();
    }

    void ServerConnection::Update(float deltaTime)
    {
        for (const auto &packet : _network.PollReceived())
            HandlePacket(packet);

        if (_status == ConnectionStatus::Connected) {
            _silentTime += deltaTime;
            if (_silentTime >= LostTimeout)
                _status = ConnectionStatus::Lost;
            return;
        }
        if (_status != ConnectionStatus::Connecting)
            return;
        _connectingTime += deltaTime;

        if (_connectingTime >= ConnectTimeout) {
            _status = ConnectionStatus::Unreachable;
            return;
        }
        _retryTimer += deltaTime;
        if (_retryTimer >= ConnectRetryDelay) {
            _retryTimer = 0.0f;
            SendConnect();
        }
    }

    void ServerConnection::SendInput(rtype::InputState input)
    {
        using namespace Common::Network;
        using namespace rtype::Network;

        if (_status != ConnectionStatus::Connected)
            return;
        _network.Send(BuildPacket(MessageType::Input, _sequence++, EncodeInput({_frame++, input})));
    }

    void ServerConnection::SendConnect()
    {
        using namespace Common::Network;
        using namespace rtype::Network;

        _network.Send(BuildPacket(MessageType::Connect, _sequence++, EncodeConnect({_name})));
    }

    void ServerConnection::HandlePacket(std::span<const std::uint8_t> packet)
    {
        Common::Network::BinaryReader reader(packet);
        const auto header = Common::Network::ReadHeader(reader);

        //^ proof of valid packet
        if (!header)
            return;
        _silentTime = 0.0f;
        if (_status == ConnectionStatus::Connecting)
            HandleConnectAnswer(header->type, reader);
        else if (_status == ConnectionStatus::Connected && header->type == Common::Network::MessageType::Snapshot)
            HandleSnapshot(reader);
    }

    void ServerConnection::HandleConnectAnswer(Common::Network::MessageType type, Common::Network::BinaryReader &reader)
    {
        using namespace Common::Network;
        using namespace rtype::Network;

        if (type == MessageType::Accept) {
            if (const auto accept = DecodeAccept(reader)) {
                _playerId = accept->playerId;
                _status = ConnectionStatus::Connected;
            }
        } else if (type == MessageType::Reject) {
            if (const auto reject = DecodeReject(reader)) {
                _rejectReason = reject->reason;
                _status = ConnectionStatus::Rejected;
            }
        }
    }

    void ServerConnection::HandleSnapshot(Common::Network::BinaryReader &reader)
    {
        auto snapshot = rtype::Network::DecodeSnapshot(reader);

        if (!snapshot || (_snapshot && snapshot->tick <= _snapshot->tick))
            return;
        _snapshot = std::move(snapshot);
    }

} //* namespace Client