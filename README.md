# Labyrinth Generator and Solver

![Logo](labyrinth.svg)

## 📘 Project Description

This project is a console application for generating and visualizing labyrinths, as well as modeling and solving "cave-like" environments with the ability to find the shortest path.

---

## 🔧 Features


- Labyrinth generation (maximum size: 50×50):
  
  🔲 **Labyrinth generation** using Eller’s algorithm\
  🧱 **Cave structure generation** with customizable fill probability

- Labyrinth solving:
  
  🔁 **Pathfinding between two points** using the BFS algorithm\
  🧠 **Reinforcement learning mode (Q-learning)** for finding the optimal path: training an agent and solving with the Q-table\
  📂 **Loading labyrinths/caves from files** — supports `.txt` format with structure description\
  🖥️ **Console visualization** — adapts to current terminal size\
  🕹️ **Manual and automatic modes** — step-by-step or full path display\
  📋 **CLI control menu** — easy navigation of actions and modes\
  🧱 **MVC architecture** — separation of model, view, and controller\
  ✅ **Unit tests and coverage** — automatic logic testing via `make test`, `gcov`\
  🔍 **Static analysis** — using `cppcheck`, `valgrind` for code verification

---

## 📁 Project Structure

```text
src/
├── controller/       # User input handling
├── model/            # Labyrinth logic, generation and solving algorithms
├── view/             # Display, CLI interface
├── q_learning/       # Q-Learning algorithms
├── tests/            # Unit tests
├── Makefile          # Build and project management
└── main.c            # Entry point
```

## ▶️ Installation and Launch

- ### 🧱 Dependencies

  GCC\
  Make\
 (Optional) valgrind, gcov, cppcheck

- ### 🛠️ Build

```bash
make
```

- ### 🚀 Run

```bash
./labyrinth
```


## 📜 Usage Examples

After launching the program, you'll see the start menu:

```bash
To create a labyrinth press 'm' button. To create a cave press 'c' button. To quit press 'q' button.
```

### Main Modes:

- **Labyrinth (m)**
  
Generate a random labyrinth: specify size (row, column)\
Load labyrinth from file\
Pathfinding between two points: specify start and end points\
Q-learning mode for training an agent to navigate

- **Cave (c)**

Generate a random cave with a given fill probability: specify size and probability\
Load cave from file\
Automatic cave evolution using cellular automata rules: specify "birth" and "death" parameters\
Watch the solution in the console.

### Controls

The program uses an interactive menu with hints. Main control keys:

`m` - labyrinth mode\
`c` - cave mode\
`f` - load from file\
`q` - return to previous menu

### Example Commands

Choose mode (m — labyrinth, c — cave): m \
Create (c) or load (f): c\
Enter size: 10 10\
Choose pathfinding mode (c — BFS, l — Q-Learning): c\
Enter start point: 1 1\
Enter end point: 10 10\
(The program will show and visualize the path in the console.)\

### 📄 File Format

Files must contain:

First line — size (rows and columns)\
Then — vertical wall matrix\
For labyrinths — horizontal wall matrix

Example:

```
3 3
0 1 0
1 0 1
0 1 0

1 0 1
0 1 0
1 0 1
```

## 🧪 Testing and Analysis

Run tests: `make tests`\
Check code style: `make cl`\
Memory leak check: `make valgrind`\
Coverage report generation: `make gcov_report`\
Static code analysis: `make cpp`

## 🧰 Requirements

GCC compiler\
C11 standard\
Unix-like system (for terminal compatibility)

