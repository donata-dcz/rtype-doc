/*
** EPITECH PROJECT, 2026
** r-type
** File description:
** writes and reads number in network byte order
*/

#pragma once

#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>

namespace Common::Network {

    // largest datagram send or accepted is : 1400
    constexpr std::size_t MaxPacketSize = 1400;

    // append number in network byte order no matter the machine
    class BinaryWriter
    {
    public:
        void WriteU8(std::uint8_t value);
        void WriteU16(std::uint16_t value);
        void WriteU32(std::uint32_t value);
        void WriteI16(std::int16_t value);

        // append bytes as they are, playload already encoded
        void WriteBytes(std::span<const std::uint8_t> bytes);

        const std::vector<std::uint8_t> &GetBuffer() const;

    private:
        std::vector<std::uint8_t> _buffer;
    };

    // read what BinaryWriter wrote every read check enough byte are left
    // short or corupted packet = failed instead of reading out of memeory
    class BinaryReader
    {
    public:
        explicit BinaryReader(std::span<const std::uint8_t> data);

        bool ReadU8(std::uint8_t &value);
        bool ReadU16(std::uint16_t &value);
        bool ReadU32(std::uint32_t &value);
        bool ReadI16(std::int16_t &value);

        std::size_t GetRemaining() const;

    private:
        std::span<const std::uint8_t> _data;
        std::size_t _position = 0;
    };

} //* namepsace Common::Network
