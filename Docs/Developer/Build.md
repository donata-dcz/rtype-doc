# Build and run

### Requirements

- Git
- CMake 3.28+
- A C++20 compiler

Third-party libraries (Raylib, Asio) are handled by [vcpkg](https://vcpkg.io/).
It is cloned automatically by CMake on the first configuration.

### Pre-built binaries

Every [release](https://github.com/donata-dcz/rtype-doc/releases) of the project has binaries for Linux and Windows, so you can play without building. To work on the code, build it as below.

### Build

Run both commands from the root of the repository:

```bash
git clone https://github.com/alexis801/RType.git
cd RType
cmake --preset default
cmake --build --preset default
```

### Server

Launch the server first with this command on Linux:

```bash
./build/default/Server/r-type_server [-p <port>]
```

| Option | Description | Default |
|--------|-------------|---------|
| `-p <port>` | UDP port the server listens on (1-65535) | `4242` |

### Client

Launch the client with this command on Linux:

```bash
./build/default/Client/r-type_client [-h <host>] [-p <port>]
```

| Option | Description | Default |
|--------|-------------|---------|
| `-h <host>` | Address of the server: IPv4, IPv6 or host name | `127.0.0.1` |
| `-p <port>` | UDP port of the server (1-65535) | `4242` |

The **Play** button of the menu connects to this server. The menu does not ask for an address, a port or a name.

If the server does not use the default port, give the same port to both programs:

```bash
./build/default/Server/r-type_server -p 2525
./build/default/Client/r-type_client -p 2525
```

## Controls

| Action | Effect |
|--------|--------|
| Up, down, left, right | Move the ship |
| Shoot | Fire a missile |

In the menu, use the arrow keys, Enter and Esc. Esc goes back to the menu, also during a game. It never closes the window: use **Quit** or the close button.
