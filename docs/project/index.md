---
title: About R-Type
description: What R-Type is and how the project is made.
---

# About R-Type

R-Type is a network version of the classic side-scrolling shooter, made at Epitech. Up to four players fly together against the Bydos, each one on its own computer.

The project is made of two programs and a custom game engine:

| Program | What it does |
|---------|--------------|
| `r-type_server` | Runs the game. It decides everything: moves, shots, collisions, score. |
| `r-type_client` | Shows the game (Raylib) and sends what the player presses. |

!!! info "The server is the authority"
    A client never says where its ship is, only which keys are pressed. So nobody can cheat by changing the client.

## Where to go next

| I want to... | Go to |
|--------------|-------|
| Build and play | [Play](./play.md) |
| Know what is accessible in the game | [Accessibility](./accessibility.md) |
| Work on the code | [Developer docs](../dev/index.md) |
