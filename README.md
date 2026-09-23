# Minesweeper_game
Minesweeper is an animated, terminal-based C++ game that brings the classic puzzle to life. It features dynamic difficulty, color-coded UI, and recursive board clearing. With real-time animations for game states and a "safe start" algorithm to ensure the first move is valid, it provides a polished and engaging terminal experience.
# 💣 Terminal Minesweeper

A modern, animated, and feature-rich terminal-based implementation of the classic Minesweeper puzzle game, developed in C++.
 
## 🎮 Overview 
Test your logic and intuition! Clear the minefield by revealing safe tiles while avoiding hidden mines. This project brings a classic game to life in your terminal with smooth animations, color-coded numbers, and a responsive, cross-platform interface.
 
---   
 
## ⚡ Key Features
* **🎯 Multiple Difficulty Levels:** Choose between Beginner (9x9), Intermediate (16x16), and Advanced (24x24) grids.
* **✨ Visual Polish:** Real-time terminal animations for loading sequences, explosions, and victory celebrations.
* **🌈 Color-Coded UI:** Utilizes ANSI color codes to distinguish numbers, flags, and mines for better readability.
* **🛡️ Safe First Move:** Intelligent algorithm ensures your first click is always safe and reveals a wider area.
* **⚙️ Cross-Platform:** Engineered to support both Windows (via `system("cls")` and `Sleep`) and Unix-based systems (via `system("clear")` and `usleep`).
* **🧩 Advanced Logic:** Implements recursive flood-fill algorithms to efficiently clear empty zones.

---

## 🚀 How to Play
1. **Reveal (r):** Enter row and column coordinates followed by `r` to uncover a tile.
2. **Flag (f):** Enter coordinates followed by `f` to mark a potential mine.
3. **Win Condition:** Uncover every tile that does not contain a mine.
4. **Game Over:** Stepping on a mine triggers an explosion animation and ends the round.

---

## 🛠️ Getting Started
### Prerequisites
* A C++ compiler.

### Compilation and Running

# Compile the source:

g++ -o minesweeper main.cpp

# Run the executable:
 
Windows: minesweeper.exe
Linux/macOS: ./minesweeper

# 🏗️ Technical Implementation

The project is structured to prioritize clean code and modularity:

Board Management: Uses two distinct 2D arrays—one for the game logic (mines) and one for the player's view (revealed tiles).

Recursive Reveal: Employs a depth-first search approach to handle the cascading revealing of empty cells.

Input Validation: Robust handling of user input to ensure the game remains stable during invalid coordinates.

Animation System: Custom functions interface with OS-specific timing libraries to create smooth visual feedback.

#📜 License
This project is open-source and available under the MIT License. Feel free to fork, modify, and improve the game!

Made with ❤️ in C++.
