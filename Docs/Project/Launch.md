# Launch the game

Start the server first, then one client per player (up to 4).

## 1. Start the server

On Linux:

```bash
./r-type_server
```

On Windows:

Launch the file `r-type_server.exe`.

The server listens on UDP port `4242`. Use `-p <port>` to choose another one. It prints one line every second, for example `server : 60 ticks/s, 12 entities, 2 players, out 3.52 KB/s (0 full + 60 delta snapshots), in 0.82 KB/s`. `ticks/s` should be 60.

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

Choose **Play** in the menu: the client connects using the command-line
host and port. The menu also contains **Options** for volume and text size.

| Key | Action |
|-----|--------|
| Arrow keys | Move the ship |
| Space | Shoot |
| Esc | Back to the menu (also during a game) |

With a gamepad:

| Button | Action |
|--------|--------|
| D-pad or either stick | Move the ship |
| Face buttons or triggers | Shoot |
| Start or Select | Back to the menu (also during a game) |

The client predicts the local ship movement immediately, then reconciles it
with the authoritative server state. Other players and entities continue to
use interpolated snapshots.

The HUD shows crew score/lives and a boss health bar. On game over, a
translucent black overlay is drawn over the game. The server loads the four
JSON levels in `Assets/Levels` and advances automatically after each boss.
The client plays dedicated boss music while a boss is present and plays a
non-looping boss-defeat track when the boss disappears from the client
snapshot, then resumes the stage music when that track ends.

The server supports multiple independent rooms through its lobby. A player
joining through `CONNECT` is assigned to the first room with an available
slot; if all rooms are full, the server creates another room. The client can
request the room list with `ListRoomsRequest`, although room creation and
selection are not currently exposed in the graphical menu.
