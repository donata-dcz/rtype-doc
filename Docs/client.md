# Client

The client is the graphical part of the game. It draws, plays the sounds, reads the keyboard and talks to the server. It holds no game rule: what happens in the game is decided by the server.

## One frame

`Client/Src/Main.cpp` holds the loop. At each frame:

1. Update the star-field and the server connection.
2. Set the audio volume from the options, and keep the music going.
3. If the player is in the menu: update the menu. A click on **Play** creates the connection. When the server answers `Accept`, the game starts.
4. If the player is playing: read the keyboard and send it, then check for game over.
5. Draw: the star-field, then either the game (from the last snapshot) or the menu.

Esc, or a lost connection, brings the player back to the menu. Esc never closes the window.

## Main classes

| Class | File | Role |
|-------|------|------|
| `Menu` | `Menu.hpp/.cpp` | Screens (main, connect, options), keyboard and mouse navigation. Returns what the player chose (`Connect`, `Quit`). |
| `MenuRenderer` | `MenuRenderer.hpp/.cpp` | Draws the rows, the title and the help line. |
| `ConnectForm` | `ConnectForm.hpp/.cpp` | The address, port and name typed by the player, and their validation. |
| `Settings` | `Settings.hpp` | The options: volume and text size. |
| `ServerConnection` | `ServerConnection.hpp/.cpp` | Sends the inputs, receives the snapshots, and tells the connection status. |
| `SnapshotRenderer` | `SnapshotRenderer.hpp/.cpp` | Draws a snapshot. |
| `Starfield` | `Starfield.hpp/.cpp` | The scrolling background (the same seed for everybody, so everybody sees the same stars). |
| `ResourceManager` | `ResourceManager.hpp/.cpp` | Loads textures once and keeps them. |
| `Audio::AudioSystem` | `Audio.hpp/.cpp` | Music and sound effects. |

The connection status is one of `Connecting`, `Connected`, `Rejected`, `Unreachable` and `Lost`. The menu shows it in plain words.

## Connection rules

| Step | Rule |
|------|------|
| Connecting | Send `Connect` every 0.5 s until `Accept` or `Reject` arrives. After 5 s without an answer the status becomes `Unreachable` ("The server does not answer"). |
| Playing | Send an `Input` every frame (60 per second). Keep the player id from `Accept`. |
| Silence | If nothing arrives from the server for more than 3 s, the status becomes `Lost` and the player goes back to the menu. |
| Leaving | The server also removes a player (and the ship) after 3 s without packets. |

## The menu

The `Menu` has three screens (main, connect, options). Each frame, `Update` moves the selection (keyboard, then mouse), lets the player type in the selected field, reads the keys of the frame into an `Input`, and sends it to the handler of the current screen.

- The address, port and name are checked by `ConnectForm::Validate` when the player chooses **Connect**. The error is shown under the buttons.
- The window can be resized (`FLAG_WINDOW_RESIZABLE` before `InitWindow`), and the menu is laid out again from the size of the window at each frame.
- `SetExitKey(KEY_NULL)` stops Esc from closing the window, because Esc means "back".

To add an option, see [Add a menu option](how-to.md#add-a-menu-option).

## Sound

`Audio::AudioSystem` is created after the window (the audio device needs it). Without any audio device it does nothing and the game stays silent.

| Call | When |
|------|------|
| `Init(directory)` | Once, after `InitWindow` |
| `SetMasterVolume(v)` | Every frame, from the volume option (0.0 to 1.0) |
| `Update()` | Every frame, it keeps the music streaming |
| `PlayTitle()` | In the menus. Can be called every frame: it does nothing if the title music already plays. |
| `PlayStage(n)` | Once, when the game starts |
| `PlayGameOver()` | Once, when the player dies for the last time |
| `Play(Effect::Shoot)` | When the player shoots (with a short delay between two sounds) |

The files are in `Assets/Sounds`. Their names are listed in `Audio.hpp`. A shot can play up to 4 times at once (`VoicesPerEffect`). If the sound crackles, lower the volume first: several sounds added together can exceed the maximum level.

## Assets

`ResourceManager::GetTexture("Title/RTypeTitle.png")` loads the image from the `Assets` folder next to the executable the first time, and gives the same texture after that. Textures can only be loaded after `InitWindow`.

