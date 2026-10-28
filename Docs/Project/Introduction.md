# R-Type

R-Type is a remake of the classic horizontal shoot'em up, played together over the network. Up to four players fly their own ship, each one on their own computer, and fight waves of enemies on the same screen.

## How the game works

- A slow horizontal scrolling star-field in the background.
- Up to 4 players, each one with a different ship.
- Move with the arrow keys and shoot missiles.
- Waves of monsters come from the right of the screen.
- The game is played on a **server**: it decides everything, and every player sees the same world. If one client crashes, the game keeps running for the others.

## What is in the project

| Part | Role |
|------|------|
| **Server** (`r-type_server`) | Runs the game and sends the state to every player |
| **Client** (`r-type_client`) | Shows the game, plays the sound and sends your keys |
| **Engine** | A small game engine, reusable for other games (a Pacman is included as an example) |

## Where to go next

| I want to... | Read |
|--------------|------|
| Get the game | [Download](Download/Client.md) |
| Start a game | [Launch the game](Launch.md) |
| Understand why we chose Raylib, Asio and UDP | [Technical choices](TechnicalChoices.md) |
| Work on the code | [Developer documentation](../Developer/Introduction.md) |
