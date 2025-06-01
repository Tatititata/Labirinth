#ifndef CONTROLLER_H
#define CONTROLLER_H
#define _POSIX_C_SOURCE 199309L

#include <termios.h>
#include <unistd.h>

#include "../model/model.h"
#include "../q_learning/learning.h"
#include "../view/ui_console.h"
#include "labyrinth.h"

int game_loop();
void proceed_labyrinth(char c);
bool create_labyrinth(Labyrinth *labyrinth, char c);
void solve_labyrinth(Labyrinth *labyrinth, char c);
void find_pass(Labyrinth *labyrinth);
void output_labyrinth(Labyrinth *labyrinth, Pass *pass);
bool load_labyrinth(Labyrinth *labyrinth, char c);
void delay_ms(int milliseconds);
void solve_and_output_cave(Labyrinth *cave);
void wait_for_any_key();
double get_chance();
int getch(void);
void q_learning_mode(Labyrinth *labyrinth);
bool q_agent_find_path(QLearningAgent *agent, Labyrinth *labyrinth, Point start,
                       Pass *pass);

#endif