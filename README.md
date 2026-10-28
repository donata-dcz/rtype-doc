# R-TYPE

R-Type is a networked game where several players fight the enemy together, each one on its own client, connected to a central authoritative server. It is a remake of the classic shoot'em up, built on a custom C++ game engine.

---

## Overview

R-Type is made of two independent binaries that communicate over a binary UDP protocol defined later:

| Binary | Language | Role |
|--------|----------|------|
| `r-type_server` | C++ (Asio) | Runs the game logic and has the final authority on the game world |
| `r-type_client` | C++ (Raylib) | Displays the game and sends the player inputs |

Both binaries are built on a shared game engine (`Engine/`) and a shared protocol library (`Common/`).

Supported platforms: **Linux** and **Windows** (both built and tested by the CI).

---

## Usage

### Requirements

- Git
- CMake 3.28+
- A C++20 compiler

Third-party libraries (Raylib, Asio) are handled by [vcpkg](https://vcpkg.io/).
It is cloned automatically by CMake on the first configuration.

### Build

```bash
git clone https://github.com/alexis801/RType.git
cd RType
cmake --preset default
cmake --build --preset default
```

> We have [releases](https://github.com/alexis801/RType/releases) on our GitHub page, so you can also download pre-built binaries for Linux and Windows instead of building from source.

### Server

Launch the server first with this command on Linux:

```bash
./build/default/Server/r-type_server [-p <port>]
```

| Option | Description | Default |
|--------|-------------|---------|
| `-p <port>` | UDP port the server listens on (1-65535) | `4242` |

### Client

Launch the client with this command on Linux:

```bash
./build/default/Client/r-type_client [-h <host>] [-p <port>]
```

| Option | Description | Default |
|--------|-------------|---------|
| `-h <host>` | Address of the server: IPv4, IPv6 or host name | `127.0.0.1` |
| `-p <port>` | UDP port of the server (1-65535) | `4242` |

The **Play** button of the menu connects to this server. The menu does not ask for an address, a port or a name.

If the server does not use the default port, give the same port to both programs:

```bash
./build/default/Server/r-type_server -p 2525
./build/default/Client/r-type_client -p 2525
```

> On Windows, the executables are in a folder named after the configuration, for example `build\default\Server\Debug\r-type_server.exe`.

### Tests

Launch the tests with this command on Linux:

```bash
ctest --test-dir build/default
```

---

## Gameplay

- A slow horizontal scrolling star-field in the background
- Up to 4 players, each one distinctly identifiable (color or sprite)
- Players move with the arrow keys and shoot missiles
- Waves of monsters spawning randomly on the right of the screen
- The server notifies every client of spawns, moves, shots and deaths, and keeps running if a client crashes

---

## Project Structure

| Folder | Content |
|--------|---------|
| `Engine/` | Game engine (generic, no R-Type logic) |
| `Common/` | Code shared by the client and the server (network protocol) |
| `Server/` | Authoritative game server |
| `Client/` | Graphical client |
| `Tests/` | Unit tests |
| `Assets/` | Game resources (sprites, sounds) |
| `Docs/` | Documentation |
| `Cmake/` | CMake modules (dependencies, warnings) |

---

## Documentation

The documentation is fully available deployed on [Github Pages](https://donata-dcz.github.io/rtype-doc/Introduction/).

---

## Contributors

- Adrien Grégoire
- Alexis Bezzaouya Lemonnier
- Donata Contant
- Kylian Gicquel
