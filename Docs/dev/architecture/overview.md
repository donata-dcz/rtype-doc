---
title: Overview
description: How the client, the server and the shared code fit together.
---

# Architecture overview

```text
+-------------------+          UDP, port 4242          +-------------------+
|   r-type_client   | <-----------------------------> |   r-type_server   |
|  menu, drawing,   |   inputs  ---------------->     |  game loop (60 Hz)|
|  sound, keyboard  |   <---------------  snapshots   |  ECS registry     |
+-------------------+                                 +-------------------+
          \                                                   /
           \________________  Common  ______________________/
                    (protocol, snapshot, inputs)
```

*Text description of the diagram: the client and the server exchange UDP datagrams on port 4242. The client sends the keys pressed (inputs). The server sends back the state of the game (snapshots). Both programs use the same `Common` code for the protocol, the snapshots and the inputs.*

## One rule: the server decides

- The **client** sends intentions ("up and shoot are pressed"), never positions.
- The **server** runs the game 60 times per second (a *tick*), checks the collisions, and sends the result to every client about 30 times per second as a *snapshot*.
- The **client** draws the last snapshot it received.

This keeps every player seeing the same game, and a modified client cannot move its ship where it wants.

## Data that travels

| Direction | Data | Details |
|-----------|------|---------|
| Client to server | Connect, inputs, ping, disconnect | One byte of keys per frame |
| Server to client | Accept or reject, snapshots, events, pong | Positions of everything on the screen |

Every message is a UDP datagram with the same 10-byte header. See the [network protocol](../networking/protocol.md) for the exact bytes.

## The server

- A session is identified by the address and the port of the client (`IP:port`).
- At most 4 players can play at the same time.
- A player who sends nothing for 3 seconds is removed with its ship.
- The game state is kept in an ECS registry (entities, components and systems).

## The client

The client has one loop that, at each frame, reads the keyboard, talks to the server, plays the sounds and draws. It has two states: the **menu** and the **game**. See [Client](./client.md) and [Audio](./audio.md).

## Shared code

The code that both programs include is what must stay identical on both sides:

| Part | Role |
|------|------|
| `Protocol` | The packet header, the message types, `BuildPacket` and `ReadHeader` |
| `Binary` | Reading and writing bytes (`BinaryReader`, `BinaryWriter`) |
| `Snapshot` | What the client needs to draw one moment of the game |
| `InputState` | The pressed actions packed in one byte |

!!! warning
    If you change something in the shared code, you change both programs and the [protocol page](../networking/protocol.md). Update them together.
