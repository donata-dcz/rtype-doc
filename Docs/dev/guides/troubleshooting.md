---
title: Troubleshooting
description: Common problems and what to do about them.
---

# Troubleshooting

## Build

| Symptom | Probably | What to do |
|---------|----------|------------|
| `undefined reference` when linking | The new `.cpp` is not in the `CMakeLists.txt` | Add it, then run `cmake --preset default` |
| `...hpp: No such file or directory` | The project was not configured, or you are in the wrong folder | Run `pwd`, go to the root of the repository, run `cmake --preset default` |
| `build/default is not a directory` | The configure step was never done, or the repository is in a folder inside another one (`RType/RType`) | Configure first. Check with `git ls-files \| grep '^RType/'` that no extra folder was committed. |
| Conflict markers (`<<<<<<<`) in a file | A merge was not finished | Open the file, keep the right part, remove the markers |

## Git and CI

| Symptom | What to do |
|---------|------------|
| `git merge origin/dev` says "Already up to date" | Run `git fetch origin` first, then merge again |
| The CI refuses a file name | The name has an underscore. Rename it in PascalCase with `git mv`, and update the places that use it. |
| The CI refuses a pull request to `main` | A pull request to `main` only comes from `dev` |

## Running

| Symptom | Probably | What to do |
|---------|----------|------------|
| "The server does not answer" | The server is not running, or the port is not the same | Start the server, and use the same `-p` on both sides |
| The client stops with code 84 | A wrong command-line option | Read the usage that was printed: port from 1 to 65535, `-h` with an address |
| An image is missing | Not copied next to the executable, or the name differs | Check the path and the capital letters |
| No sound at all | No audio device, or the files are missing | Look for an `AUDIO:` warning in the console |
| The sound crackles | Several sounds add up above the maximum, or the audio of the computer is slow (WSL and virtual machines often are) | Lower the volumes. Try the game on a real machine. |

!!! tip "Still stuck?"
    Ask on the team channel with the exact error message and the command you ran.
