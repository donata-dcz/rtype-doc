---
title: Add a message type
description: Add a new message to the network protocol.
---

# Add a message type

A new message touches both programs. Do the steps in this order.

1. **Choose the value.** Under `0x80` if the client sends it, from `0x80` if the server sends it. Take the next free value.
2. **Add it to `MessageType`** in `Protocol.hpp`.
3. **Add it to `IsKnownType`** in `Protocol.cpp`:

   ```cpp
   case MessageType::MyMessage:
       return true;
   ```

   !!! danger "Do not forget this one"
       A type that is not in this list is rejected by `ReadHeader` as unknown. The message is silently dropped and nothing tells you why.

4. **Write the payload** with `BinaryWriter` and send it with `BuildPacket(type, sequence, payload)`.
5. **Read it** on the other side with `BinaryReader`, after `ReadHeader` has accepted the datagram. Check that every read worked: a datagram can be cut or forged.
6. **Update the [protocol page](../networking/protocol.md)**: the table of messages and the payload.
7. **Change `ProtocolVersion`** if an old client could no longer understand the new server (a change of an existing payload). A new type that old programs simply ignore does not need it.

The multi-byte numbers are always big-endian. Keep the payload small: a datagram is never bigger than 1400 bytes.
