# Launch the game

Start the server first, then one client per player (up to 4).

## 1. Start the server

On Linux:

```bash
./r-type_server
```

On Windows:

Launch the file `r-type_server.exe`.

The server listens on UDP port `4242`. Use `-p <port>` to choose another one. It prints one line every second, for example `server : 60 ticks/s, 3 entities, 1 players`.

## 2. Start a client

On the same computer as the server:

```bash
./r-type_client
```

On another computer, give the address of the server:

```bash
./r-type_client -h 192.168.1.20 -p 4242
```

| Option | Description | Default |
|--------|-------------|---------|
| `-h <host>` | Address of the server: IPv4, IPv6 or host name | `127.0.0.1` |
| `-p <port>` | UDP port of the server (1-65535) | `4242` |

If the server uses another port, give the same one to the client.

## 3. Play

Choose **Play** in the menu: the client connects to the server. The menu does not ask for an address, a port or a name.

| Key | Action |
|-----|--------|
| Arrow keys | Move the ship |
| Space | Shoot |
| Esc | Back to the menu (also during a game) |

In the menu, use the arrow keys, Enter and Esc. **Options** changes the volume and the text size. To close the game, use **Quit** or the window close button.
