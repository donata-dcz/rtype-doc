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
| 2 | `version` | `u8` | 1 byte | Protocol Version: `1` |
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

---

## 6. Protocol Messages

Messages with OpCodes `< 0x80` flow from **Client → Server**. 
Messages with OpCodes `>= 0x80` flow from **Server → Client**.

| OpCode | Name | Direction | Payload Description |
|---|---|---|---|
| `0x01` | `CONNECT` | Client → Server | Name length + Printable ASCII Name |
| `0x02` | `INPUT` | Client → Server | Frame counter + Action bitmask |
| `0x03` | `PING` | Client → Server | Reserved (Heartbeat) |
| `0x04` | `DISCONNECT` | Client → Server | Reserved (Explicit disconnect) |
| `0x81` | `ACCEPT` | Server → Client | Assigned `playerId` |
| `0x82` | `REJECT` | Server → Client | Rejection reason code |
| `0x83` | `SNAPSHOT` | Server → Client | Full world state snapshot |
| `0x84` | `EVENT` | Server → Client | Reserved (One-off events) |
| `0x85` | `PONG` | Server → Client | Reserved |

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

### 6.5 `SNAPSHOT` (`0x83`)
- `tick` (`u32`): Server tick index.
- `playerCount` (`u8`): Active player entries (`0` to `4`).
- `players` (`Array`): `playerCount` × 7 bytes:
  - `id` (`u8`): Player ID (`0` to `3`).
  - `flags` (`u8`): Bit 0 = Alive, Bit 1 = Invincible.
  - `lives` (`u8`): Remaining lives.
  - `score` (`u32`): Total score.
- `entityCount` (`u16`): Active game entities (`0` to `128`).
- `entities` (`Array`): `entityCount` × 9 bytes:
  - `id` (`u32`): Unique Entity ID.
  - `type` (`u8`): `1` = Ship, `2` = PlayerMissile, `3` = Bydos, `4` = EnemyMissile.
  - `x` (`i16`): Horizontal position.
  - `y` (`i16`): Vertical position.

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

---

## 8. Game World Reference

- **Dimensions**: `800 × 600` virtual space units.
- **Origin**: `(0, 0)` is the **top-left corner**.
- **Entity Dimensions**:
  - `Ship`: 99 × 51 units (IDs `0` to `3`).
  - `PlayerMissile`: 16 × 4 units.
  - `Bydos`: 48 × 36 units.
  - `EnemyMissile`: 16 × 4 units.

---

## 9. Error Handling, Robustness & Security

A receiver **MUST SILENTLY DROP** any datagram if:
1. Total size exceeds **1400 bytes**.
2. Total size is less than the **10-byte header**.
3. `magic` != `0x5254` or `version` != `1`.
4. `payloadSize` does not match the actual remaining bytes.
5. `actions` or `flags` contain non-zero reserved bits.

---

## 10. Implementation Checklist

1. [ ] Open UDP Socket on port `4242`.
2. [ ] Send `CONNECT` every 0.5s until `ACCEPT` or `REJECT` is received (timeout after 5s).
3. [ ] Store `playerId` from `ACCEPT`.
4. [ ] Send `INPUT` packet every frame (60 Hz).
5. [ ] Process incoming `SNAPSHOT` packets; discard any snapshot with a `tick` lower than the highest received `tick`.
6. [ ] Disconnect and notify user if no packets are received from server for > 3.0s.

---

## Appendix A - Packet Binary Examples

### A.1 `CONNECT` Packet Example
Client sends player name "Player":

```text
52 54 01 01 00 00 00 00 00 07 06 50 6C 61 79 65 72
|---| |  |  |-----------| |-| |-| |-------------|
Magic V  Type Sequence    Size  Len Name ("Player")
```

### A.2 `SNAPSHOT` Packet Example
Server broadcasts 1 player and 1 entity at tick 120:

```text
52 54 01 83 00 00 00 3D 00 17 00 00 00 78 01 00 01 03 00 00 00 C8 00 01 00 00 00 00 01 00 3C 00 5A
|---| |  |  |-----------| |-| |-----------| || |  |  |-----------| |---| |-----------| |  |---| |---|
Magic V  Type Sequence    Size Tick (120)   PC ID Fl Lives Score   EC    Entity ID   T  X(60) Y(90)
```
