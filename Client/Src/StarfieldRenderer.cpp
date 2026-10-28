/*
** EPITECH PROJECT, 2026
** r-type
** File description:
** Draw the starfield
*/

#include <raylib.h>

#include <Client/StarfieldRenderer.hpp>

namespace Client {

    void DrawStarfield(const Starfield &starfield)
    {
        for (const Star &star : starfield.GetStars()) {
            const StarLayer &layer = starfield.GetLayers()[star.layer];

            DrawRectangleV({star.x, star.y}, {layer.size, layer.size}, Color{255, 255, 255, layer.brightness});
        }
    }

} //* namespace Client
