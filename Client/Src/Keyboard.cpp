/*
** EPITECH PROJECT, 2026
** r-type
** File description:
** Turn keyboard into player actions
*/

#include <raylib.h>

#include <Client/Keyboard.hpp>

namespace Client {

    rtype::InputState ReadKeyboard()
    {
        rtype::InputState input;

        input.Set(rtype::Action::MoveUp, IsKeyDown(KEY_UP));
        input.Set(rtype::Action::MoveDown, IsKeyDown(KEY_DOWN));
        input.Set(rtype::Action::MoveLeft, IsKeyDown(KEY_LEFT));
        input.Set(rtype::Action::MoveRight, IsKeyDown(KEY_RIGHT));
        input.Set(rtype::Action::Shoot, IsKeyDown(KEY_SPACE));
        return input;
    }

} //* namespace Client
