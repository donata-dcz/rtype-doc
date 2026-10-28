/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** EventBus header with mediator pattern
*/

#pragma once
#include <memory>
#include <functional>
#include <typeindex>
#include <unordered_map>
#include <algorithm>
#include <vector>

namespace Engine {

    class EventBus {
    using SubscriptionId = std::size_t;

    public:
        EventBus() = default;
        ~EventBus() = default;
        EventBus(const EventBus &) = delete;
        EventBus &operator=(const EventBus &) = delete;
        EventBus(EventBus &&) noexcept = default;
        EventBus &operator=(EventBus &&) noexcept = default;

        template <typename EventType>
        SubscriptionId Subscribe(std::function<void(const EventType &)> callback)
        {
            const std::type_index typeId = std::type_index(typeid(EventType));
            const SubscriptionId subId = _nextSubscriptionId++;
            auto wrapper = [callback](const void *eventData) {
                callback(*static_cast<const EventType *>(eventData));
            };
            _subscribers[typeId].push_back({subId, wrapper});
            return subId;
        }

        template <typename EventType>
        void Publish(const EventType &event)
        {
            const std::type_index typeId = std::type_index(typeid(EventType));
            const auto it = _subscribers.find(typeId);
            if (it == _subscribers.end())
                return;
            const auto subsCopy = it->second;
            for (const auto &sub : subsCopy) {
                if (sub.callback)
                    sub.callback(static_cast<const void*>(&event));
            }
        }

        void Unsubscribe(SubscriptionId id);
        void Clear();

    private:
        using InternalCallback = std::function<void(const void *)>;

        struct Subscriber {
            SubscriptionId id;
            InternalCallback callback;
        };

        std::unordered_map<std::type_index, std::vector<Subscriber>> _subscribers;
        SubscriptionId _nextSubscriptionId{1};
    };

} //* namespace Engine
