# Client

The client is the `r-type_client` program. It opens a window with Raylib, shows a menu, connects to the server, sends the keyboard, and draws what the server says.

```bash
./build/default/Server/r-type_server [-h <ip-adress> -p <port>]
```

It listens on UDP port `4242` unless `-p` gives another one.

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

The menu has two screens: the main one (Play, Options, Quit) and the options (Volume, Text size, Back).

- The keys are the same on every screen: up and down to move, Enter to select, left and right to change a value, Esc to go back. A help line at the bottom reminds them.
- The mouse works too: hovering a row selects it and a click validates it.
- The layout is computed from the size of the window at each frame, so the window can be resized. The text size option changes the height of the rows.
- The selected row is shown by a `>` marker and a filled background, not only by a color.

## Assets

`ResourceManager::GetTexture("Title/RTypeTitle.png")` loads the image from the `Assets` folder next to the executable the first time, then gives the same texture. Textures can only be loaded after `InitWindow`.

## Audio

All the sound is in one class, `Audio::AudioSystem` (`Client/Include/Client/Audio.hpp` and `Client/Src/Audio.cpp`). 
The sounds are in `Assets/Sounds`, next to the client. Their names are written in `Audio.hpp`:
