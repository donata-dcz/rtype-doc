# Client

The client is the `r-type_client` program. It opens a window with Raylib, shows a menu, connects to the server, sends the keyboard, and draws what the server says.

```bash
./build/default/Client/r-type_client [-h <host>] [-p <port>]
```

It connects to `127.0.0.1:4242` unless `-h` and `-p` say otherwise. **Play**
uses these values: the menu does not ask for an address.

## Connection status

`ServerConnection` is always in one of these states. The menu shows it in plain words.

| Status | Text in the menu | When |
|--------|------------------|------|
| `Connecting` | Connecting... | `Connect` is sent every 0.5 s and nothing came back yet |
| `Connected` | Connected as player N | `Accept` received |
| `Rejected` | Rejected: server is full (4 players max) | `Reject` received for a full server |
| `Rejected` | Rejected: wrong protocol version | `Reject` received for another version |
| `Unreachable` | The server does not answer | No answer after 5 s |
| `Lost` | Connection Lost | Nothing from the server for more than 3 s |

When the connection is lost, the player goes back to the menu and the message stays visible.

## The menu

The menu has two screens: the main one (Play, Options, Quit) and the options screen (volume, text size and Back).

- The keys are the same on every screen: up and down to move, Enter to select, left and right to change a value, Esc to go back. A help line at the bottom reminds them.
- The mouse works too: hovering a row selects it and a click validates it.
- The layout is computed from the size of the window at each frame, so the window can be resized. The text size option changes the height of the rows.
- The selected row is shown by a `>` marker and a filled background, not only by a color.

## Assets

`ResourceManager::GetTexture("Title/RTypeTitle.png")` loads the image from the `Assets` folder next to the executable the first time, then gives the same texture. Textures can only be loaded after `InitWindow`.

## Audio

All the sound is in one class, `Audio::AudioSystem` (`Client/Include/Client/Audio.hpp` and `Client/Src/Audio.cpp`). 
The sounds are in `Assets/Sounds`, next to the client. Their names are written in `Audio.hpp`. Without any audio device, every call does nothing and the game runs silently.

**Options > Volume** sets the master volume. Asking for the music that is already playing does nothing, so the main loop can ask every frame; `AudioSystem::Update` is called once per frame to keep the music going.

## At each frame

1. `ServerConnection::Update` reads the packets received (network thread, Asio) and updates the status.
2. In the menu: `Menu::Update` returns an action (`Connect` or `Quit`).
3. In a game, `ClientPredictionSystem` applies the local input immediately and stores it in a prediction buffer.
4. `SendInput` includes the local frame number. The server replies with `InputAck`; the client reconciles the acknowledged server position and replays inputs that were not acknowledged yet.
5. `SnapshotInterpolator` keeps the last snapshots and gives the picture to draw, while `SnapshotRenderer::Draw` renders the result.

The in-game HUD displays each player's score and lives and a boss health bar.
When the local player has no lives left, it displays `GAME OVER` over a
translucent black overlay so the game remains visible behind the alert.

The game loads every valid JSON file from `Assets/Levels` in sorted order.
After a boss is defeated, the server advances to the next level.
