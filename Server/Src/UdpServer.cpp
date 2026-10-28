/*
** EPITECH PROJECT, 2026
** RType
** File description:
** UdpServer
*/

#include "UdpServer.hpp"

#include <iostream>
#include <utility>

namespace serv {

UdpServer::UdpServer(std::uint16_t port)
    : _socket(_io, asio::ip::udp::endpoint(asio::ip::udp::v4(), port))
{
    Receive();
    _thread = std::thread([this]() {
        _io.run();
    });
}

UdpServer::~UdpServer()
{
    _io.stop();
    if (_thread.joinable())
        _thread.join();
}

void UdpServer::Receive()
{
    _socket.async_receive_from(asio::buffer(_buffer), _from,
        [this](const asio::error_code& error, std::size_t bytesReceived) {
            //* Bigger than the largest packet: it is not ours, drop it
            if (!error && bytesReceived <= Common::Network::MaxPacketSize) {
                ReceivedPacket packet{_from, std::vector<std::uint8_t>(
                    _buffer.begin(), _buffer.begin() + static_cast<std::ptrdiff_t>(bytesReceived))};
                std::lock_guard<std::mutex> lock(_mutex);

                _received.push_back(std::move(packet));
            }
            //* Any other error (e.g. a client that vanished, on Windows) must not stop the server
            if (error != asio::error::operation_aborted)
                Receive();
        });
}

std::vector<ReceivedPacket> UdpServer::PollReceived()
{
    std::lock_guard<std::mutex> lock(_mutex);

    return std::exchange(_received, {});
}

void UdpServer::Send(const asio::ip::udp::endpoint& to, std::vector<std::uint8_t> bytes)
{
    if (bytes.size() > Common::Network::MaxPacketSize) {
        std::cerr << "UDP packet too large, not sent" << std::endl;
        return;
    }
    asio::post(_io, [this, to, bytes = std::move(bytes)]() {
        asio::error_code error;

        _socket.send_to(asio::buffer(bytes), to, 0, error);
        if (error)
            std::cerr << "UDP send error: " << error.message() << std::endl;
    });
}

} //* namespace serv