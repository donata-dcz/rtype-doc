# R-Type

R-Type is a network version of the classic side-scrolling shooter, made at Epitech. Up to four players fly together against the Bydos, each one on its own computer.

This documentation is for the people who work on the code: how it is built, how the pieces talk to each other, and how to add something without breaking the rest.

## The project in one minute

| Program | What it does |
|---------|--------------|
| `r-type_server` | Runs the game. It decides everything: moves, shots, collisions, score. |
| `r-type_client` | Shows the game (Raylib) and sends what the player presses. |

- The two programs talk with **UDP datagrams**, in a small binary protocol shared in the `Common` folder.
- Both are built on the game engine in the `Engine` folder.
- The documentation you read is built from the `Docs` folder of the repository.

!!! info "The server is the authority"
    A client never says where its ship is, only which keys are pressed. So nobody can cheat by changing the client.

## Where to start

| If you want to... | Read |
|-------------------|------|
| Build the project and run it | [Build and run](build.md) |
| Send your work to the repository | [Contributing](contributing.md) |
| See how the project is organized | [Architecture](architecture.md) |
| Work on the menu, the window, the drawing or the sound | [Client](client.md) |
| Work on the network | [Protocol](Protocol.md) |
| Add a sound, a message, an image or a menu option, or fix a problem | [Guides](guides.md) |
| Know what is accessible in the game | [Accessibility](Accessibility.md) |
| Understand why we chose these technologies | [Technical choices](technicalChoices.md) |

## Quick start

```bash
cmake --preset default
cmake --build --preset default
./build/default/Server/r-type_server
./build/default/Client/r-type_client
```

See [Build and run](build.md) for the requirements and the options.

!!! note "Work in progress"
    The pages about the engine and the server are being written.
