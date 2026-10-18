---
title: Add a sound
description: Add a sound effect or a music to the client.
---

# Add a sound

## A sound effect

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

## A stage music

Add the file to `StageMusics` in `Audio.hpp`, and put it in `Assets/Sounds`. The first stage uses the first file. A stage after the last one keeps the last music.

```cpp
constexpr std::array<const char *, 2> StageMusics = {"AsWetAsAFish.mp3", "SecondStage.mp3"};
```

Then call `audio.PlayStage(n)` when stage `n` starts. Do not call it at every frame: a music that already plays is not restarted, but the game over music would be replaced.

## If the sound does not play

- Look in the console for a warning starting with `AUDIO:`. It prints the path that could not be loaded.
- Check that `Assets/Sounds` was copied next to the executable, and that the name is spelled the same, capital letters included.
- If it crackles, lower the volume: sounds added together can exceed the maximum level.
