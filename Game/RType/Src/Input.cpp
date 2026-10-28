/*
** EPITECH PROJECT, 2026
** r-type
** File description:
** Player actions shared by the client and the server
*/

#include "Input.hpp"

namespace rtype {

    namespace {

        //& Actions are stored in bit / Up = 0b00001 Down = 0b00010 Shoot = 0b10000
        std::uint8_t Mask(Action action)
        {
            return static_cast<std::uint8_t>(1u << static_cast<unsigned>(action));
        }

    } //* namespace anonyme

    void InputState::Set(Action action, bool active)
    {
        if (active)
            _bits = static_cast<std::uint8_t>(_bits | Mask(action));
        else
            _bits = static_cast<std::uint8_t>(_bits & ~Mask(action));
    }

    bool InputState::IsActive(Action action) const
    {
        return (_bits & Mask(action)) != 0;
    }

    std::uint8_t InputState::GetBits() const
    {
        return _bits;
    }

    InputState InputState::FromBits(std::uint8_t bits)
    {
        InputState state;

        state._bits = bits;
        return state;
    }

} //* namespace rtype
