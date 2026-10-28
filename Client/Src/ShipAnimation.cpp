/*
** EPITECH PROJECT, 2026
** r-type
** File description:
** Tilt ship sprite when ship moves up or down
*/

#include <algorithm>
#include <cmath>

#include <Client/ShipAnimation.hpp>

namespace Client {

    namespace {

        constexpr float MaxTilt = 2.0f;

        //^ column per second flat -> tilted 2/10 -> 0.2s
        constexpr float TiltSpeed = 10.0f;

    } //* namespace anonyme

    void ShipAnimation::Update(float verticalMove, float deltaTime)
    {
        float target = 0.0f;

        //^ point down moove up -> negative
        if (verticalMove < 0.0f)
            target = MaxTilt;
        else if (verticalMove > 0.0f)
            target = -MaxTilt;
        if (_tilt < target)
            _tilt = std::min(_tilt + TiltSpeed * deltaTime, target);
        else
            _tilt = std::max(_tilt - TiltSpeed * deltaTime, target);
    }

    int ShipAnimation::GetColumn() const
    {
        return 2 + static_cast<int>(std::lround(_tilt));
    }

} //* namespace Client
