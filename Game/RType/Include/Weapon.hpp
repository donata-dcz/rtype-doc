/*
** EPITECH PROJECT, 2026
** r-type
** File description:
** Player missiles and fire rate shared by client and server
*/

#pragma once

#include <cstdint>
#include <vector>

namespace rtype {

    //^ pixel per sec to rigth
    constexpr float MissileSpeed = 600.0f;
    constexpr float EnemyMissileSpeed = 300.0f;
    constexpr float MissileWidth = 16.0f;
    constexpr float MissileHeight = 4.0f;

    //^ second beetween 2 shot -> holding = 5 shot / s
    constexpr float FireCooldown = 0.2f;

    //& limit the fire rate
    class Gun {
    public:
        //^ true when missile need to be fire by frame
        bool Update(bool triggerHeld, float deltaTime);

    private:
        float _cooldown = 0.0f;
    };

} //* namespace rtype
