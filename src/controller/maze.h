#ifndef MAZE_H
#define MAZE_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SIZE (int)50

typedef struct {
  int rows, cols;
  int **v_w;   // vertical walls
  int **h_w;   // horizontal walls
  int **pass;  // pass
} Maze;

#endif