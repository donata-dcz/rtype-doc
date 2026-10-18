# Client

The client is the `r-type_client` program. It opens a window with Raylib, shows a menu, connects to the server, sends the keyboard, and draws what the server says.

## One frame

The loop is in `Client/Src/Main.cpp`. At each frame (60 per second):

1. Update the star-field and the connection to the server.
2. Set the volume from the options and keep the music going ([Audio](#audio)).
3. **In the menu:** update the menu. A click on **Play** creates the connection. When the server answers `Accept`, the game starts.
4. **In the game:** read the keyboard and send it, play the shot sound, and check for game over.
5. Draw the star-field, then the game (from the last snapshot) or the menu.

Esc, or a lost connection, brings the player back to the menu. Esc never closes the window.

## Main classes

| Class | Role |
|-------|------|
| `Menu` | The screens (main, options) and their navigation with keyboard and mouse. Returns what the player chose (`Connect`, `Quit`). |
| `MenuRenderer` | Draws the rows, the title and the help line of the menu. |
| `Settings` | The options: volume and text size. |
| `ServerConnection` | Sends the inputs, receives the snapshots and tells the connection status. |
| `SnapshotRenderer` | Draws one snapshot. |
| `Starfield` | The scrolling background. The seed is the same for everybody, so everybody sees the same stars. |
| `ResourceManager` | Loads the textures once and keeps them. |
| `Audio::AudioSystem` | Music and sound effects. See [Audio](#audio). |

## The command line

`Main.cpp` reads `-h <host>` and `-p <port>` before opening the window. The port must be a number from 1 to 65535 and the host cannot be empty. If something is wrong, the usage is printed and the program returns **84**. The address and the port are given to the menu, so **Play** can connect without asking anything.

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

## The window

- `FLAG_WINDOW_RESIZABLE` is set before `InitWindow` so the window can be resized.
- `SetExitKey(KEY_NULL)` stops Esc from closing the window, because Esc means "back".
- The audio device is opened after the window.

## Assets

`ResourceManager::GetTexture("Title/RTypeTitle.png")` loads the image from the `Assets` folder next to the executable the first time, then gives the same texture. Textures can only be loaded after `InitWindow`.

The name must be spelled the same, with the same capital letters, and must be PascalCase like every file of the repository.

## Audio

All the sound is in one class, `Audio::AudioSystem` (`Client/Include/Client/Audio.hpp` and `Client/Src/Audio.cpp`). The game has a **title music** (menus), a **stage music** (during a game), a **game over music** (played once when the player dies for the last time) and a **shot sound**.

If the computer has no audio device, every call does nothing and the game runs silently. A warning starting with `AUDIO:` is printed in the console.

The sounds are in `Assets/Sounds`, next to the client. Their names are written in `Audio.hpp`:

| What | Constant | File |
|------|----------|------|
| Title music | `TitleMusic` | `MenuScreen.mp3` |
| Stage music | `StageMusics` | `AsWetAsAFish.mp3` |
| Game over music | `GameOverMusic` | `GameOver.mp3` |
| Shot | `EffectFiles` | `Shoot.wav` |

The musics are streamed from the file while they play. The effects are loaded once at the start. Only one music is loaded at a time, and the same effect can play up to 4 times at once.

The object is created after the window, because the audio device needs it:

| Call | When |
|------|------|
| `Init(directory)` | Once, after `InitWindow` |
| `SetMasterVolume(v)` | Every frame, from the volume option. `v` goes from 0.0 to 1.0. |
| `SetEffectsVolume(v)` | Once, to make the effects quieter than the music, if needed |
| `Update()` | Every frame. It keeps the music streaming. |
| `PlayTitle()` | In the menus. Can be called every frame: it does nothing if the title music already plays. |
| `PlayStage(n)` | Once, when the game starts |
| `PlayGameOver()` | Once, when the player dies for the last time (`alive` is false and `lives` is 0 in the snapshot) |
| `Play(Effect::Shoot)` | When the player shoots, with a short delay between two sounds |

Rules for the sound files: names in PascalCase, a level under the maximum (several sounds added together can crackle), a music that loops starts and ends quietly (otherwise there is a click), and only sounds we have the right to use, with their origin written down. No information should exist only as a sound: see [Accessibility](Accessibility.md). To add a sound, see [Guides](guides.md#add-a-sound).
