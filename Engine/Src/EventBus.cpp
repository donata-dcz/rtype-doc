/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** EventBus header with mediator pattern
*/

#include <Engine/EventBus.hpp>

namespace Engine {

    void EventBus::Unsubscribe(EventBus::SubscriptionId id)
    {
        for (auto &[typeIndex, subscriberList] : _subscribers) {
            auto it = std::remove_if(subscriberList.begin(), subscriberList.end(), [id](const Subscriber &sub) {
                return sub.id == id;
            });
            if (it != subscriberList.end()) {
                subscriberList.erase(it, subscriberList.end());
                return;
            }
        }
    }

    void EventBus::Clear()
    {
        _subscribers.clear();
    }

} //* namespcae Engine
