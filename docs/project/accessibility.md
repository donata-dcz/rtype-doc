---
title: Accessibility
description: What is accessible in R-Type, and what is planned.
---

# Accessibility

We want R-Type to be playable by as many people as possible. This page says what we did so far and what we plan to do.

## What we did

| Category | Feature | Details |
|----------|---------|---------|
| Motor | Keyboard menu | Up/Down to move, Enter to select, Left/Right to change a value, Esc to go back. No mouse needed. |
| Motor | Mouse menu | Hovering a row selects it, a click validates it. Keyboard and mouse can be mixed. |
| Motor | Held keys repeat | Nobody has to tap a key many times in the menu. |
| Motor | Nothing to type | **Play** connects directly. The server address and port come from the command line (`-h`, `-p`). |
| Motor | Safe Esc | Esc only goes back. The window is closed with **Quit** or its close button. |
| Visual | Resizable window | The menu adapts to the new size. |
| Visual | Text size | **Options > Text size**: 125 % or 150 %. |
| Visual | Readable text | The lowest contrast we measured is 5.6:1 on black (WCAG AA asks for 4.5:1). |
| Visual | Selection not only by color | The selected row has a `>` marker and a filled background. |
| Visual | Color-blind-safe accents | Sky blue and vermillion, from the Okabe-Ito palette. |
| Audio | Volume | **Options > Volume**, from 0 % to 100 %, for all sounds. |
| Audio | Sound design | Title music, stage music, game over music and shot sound. The game runs without sound if there is no audio device. |
| Cognitive | Short menu | Three entries per screen, no deep navigation. |
| Cognitive | Same keys everywhere | A help line at the bottom of each screen reminds the keys. |
| Cognitive | Plain messages | "The server does not answer", "Rejected: server is full", "Connection Lost". |
| Cognitive | Easy way back | Esc returns to the menu, also from the game. If the connection is lost, the player goes back to the menu with an explanation. |
| Documentation | Plain Markdown | Readable with a screen reader or any text browser. |
| Documentation | Command-line options | `-h` and `-p` are explained in the README. |
| Documentation | Documentation website | Navigation, search, dark and light themes, a visible keyboard focus, and a text description for every diagram. |

## What we plan

| Category | Feature | Details |
|----------|---------|---------|
| Motor | Remappable keys | Also WASD and ZQSD layouts. This is the main gap today. |
| Motor | Gamepad | Raylib can already handle it. |
| Motor | Auto-fire or toggle shooting | So nobody has to hold or hammer a key. |
| Motor | Assist options | Slower game, more lives. |
| Visual | High-contrast mode | Pure white on black, thicker outlines. |
| Visual | Reduced motion | Freezes the scrolling star-field. |
| Visual | Players told apart without color | A number or a shape on each ship. |
| Audio | Visual cue for every important sound | Hit, death, game over. We have not checked yet that a deaf player gets the same information from the screen. |
| Audio | Separate music and effects volumes | To keep the effects and lower the music. |
| Cognitive | Controls reminder | Shown before the first game. |
| Cognitive | No flashing effects | No more than three flashes per second (WCAG 2.3.1). |
| Cognitive | Easier difficulty | Fewer enemies and slower missiles. |

## Known limitations

The game itself has no screen reader support: it is drawn by Raylib, so only the documentation can be read by a screen reader.
