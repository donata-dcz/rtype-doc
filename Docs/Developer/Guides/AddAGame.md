# Add a new game

R-Type is two things: a **game engine** (`Engine`, generic) and a **game** (`Game/RType`). The repository already holds a second game, `Game/Pacman`, to prove that the engine is reusable.

## Recipe to create a new game

1. Create `Game/<Name>/` with `Include/`, `Src/` and a `CMakeLists.txt`.
2. Write your **components** (plain structs) and **systems** (classes derived from `ecs::ISystem`). Reuse `Transform`, `Velocity`, `Hitbox`, `Sprite`, `MovementSystem`, `BoundsSystem`, `DespawnSystem` from the engine when they fit. See [The engine](Engine.md).
3. Write one **game class** that owns an `ecs::Registry`, a list of systems and, if useful, an `Engine::EventBus`. Its `Update`/`Tick` runs the systems in a fixed order.
4. Build the rules as a **static library**, without any window code, so they can be tested. Put the executable in a separate file.
5. Add the folder with `add_subdirectory(Game/<Name>)` in the root `CMakeLists.txt`.
6. Include the new game in the main `CMakeLists.txt` file.

## If you want to make a local game (like Pacman)

`Game/Pacman/CMakeLists.txt` is the model:

```cmake
add_library(PacmanCore STATIC Src/PacmanGame.cpp Src/Systems.cpp)
# links Engine, include directory Include

add_executable(pacman_game Src/Main.cpp)
# links PacmanCore raylib glfw, and copies Assets next to the executable
```

The main loop only connects the pieces:

```cpp
Engine::GameLoop loop(60.0);
loop.SetTickCallback([&](double dt) { game.Update(static_cast<float>(dt)); });
while (!WindowShouldClose()) {
    ProcessInput();
    loop.Step();
    RenderGame();
}
```

## If you want to make a networked game (like R-Type)

The server only needs your game class:

```cpp
class Game {
    public:
        explicit Game(std::uint32_t seed);
        bool AddPlayer(std::uint8_t id);
        void RemovePlayer(std::uint8_t id);
        void SetInput(std::uint8_t id, InputState input);
        void Tick(float deltaTime);
        Snapshot BuildSnapshot() const;
};
```

What you write, using `Game/RType` as the model:

The generic part is already in `Common` and does not change: header, `BuildPacket`, `ReadHeader`, `BinaryReader`/`BinaryWriter`, the message types and the 1400-byte limit.

CMake pattern, from `Game/RType/CMakeLists.txt`:

```cmake
add_library(MyGame STATIC Src/Game.cpp Src/Network/Messages.cpp ...)
target_include_directories(MyGame PUBLIC Include Include/Network)
target_link_libraries(MyGame PUBLIC Common Engine)
```

Then:

1. Make the server run your class. Today `Server/Src/Ticks.*` and `Server/Src/Main.cpp` include `Game.hpp` and use `rtype::Game` directly, so **switching games means changing those includes and the type name**. There is no runtime choice of game yet (see [Known limits](../KnownLimits.md)).
2. Make the client draw your snapshot: replace `SnapshotRenderer` (a `switch` on `EntityType`) and the keys in `Keyboard.cpp`.
3. Keep the rule of the project: the server decides, the client sends keys and draws.
