# R-Type documentation

This site is the documentation for people who work on the R-Type project: a networked shoot'em up with a graphical client and an authoritative server.

If you are new, read the pages in this order:

| Page | What you will find |
|------|--------------------|
| [About](project/index.md) | What R-Type is and how the project is made |
| [Play](project/play.md) | Build the game, start a server and connect clients |
| [Build and run](dev/get-started/build-and-run.md) | Build the project, start the server and the client |
| [Contributing](dev/get-started/contributing.md) | Code style, file names, Git rules, CI |
| [Architecture](dev/architecture/overview.md) | The big picture: folders, programs, how a frame goes |
| [Client](dev/architecture/client.md) | The client in detail: menu, connection, assets |
| [Audio](dev/architecture/audio.md) | The music and the sound effects |
| [Network protocol](dev/networking/protocol.md) | What the client and the server send each other |
| [Accessibility](project/accessibility.md) | What is accessible in the game, and what is planned |
| [Guides](dev/guides/add-a-sound.md) | Step by step: add a sound, an image, a message, a menu option |
| [Troubleshooting](dev/guides/troubleshooting.md) | A symptom, and where to look |

## Quick start

```bash
cmake --preset default
cmake --build --preset default
./build/default/Server/r-type_server
./build/default/Client/r-type_client
```

See [Build and run](dev/get-started/build-and-run.md) for the requirements and the options.

## Using this site

- Use the menu on the left to move between the pages, and the table of contents on the right to jump inside a page.
- Use the search bar at the top to find a word in all the pages.
- The button in the header switches between the light and the dark theme.
- Every code block has a button to copy it.
