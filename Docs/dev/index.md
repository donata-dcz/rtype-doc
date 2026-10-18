---
title: Introduction
description: Welcome to the developer documentation of R-Type.
---

# Introduction

This documentation is for the people who work on the R-Type code: how it is built, how the pieces talk to each other, and how to add something without breaking the rest.

## In one minute

- The **server** runs the game and decides everything.
- The **client** shows the game and sends the keys pressed.
- They talk with **UDP datagrams**, in a small binary protocol shared in the `Common` folder.
- The documentation you read is built from the `Docs` folder of the repository.

## Where to start

| If you want to... | Read |
|-------------------|------|
| Build the project and run it | [Build and run](./get-started/build-and-run.md) |
| Send your work to the repository | [Contributing](./get-started/contributing.md) |
| See how the project is organized | [Architecture overview](./architecture/overview.md) |
| Work on the menu, the window or the drawing | [Client](./architecture/client.md) |
| Work on the music and sound effects | [Audio](./architecture/audio.md) |
| Work on the network | [Network protocol](./networking/protocol.md) |
| Add a sound, a message or an image | [Guides](./guides/add-a-sound.md) |
| Fix a problem | [Troubleshooting](./guides/troubleshooting.md) |

!!! note "Work in progress"
    Some pages are short for now. The client, the audio and the protocol are the most complete. The server and the engine will get their own pages.
