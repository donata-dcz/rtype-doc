# Build and run

## What you need

- A C++20 compiler (GCC or Clang) and CMake 3.28 or newer.
- Git.
- [vcpkg](https://github.com/microsoft/vcpkg), in the `vcpkg` folder at the root of the repository: the CMake presets use its toolchain file.
- On Linux, the system libraries Raylib needs to open a window and play sound (X11 and ALSA development packages). If the build stops on a missing library, vcpkg prints which one.

The dependencies (Raylib and Asio) are listed in `vcpkg.json` and installed by vcpkg in manifest mode when you configure, so you do not install them by hand.

## Build

Run both commands from the root of the repository:

```bash
cmake --preset default
cmake --build --preset default
```

The first command downloads the dependencies, so it can be long the first time. The programs are created in `build/default`.

!!! warning "You added a `.cpp` file?"
    Add it to the `CMakeLists.txt` of its folder, then run `cmake --preset default` again. If you forget, the code compiles but the linker says `undefined reference`.

## Run

Start the server first, then one client for each player:

```bash
./build/default/Server/r-type_server [-p <port>]
./build/default/Client/r-type_client [-h <host>] [-p <port>]
```

| Option | Program | Default | Meaning |
|--------|---------|---------|---------|
| `-p <port>` | server, client | `4242` | UDP port, from 1 to 65535 |
| `-h <host>` | client | `127.0.0.1` | Address of the server |

On the same computer, nothing else is needed. To play over a network, give the address of the server to the clients:

```bash
./build/default/Server/r-type_server -p 2525
./build/default/Client/r-type_client -h 192.168.1.20 -p 2525
```

A wrong option prints the usage and the program exits with the code **84**, the Epitech convention for errors.

In the client, **Play** connects to the server given on the command line. The menu has no field to type: the address and the port come only from `-h` and `-p`.

## Controls

| Action | Effect |
|--------|--------|
| Up, down, left, right | Move the ship |
| Shoot | Fire a missile |

In the menu, use the arrow keys, Enter and Esc. Esc goes back to the menu, also during a game. It never closes the window: use **Quit** or the close button.

## See also

- [Contributing](contributing.md) for the rules of the repository.
- [Troubleshooting](guides.md#troubleshooting) if something does not work.
