/*
** EPITECH PROJECT, 2026
** r-type
** File description:
** Sends and receives UDP datagrams
*/

#include <array>
#include <mutex>
#include <thread>
#include <utility>

#include <asio.hpp>

#include "Client/NetworkClient.hpp"
#include "Network/Binary.hpp"

namespace Client {

    struct NetworkClient::Impl {
        asio::io_context io;
        asio::ip::udp::socket socket{io};
        asio::ip::udp::endpoint server;
        asio::ip::udp::endpoint sender;

        // One byte more to drop too big datagrams
        std::array<std::uint8_t, Common::Network::MaxPacketSize + 1> buffer{};
        std::mutex mutex;
        std::vector<std::vector<std::uint8_t>> received;
        std::thread thread;

        void Receive()
        {
            socket.async_receive_from(
                asio::buffer(buffer), sender, [this](const asio::error_code &error, std::size_t size) {
                    if (error == asio::error::operation_aborted)
                        return;

                    //! only server can talk to client and no more than max size
                    if (!error && sender == server && size <= Common::Network::MaxPacketSize) {
                        std::lock_guard lock(mutex);
                        received.emplace_back(buffer.begin(), buffer.begin() + static_cast<std::ptrdiff_t>(size));
                    }
                    Receive();
                });
        }
    };

    NetworkClient::NetworkClient(const std::string &host, std::uint16_t port) : _impl(std::make_unique<Impl>())
    {
        asio::ip::udp::resolver resolver(_impl->io);

        _impl->server = *resolver.resolve(asio::ip::udp::v4(), host, std::to_string(port)).begin();

        //^ port 0 system pick free port
        _impl->socket.open(asio::ip::udp::v4());
        _impl->socket.bind(asio::ip::udp::endpoint(asio::ip::udp::v4(), 0));
        _impl->Receive();
        _impl->thread = std::thread([impl = _impl.get()] { impl->io.run(); });
    }

    NetworkClient::~NetworkClient()
    {
        _impl->io.stop();
        if (_impl->thread.joinable())
            _impl->thread.join();
    }

    void NetworkClient::Send(std::vector<std::uint8_t> datagram)
    {
        auto packet = std::make_shared<std::vector<std::uint8_t>>(std::move(datagram));

        //^ only network thread touche socket nyte handed over 
        asio::post(_impl->io, [impl = _impl.get(), packet] {
            impl->socket.async_send_to(asio::buffer(*packet), impl->server, [packet](const asio::error_code &, std::size_t) {});
        });
    }

    std::vector<std::vector<std::uint8_t>> NetworkClient::PollReceived()
    {
        std::lock_guard lock(_impl->mutex);
        return std::exchange(_impl->received, {});

    }

} //* namespace Client
