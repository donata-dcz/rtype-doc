---
title: Build and run
description: Build the project with CMake and vcpkg, then run the server and the client.
---

# Build and run

The dependencies (Raylib 5.5 and Asio) come from vcpkg in manifest mode, so nothing has to be installed by hand.

```bash
cmake --preset default
cmake --build --preset default
```

The programs are in `build/default`:

```bash
./build/default/Server/r-type_server [-p <port>]
./build/default/Client/r-type_client [-h <host>] [-p <port>]
```

| Option | Program | Default | Meaning |
|--------|---------|---------|---------|
| `-p <port>` | server, client | `4242` | Port, from 1 to 65535 |
| `-h <host>` | client | `127.0.0.1` | Server address |

A wrong option prints the usage and the program exits with the code **84**, the Epitech convention for errors.

## Repository layout

| Folder | What is inside |
|--------|----------------|
| `Engine` | The game engine |
| `Common` | Code shared by the client and the server |
| `Server` | The `r-type_server` program |
| `Client` | The `r-type_client` program (Raylib) |
| `Tests` | Unit tests |
| `Assets` | Images and sounds, copied next to the client |
| `Docs` | The documentation, including this website |
| `Cmake` | CMake helpers |

!!! warning "You added a `.cpp` file?"
    Add it to the `CMakeLists.txt` of its folder, then run `cmake --preset default` again. If you forget, the code compiles but the linker says `undefined reference`.

## See also

- [Contributing](./contributing.md) for the rules of the repository.
- [Troubleshooting](../guides/troubleshooting.md) if the build fails.
