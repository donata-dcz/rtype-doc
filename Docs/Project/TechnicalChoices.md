# Technical choices

Each choice below says what was chosen, what the alternatives were, and why. The "why" of the choices marked *(to confirm)* is deduced from the code by the author of this page: the team should check it matches what was really decided.

## Graphics: Raylib

**Alternatives:** SDL, SFML, OpenGL directly.

Raylib gives a window, keyboard and mouse input, 2D drawing, textures and audio with a small, simple API, and it is portable. OpenGL alone would mean writing the window and context handling ourselves. SDL is more complete but more low-level: Raylib gets a working display with much less code. Raylib is only used in `Client` and in the Pacman executable: the engine and the server do not depend on it.

## Network library: Asio

**Alternatives:** Boost.Asio, the system socket API, a higher-level networking library.

Standalone Asio is a C++ library that hides the differences between Linux and Windows sockets and offers asynchronous I/O without blocking the program. It is header-only (no Boost needed) and leaves us free to define our own protocol, which a higher-level library would not. Asio is hidden inside `.cpp` files (`NetworkClient`, `UdpServer`): the rest of the code only sees bytes.

## Transport: UDP *(to confirm)*

**Alternative:** TCP.

A shoot'em up sends the full state many times per second, so an old packet is useless once a newer one exists. UDP has no retransmission and no head-of-line blocking, which fits. The price is paid in the protocol: sequence numbers, the client ignoring an older snapshot than the last one, the periodic `Input` as a "still alive" signal, and a 3 s session timeout. See [Protocol](../Developer/Protocol.md) and [Known limits](../Developer/KnownLimits.md).

## Authoritative server *(to confirm)*

The server runs the game and the clients only send keys and draw. A client cannot cheat on a position, and every player sees the same world. The cost: no prediction on the client, the screen is 30 snapshots per second.

## Binary protocol, written by hand

Big-endian fields, a fixed 10-byte header, a 1400-byte maximum so a message fits in one datagram. `BinaryReader` checks every read, so a malformed packet from the network cannot read outside the buffer. No serialization library: the format is small and we keep full control of it.

## Own ECS in the engine *(to confirm)*

Entities are numbers, components are plain structs, systems are classes. A game is a list of systems and a registry; the engine knows nothing about R-Type, which is how a second game (Pacman) could be built on it. See [The engine](../Developer/Guides/Engine.md).

## Fixed time step

`Engine::GameLoop` advances the game by 1/60 s steps with an accumulator (capped at 0.25 s so a long pause does not cause a burst of steps). The simulation is the same whatever the frame rate, and a clock can be injected, which makes the loop testable.

## Two threads on the server

One thread for the network (Asio), one for the game. They share only a mutex-protected list of received packets, and sends are posted to the network thread, so the game code never has to be thread-safe.

## Build: C++20, CMake presets, vcpkg *(to confirm)*

C++20 for `std::span`, concepts and similar tools. CMake presets give the same two commands on Linux and Windows. vcpkg in manifest mode downloads Raylib and Asio, so nothing is installed by hand.

## Summary

| Need | Choice | Main reason |
|------|--------|-------------|
| Graphics | Raylib | Simple, light, portable |
| Network library | Asio | Asynchronous, portable, our own protocol |
| Transport | UDP | Fresh state matters more than reliability |
| Game authority | Server | One truth, no cheating on positions |
| Game structure | Own ECS | Reusable for other games |
| Build | CMake + vcpkg | Same commands on both systems |
