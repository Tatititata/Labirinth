#ifndef MODEL_H
#define MODEL_H

#define STR_SIZE 128

#include "../controller/maze.h"

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

// maze_eller
bool generate_eller_maze(Maze *maze);
void make_vertical_walls(Maze *maze, int *set, int i);
void make_last_line(Maze *maze, int *set);
void check_horizontal_pass(Maze *maze, const int *set, int i, int uniques);
bool create_matrix(int ***pass, int rows, int cols);
void free_matrix(int ***matrix);
void destroy_maze(Maze *maze);

// maze_solver
void bfs(Maze *maze, Point end, Point start, Pass *pass);
bool queue_init(Queue *q, int size);
bool queue_empty(const Queue *q);
void enqueue(Queue *q, Point p);
Point dequeue(Queue *q);
bool can_go(Maze *maze, int r1, int c1, int r2, int c2);
bool empty_point(const Point *point);
bool create_prev_tmp_matrix(Point ***prev, int rows, int cols);
bool create_array(Point **ptr, int size);
void clear_pass(Maze *maze, Pass *pass);
void create_next_step(Maze *maze, Pass *pass, int i);

// cave
bool generate_cave(Maze *cave, double chance);
bool solve_cave(Maze *cave, int birth, int death);
void swap_matrix(int ***a, int ***b);
void proceed_alive(Maze *cave, int i, int j, int death);
void proceed_dead(Maze *cave, int i, int j, int birth);

// parser
bool load_matrix(FILE *f, int **matrix, int rows, int cols);
bool manual_or_automatic();
bool parse_maze_file(FILE *f, Maze *maze, char c);
double get_chance_value();
int get_milliseconds();
void get_row_col(int *rows, int *cols, char ch);
bool get_string(char str[STR_SIZE], FILE *input);

#endif  // MODEL_H