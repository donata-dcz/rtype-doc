/*
** EPITECH PROJECT, 2026
** r-type
** File description:
** Parallax starfield scrolling
*/

#include <utility>

#include <Client/Starfield.hpp>

namespace Client {

    Starfield::Starfield(float width, float height, std::vector<StarLayer> layers, std::uint32_t seed)
        : _width(width), _height(height), _layers(std::move(layers)), _random(seed)
    {
        for (std::size_t layer = 0; layer < _layers.size(); layer++) {
            for (std::size_t i = 0; i < _layers[layer].count; i++)
                _stars.push_back({Random(_width), Random(_height), layer});
        }
    }

    void Starfield::Update(float deltaTime)
    {
        for (Star &star : _stars) {
            const StarLayer &layer = _layers[star.layer];

            star.x -= layer.speed * deltaTime;
            if (star.x < -layer.size) {
                star.x += _width + layer.size;
                star.y = Random(_height);
            }
        }
    }

    const std::vector<Star> &Starfield::GetStars() const
    {
        return _stars;
    }

    const std::vector<StarLayer> &Starfield::GetLayers() const
    {
        return _layers;
    }

    std::vector<StarLayer> Starfield::DefaultLayers()
    {
        return {
            {12.0f, 1.0f, 90, 140}, //^ far: slow, small, dim, many
            {30.0f, 2.0f, 160, 70}, //^ middle
            {60.0f, 3.0f, 230, 30}, //^ near: fast, big, bright, less
        };
    }

    float Starfield::Random(float max)
    {
        return std::uniform_real_distribution<float>(0.0f, max)(_random);
    }

} //* namespace Client
