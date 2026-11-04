# Features

This page provides an overview of the project's implementation against the requirements and topics described in the official R-Type subject.

The requirements are listed in the same order and structure as the original subject. Each item indicates its current implementation status, with additional notes when clarification is useful.

Features that go beyond the original subject are explicitly marked as **(bonus)**.

## Common Part

### Software Engineering Requirements

| Requirement | Status | Notes |
|---|---|---|
| Build System Generator | Done | CMake with presets. See [Build](Build.md) |
| Package Manager | Done | vcpkg |
| Run on Linux, for both the client and the server | Done | |
| Cross-Platform | Done | Linux and Windows are built by the CI |
| Well-defined software development workflow | Done | Feature branches, pull requests and issues |
| CI/CD workflow | Partial | The CI builds and releases the project. Tests are currently run locally but are not yet integrated into the CI |
| C++ linters and formatters | Partial | `clang-format` is configured. `clang-tidy` is not yet integrated |
| **(bonus)** File naming convention checked by the CI | Done | PascalCase naming convention checked on every pull request |
| **(bonus)** Branch rule checked by the CI | Done | Only `dev` can be merged into `main` |

### Documentation Requirements

| Requirement | Status | Notes |
|---|---|---|
| Documentation written in English | Done | |
| README | Done | |
| Developer Documentation: architectural diagrams | Done | See [Architecture](Architecture.md) |
| Developer Documentation: main systems overviews and descriptions | Done | [Client](Guides/Client.md), [Engine](Guides/Engine.md), [Server](Guides/Server.md) |
| Developer Documentation: tutorials and How-To's | Done | [Add a new game](Guides/AddAGame.md) |
| Contribution guidelines and coding conventions | Not done | No dedicated page yet. The formatting rules are enforced by `clang-format` and the file naming rule by the CI |
| Technical and Comparative Study | Partial | See [Technical choices](../Project/TechnicalChoices.md). Some sections still need confirmation or completion |
| Algorithms and Data Structures | Partial | ECS, fixed time step and delta snapshots are justified. Custom algorithms still need their own section |
| Storage | Not done | No comparative study yet |
| Security | Partial | Packet validation is documented in [Protocol](Protocol.md), section 9. No comparative study yet |
| Documentation available and accessible in a modern way | Done | Static documentation website generated with MkDocs Material |
| Protocol documentation | Done | Written as an RFC, including binary packet examples. See [Protocol](Protocol.md) |

### Accessibility Requirements

The project, including its documentation, must be accessible to people with disabilities.

| Requirement | Status | Notes |
|---|---|---|
| Physical and Motor Disabilities | Done | Keyboard-only menus and safe Esc key handling |
| Audio and Visual Disabilities | Done | Contrast measured against WCAG AA. Selection is not indicated by color alone. The game runs without an audio device |
| Mental and Cognitive Disabilities | Done | Easy way back to the menu and clearly explained connection errors |

See [Accessibility](Accessibility.md) for details and planned improvements.

## Software Architecture & First Game Prototype

### General Goals

| Requirement | Status | Notes |
|---|---|---|
| The game MUST be playable at the end of this part | Done | Star-field background, Bydos enemies, player spaceships and missiles |
| The game MUST be a networked game | Done | Each player uses a distinct client connected to a central authoritative server |
| The game MUST demonstrate the foundations of a game engine | Done | Decoupled Rendering, Networking and Game Logic subsystems. See [Architecture](Architecture.md) |

### Server

| Requirement | Status | Notes |
|---|---|---|
| The server MUST notify each client of the game events (monsters, missiles, players) and of the other clients' actions | Done | Entities and their states are sent to every client in snapshots |
| The server MUST be multithreaded | Done | One network thread using Asio and one game thread |
| If a client crashes, the server MUST continue to work | Done | The disconnected session is removed after a 3-second timeout |
| If a client crashes, the server MUST notify the other clients in the same game | Partial | The `EVENT` message is defined but not emitted yet |
| Asio library for networking (MAY) | Done | Asio is used inside the network classes |

### Client

| Requirement | Status | Notes |
|---|---|---|
| The client MUST contain everything necessary to display the game and handle player input | Done | Graphical client with keyboard input handling |
| Everything related to gameplay logic shall occur on the server | Done | The server remains authoritative over gameplay |
| The client MAY run parts of the game logic | Not done | Not used: the client only sends inputs and draws the world |

### Protocol

| Requirement | Status | Notes |
|---|---|---|
| Design a binary protocol for client/server communications | Done | Custom binary protocol |
| Use UDP for communications between the server and the clients | Done | All in-game communications use UDP |
| Malformed packets MUST NOT crash the client or the server | Done | Magic, version, packet size and bounded reads are validated |
| Document the protocol | Done | RFC-style documentation with binary packet examples. See [Protocol](Protocol.md) |

### Game Engine

| Requirement | Status | Notes |
|---|---|---|
| Create a prototype game engine before developing the game | Done | Separate `Engine/` directory |
| Decouple the different systems of the engine | Done | ECS architecture and separated systems |
| Create abstractions and reusable code | Done | The engine contains no R-Type-specific game logic |

### Gameplay

| Requirement | Status | Notes |
|---|---|---|
| Slow horizontal scrolling background representing space (star-field) | Done | |
| Scrolling, entity behavior and time flow MUST NOT be tied to the CPU speed | Done | Fixed 1/60 s game loop and timers |
| Players MUST be able to move using the arrow keys | Done | |
| There MUST be Bydos slaves in the game, as well as missiles | Done | |
| Monsters MUST be able to spawn randomly on the right of the screen | Done | |
| The four players MUST be distinctly identifiable | Done | One ship color per player |
| Basic sound design | Done | Title, stage and game over music, and a shot sound |

## Advanced Topics

The subject presents three technical tracks. Not every feature is mandatory to validate the project. Each track has its own page, with the items of the subject in the same order:

| Track | Page |
|---|---|
| Track #1: Advanced Architecture - Building a Real Game Engine | [Advanced Architecture](Tracks/AdvancedArchitecture.md) |
| Track #2: Advanced Networking - Building a Reliable Network Game | [Advanced Networking](Tracks/AdvancedNetworking.md) |
| Track #3: Advanced Gameplay - Building a Fun and Complete Video Game | [Advanced Gameplay](Tracks/AdvancedGameplay.md) |
