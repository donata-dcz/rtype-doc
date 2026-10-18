---
title: Audio
description: The music and sound effects of the client.
---

# Audio

All the sound is in one class, `Audio::AudioSystem` (`Client/Include/Client/Audio.hpp` and `Client/Src/Audio.cpp`).

The game has:

- a **title music**, played in the menus,
- a **stage music**, played during a game,
- a **game over music**, played once when the player dies for the last time,
- a **shot sound**.

If the computer has no audio device, every call does nothing and the game runs silently. A warning starting with `AUDIO:` is printed in the console.

## Files

The sounds are in `Assets/Sounds`, next to the client. Their names are written in `Audio.hpp`:

| What | Constant | File |
|------|----------|------|
| Title music | `TitleMusic` | `MenuScreen.mp3` |
| Stage music | `StageMusics` | `AsWetAsAFish.mp3` |
| Game over music | `GameOverMusic` | `GameOver.mp3` |
| Shot | `EffectFiles` | `Shoot.wav` |

The musics are streamed from the file while they play. The effects are loaded once at the start.

## Using it

The object is created after the window, because the audio device needs it.

| Call | When |
|------|------|
| `Init(directory)` | Once, after `InitWindow` |
| `SetMasterVolume(v)` | Every frame, from the volume option. `v` goes from 0.0 to 1.0. |
| `SetEffectsVolume(v)` | Once, to make the effects quieter than the music, if needed |
| `Update()` | Every frame. It keeps the music streaming. |
| `PlayTitle()` | In the menus. Can be called every frame: it does nothing if the title music already plays. |
| `PlayStage(n)` | Once, when the game starts |
| `PlayGameOver()` | Once, when the player dies for the last time |
| `Play(Effect::Shoot)` | When the player shoots, with a short delay between two sounds |

!!! note "Game over"
    The game over is detected from the snapshot: the player is dead and has no life left (`alive` is false and `lives` is 0).

## How it works

- Only one music is loaded at a time. Asking for another one stops the first.
- The same effect can play up to 4 times at once (`VoicesPerEffect`). The extra voices are aliases that share the data of the first one, so memory does not grow.
- Volumes are clamped between 0 and 1.
- The `::SetMasterVolume` in the code, with the two colons, is the Raylib function. The class has a method with the same name, so the colons tell the compiler which one to call.

## Rules for the sound files

- Names in PascalCase, like every file of the repository.
- Keep the level under the maximum: several sounds added together can exceed it and crackle.
- A music that loops should start and end at a quiet level, otherwise there is a small click at each loop.
- Only use sounds we have the right to use, and write where they come from.

## Accessibility

The volume option changes all the sounds. No information should exist only as a sound: see [Accessibility](../../project/accessibility.md) for what is planned.

To add a sound, see [Add a sound](../guides/add-a-sound.md).
