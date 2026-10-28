/*
** EPITECH PROJECT, 2026
** r-type
** File description:
** Tests for Engine
*/

#include <gtest/gtest.h>
#include <thread>
#include <Engine/GameLoop.hpp>
#include <Engine/EventBus.hpp>
#include <ECS/Registry.hpp>
#include <ECS/Component.hpp>

struct NetworkInputEvent {
    ecs::Entity entity;
    float dx;
    float dy;
};

struct RenderStateUpdatedEvent {
    std::size_t entityCount;
};

TEST(GameLoop, FixedTimestepAccumulator)
{
    double fakeTime = 0.0;
    auto fakeClock = [&fakeTime]() { return fakeTime; };
    Engine::GameLoop loop(60.0, fakeClock);
    int tickCount = 0;

    loop.SetTickCallback([&tickCount](double /*dt*/) {
        tickCount++;
    });
    fakeTime = 1.0;
    loop.Step();
    EXPECT_EQ(tickCount, 0);
    fakeTime += 2.0 / 60.0;
    loop.Step();
    EXPECT_EQ(tickCount, 2);
}

TEST(GameLoop, SpiralOfDeathCap)
{
    double fakeTime = 0.0;
    auto fakeClock = [&fakeTime]() { return fakeTime; };
    Engine::GameLoop loop(60.0, fakeClock);
    loop.SetMaxAccumulatedTime(0.1);
    int tickCount = 0;

    loop.SetTickCallback([&tickCount](double /*dt*/) {
        tickCount++;
    });
    fakeTime = 1.0;
    loop.Step();
    fakeTime += 2.0;
    loop.Step();
    EXPECT_EQ(tickCount, 6);
}

struct PlayerHitEvent {
    int playerId;
    int damage;
};

struct PlaySoundEvent {
    std::string soundName;
};

TEST(EventBus, TwoSystemsCommunication)
{
    Engine::EventBus bus;

    int systemAudioReceivedCount = 0;
    std::string lastSoundPlayed = "";
    int systemNetworkReceivedDamage = 0;

    bus.Subscribe<PlayerHitEvent>([&bus, &systemAudioReceivedCount](const PlayerHitEvent &) {
        systemAudioReceivedCount++;
        bus.Publish<PlaySoundEvent>({"explosion.wav"});
    });

    bus.Subscribe<PlaySoundEvent>([&lastSoundPlayed](const PlaySoundEvent &event) {
        lastSoundPlayed = event.soundName;
    });
    bus.Subscribe<PlayerHitEvent>([&systemNetworkReceivedDamage](const PlayerHitEvent &event) {
        systemNetworkReceivedDamage += event.damage;
    });
    bus.Publish<PlayerHitEvent>({1, 50});
    EXPECT_EQ(systemAudioReceivedCount, 1);
    EXPECT_EQ(lastSoundPlayed, "explosion.wav");
    EXPECT_EQ(systemNetworkReceivedDamage, 50);
}

TEST(EventBus, UnsubscribeAndClear)
{
    Engine::EventBus bus;
    int callCount = 0;
    auto subId = bus.Subscribe<PlayerHitEvent>([&callCount](const PlayerHitEvent &) {
        callCount++;
    });

    bus.Publish<PlayerHitEvent>({1, 10});
    EXPECT_EQ(callCount, 1);
    bus.Unsubscribe(subId);
    bus.Publish<PlayerHitEvent>({1, 10});
    EXPECT_EQ(callCount, 1);
}

TEST(ServerMultithread, ConstantTickRateWithoutClients)
{
    Engine::GameLoop loop(60.0);
    int tickCount = 0;

    loop.SetTickCallback([&tickCount](double /*dt*/) {
        tickCount++;
    });
    std::atomic<bool> running{true};
    std::thread networkThread([&running]() {
        while (running.load())
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
    });
    const auto start = std::chrono::steady_clock::now();
    while (std::chrono::steady_clock::now() - start < std::chrono::seconds(1)) {
        loop.Step();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    running.store(false);
    if (networkThread.joinable())
        networkThread.join();
    EXPECT_GE(tickCount, 58);
    EXPECT_LE(tickCount, 62);
}

TEST(EngineArchitecture, DecoupledLayersCommunication)
{
    ecs::Registry registry;
    Engine::EventBus eventBus;
    ecs::Entity player = registry.create();
    bool renderEventReceived = false;

    registry.add<ecs::Transform>(player, {0.0f, 0.0f, 0.0f});
    registry.add<ecs::Sprite>(player, {"ship.png"});
    eventBus.Subscribe<NetworkInputEvent>([&registry, &eventBus](const NetworkInputEvent &evt) {
        if (registry.has<ecs::Transform>(evt.entity)) {
            auto &transform = registry.get<ecs::Transform>(evt.entity);
            transform.x += evt.dx;
            transform.y += evt.dy;
        }
        eventBus.Publish<RenderStateUpdatedEvent>({1});
    });
    eventBus.Subscribe<RenderStateUpdatedEvent>([&registry, &renderEventReceived, player](const RenderStateUpdatedEvent &) {
        renderEventReceived = true;
        ASSERT_TRUE(registry.has<ecs::Transform>(player));
        ASSERT_TRUE(registry.has<ecs::Sprite>(player));
        
        const auto &transform = registry.get<ecs::Transform>(player);
        const auto &sprite = registry.get<ecs::Sprite>(player);

        EXPECT_EQ(transform.x, 10.0f);
        EXPECT_EQ(transform.y, 5.0f);
        EXPECT_EQ(sprite.textureId, "ship.png");
    });
    eventBus.Publish<NetworkInputEvent>({player, 10.0f, 5.0f});
    EXPECT_TRUE(renderEventReceived);
}