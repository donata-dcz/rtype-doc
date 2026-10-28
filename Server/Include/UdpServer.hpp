/*
** EPITECH PROJECT, 2026
** RType
** File description:
** UdpServer
*/

#pragma once

#include <asio.hpp>

#include <array>
#include <cstdint>
#include <mutex>
#include <thread>
#include <vector>

#include "Binary.hpp"

namespace serv {

struct ReceivedPacket {
    asio::ip::udp::endpoint from;
    std::vector<std::uint8_t> bytes;
};

class UdpServer {
public:
    explicit UdpServer(std::uint16_t port);
    ~UdpServer();

    UdpServer(const UdpServer&) = delete;
    UdpServer& operator=(const UdpServer&) = delete;

    //* Every datagram received since the last call: the game loop calls it at each tick
    std::vector<ReceivedPacket> PollReceived();

    //* Hands the bytes over to the network thread, the only one allowed to touch the socket
    void Send(const asio::ip::udp::endpoint& to, std::vector<std::uint8_t> bytes);

private:
    void Receive();

    asio::io_context _io;
    asio::ip::udp::socket _socket;
    std::thread _thread;

    std::mutex _mutex;
    std::vector<ReceivedPacket> _received;

    //* One byte more than the largest packet, to spot and drop datagrams that are too big
    std::array<std::uint8_t, Common::Network::MaxPacketSize + 1> _buffer{};
    asio::ip::udp::endpoint _from;
};

} //* namespace serv