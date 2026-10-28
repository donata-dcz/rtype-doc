/*
** EPITECH PROJECT, 2026
** r-type
** File description:
** Player actions shared by client and server
*/

#pragma once

#include <cstdint>

namespace rtype {

    //^ Action defining the player input
    enum class Action : std::uint8_t {
        MoveUp,
        MoveDown,
        MoveLeft,
        MoveRight,
        Shoot,
    };

    //& every action during one frame packed in single byte for network
    class InputState {
    public:
        void Set(Action action, bool active);
        bool IsActive(Action action) const;
        std::uint8_t GetBits() const;

        //^ rebuilt state recceived from network
        static InputState FromBits(std::uint8_t bits);

    private:
        std::uint8_t _bits = 0;
    };

} //* namespace rtype
