#include "model.h"

bool generate_eller_maze(Maze* maze) {
  bool result = create_matrix(&maze->v_w, maze->rows, maze->cols);
  result = result && create_matrix(&maze->h_w, maze->rows, maze->cols);
  result = result && create_matrix(&maze->pass, maze->rows, maze->cols);
  int* set = (int*)malloc(sizeof(int) * maze->cols);

  if (set && result) {
    int uniques = 1;

    for (int j = 0; j < maze->cols; ++j) {
      set[j] = uniques++;
    }

    for (int i = 0; i < maze->rows - 1; ++i) {
      make_vertical_walls(maze, set, i);

      for (int j = 0; j < maze->cols; ++j) {
        maze->h_w[i][j] = rand() % 2;
      }

      check_horizontal_pass(maze, set, i, uniques);

      for (int j = 0; j < maze->cols; ++j) {
        if (maze->h_w[i][j]) {
          set[j] = uniques++;
        }
      }
    }
    make_last_line(maze, set);
  }
  if (set) {
    free(set);
    set = NULL;
  }
  if (!result) {
    destroy_maze(maze);
  }
  return result;
}

void make_vertical_walls(Maze* maze, int* set, int i) {
  for (int j = 0; j < maze->cols - 1; ++j) {
    if (set[j] == set[j + 1])
      maze->v_w[i][j] = 1;
    else {
      maze->v_w[i][j] = rand() % 2;
    }
    if (!maze->v_w[i][j]) {
      int changing_set = set[j + 1];
      for (int k = 0; k < maze->cols; ++k) {
        if (set[k] == changing_set) set[k] = set[j];
      }
    }
  }
  maze->v_w[i][maze->cols - 1] = 1;
}

void make_last_line(Maze* maze, int* set) {
  make_vertical_walls(maze, set, maze->rows - 1);
  for (int j = 0; j < maze->cols - 1; ++j) {
    if (set[j] != set[j + 1]) {
      maze->v_w[maze->rows - 1][j] = 0;
      int changing_set = set[j + 1];
      for (int k = 0; k < maze->cols; ++k) {
        if (set[k] == changing_set) set[k] = set[j];
      }
    }
    maze->h_w[maze->rows - 1][j] = 1;
  }
  maze->h_w[maze->rows - 1][maze->cols - 1] = 1;
}

void check_horizontal_pass(Maze* maze, const int* set, int i, int uniques) {
  for (int s = 1; s < uniques; ++s) {
    int cells[MAX_SIZE] = {0};
    int count = 0;

    for (int j = 0; j < maze->cols; ++j)
      if (set[j] == s) {
        cells[count++] = j;
      }
    if (count) {
      bool pass_found = false;
      for (int k = 0; k < count && !pass_found; ++k) {
        if (!maze->h_w[i][cells[k]]) pass_found = true;
      }
      if (!pass_found) maze->h_w[i][cells[rand() % count]] = 0;
    }
  }
}

bool create_matrix(int*** pass, int rows, int cols) {
  bool result = false;
  *pass = (int**)malloc(rows * sizeof(int*) + rows * cols * sizeof(int));
  if ((*pass)) {
    result = true;
    int* ptr = (int*)((*pass) + rows);
    for (int i = 0; i < rows; ++i) {
      (*pass)[i] = ptr + i * cols;
    }
    memset(ptr, 0, rows * cols * sizeof(int));
  }
  return result;
}

void free_matrix(int*** matrix) {
  if (*matrix) {
    free(*matrix);
    *matrix = NULL;
  }
}

void destroy_maze(Maze* maze) {
  if (maze) {
    free_matrix(&maze->v_w);
    free_matrix(&maze->h_w);
    free_matrix(&maze->pass);
  }
}
