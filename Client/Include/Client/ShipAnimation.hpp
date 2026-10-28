/*
** EPITECH PROJECT, 2026
** r-type
** File description:
** Tilt ship sprite when ship moves up or down
*/

#pragma once

namespace Client {

    //& Picks the column of the ship in the sprite sheet: 2 is flat, 3 and 4 tilted up, 1 and 0 tilted down
    class ShipAnimation {
    public:
        void Update(float verticalMove, float deltaTime);
        int GetColumn() const;

    private:
        //^ -2 tilted down +2 tilted up
        float _tilt = 0.0f;
    };

} //* namespace Client
