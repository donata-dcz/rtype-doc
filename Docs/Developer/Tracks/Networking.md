# Advanced Networking Features

This page lists the items of this track in the subject, in the same order, with what we implemented.

## Multi-instance Server

| Feature | Status | Notes |
|---|---|---|
| Ability for the server to run several different game instances in parallel | Partial | `Room` (one thread per game) is implemented and tested. The server executable still runs only one game |
| Lobby/Room system | Partial | `Lobby` supports creating, listing, joining, leaving and deleting rooms. No protocol message currently allows clients to list or join rooms |
| Users and identities management: storage, sessions, authentication, etc. | Not done | Only a nickname is provided through `CONNECT` |
| Communication between users: text chat or voice | Not done | |
| Game rules management per game instance | Not done | |
| Global scoreboard and/or ranking system | Not done | |
| Administration dashboard | Not done | No interface for operations such as kicking or banning users |

## Data Transmission Efficiency and Reliability

| Feature | Status | Notes |
|---|---|---|
| Data packing | Done | Right-sized types, 10-bit position quantization, `BitWriter` and `BitReader` |
| General-purpose data compression | Not done | No general-purpose algorithm such as RLE, Huffman, LZ4 or zlib. Delta snapshots already remove most of the redundancy |
| Game-specific compression: delta snapshot compression | Done | Based on the last snapshot confirmed by the client (`SNAPSHOT_ACK`), using the server-side snapshot history |
| Network errors mitigation: packet drops | Done | Lost snapshots are not retransmitted. The next delta uses a baseline already known by the client |
| Network errors mitigation: packet reordering | Partial | The client discards snapshots whose tick is not newer than the last received snapshot. Other messages are not covered |
| Network errors mitigation: packet duplication | Partial | Duplicated snapshots are discarded the same way. No general duplicate handling for all messages |
| Message reliability | Partial | `ReliableSender`, `ReliableReceiver` and `ReliableConnection` implement ACK, retransmission and a duplicate window. They are tested but not yet used by the server or the client. The `CONNECT` handshake uses a simple retry |
| **(bonus)** Packet validation | Done | See [Protocol](../Protocol.md), section 9 |

## High-level Networking Engine Architecture

| Feature | Status | Notes |
|---|---|---|
| Client-side prediction with server reconciliation | Not done | |
| Low-frequency server updates with entity state interpolation | Not done | The client currently draws snapshots as received |
| Server-side lag compensation | Not done | |
| Input delaying | Not done | |
| Rollback netcode | Not done | |
| Real measurements: bandwidth usage | Partial | `Bandwidth.BeforeAndAfterOneMinuteOfMatch` measures a reduction from 155 to 77 bytes per snapshot (-50%). See [Protocol](../Protocol.md), section 12. More live measurements are still needed |
| Real measurements: responsiveness in front of lag | Not done | No measurements or tests yet |
| **(bonus)** Live bandwidth display on the server | Done | Current bandwidth usage is displayed in KB/s while the server is running |
| Ping/Pong (base of a lagometer) | Not done | Message types are reserved but not yet handled |
