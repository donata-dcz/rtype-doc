/*
** EPITECH PROJECT, 2026
** r-type
** File description:
** Sends and receives UDP datagrams
*/

#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace Client {

    //& only moove byte : protocol give them meaning asio hidden in the cpp
    class NetworkClient {
    public:
        //^ error if host could not be resolved
        NetworkClient(const std::string &host, std::uint16_t port);
        ~NetworkClient();

        NetworkClient(const NetworkClient &) = delete;
        NetworkClient &operator=(const NetworkClient &) = delete;

        void Send(std::vector<std::uint8_t> datagram);
        //^ every datagram received since last call 
        std::vector<std::vector<std::uint8_t>> PollReceived();

    private:
        struct Impl;
        std::unique_ptr<Impl> _impl;
    };

} //* namespace Client
