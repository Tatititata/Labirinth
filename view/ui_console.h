#ifndef UI_CONSOLE_H
#define UI_CONSOLE_H

#include <sys/ioctl.h>
#include <unistd.h>

#include "../controller/labyrinth.h"

#define MAX_BUF_SIZE 320000

void draw_labyrinth_matrix(const Labyrinth* labyrinth);
void draw_labyrinth(const Labyrinth* labyrinth);
void print_chance_menu();
void print_find_pass_menu(char c);
void print_get_millseconds_menu();
void print_input_file_menu();
void print_input_string();
void print_load_or_create_menu();
void print_output_menu(char c);
void print_point_menu(int row, int col, const char* c);
void print_solve_cave_menu();
void print_start_menu();
void print_labyrinth(const Labyrinth* labyrinth);
void print_cave(const Labyrinth* cave);
void print_q_learning_menu();
void clear_screen();

#endif