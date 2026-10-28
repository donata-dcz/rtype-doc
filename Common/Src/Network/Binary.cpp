/*
** EPITECH PROJECT, 2026
** r-type
** File description:
** writes and reads number in network byte order
*/

#include "Binary.hpp"

namespace Common::Network {

    void BinaryWriter::WriteU8(std::uint8_t value)
    {
        _buffer.push_back(value);
    }

    void BinaryWriter::WriteU16(std::uint16_t value)
    {
        // most important byte first 0x5254 = 0x52 then 0x54
        _buffer.push_back(static_cast<std::uint8_t>(value >> 8));
        _buffer.push_back(static_cast<std::uint8_t>(value & 0xFF));
    }

    void BinaryWriter::WriteU32(std::uint32_t value)
    {
        WriteU16(static_cast<std::uint16_t>(value >> 16));
        WriteU16(static_cast<std::uint16_t>(value & 0xFFFF));
    }

    void BinaryWriter::WriteI16(std::int16_t value)
    {
        // negative number travel as complement bit pattern
        WriteU16(static_cast<std::uint16_t>(value));
    }

    void BinaryWriter::WriteBytes(std::span<const std::uint8_t> bytes)
    {
        _buffer.insert(_buffer.end(), bytes.begin(), bytes.end());
    }

    const std::vector<std::uint8_t> &BinaryWriter::GetBuffer() const
    {
        return _buffer;
    }

    BinaryReader::BinaryReader(std::span<const std::uint8_t> data) : _data(data)
    {
    }

    bool BinaryReader::ReadU8(std::uint8_t &value)
    {
        if (GetRemaining() < 1)
            return false;
        value = _data[_position];
        _position += 1;
        return true;
    }

    bool BinaryReader::ReadU16(std::uint16_t &value)
    {
        if (GetRemaining() < 2)
            return false;
        value = static_cast<std::uint16_t>((_data[_position] << 8) | _data[_position + 1]);
        _position += 2;
        return true;
    }

    bool BinaryReader::ReadU32(std::uint32_t &value)
    {
        std::uint16_t high = 0;
        std::uint16_t low = 0;

        if (GetRemaining() < 4)
            return false;
        ReadU16(high);
        ReadU16(low);
        value = (static_cast<std::uint32_t>(high) << 16) | low;
        return true;
    }

    bool BinaryReader::ReadI16(std::int16_t &value)
    {
        std::uint16_t bits = 0;

        if (!ReadU16(bits))
            return false;
        value = static_cast<std::int16_t>(bits);
        return true;
    }

    std::size_t BinaryReader::GetRemaining() const
    {
        return _data.size() - _position;
    }

} //* namespace Common::Network

