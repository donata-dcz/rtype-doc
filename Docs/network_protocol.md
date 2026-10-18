# Network protocol

The client and the server talk with **UDP** datagrams, by default on port `4242`. The server is the authority: the client only sends what the player presses (never a position), and the server sends back what happened. The server simulates at 60 ticks per second and sends a snapshot about 30 times per second.

The code is in `Common`. The namespace is `Common::Network` for the packet header and `rtype` for the game data. All numbers of more than one byte are in **big-endian** (network order).

## Packet header (10 bytes)

Every datagram is a header followed by a payload.

| Offset | Field | Size | Meaning |
|--------|-------|------|---------|
| 0 | Magic | 2 bytes | `0x5254` ("RT" in ASCII). Any other datagram is not ours. |
| 2 | Version | 1 byte | Protocol version, currently `1`. |
| 3 | Type | 1 byte | Message type (see below). |
| 4 | Sequence | 4 bytes | Grows by one for every packet sent. |
| 8 | Payload size | 2 bytes | Number of bytes after the header. Must match the real size. |

`BuildPacket` writes a header and a payload into one datagram. `ReadHeader` checks everything and returns nothing if the datagram must be dropped. When it succeeds, the reader is on the first byte of the payload.

A datagram is **silently dropped** when it:

- is bigger than 1400 bytes, or smaller than the header;
- has a wrong magic or version;
- has a payload size that does not match its real size;
- has a reserved bit set to 1.

A malformed packet never crashes the client or the server, and never makes them allocate memory.

## Message types

Types under `0x80` go from the client to the server, the others the other way.

| Value | Name | Payload |
|-------|------|---------|
| `0x01` | Connect | Length of the name (1 byte, 0 to 16), then the name in printable ASCII |
| `0x02` | Input | Frame number (4 bytes), then the pressed actions (1 byte) |
| `0x03` | Ping | Nothing yet (heartbeat) |
| `0x04` | Disconnect | Nothing yet |
| `0x81` | Accept | The id of the player (1 byte, 0 to 3) |
| `0x82` | Reject | The reason (1 byte): `1` server full, `2` incompatible version |
| `0x83` | Snapshot | The state of the game (see below) |
| `0x84` | Event | Reserved, not used yet |
| `0x85` | Pong | Reserved |

## Inputs

The keys are packed in **one byte**, one bit per action. Bits 5 to 7 must stay at 0.

| Bit | Value | Action |
|-----|-------|--------|
| 0 | `0x01` | Move up |
| 1 | `0x02` | Move down |
| 2 | `0x04` | Move left |
| 3 | `0x08` | Move right |
| 4 | `0x10` | Shoot |

`InputState` (namespace `rtype`) hides the bits:

```cpp
InputState state;
state.Set(Action::..., true);                  // press
bool pressed = state.IsActive(Action::...);    // read
std::uint8_t bits = state.GetBits();           // what goes on the network
InputState back = InputState::FromBits(bits);  // rebuilt by the server
```

## Snapshot

A snapshot is everything the client needs to draw one moment of the match.

| Structure | Fields |
|-----------|--------|
| `Snapshot` | `tick`, a list of `players`, a list of `entities` |
| `SnapshotPlayer` (7 bytes) | `id`, `alive` and `invincible` (two bits of one byte), `lives`, `score` |
| `SnapshotEntity` (9 bytes) | `id` (4 bytes), `type`, `x` and `y` (rounded position, 2 bytes each, can be negative) |

Entity types are `1` Ship, `2` PlayerMissile, `3` Bydos and `4` EnemyMissile. There are at most 4 players (`MaxPlayers`) and 128 entities (`MaxSnapshotEntities`), so a snapshot always fits in one datagram.

UDP can deliver snapshots late or in the wrong order, so the client ignores a snapshot whose `tick` is lower than the highest one it already received.

## Example

An `Input` packet: sequence number 5, frame 42, keys "move right" and "shoot" (`0x08 + 0x10 = 0x18`). It is 15 bytes: 10 for the header and 5 for the payload.

```text
52 54        magic "RT"
01           version 1
02           type Input
00 00 00 05  sequence 5
00 05        payload size 5
00 00 00 2A  frame 42
18           actions: right + shoot
```

## What the client does with it

See [Connection rules](client.md#connection-rules) for the timings (connection retries, silence timeout).
