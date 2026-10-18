# Build and run

## Build

The dependencies (Raylib 5.5 and Asio) come from vcpkg in manifest mode, so nothing has to be installed by hand. Only a C++ compiler, CMake and the usual platform libraries (OpenGL, X11 on Linux) are needed.

```bash
cmake --preset default
cmake --build --preset default
```

The project builds on Linux and Windows.

## Run

The executables are in `build/default`:

```bash
./build/default/Server/r-type_server [-p <port>]
./build/default/Client/r-type_client [-h <host>] [-p <port>]
```

| Option | Program | Default | Meaning |
|--------|---------|---------|---------|
| `-p <port>` | server, client | `4242` | Port, from 1 to 65535 |
| `-h <host>` | client | `127.0.0.1` | Server address |

A wrong option prints the usage, and the program exits with the code **84** (the Epitech convention for errors).

Start the server first, then one client per player (up to 4).

## Add a source file

Add the `.cpp` file to the `CMakeLists.txt` of its folder. If you forget it, the code compiles but the linker fails with `undefined reference`.

## Assets

The `Assets` folder is copied next to the client executable at build time. If an image or a sound is missing in the game, check that it was copied and that its name is written exactly the same, with the same capital letters.
