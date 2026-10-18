---
title: R-Type
description: Documentation of the R-Type project.
hide:
  - navigation
  - toc
---

<div class="rt-hero" markdown>

![R-Type logo, a small spaceship](assets/Logo.svg){ .rt-logo }

# R-TYPE

A network version of the classic shooter. Up to four players, one server, one custom engine.

[Developer docs](dev/index.md){ .md-button .md-button--primary }
[Play](project/play.md){ .md-button .rt-button-accent }

</div>

<div class="grid cards rt-four" markdown>

- :material-server: **The server decides**

    The game runs on the server, 60 times per second. A client only says which keys are pressed, so nobody can cheat by changing it.

- :material-lan: **A small binary protocol**

    Everything travels in UDP datagrams with a 10-byte header. The protocol is written down so anyone can write a compatible client.

- :material-gamepad-variant: **A Raylib client**

    A resizable window, a menu for keyboard and mouse, a scrolling star-field, music and sound effects.

- :material-human-wheelchair: **Built to be accessible**

    Full keyboard menu, readable colors for color-blind players, text size option. [See what is done and what is planned.](project/accessibility.md)

</div>

## Reading order

| Page | What you find there |
|------|---------------------|
| [Build and run](dev/get-started/build-and-run.md) | Build the project and start the server and a client |
| [Contributing](dev/get-started/contributing.md) | Code style, file names, Git rules, CI |
| [Architecture](dev/architecture/overview.md) | Folders, programs, and the flow of a frame |
| [Client](dev/architecture/client.md) | Menu, connection, window, assets |
| [Audio](dev/architecture/audio.md) | Music and sound effects |
| [Network protocol](dev/networking/protocol.md) | The messages between client and server |
| [Guides](dev/guides/index.md) | Add a sound, an image, a message, a menu option |
| [Troubleshooting](dev/guides/troubleshooting.md) | Symptoms and where to look |

## Quick start

```bash
cmake --preset default
cmake --build --preset default
./build/default/Server/r-type_server
./build/default/Client/r-type_client
```

The dependencies are fetched by vcpkg, so nothing has to be installed by hand.

## Using this site

- Press `/` or `s` to go to the search bar.
- The colors follow your system, and the button in the header switches between the light and the dark theme.
- Every page can be read with the keyboard only, and every diagram has a text description.
