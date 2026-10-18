# R-Type

R-Type is a networked game where several players fight the enemy together, each one on its own client, connected to a central authoritative server. It is a remake of the classic shoot'em up, built on a custom C++ game engine.

## Overview

R-Type is made of two independent binaries that communicate over a binary UDP protocol defined later:

| Binary | Language | Role |
|--------|----------|------|
| `r-type_server` | C++ (Asio) | Runs the game logic and has the final authority on the game world |
| `r-type_client` | C++ (Raylib) | Displays the game and sends the player inputs |

Both binaries are built on a shared game engine (`Engine/`) and a shared protocol library (`Common/`).

Supported platforms: **Linux** and **Windows** (both built and tested by the CI).

## Where to start

| If you want to... | Read |
|-------------------|------|
| Build the project and run it | [Build and run](Build.md) |
| See how the project is organized | [Architecture](Architecture.md) |
| Work on the network | [Protocol](Protocol.md) |
| Add a sound, a message, an image or a menu option | [Guides](Guides/AddASound.md) |
| Fix a problem | [Troubleshooting](Guides/Troubleshooting.md) |
| Know what is accessible in the game | [Accessibility](Accessibility.md) |
| Understand why we chose these technologies | [Technical choices](TechnicalChoices.md) |
