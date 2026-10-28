/*
** EPITECH PROJECT, 2026
** r-type
** File description:
** Draws the starfield
*/

#pragma once

#include <Client/Starfield.hpp>

namespace Client {

    //& need to be called beetween Begin() and EndDrawing() of raylib
    void DrawStarfield(const Starfield &starfield);

} //* namespace Client
