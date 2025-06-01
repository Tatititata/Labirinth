#include "model.h"

bool generate_eller_labyrinth(Labyrinth* labyrinth) {
  bool result = create_matrix(&labyrinth->v_w, labyrinth->rows, labyrinth->cols);
  result = result && create_matrix(&labyrinth->h_w, labyrinth->rows, labyrinth->cols);
  result = result && create_matrix(&labyrinth->pass, labyrinth->rows, labyrinth->cols);
  int* set = (int*)malloc(sizeof(int) * labyrinth->cols);

  if (set && result) {
    int uniques = 1;

    for (int j = 0; j < labyrinth->cols; ++j) {
      set[j] = uniques++;
    }

    for (int i = 0; i < labyrinth->rows - 1; ++i) {
      make_vertical_walls(labyrinth, set, i);

      for (int j = 0; j < labyrinth->cols; ++j) {
        labyrinth->h_w[i][j] = rand() % 2;
      }

      check_horizontal_pass(labyrinth, set, i, uniques);

      for (int j = 0; j < labyrinth->cols; ++j) {
        if (labyrinth->h_w[i][j]) {
          set[j] = uniques++;
        }
      }
    }
    make_last_line(labyrinth, set);
  }
  if (set) {
    free(set);
    set = NULL;
  }
  if (!result) {
    destroy_labyrinth(labyrinth);
  }
  return result;
}

void make_vertical_walls(Labyrinth* labyrinth, int* set, int i) {
  for (int j = 0; j < labyrinth->cols - 1; ++j) {
    if (set[j] == set[j + 1])
      labyrinth->v_w[i][j] = 1;
    else {
      labyrinth->v_w[i][j] = rand() % 2;
    }
    if (!labyrinth->v_w[i][j]) {
      int changing_set = set[j + 1];
      for (int k = 0; k < labyrinth->cols; ++k) {
        if (set[k] == changing_set) set[k] = set[j];
      }
    }
  }
  labyrinth->v_w[i][labyrinth->cols - 1] = 1;
}

void make_last_line(Labyrinth* labyrinth, int* set) {
  make_vertical_walls(labyrinth, set, labyrinth->rows - 1);
  for (int j = 0; j < labyrinth->cols - 1; ++j) {
    if (set[j] != set[j + 1]) {
      labyrinth->v_w[labyrinth->rows - 1][j] = 0;
      int changing_set = set[j + 1];
      for (int k = 0; k < labyrinth->cols; ++k) {
        if (set[k] == changing_set) set[k] = set[j];
      }
    }
    labyrinth->h_w[labyrinth->rows - 1][j] = 1;
  }
  labyrinth->h_w[labyrinth->rows - 1][labyrinth->cols - 1] = 1;
}

void check_horizontal_pass(Labyrinth* labyrinth, const int* set, int i, int uniques) {
  for (int s = 1; s < uniques; ++s) {
    int cells[MAX_SIZE] = {0};
    int count = 0;

    for (int j = 0; j < labyrinth->cols; ++j)
      if (set[j] == s) {
        cells[count++] = j;
      }
    if (count) {
      bool pass_found = false;
      for (int k = 0; k < count && !pass_found; ++k) {
        if (!labyrinth->h_w[i][cells[k]]) pass_found = true;
      }
      if (!pass_found) labyrinth->h_w[i][cells[rand() % count]] = 0;
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

void destroy_labyrinth(Labyrinth* labyrinth) {
  if (labyrinth) {
    free_matrix(&labyrinth->v_w);
    free_matrix(&labyrinth->h_w);
    free_matrix(&labyrinth->pass);
  }
}
