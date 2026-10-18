# R-Type documentation

This site is the documentation for people who work on the R-Type project: a networked shoot'em up with a graphical client and an authoritative server.

If you are new, read the pages in this order:

| Page | What you will find |
|------|--------------------|
| [Build and run](build.md) | Build the project, start the server and the client |
| [Contributing](contributing.md) | Code style, file names, Git rules, CI |
| [Architecture](architecture.md) | The big picture: folders, programs, how a frame goes |
| [Client](client.md) | The client in detail: menu, connection, sound, assets |
| [Network protocol](protocol.md) | What the client and the server send each other |
| [How-to guides](how-to.md) | Step by step: add a sound, a texture, a menu option |
| [Troubleshooting](troubleshooting.md) | A symptom, and where to look |

## Quick start

```bash
cmake --preset default
cmake --build --preset default
./build/default/Server/r-type_server
./build/default/Client/r-type_client
```

Nothing has to be installed by hand: the dependencies come from vcpkg.

## Using this site

- The search bar is at the top of every page. Press `/` or `s` to jump to it.
- The site follows the colors of your system (light or dark). The button in the header changes it.
- Every page can be read with the keyboard only, and every diagram comes with a text description.
