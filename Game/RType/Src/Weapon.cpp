/*
** EPITECH PROJECT, 2026
** r-type
** File description:
** Player missiles and fire rate shared by client and server
*/

#include <algorithm>

#include "Weapon.hpp"

namespace rtype {

    bool Gun::Update(bool triggerHeld, float deltaTime)
    {
        _cooldown = std::max(_cooldown - deltaTime, 0.0f);
        if (!triggerHeld || _cooldown > 0.0f)
            return false;
        _cooldown = FireCooldown;
        return true;
    }

} //* namespace rtype
