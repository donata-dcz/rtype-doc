/*
** EPITECH PROJECT, 2026
** r-type
** File description:
** Parallax starfield scrolling in the background
*/

#pragma once

#include <cstddef>
#include <cstdint>
#include <random>
#include <vector>

namespace Client {

    //^ One depth plane of sky:the farther -> the slower -> smaller 
    struct StarLayer {
        float speed;
        float size;
        std::uint8_t brightness;
        std::size_t count;
    };

    struct Star {
        float x;
        float y;
        std::size_t layer;
    };

    //& Logic star moove with elapsed time and not the frame rate
    class Starfield {
    public:
        Starfield(float width, float height, std::vector<StarLayer> layers, std::uint32_t seed);

        void Update(float deltaTime);

        const std::vector<Star> &GetStars() const;
        const std::vector<StarLayer> &GetLayers() const;
        static std::vector<StarLayer> DefaultLayers();

    private:
        float Random(float max);
        float _width;
        float _height;
        std::vector<StarLayer> _layers;
        std::vector<Star> _stars;
        std::mt19937 _random;
    };

} //* namespace Client
