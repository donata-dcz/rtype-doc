# Advanced Architecture Features

This page lists the items of the advanced architecture track and what we implemented.

## Track Goals

| Requirement | Status | Notes |
|---|---|---|
| Deliver a well-defined engine with proper abstractions, decoupling, features and tools | Partial | Core architecture, ECS and reusable systems are implemented. Some engine subsystems and tooling are still missing |
| The engine knows nothing about R-Type | Done | `Engine/` contains no R-Type-specific game logic or assets |
| The engine is delivered as a separate and standalone project | Partial | The engine is a separate library used by the games, but it lives in the same repository as R-Type |
| Create a second sample game using the engine | Done | Pacman, including tests |

## Modularity

| Feature | Status | Notes |
|---|---|---|
| Compile time | Not done | No module selection through build-system or package-manager flags |
| Link-time | Done | Engine and common code are provided as static libraries |
| Run-time plugin API | Not done | No shared-object plugin loading system |

## Engine Subsystems

| Feature | Status | Notes |
|---|---|---|
| Rendering Engine | Partial | Raylib is used directly in the client. No reusable rendering layer in the engine |
| Physics engine | Partial | Each game has its own `CollisionSystem`. No shared physics system in the engine |
| Audio Engine | Done | `Audio::AudioSystem` in the client |
| Human-Machine interface | Done | Keyboard handling in the client |
| Message passing interface | Done | `EventBus` in the engine |
| Resources & asset management | Done | `ResourceManager` loads textures once and caches them by name |
| Scripting | Not done | No Lua, Python or other scripting integration |

## Tooling

| Feature | Status | Notes |
|---|---|---|
| Developer console | Not done | |
| In-game metrics and profiling | Not done | No in-game FPS, resource usage or profiling interface |
| World/scene/assets Editor | Not done | |

## Additional Features

| Feature | Status | Notes |
|---|---|---|
| **(bonus)** Fixed-step game loop with an injectable clock | Done | `Engine::GameLoop`, designed to be testable |
