# R-Type Network Protocol Specification (RFC)

## Table of Contents

1. Introduction
2. Terminology & Conventions
3. Connection & Transport Architecture
4. Packet Structure & Binary Format
5. Data Types & Endianness
6. Protocol Messages
7. Message Sequences & Exchange Diagrams
8. Game World Reference
9. Error Handling, Robustness & Security
10. Implementation Checklist
11. Appendix A - Packet Binary Examples
12. Bandwidth Measurements

---

## 1. Introduction

This document specifies the official binary network protocol for the **R-Type** multiplayer game engine.
It is designed to be fully deterministic, authoritative on the server side, and efficient over low-bandwidth connection constraints.

The objective of this specification is to allow any third-party developer to implement a completely compatible client or server from scratch **without accessing the original codebase**.

---

## 2. Terminology & Conventions

The key words **MUST**, **MUST NOT**, **REQUIRED**, **SHALL**, **SHALL NOT**, **SHOULD**, **SHOULD NOT**, **RECOMMENDED**, **MAY**, and **OPTIONAL** in this document are to be interpreted as described in [RFC 2119](https://www.rfc-editor.org/rfc/rfc2119).

- **Server**: The central authority running the game physics, logic, state, and collision checks.
- **Client**: A remote terminal sending user inputs and rendering visual representations based on snapshots.
- **Tick**: A single discrete step of the server simulation loop (60 ticks/second).
- **Snapshot**: A game state broadcast containing positions, entity statuses, and player statistics.

---

## 3. Connection & Transport Architecture

The protocol operates over **UDP** (User Datagram Protocol) to minimize latency.

```text
+-------------------+                               +-------------------+
|   R-Type Client   | <======== UDP Port 4242 =====> |   R-Type Server   |
+-------------------+                               +-------------------+
                                                              |
                                                    +---------+---------+
                                                    | ECS Registry &    |
                                                    | Authoritative Game|
                                                    | Loop (60 Hz)      |
                                                    +-------------------+
```

- **Transport**: Pure UDP socket. Default port is `4242`.
- **Client Identification**: Sessions are bound to the client's socket address (`IP:Port`).
- **State Machine**: Server is strictly authoritative; clients send intentions (key presses), not coordinates.

---

## 4. Packet Structure & Binary Format

Every datagram MUST consist of a **10-byte fixed Header** immediately followed by a variable-length **Payload**.

### 4.1 Header Format

| Offset (Bytes) | Field | Type | Size | Description / Value |
|---|---|---|---|---|
| 0 | `magic` | `u16` | 2 bytes | Protocol Magic Identifier: `0x5254` (`"RT"` ASCII) |
| 2 | `version` | `u8` | 1 byte | Protocol Version: `4` |
| 3 | `type` | `u8` | 1 byte | Message Type Identifier (Section 6) |
| 4 | `sequence` | `u32` | 4 bytes | Monotonic packet sequence counter |
| 8 | `payloadSize` | `u16` | 2 bytes | Length of the payload in bytes |
| 10 | `payload` | -| Var | Byte array of size `payloadSize` |

---

## 5. Data Types & Endianness

All multi-byte numeric fields MUST be transmitted using **Network Byte Order (Big-Endian)**.

| Type | Size | Encoding | Range |
|---|---|---|---|
| `u8` | 1 byte | Unsigned Integer | `0` to `255` |
| `u16` | 2 bytes | Unsigned Integer (Big-Endian) | `0` to `65,535` |
| `u32` | 4 bytes | Unsigned Integer (Big-Endian) | `0` to `4,294,967,295` |
| `i16` | 2 bytes | Signed Integer (Two's Complement, Big-Endian) | `-32,768` to `32,767` |

*Example:* The integer `-12` in `i16` is formatted as `0xFFF4`.

### 5.1 Bit fields

`SNAPSHOT` and `DELTA_SNAPSHOT` payloads pack most of their fields on fewer bits than a byte. Those fields are written **after** the byte-aligned fields of the message, one after the other, with no alignment between them:

- A field of `n` bits (`1 <= n <= 32`) is written **least significant bit first**: its lowest bit goes in the first free bit of the current byte, starting from bit 0 of the byte.
- The last byte is padded with `0` bits. A receiver MUST drop the datagram if one padding bit is set, or if bytes remain after the last field.
- Field widths are fixed by this document, never by a length prefix.

---

## 6. Protocol Messages

Messages with OpCodes `< 0x80` flow from **Client → Server**. 
Messages with OpCodes `>= 0x80` flow from **Server → Client**.

| OpCode | Name | Direction | Payload Description |
|---|---|---|---|
| `0x01` | `CONNECT` | Client → Server | Name length + Printable ASCII Name |
| `0x02` | `INPUT` | Client → Server | Frame counter + Action bitmask |
| `0x03` | `PING` | Client → Server | Empty heartbeat used to measure round-trip time |
| `0x07` | `LIST_ROOMS_REQUEST` | Client → Server | Empty request for the current room list |
| `0x05` | `SNAPSHOT_ACK` | Client → Server | Tick of the last snapshot rebuilt |
| `0x04` | `DISCONNECT` | Client → Server | Reserved (Explicit disconnect) |
| `0x81` | `ACCEPT` | Server → Client | Assigned `playerId` |
| `0x82` | `REJECT` | Server → Client | Rejection reason code |
| `0x83` | `SNAPSHOT` | Server → Client | Full world state snapshot (bit-packed) |
| `0x84` | `EVENT` | Server → Client | Reliable one-off game event |
| `0x85` | `PONG` | Server → Client | Empty response carrying the request sequence |
| `0x86` | `DELTA_SNAPSHOT` | Server → Client | Changes since a snapshot the client confirmed |
| `0x87` | `INPUT_ACK` | Server → Client | Highest input frame consumed by the server |
| `0x08` | `LIST_ROOMS_RESPONSE` | Server → Client | Room count and room metadata |

### 6.1 `CONNECT` (`0x01`)
- `nameLength` (`u8`): Length of name (0 to 16).
- `name` (`u8[]`): Printable ASCII characters (`0x20` to `0x7E`).

### 6.2 `ACCEPT` (`0x81`)
- `playerId` (`u8`): Assigned slot index (`0` to `3`).

### 6.3 `REJECT` (`0x82`)
- `reason` (`u8`): `1` = Server full, `2` = Incompatible version.

### 6.4 `INPUT` (`0x02`)
- `frame` (`u32`): Incremental frame index of client.
- `actions` (`u8`): Bitmask defined as follows:
  - Bit 0 (`0x01`): Move Up
  - Bit 1 (`0x02`): Move Down
  - Bit 2 (`0x04`): Move Left
  - Bit 3 (`0x08`): Move Right
  - Bit 4 (`0x10`): Primary Shoot
  - Bits 5–7: MUST be `0`.

### 6.4.1 `PING` (`0x03`) and `PONG` (`0x85`)

`PING` has an empty payload. The server answers with an empty `PONG` using
the same sequence number. The client uses the elapsed time between the two
packets as its round-trip-time measurement.

### 6.4.2 `EVENT` (`0x84`)

`EVENT` packets are sent reliably by the server for one-off game events. The
payload starts with an event type (`u8`):

| Type | Name | Additional payload |
|---:|---|---|
| `1` | `PLAYER_DIED` | Player ID (`u8`) |
| `2` | `GAME_OVER` | None |
| `3` | `PLAYER_LEFT` | Player ID (`u8`) |

The client acknowledges reliable events with an `ACK` packet and ignores
duplicates. Boss defeat is not a protocol event: the client detects the
boss's disappearance between snapshots and controls the boss-defeat music
locally.

### 6.4.3 `LIST_ROOMS_REQUEST` (`0x07`) and `LIST_ROOMS_RESPONSE` (`0x08`)

`LIST_ROOMS_REQUEST` has an empty payload and is accepted only for an already
connected session. The response payload is:

- `roomCount` (`u16`): number of entries, at most 100;
- for each room:
  - `id` (`u32`);
  - `nameLength` (`u16`) and `name` (`u8[]`), at most 255 bytes;
  - `playerCount` (`u32`);
  - `maxPlayers` (`u32`);
  - `isRunning` (`u8`): `0` or `1`.

The current server uses the room list for discovery and automatically assigns
a connecting player to the first room with an available slot. These messages
do not create, join, or remove a specific room.

### 6.5 `SNAPSHOT` (`0x83`)

Full world state. Sent to a player who has not confirmed any snapshot yet (`SNAPSHOT_ACK`), when the snapshot it confirmed is too old, or when it is smaller than the delta.

Byte-aligned part:
- `tick` (`u32`): Server tick index.

Bit part (Section 5.1), in this order:

| Field | Bits | Description |
|---|---|---|
| `playerCount` | 3 | Active player entries (`0` to `4`) |
| `entityCount` | 8 | Active game entities (`0` to `128`) |
| `players` | `playerCount` × 39 | See below |
| `entities` | `entityCount` × 40 (or 47 for a boss) | See below |

Player (39 bits):

| Field | Bits | Description |
|---|---|---|
| `id` | 2 | Player ID (`0` to `3`) |
| `flags` | 2 | Bit 0 = Alive, Bit 1 = Invincible |
| `lives` | 3 | Remaining lives (`0` to `7`) |
| `score` | 32 | Total score |

Entity (38 bits):

| Field | Bits | Description |
|---|---|---|
| `id` | 16 | Unique entity ID. A ship uses its player ID (`0` to `3`); every other entity uses `4` to `65535`, wrapping around |
| `type` | 4 | `type - 1`: Ship, PlayerMissile, Bydos, EnemyMissile, Kamikaze, Turret, Boss, BossShot |
| `x` | 10 | Horizontal position + `64` (so `-64` to `959`) |
| `y` | 10 | Vertical position + `64` (so `-64` to `959`) |

The server clamps a position that would not fit. Entities are sent in drawing order: ships by id, then Bydos by id, then missiles by id. Players are sent by id.

### 6.6 `SNAPSHOT_ACK` (`0x05`)
- `tick` (`u32`): `tick` of a snapshot the client has just rebuilt (from a `SNAPSHOT` or a `DELTA_SNAPSHOT`).

The client sends one for every snapshot it accepts. The server ignores an ack for a tick it does not remember, and an ack older than the last one.

### 6.7 `DELTA_SNAPSHOT` (`0x86`)

Only what changed between a **baseline** (the last snapshot the player confirmed) and the current snapshot. The client rebuilds the current snapshot from the baseline it kept. The baseline is always one of the last `32` snapshots sent, so a client MUST keep the last `32` snapshots it rebuilt.

Byte-aligned part:
- `tick` (`u32`): Server tick index of the current snapshot.
- `age` (`u8`): `tick` minus the tick of the baseline (`1` to `255`).

Bit part (Section 5.1), in this order:

| Field | Bits | Description |
|---|---|---|
| `playersPresent` | 4 | Bit `i` set: player `i` is in the current snapshot |
| `playersSent` | 4 | Bit `i` set: player `i` is new or changed (subset of `playersPresent`) |
| `players` | 37 each | For each bit set in `playersSent`, by id: `flags` (2), `lives` (3), `score` (32) |
| `removedCount` | 8 | Entities of the baseline that are gone |
| `removed` | 16 each | Their ids |
| `addedCount` | 8 | Entities that were not in the baseline |
| `added` | 40 each (or 47 for a boss) | Full entities (same layout as in `SNAPSHOT`) |
| `movedCount` | 8 | Entities of the baseline whose position changed |
| `moved` | 36 each, or 43 for a boss | `id` (16), `x` (10), `y` (10), and boss health when applicable |

Everything not named (a player not in `playersSent`, an entity neither removed nor moved) is copied from the baseline. Entities then go in the drawing order of Section 6.5. A receiver MUST drop a delta whose `age` does not match the baseline it has, that removes or moves an id absent from the baseline, or that adds an id already present.

The server sends a `DELTA_SNAPSHOT` only if it is smaller than the `SNAPSHOT` it would send instead.

### 6.8 `INPUT_ACK` (`0x87`)

- `frame` (`u32`): highest input frame consumed by the server for this session.

The client uses this acknowledgement to discard confirmed inputs from its
prediction buffer and replay the remaining inputs after the authoritative
server position.

---

## 7. Message Sequences & Exchange Diagrams

### 7.1 Connection Handshake & Session Establishment

```text
Client                                                 Server
  |                                                      |
  |--- CONNECT (name) ---------------------------------->| (Check available slot)
  |<-- ACCEPT (playerId) --------------------------------| (Session created)
  |                                                      |
  |--- INPUT (frame=1, keys) --------------------------->| (Tick processing)
  |<-- SNAPSHOT (tick=1, world) -------------------------| (Broadcast 30Hz)
  |                                                      |
```

### 7.2 Connection Rejection (Server Full)

```text
Client                                                 Server
  |                                                      |
  |--- CONNECT (name) ---------------------------------->| (Slots = 4/4)
  |<-- REJECT (reason=1) --------------------------------| (Drop connection)
```

### 7.3 Disconnection & Timeout Handling

```text
Client                                                 Server
  |                                                      |
  | (Client crashes / drops connection)                  |
  | X                                                    |
  |                                                      | (No packets for 3.0s)
  |                                                      | [Timeout triggered]
  |                                                      | -> Remove session & ship
```

### 7.4 Snapshot Confirmation & Delta

```text
Client                                                 Server
  |                                                      |
  |<-- SNAPSHOT (tick=10, full) --------------------------| (no ack yet)
  |--- SNAPSHOT_ACK (tick=10) --------------------------->| (baseline = 10)
  |<-- DELTA_SNAPSHOT (tick=12, age=2) -------------------| (changes since 10)
  |<-- DELTA_SNAPSHOT (tick=14, age=4) -------------------| (ack of 12 not here yet)
  |--- SNAPSHOT_ACK (tick=12) --------------------------->| (baseline = 12)
  |<-- DELTA_SNAPSHOT (tick=16, age=4) -------------------|
```

A lost snapshot is never repaired: the next one is built on a baseline the client does have. If the acks stop for longer than the server history (32 snapshots), the server sends full snapshots again.

---

## 8. Game World Reference

- **Dimensions**: `800 × 600` virtual space units.
- **Origin**: `(0, 0)` is the **top-left corner**.
- **Entity Dimensions**:
  - `Ship`: 99 × 51 units (IDs `0` to `3`).
  - `PlayerMissile`: 32 × 16 units.
  - `Bydos`: 42 × 48 units.
  - `EnemyMissile`: 32 × 16 units.

---

## 9. Error Handling, Robustness & Security

A receiver **MUST SILENTLY DROP** any datagram if:
1. Total size exceeds **1400 bytes**.
2. Total size is less than the **10-byte header**.
3. `magic` != `0x5254` or `version` != `2`.
4. `payloadSize` does not match the actual remaining bytes.
5. `actions` contain non-zero reserved bits.
6. A bit-packed payload is shorter or longer than its fields, or has a non-zero padding bit (Section 5.1).
7. A `DELTA_SNAPSHOT` does not fit the baseline the receiver has (Section 6.7).

---

## 10. Implementation Checklist

1. [ ] Open UDP Socket on port `4242`.
2. [ ] Send `CONNECT` every 0.5s until `ACCEPT` or `REJECT` is received (timeout after 5s).
3. [ ] Store `playerId` from `ACCEPT`.
4. [ ] Send `INPUT` packet every frame (60 Hz).
5. [ ] Process incoming `SNAPSHOT` and `DELTA_SNAPSHOT` packets; discard any snapshot with a `tick` lower than or equal to the highest received `tick`.
6. [ ] Keep the last 32 snapshots rebuilt and send a `SNAPSHOT_ACK` for each new one.
7. [ ] Disconnect and notify user if no packets are received from server for > 3.0s.

---

## 11. Appendix A - Packet Binary Examples

### A.1 `CONNECT` Packet Example
Client sends player name "Player":

```text
52 54 02 01 00 00 00 00 00 07 06 50 6C 61 79 65 72
|---| |  |  |-----------| |-| |-| |-------------|
Magic V  Type Sequence    Size  Len Name ("Player")
```

### A.2 `SNAPSHOT` Packet Example
Server broadcasts 1 player (id 0, alive, 3 lives, score 200) and 1 entity (ship 0 at x=60, y=90) at tick 120:

```text
52 54 02 83 00 00 00 3D 00 0F 00 00 00 78 09 A0 21 03 00 00 00 00 C0 87 26
|---| |  |  |-----------| |---| |-----------| |------------------------|
Magic V  Type Sequence    Size  Tick (120)    Bit part: 88 bits = 11 bytes
```

Bit part, least significant bit first (the payload is 23 bytes in protocol 1 and 15 bytes in protocol 2):

```text
playerCount=1 (3) | entityCount=1 (8) | player: id=0 (2) flags=01 (2) lives=3 (3) score=200 (32)
| entity: id=0 (16) type=0 (4) x=60+64=124 (10) y=90+64=154 (10)
```

### A.3 `SNAPSHOT_ACK` Packet Example
Client confirms tick 120:

```text
52 54 02 05 00 00 00 05 00 04 00 00 00 78
|---| |  |  |-----------| |---| |-----------|
Magic V  Type Sequence    Size  Tick (120)
```

---

## 12. Bandwidth Measurements

Goal of protocol 2: send less data per snapshot, with three steps.

1. **Types of the right size and quantization**: fields use the bits they need (Section 6.5), positions are cut to 10 bits (no loss: they were already whole pixels).
2. **Bit-packing**: no padding between fields.
3. **Delta compression**: only what changed since the last snapshot the client confirmed (Section 6.7).

Measurement (`Tests/Src/ProtocolTests.cpp`, test `Bandwidth.BeforeAndAfterOneMinuteOfMatch`, run `ctest -V -R Bandwidth`): 60 s of a match with 4 players holding fire, seed 42, 30 snapshots/s, one client that confirms with a delay of about 100 ms and loses 1 snapshot out of 10 and 1 ack out of 7. Sizes include the 10-byte header. The test also checks that every snapshot rebuilt by the client is identical to the server one.

| Protocol | Bytes / snapshot | KB/s downstream (1 player) | Change |
|---|---|---|---|
| 1 (before) | 155.1 | 4.54 | - |
| 2, bit-packing only | 93.5 | 2.74 | -39.7 % |
| 2, bit-packing + delta | 77.3 | 2.27 | **-50.1 %** |

The acks add `0.37` KB/s upstream (14 bytes per snapshot). The scene is calm (12 entities on average): with a full snapshot of 128 entities the bit-packing alone saves about 47 % of the entities part.

In an R-Type almost every entity moves between two snapshots, which is why the delta saves less than in a static world: it mainly avoids resending the type, the entities that did not move, and the players that did not change.

Live measurement: the server prints `out ... KB/s (N full + M delta snapshots), in ... KB/s` every second and an average when it stops (Ctrl+C).
