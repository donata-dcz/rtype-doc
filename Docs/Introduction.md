# R-Type

R-Type is a networked remake of the classic side-scrolling shoot'em up. Several players fight the enemy together, each one on its own client, connected to a central authoritative server. It is built on a custom C++ game engine, and developed by a team of students at Epitech.

## Play now

1. **Download the game** for your platform. R-Type is available for Windows (x64) and Linux (x64).
    Every player needs the **client**. One computer also runs the **server**, and every client connects to it. See the [client](Download/Client.md) and [server](Download/Server.md) pages for details.

2. **Start the server, then the clients.** The steps are in [Launch the game](Launch.md).

3. **Play and enjoy.** Find out more about the choices behind the game in the [technical choices](TechnicalChoices.md).

## How it works

R-Type is made of two independent programs that communicate over a binary UDP protocol:

| Program | Language | Role |
|---------|----------|------|
| `r-type_server` | C++ (Asio) | Runs the game logic and has the final authority on the game world |
| `r-type_client` | C++ (Raylib) | Displays the game and sends the player inputs |

## Want to contribute?

The [developer documentation](../Developer/Introduction.md) explains how to build the project, its architecture, the network protocol, and how to add a new game.

## Authors

- [Adrien Grégoire](https://github.com/AdrienGregoire)
- [Alexis Bezzaouya Lemonnier](https://github.com/alexis801)
- [Donata Contant](https://github.com/donata-dcz)
- [Kylian Gicquel](https://github.com/kyliangi)
