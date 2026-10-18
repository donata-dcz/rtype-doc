---
title: Network protocol
description: The binary UDP protocol between the R-Type client and server.
---

# Network protocol

This page is the specification of the binary protocol of R-Type. It is written so that anyone can build a compatible client or server without reading the code. The key words MUST, MUST NOT, SHOULD and MAY are used as in [RFC 2119](https://www.rfc-editor.org/rfc/rfc2119).

## Terms

| Term | Meaning |
|------|---------|
| Server | The authority: it runs the game, the physics and the collisions. |
| Client | A remote player: it sends the keys pressed and draws what the server says. |
| Tick | One step of the simulation of the server. There are 60 per second. |
| Snapshot | The state of the game sent by the server: positions, status of the players. |

## Transport

The protocol uses **UDP** to keep the delay as small as possible. The default port is `4242`.

```text
+-------------------+          UDP, port 4242          +-------------------+
|   R-Type client   | <-----------------------------> |   R-Type server   |
+-------------------+                                 +-------------------+
                                                                |
                                                      +---------+---------+
                                                      | ECS registry and  |
                                                      | game loop (60 Hz) |
                                                      +-------------------+
```

*Text description of the diagram: a client and the server exchange datagrams on UDP port 4242. Inside the server there is the ECS registry and the game loop, which runs at 60 Hz.*

- A session is bound to the address of the client (`IP:port`).
- The server is strictly authoritative. A client sends intentions (keys pressed), never coordinates.
- Every datagram is a **10-byte header** followed by a **payload** of variable size.

## Header

| Offset | Field | Type | Size | Value |
|--------|-------|------|------|-------|
| 0 | `magic` | `u16` | 2 bytes | `0x5254` (`RT` in ASCII) |
| 2 | `version` | `u8` | 1 byte | `1` |
| 3 | `type` | `u8` | 1 byte | Message type (see below) |
| 4 | `sequence` | `u32` | 4 bytes | Grows by one for every packet sent |
| 8 | `payloadSize` | `u16` | 2 bytes | Number of bytes of the payload |
| 10 | `payload` | | `payloadSize` bytes | Content of the message |

## Data types

All the numbers of more than one byte are sent in **network byte order (big-endian)**.

| Type | Size | Encoding | Range |
|------|------|----------|-------|
| `u8` | 1 byte | Unsigned | 0 to 255 |
| `u16` | 2 bytes | Unsigned, big-endian | 0 to 65 535 |
| `u32` | 4 bytes | Unsigned, big-endian | 0 to 4 294 967 295 |
| `i16` | 2 bytes | Signed (two's complement), big-endian | -32 768 to 32 767 |

For example, `-12` as an `i16` is `0xFFF4`.

## Messages

The types under `0x80` go from the client to the server. The others go from the server to the client.

| Value | Name | Direction | Payload |
|-------|------|-----------|---------|
| `0x01` | `CONNECT` | Client to server | Length of the name, then the name |
| `0x02` | `INPUT` | Client to server | Frame number, then the actions |
| `0x03` | `PING` | Client to server | Reserved (heartbeat) |
| `0x04` | `DISCONNECT` | Client to server | Reserved |
| `0x81` | `ACCEPT` | Server to client | The id of the player |
| `0x82` | `REJECT` | Server to client | The reason |
| `0x83` | `SNAPSHOT` | Server to client | The state of the game |
| `0x84` | `EVENT` | Server to client | Reserved (one-time events) |
| `0x85` | `PONG` | Server to client | Reserved |

### CONNECT (`0x01`)

| Field | Type | Meaning |
|-------|------|---------|
| `nameLength` | `u8` | Length of the name, from 0 to 16 |
| `name` | `u8[]` | Printable ASCII characters (`0x20` to `0x7E`) |

### ACCEPT (`0x81`)

| Field | Type | Meaning |
|-------|------|---------|
| `playerId` | `u8` | Slot given to the player, from 0 to 3 |

### REJECT (`0x82`)

| Field | Type | Meaning |
|-------|------|---------|
| `reason` | `u8` | `1` server full, `2` incompatible version |

### INPUT (`0x02`)

| Field | Type | Meaning |
|-------|------|---------|
| `frame` | `u32` | Number of the frame of the client, grows by one |
| `actions` | `u8` | The actions pressed, one bit each (see below) |

| Bit | Value | Action |
|-----|-------|--------|
| 0 | `0x01` | Move up |
| 1 | `0x02` | Move down |
| 2 | `0x04` | Move left |
| 3 | `0x08` | Move right |
| 4 | `0x10` | Shoot |
| 5 to 7 | | MUST be 0 |

### SNAPSHOT (`0x83`)

| Field | Type | Meaning |
|-------|------|---------|
| `tick` | `u32` | Tick of the server |
| `playerCount` | `u8` | Number of players, from 0 to 4 |
| `players` | 7 bytes each | `playerCount` entries (below) |
| `entityCount` | `u16` | Number of entities, from 0 to 128 |
| `entities` | 9 bytes each | `entityCount` entries (below) |

A player entry:

| Field | Type | Meaning |
|-------|------|---------|
| `id` | `u8` | Id of the player, from 0 to 3 |
| `flags` | `u8` | Bit 0: alive. Bit 1: invincible. |
| `lives` | `u8` | Lives left |
| `score` | `u32` | Score |

An entity entry:

| Field | Type | Meaning |
|-------|------|---------|
| `id` | `u32` | Unique id of the entity |
| `type` | `u8` | `1` ship, `2` player missile, `3` Bydos, `4` enemy missile |
| `x` | `i16` | Horizontal position |
| `y` | `i16` | Vertical position |

## Sequences

### Joining a game

```text
Client                                                 Server
  |--- CONNECT (name) ---------------------------------->|  a slot is free?
  |<-- ACCEPT (playerId) --------------------------------|  session created
  |--- INPUT (frame=1, actions) ------------------------>|
  |<-- SNAPSHOT (tick=1, world) -------------------------|  about 30 per second
```

*Text description: the client sends CONNECT with its name. If there is a free slot, the server answers ACCEPT with the id of the player. Then the client sends INPUT messages and the server sends SNAPSHOT messages, about 30 per second.*

### Server full

```text
Client                                                 Server
  |--- CONNECT (name) ---------------------------------->|  4 players of 4
  |<-- REJECT (reason=1) --------------------------------|
```

### Silence

A server that receives nothing from a client for 3 seconds removes its session and its ship. A client that receives nothing from the server for more than 3 seconds considers that the connection is lost.

## The game world

- The world is `800 x 600` units. The point `(0, 0)` is the **top-left corner**.
- Sizes: ship `99 x 51` (ids 0 to 3), player missile `16 x 4`, Bydos `48 x 36`, enemy missile `16 x 4`.

## Errors and robustness

A receiver MUST **silently drop** a datagram when:

1. it is bigger than **1400 bytes**,
2. it is smaller than the 10-byte header,
3. `magic` is not `0x5254` or `version` is not `1`,
4. `payloadSize` does not match the number of bytes that follow,
5. `actions` or `flags` have a reserved bit set to 1.

## Writing a client

1. Open a UDP socket (the server listens on port `4242` by default).
2. Send `CONNECT` every 0.5 s until `ACCEPT` or `REJECT` arrives. Give up after 5 s.
3. Keep the `playerId` of `ACCEPT`.
4. Send an `INPUT` every frame (60 per second).
5. Read the `SNAPSHOT` messages and ignore any whose `tick` is lower than the highest one received.
6. If nothing arrives from the server for more than 3 s, disconnect and tell the player.

## In the code

| What | Where |
|------|-------|
| Message types, `Header`, `BuildPacket`, `ReadHeader` | `Common::Network` (`Protocol.hpp`) |
| Reading and writing bytes | `BinaryReader` and `BinaryWriter` (`Binary.hpp`) |
| `Snapshot`, `SnapshotPlayer`, `SnapshotEntity`, `EntityType` | `rtype` namespace |
| `InputState`, `Action` | `rtype` namespace |

`ReadHeader` does the checks of the previous section and returns nothing if the datagram must be dropped. When it succeeds, the reader is on the first byte of the payload.

## Example: a CONNECT packet

The client "Player" asks to join:

```text
52 54 01 01 00 00 00 00 00 07 06 50 6C 61 79 65 72
```

| Bytes | Field | Value |
|-------|-------|-------|
| `52 54` | `magic` | `RT` |
| `01` | `version` | 1 |
| `01` | `type` | `CONNECT` |
| `00 00 00 00` | `sequence` | 0 |
| `00 07` | `payloadSize` | 7 |
| `06` | `nameLength` | 6 |
| `50 6C 61 79 65 72` | `name` | `Player` |

## Example: a SNAPSHOT packet

The server sends the tick 120, with one player and one entity:

```text
52 54 01 83 00 00 00 3D 00 17 00 00 00 78 01 00 01 03 00 00 00 C8 00 01 00 00 00 00 01 00 3C 00 5A
```

| Bytes | Field | Value |
|-------|-------|-------|
| `52 54` | `magic` | `RT` |
| `01` | `version` | 1 |
| `83` | `type` | `SNAPSHOT` |
| `00 00 00 3D` | `sequence` | 61 |
| `00 17` | `payloadSize` | 23 |
| `00 00 00 78` | `tick` | 120 |
| `01` | `playerCount` | 1 |
| `00` | player `id` | 0 |
| `01` | player `flags` | alive |
| `03` | player `lives` | 3 |
| `00 00 00 C8` | player `score` | 200 |
| `00 01` | `entityCount` | 1 |
| `00 00 00 00` | entity `id` | 0 |
| `01` | entity `type` | ship |
| `00 3C` | entity `x` | 60 |
| `00 5A` | entity `y` | 90 |

The payload is 23 bytes: 4 (tick) + 1 + 7 (the player) + 2 + 9 (the entity).
