/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** GameLoop
*/

#include <Engine/GameLoop.hpp>
#include <algorithm>
#include <chrono>

namespace Engine {

    double GameLoop::DefaultTimeSource()
    {
        using namespace std::chrono;
        const auto now = steady_clock::now();
        const duration<double> seconds = now.time_since_epoch();
        return seconds.count();
    }

    GameLoop::GameLoop(double tickRate, TimeSource timeSource)
        : _timeSource(timeSource ? std::move(timeSource) : DefaultTimeSource),
          _onTick(nullptr),
          _onFrame(nullptr),
          _targetDeltaTime(tickRate > 0.0 ? 1.0 / tickRate : 1.0 / 60.0),
          _maxAccumulatedTime(0.25),
          _accumulator(0.0),
          _lastTime(0.0),
          _isRunning(false)
    {}

    void GameLoop::SetTickCallback(TickCallback callback)
    {
        _onTick = std::move(callback);
    }

    void GameLoop::SetFrameCallback(FrameCallback callback)
    {
        _onFrame = std::move(callback);
    }

    void GameLoop::SetTickRate(double tickRate)
    {
        if (tickRate > 0.0)
            _targetDeltaTime = 1.0 / tickRate;
    }

    void GameLoop::SetMaxAccumulatedTime(double maxSeconds)
    {
        if (maxSeconds > 0.0)
            _maxAccumulatedTime = maxSeconds;
    }

    void GameLoop::Step()
    {
        const double currentTime = _timeSource();
        double frameTime = 0.0;

        if (_lastTime <= 0.0) {
            _lastTime = currentTime;
            return;
        }
        frameTime = currentTime - _lastTime;
        _lastTime = currentTime;
        if (frameTime < 0.0)
            frameTime = 0.0;
        frameTime = std::min(frameTime, _maxAccumulatedTime);
        _accumulator += frameTime;
        while (_accumulator >= _targetDeltaTime) {
            if (_onTick)
                _onTick(_targetDeltaTime);
            _accumulator -= _targetDeltaTime;
        }
        if (_onFrame)
            _onFrame(frameTime);
    }

    void GameLoop::Run()
    {
        _isRunning = true;
        _lastTime = _timeSource();
        while (_isRunning)
            Step();
    }

    void GameLoop::Stop()
    {
        _isRunning = false;
        _lastTime = 0.0;
    }

    bool GameLoop::IsRunning() const
    {
        return _isRunning;
    }

    double GameLoop::GetTargetDeltaTime() const
    {
        return _targetDeltaTime;
    }

    double GameLoop::GetAccumulatedTime() const
    {
        return _accumulator;
    }

} //* Engine namespace
