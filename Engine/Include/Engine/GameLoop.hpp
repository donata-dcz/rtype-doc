/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** GameLoop
*/

#pragma once

#include <chrono>
#include <functional>

namespace Engine {

    class GameLoop {
    public:
        using TimeSource = std::function<double()>;
        using TickCallback = std::function<void(double)>;
        using FrameCallback = std::function<void(double)>;
        explicit GameLoop(double tickRate = 60.0, TimeSource timeSource = nullptr);
        ~GameLoop() = default;

        GameLoop(const GameLoop &) = delete;
        GameLoop &operator=(const GameLoop &) = delete;
        GameLoop(GameLoop &&) noexcept = default;
        GameLoop &operator=(GameLoop &&) noexcept = default;

        void SetTickCallback(TickCallback callback);
        void SetFrameCallback(FrameCallback callback);
        void SetTickRate(double tickRate);
        void SetMaxAccumulatedTime(double maxSeconds);
        void Step();
        void Run();
        void Stop();

        [[nodiscard]] bool IsRunning() const;
        [[nodiscard]] double GetTargetDeltaTime() const;
        [[nodiscard]] double GetAccumulatedTime() const;

    private:
        static double DefaultTimeSource();
        TimeSource _timeSource;
        TickCallback _onTick;
        FrameCallback _onFrame;
        double _targetDeltaTime;
        double _maxAccumulatedTime;
        double _accumulator;
        double _lastTime;
        bool _isRunning;
    };

} //* Engine namespace
