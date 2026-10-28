# Guides

Step by step: [add a sound](#add-a-sound), [add an image](#add-an-image), [add a message type](#add-a-message-type), [add a menu option](#add-a-menu-option), [fix a problem](#troubleshooting).

## Add a sound

### A sound effect

1. Put the file in `Assets/Sounds`. The name is in PascalCase (`Explosion.wav`). A short `.wav` is the best choice.
2. In `Audio.hpp`, add a name to `enum class Effect`, before `Count`:

   ```cpp
   enum class Effect : std::size_t {
       Shoot,
       Explosion,
       Count
   };
   ```

3. In the same file, add the file to `EffectFiles`, **in the same order as the enum**:

   ```cpp
   constexpr std::array<const char *, EffectCount> EffectFiles = {"Shoot.wav", "Explosion.wav"};
   ```

4. Play it where it happens:

   ```cpp
   audio.Play(Audio::Effect::Explosion);
   ```

!!! warning
    If the array has fewer files than the enum has names, the missing ones are empty and the program crashes at the start. Count them.

### A stage music

Add the file to `StageMusics` in `Audio.hpp`, and put it in `Assets/Sounds`. The first stage uses the first file. A stage after the last one keeps the last music.

```cpp
constexpr std::array<const char *, 2> StageMusics = {"AsWetAsAFish.mp3", "SecondStage.mp3"};
```

Then call `audio.PlayStage(n)` when stage `n` starts. Do not call it at every frame: a music that already plays is not restarted, but the game over music would be replaced.

### If the sound does not play

- Look in the console for a warning starting with `AUDIO:`. It prints the path that could not be loaded.
- Check that `Assets/Sounds` was copied next to the executable, and that the name is spelled the same, capital letters included.
- If it crackles, lower the volume: sounds added together can exceed the maximum level.

## Add an image

1. Put the image in `Assets`, in a folder if it helps (`Assets/Title/RTypeTitle.png`). The name is in PascalCase: the CI refuses underscores.
2. Load it with the `ResourceManager`, **after** `InitWindow`:

   ```cpp
   const Texture2D &logo = resources.GetTexture("Title/RTypeTitle.png");
   ```

   The first call loads the image and the next calls give the same texture.

3. Draw it with Raylib (`DrawTexturePro` or `DrawTextureEx`). Compute the size from the size of the window, so it still looks right when the window is resized.

!!! tip
    If the image does not appear, check the name first: the path is spelled the same as the file, with the same capital letters, and the file was copied next to the executable.

## Add a message type

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
6. **Update the [protocol page](Protocol.md)**: the table of messages and the payload.
7. **Change `ProtocolVersion`** if an old client could no longer understand the new server (a change of an existing payload). A new type that old programs simply ignore does not need it.

The multi-byte numbers are always big-endian. Keep the payload small: a datagram is never bigger than 1400 bytes.

## Add a menu option

!!! note
    The names below are the ones of the current menu. If a name changed, search for it in `Menu.cpp`: the idea stays the same.

The menu is made of screens. A screen has a number of rows, a title and a help line, all written in one table in `Menu`. To add a row to the options:

1. **Store the value.** Add a field to `Settings` with a default value.
2. **Count the row.** Increase the number of rows of the options screen in the table.
3. **Draw it.** In the function that draws the options, add a call to `DrawRow` with its label and its current value.
4. **Handle it.** In the function that handles the options, react to left, right and Enter on the new row.
5. **Use it.** Read the value where it matters (the volume goes to the audio, the text size goes to the layout).

Keep the same keys as the other screens: up and down to move, left and right to change a value, Enter to select, Esc to go back. This is part of the [accessibility](Accessibility.md) of the menu.

!!! warning "If you remove a row"
    Change the number of rows too. A row removed from the drawing but still counted leaves an invisible row that can be selected.

## Troubleshooting

### Build

| Symptom | Probably | What to do |
|---------|----------|------------|
| `undefined reference` when linking | The new `.cpp` is not in the `CMakeLists.txt` | Add it, then run `cmake --preset default` |
| `...hpp: No such file or directory` | The project was not configured, or you are in the wrong folder | Run `pwd`, go to the root of the repository, run `cmake --preset default` |
| `build/default is not a directory` | The configure step was never done, or the repository is in a folder inside another one (`RType/RType`) | Configure first. Check with `git ls-files \| grep '^RType/'` that no extra folder was committed. |
| Conflict markers (`<<<<<<<`) in a file | A merge was not finished | Open the file, keep the right part, remove the markers |

### Git and CI

| Symptom | What to do |
|---------|------------|
| `git merge origin/dev` says "Already up to date" | Run `git fetch origin` first, then merge again |
| The CI refuses a file name | The name has an underscore. Rename it in PascalCase with `git mv`, and update the places that use it. |
| The CI refuses a pull request to `main` | A pull request to `main` only comes from `dev` |

### Running

| Symptom | Probably | What to do |
|---------|----------|------------|
| "The server does not answer" | The server is not running, or the port is not the same | Start the server, and use the same `-p` on both sides |
| The client stops with code 84 | A wrong command-line option | Read the usage that was printed: port from 1 to 65535, `-h` with an address |
| An image is missing | Not copied next to the executable, or the name differs | Check the path and the capital letters |
| No sound at all | No audio device, or the files are missing | Look for an `AUDIO:` warning in the console |
| The sound crackles | Several sounds add up above the maximum, or the audio of the computer is slow (WSL and virtual machines often are) | Lower the volumes. Try the game on a real machine. |

!!! tip "Still stuck?"
    Ask on the team channel with the exact error message and the command you ran.
