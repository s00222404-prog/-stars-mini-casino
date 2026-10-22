# STAR PLAY — C++ mini-game prototype

A C++17 desktop game prototype built with Raylib. The interface uses glass-style cards, animated glow effects, floating particles, hover states, and a result modal.

## Mini-games

- **Hoop Hero** — basketball challenge; successful rounds award demo points.
- **Strike Zone** — bowling challenge; successful rounds award demo points.
- **Lucky Dice** — roll a die and guess even/odd.
- **Daily bonus** — claim +25 demo points once per app session.
- **Round result modal** — animated result window and a cosmetic teddy-bear illustration.

## Important scope

This is a **free demo prototype**. Points are local, temporary game points with no cash value. The project does not charge Telegram Stars, process deposits, or deliver real-world prizes. Telegram integration, persistent accounts, a database, and deployment are not included yet.

## Project layout

```text
.
├── .github/workflows/build.yml   # Linux build check
├── src/main.cpp                  # UI and game logic
├── CMakeLists.txt                # CMake build setup
├── .gitignore
└── README.md
```

## Requirements

- C++17 compiler (GCC, Clang, or MSVC)
- Raylib 5.x
- CMake 3.16+

### Linux (Debian/Ubuntu)

Install the tools and Raylib development package:

```bash
sudo apt update
sudo apt install -y build-essential cmake libraylib-dev
```

Build and run:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
./build/star-play
```

### Windows

Install a C++ toolchain and Raylib 5.x, then configure CMake with the toolchain/environment where Raylib's CMake package is discoverable:

```powershell
cmake -S . -B build
cmake --build build --config Release
```

Run the generated `star-play.exe` from the build output directory.

## Controls

- Click a game card to select it.
- Click **PLAY ROUND** to start a demo round.
- Click **DAILY BONUS** to claim the one-time session bonus.
- Press **R** to reset the demo points to 250.
- Press **Esc** to close the result window, or exit when the result window is closed.

## Current limitations

- Scores reset when the application closes.
- No network/server, Telegram bot, Telegram Mini App, payments, or real prize delivery.
- The GitHub Actions workflow attempts a Linux configure/build check on pushes and pull requests to `main`.
