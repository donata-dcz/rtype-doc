/*
** EPITECH PROJECT, 2026
** RType
** File description:
** main server
*/

#include <csignal>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <random>
#include <string_view>
#include <system_error>

#include "Game.hpp"
#include "Protocol.hpp"
#include "Engine/GameLoop.hpp"
#include "Ticks.hpp"
#include "UdpServer.hpp"

namespace {

std::uint16_t ParsePort(int argc, char **argv)
{
    for (int i = 1; i + 1 < argc; i += 2) {
        if (std::string_view(argv[i]) == "-p")
            return static_cast<std::uint16_t>(std::strtoul(argv[i + 1], nullptr, 10));
    }
    return Common::Network::DefaultPort;
}

} //* anonymous namespace

int main(int argc, char **argv)
{
    const std::uint16_t port = ParsePort(argc, argv);
    std::unique_ptr<serv::UdpServer> network;

    try {
        network = std::make_unique<serv::UdpServer>(port);
    } catch (const std::system_error& error) {
        std::cerr << "Cannot open UDP port " << port << ": " << error.what() << std::endl;
        return 84;
    }

    rtype::Game game(std::random_device{}());
    Engine::GameLoop loop(serv::TickRate);
    serv::TickState state;

    std::signal(SIGINT, serv::RequestStop);
    serv::SetTickCallback(loop, game, state, *network);
    serv::SetFrameCallback(loop, state);

    std::cout << "Server listening on UDP port " << port << ". Ctrl+C to stop." << std::endl;
    loop.Run();
    std::cout << "Server stopped after " << state.ticks << " ticks." << std::endl;
    return 0;
}