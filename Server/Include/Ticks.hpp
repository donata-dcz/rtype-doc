/*
** EPITECH PROJECT, 2026
** RType
** File description:
** Ticks
*/

#pragma once

#include <chrono>
#include <csignal>
#include <cstddef>
#include <cstdint>
#include <vector>

#include "Game.hpp"
#include "Engine/GameLoop.hpp"

#include "UdpServer.hpp"

namespace serv {

constexpr double TickRate = 60.0;
//* 1 snapshot every 2 ticks

constexpr std::uint32_t TicksPerSnapshot = 2;
constexpr auto SessionTimeout = std::chrono::seconds(3);

//* A client that completed CONNECT -> ACCEPT: only these receive snapshots
struct Session {
    asio::ip::udp::endpoint endpoint;
    std::uint8_t playerId;
    std::chrono::steady_clock::time_point lastSeen;
};

struct TickState {
    std::uint32_t ticks = 0;
    std::uint32_t ticksThisSecond = 0;
    std::size_t entities = 0;
    std::vector<Session> sessions;
    //* Grows by one for every packet the server sends
    std::uint32_t sequence = 0;

    std::chrono::steady_clock::time_point lastReport =
        std::chrono::steady_clock::now();
};

extern volatile std::sig_atomic_t stopRequested;

void RequestStop(int signal);

void SetTickCallback(
    Engine::GameLoop& loop,
    rtype::Game& game,
    TickState& state,
    UdpServer& network
);

void SetFrameCallback(
    Engine::GameLoop& loop,
    TickState& state
);

} //* namespace serv