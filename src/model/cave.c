#include "model.h"

bool generate_cave(Maze *cave, double chance) {
  bool result = create_matrix(&cave->v_w, cave->rows, cave->cols);
  result = result && create_matrix(&cave->h_w, cave->rows, cave->cols);

  if (result) {
    for (int i = 0; i < cave->rows; ++i)
      for (int j = 0; j < cave->cols; ++j) {
        if (rand() / (double)RAND_MAX < chance)
          cave->v_w[i][j] = 1;
        else
          cave->v_w[i][j] = 0;
      }
  } else {
    destroy_maze(cave);
  }
  return result;
}

bool solve_cave(Maze *cave, int birth, int death) {
  bool result = true;
  for (int i = 0; i < cave->rows; ++i) {
    for (int j = 0; j < cave->cols; ++j) {
      if (cave->v_w[i][j]) {
        proceed_alive(cave, i, j, death);
      } else {
        proceed_dead(cave, i, j, birth);
      }
      result = result && (cave->v_w[i][j] == cave->h_w[i][j]);
    }
  }
  swap_matrix(&cave->v_w, &cave->h_w);
  return result;
}

void proceed_alive(Maze *cave, int i, int j, int death) {
  int sum = -1;
  for (int di = -1; di < 2; ++di) {
    int new_i = i + di;
    for (int dj = -1; dj < 2; ++dj) {
      int new_j = j + dj;
      if (new_i < 0 || new_i >= cave->rows || new_j < 0 || new_j >= cave->cols)
        sum++;
      else {
        sum += cave->v_w[new_i][new_j];
      }
    }
  }
  cave->h_w[i][j] = !(sum < death);
}

void proceed_dead(Maze *cave, int i, int j, int birth) {
  int sum = 0;
  for (int di = -1; di < 2; ++di) {
    int new_i = i + di;
    for (int dj = -1; dj < 2; ++dj) {
      int new_j = j + dj;
      if (new_i < 0 || new_i >= cave->rows || new_j < 0 || new_j >= cave->cols)
        sum++;
      else {
        sum += cave->v_w[new_i][new_j];
      }
    }
  }
  cave->h_w[i][j] = (sum > birth);
}

void swap_matrix(int ***a, int ***b) {
  int **tmp = *a;
  *a = *b;
  *b = tmp;
}
