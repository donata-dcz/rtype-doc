# Server

`r-type_server` runs the game and it receives the keys of the players, advances the game, and sends back what happened.

```bash
./build/default/Server/r-type_server [-p <port>]
```

It listens on UDP port `4242` unless `-p` gives another one.

## Two threads

| Thread | Code | Job |
|--------|------|-----|
| Network | `UdpServer` (Asio) | Receives datagrams and sends them. It is the only thread that touches the socket |
| Game | `main`, `Engine::GameLoop` | Reads the received packets, runs the game, decides what to send |

They only meet in two places: the list of received packets, protected by a mutex (`PollReceived` empties it), and the sends, which are posted to the network thread (`Send` never writes to the socket itself). The game itself therefore never has to be thread-safe.

## One tick

The game loop runs at 60 ticks per second (`TickRate` in `Ticks.hpp`). At each tick, `SetTickCallback` does this, in order:

1. Take all the packets received since the last tick (`PollReceived`).
2. Handle each packet (see the table below).
3. Remove the players who have been silent for 3 seconds (`SessionTimeout`). Their ship disappears from the next snapshots.
4. Advance the game by one step: `game.Tick(step)`.
5. Every second tick (`TicksPerSnapshot` = 2, so 30 times per second), build one snapshot with `game.BuildSnapshot()` and send it to every connected player.

At the end of each turn of the loop, the frame callback stops the loop on Ctrl+C, prints the status line once per second, and sleeps 1 ms to leave the processor alone (`GameLoop::Run` never sleeps).

## Sessions

A **session** is a player who completed the connection (`Connect`, then `Accept`). It is made of the sender's address, a player id, and the time of the last packet received. Only sessions receive snapshots, and only sessions are listened to.

- The player id is the lowest free number below `MaxPlayers`. It is free again when a player times out.
- The player id used by the game **comes from the session**, found with the sender's address, never from the content of a packet. A client cannot play for another one.
- When all the places are taken, the server answers `Reject` (server full).

The server does not act on `Disconnect`: a player who leaves is removed after 3 seconds without packets.

## How the server uses a game

The server does not know the rules of R-Type. It only calls six functions on a game object :

| Call | When the server makes it |
|------|--------------------------|
| `Game(seed)` | Once, at start (the seed comes from `std::random_device`) |
| `AddPlayer(id)` | A player's `Connect` is accepted. Returns `false` if there is no place |
| `RemovePlayer(id)` | A session times out |
| `SetInput(id, InputState)` | An `Input` packet arrives from a session |
| `Tick(dt)` | Every tick, 1/60 s |
| `BuildSnapshot() const` | Every second tick, to send to all sessions |

Anything that provides these six calls can be run by the server. See [Add a new game](AddAGame.md).

## Status updates

Once per second the server prints one line:

```text
server : 60 ticks/s, 3 entities, 1 players
```

`ticks/s` should be 60. A lower number means the game takes too long to compute a tick. `entities` is the number of entities in the last snapshot.
