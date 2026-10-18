# Developer guide

R-Type is a networked shoot'em up: a graphical client (`r-type_client`, Raylib) and an authoritative server (`r-type_server`) that talk over UDP.

**Full documentation: <https://ORGANISATION.github.io/REPOSITORY/>**

This file is only the short version.

## Build and run

```bash
cmake --preset default
cmake --build --preset default
./build/default/Server/r-type_server
./build/default/Client/r-type_client
```

Dependencies come from vcpkg: nothing to install by hand.

## Architecture in 30 seconds

| Folder | Role |
|--------|------|
| `Engine` | The game engine |
| `Common` | Code shared by client and server: protocol, snapshot, inputs |
| `Server` | Simulates the game (60 ticks per second) and sends snapshots |
| `Client` | Draws the game, plays sounds, sends the player's keys |
| `Assets` | Images and sounds, copied next to the client |
| `Docs` | The documentation site (MkDocs) |

- The **server decides everything**. The client only sends the keys pressed, never a position.
- The client sends an `Input` every frame and draws the last `Snapshot` it received.
- All messages are binary and use UDP. The format is shared through `Common`.

## Rules in short

- Everything is in a class, functions are 25 lines at most, files are PascalCase.
- GitFlow: `feature/...` into `dev`, then `dev` into `main`.
- Run `clang-format` before committing.

## Where to read more

The documentation site has the build details, the contributing rules, the architecture, the client, the full network protocol, step by step how-to guides (add a sound, a texture, a menu option) and a troubleshooting table.

To read or edit it locally:

```bash
pip install -r Docs/requirements.txt
mkdocs serve
```
