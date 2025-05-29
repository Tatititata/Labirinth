#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../model/model.h"
#include "../q_learning/learning.h"
#include "../view/ui_console.h"
#include "maze.h"

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>

int getch(void);
#endif

int game_loop();
void proceed_maze(char c);
bool create_maze(Maze *maze, char c);
void solve_maze(Maze *maze, char c);
void find_pass(Maze *maze);
void output_maze(Maze *maze, Pass *pass);
bool load_maze(Maze *maze, char c);
void delay_ms(int milliseconds);
void solve_and_output_cave(Maze *maze);
void wait_for_any_key();
double get_chance();

void q_learning_mode(Maze *maze);
bool q_agent_find_path(QLearningAgent *agent, Maze *maze, Point start,
                       Pass *pass);

#endif