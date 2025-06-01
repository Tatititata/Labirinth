#ifndef MODEL_H
#define MODEL_H

#define STR_SIZE 128

#include "../controller/labyrinth.h"

typedef struct {
  int row, col;
} Point;

typedef struct {
  int length;
  Point *route;
} Pass;

typedef struct {
  Point *data;
  int head, tail;
} Queue;

// labyrinth_eller
bool generate_eller_labyrinth(Labyrinth *labyrinth);
void make_vertical_walls(Labyrinth *labyrinth, int *set, int i);
void make_last_line(Labyrinth *labyrinth, int *set);
void check_horizontal_pass(Labyrinth *labyrinth, const int *set, int i, int uniques);
bool create_matrix(int ***pass, int rows, int cols);
void free_matrix(int ***matrix);
void destroy_labyrinth(Labyrinth *labyrinth);

// labyrinth_solver
void bfs(Labyrinth *labyrinth, Point end, Point start, Pass *pass);
bool queue_init(Queue *q, int size);
bool queue_empty(const Queue *q);
void enqueue(Queue *q, Point p);
Point dequeue(Queue *q);
bool can_go(Labyrinth *labyrinth, int r1, int c1, int r2, int c2);
bool empty_point(const Point *point);
bool create_prev_tmp_matrix(Point ***prev, int rows, int cols);
bool create_array(Point **ptr, int size);
void clear_pass(Labyrinth *labyrinth, Pass *pass);
void create_next_step(Labyrinth *labyrinth, Pass *pass, int i);

// cave
bool generate_cave(Labyrinth *cave, double chance);
bool solve_cave(Labyrinth *cave, int birth, int death);
void swap_matrix(int ***a, int ***b);
void proceed_alive(Labyrinth *cave, int i, int j, int death);
void proceed_dead(Labyrinth *cave, int i, int j, int birth);

// parser
bool load_matrix(FILE *f, int **matrix, int rows, int cols);
bool manual_or_automatic();
bool parse_labyrinth_file(FILE *f, Labyrinth *labyrinth, char c);
double get_chance_value();
int get_milliseconds();
void get_row_col(int *rows, int *cols, char ch);
bool get_string(char str[STR_SIZE], FILE *input);

#endif  // MODEL_H