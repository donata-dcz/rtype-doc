/*
** EPITECH PROJECT, 2026
** RType
** File description:
** Ticks
*/

#include "Ticks.hpp"

#include <algorithm>
#include <iostream>
#include <optional>
#include <thread>

#include "Messages.hpp"
#include "Protocol.hpp"

namespace serv {

volatile std::sig_atomic_t stopRequested = 0;

void RequestStop(int)
{
    stopRequested = 1;
}

namespace {

using Common::Network::MessageType;

Session *FindSession(TickState& state, const asio::ip::udp::endpoint& endpoint)
{
    for (Session& session : state.sessions) {
        if (session.endpoint == endpoint)
            return &session;
    }
    return nullptr;
}

std::optional<std::uint8_t> FreePlayerId(const TickState& state)
{
    for (std::uint8_t id = 0; id < rtype::MaxPlayers; id++) {
        const bool taken = std::any_of(state.sessions.begin(), state.sessions.end(),
            [id](const Session& session) { return session.playerId == id; });

        if (!taken)
            return id;
    }
    return std::nullopt;
}

void Reply(UdpServer& network, TickState& state, const asio::ip::udp::endpoint& to,
    MessageType type, const std::vector<std::uint8_t>& payload)
{
    network.Send(to, Common::Network::BuildPacket(type, state.sequence++, payload));
}

void HandleConnect(const ReceivedPacket& packet, rtype::Game& game, TickState& state,
    UdpServer& network)
{
    Session *session = FindSession(state, packet.from);

    if (!session) {
        const std::optional<std::uint8_t> id = FreePlayerId(state);

        if (!id) {
            Reply(network, state, packet.from, MessageType::Reject,
                rtype::Network::EncodeReject({rtype::Network::RejectReason::ServerFull}));
            return;
        }
        game.AddPlayer(*id);
        state.sessions.push_back({packet.from, *id, std::chrono::steady_clock::now()});
        session = &state.sessions.back();
        std::cout << "player " << static_cast<int>(*id) << " joined from "
                  << packet.from.address().to_string() << ':' << packet.from.port() << std::endl;
    }
    session->lastSeen = std::chrono::steady_clock::now();
    //* A lost ACCEPT makes the client send CONNECT again: answer with the same id
    Reply(network, state, packet.from, MessageType::Accept,
        rtype::Network::EncodeAccept({session->playerId}));
}

void HandlePacket(const ReceivedPacket& packet, rtype::Game& game, TickState& state,
    UdpServer& network)
{
    Common::Network::BinaryReader reader(packet.bytes);
    const auto header = Common::Network::ReadHeader(reader);

    //* Not an R-Type packet, or damaged: ignored
    if (!header)
        return;
    if (header->type == MessageType::Connect) {
        if (rtype::Network::DecodeConnect(reader))
            HandleConnect(packet, game, state, network);
        return;
    }
    Session *session = FindSession(state, packet.from);

    //* Only players who completed CONNECT -> ACCEPT are listened to
    if (!session)
        return;
    session->lastSeen = std::chrono::steady_clock::now();
    if (header->type == MessageType::Input) {
        //* The player id comes from the session (the sender's address), never from the packet
        if (const auto input = rtype::Network::DecodeInput(reader))
            game.SetInput(session->playerId, input->input);
    }
}

void RemoveSilentSessions(rtype::Game& game, TickState& state)
{
    const auto now = std::chrono::steady_clock::now();

    std::erase_if(state.sessions, [&game, now](const Session& session) {
        if (now - session.lastSeen < SessionTimeout)
            return false;
        //* Its ship leaves the snapshots: the other players see it disappear
        game.RemovePlayer(session.playerId);
        std::cout << "player " << static_cast<int>(session.playerId) << " timed out" << std::endl;
        return true;
    });
}

void BroadcastSnapshot(const  rtype::Game& game, TickState& state, UdpServer& network)
{
    const rtype::Snapshot snapshot = game.BuildSnapshot();
    const std::vector<std::uint8_t> bytes = Common::Network::BuildPacket(
        MessageType::Snapshot, state.sequence++, rtype::Network::EncodeSnapshot(snapshot));

    state.entities = snapshot.entities.size();
    //* Send() takes a copy: every player gets the whole snapshot
    for (const Session& session : state.sessions)
        network.Send(session.endpoint, bytes);
}

} //* anonymous namespace

void SetTickCallback(
    Engine::GameLoop& loop,
    rtype::Game& game,
    TickState& state,
    UdpServer& network
)
{
    loop.SetTickCallback([&game, &state, &network](double deltaTime) {
        //* Read what the players sent since the last tick
        for (const ReceivedPacket& packet : network.PollReceived())
            HandlePacket(packet, game, state, network);
        RemoveSilentSessions(game, state);

        //* Update the game
        game.Tick(static_cast<float>(deltaTime));
        state.ticks++;
        state.ticksThisSecond++;

        //* Tell every player what is happening, 30 times per second
        if (state.ticks % TicksPerSnapshot == 0)
            BroadcastSnapshot(game, state, network);
    });
}

void SetFrameCallback(
    Engine::GameLoop& loop,
    TickState& state
)
{
    loop.SetFrameCallback([&loop, &state](double) {
        const auto now = std::chrono::steady_clock::now();

        if (stopRequested)
            loop.Stop();
        if (now - state.lastReport >= std::chrono::seconds(1)) {
            std::cout << "server : " << state.ticksThisSecond << " ticks/s, "
                      << state.entities << " entities, "
                      << state.sessions.size() << " players" << std::endl;
            state.ticksThisSecond = 0;
            state.lastReport = now;
        }
        //* GameLoop::Run() loops without sleeping: give the CPU back between two ticks
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    });
}

} //* namespace serv