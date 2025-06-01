#include "model.h"

bool get_string(char str[STR_SIZE], FILE *input) {
  memset(str, 0, STR_SIZE);
  char *result = fgets(str, STR_SIZE, input);
  // fgets(str, STR_SIZE, input);
  if (result) {
    char *ptr = strrchr(str, '\n');
    if (ptr) *ptr = 0;
  }
  return result;
}

void get_row_col(int *rows, int *cols, char ch) {
  int max = MAX_SIZE;
  int min = 1;
  if (ch == 'c') {
    max = 7;
    min = 0;
  }
  char str[STR_SIZE] = {0};
  get_string(str, stdin);
  char *ptr = str;
  double r = 0, c = 0;
  r = strtod(str, &ptr);
  c = strtod(ptr, &ptr);
  while (*(ptr++) == ' ');

  if (strlen(ptr) != 0 || r < min || c < min || r > max || c > max) {
    *rows = *cols = -1;

  } else {
    *rows = (int)r;
    *cols = (int)c;
  }
}

bool parse_labyrinth_file(FILE *f, Labyrinth *labyrinth, char c) {
  bool result = false;
  char str[STR_SIZE] = {0};
  get_string(str, f);
  while (!strlen(str)) get_string(str, f);
  if (sscanf(str, "%d %d", &labyrinth->rows, &labyrinth->cols) == 2) {
    result = create_matrix(&labyrinth->v_w, labyrinth->rows, labyrinth->cols);
    result = result && create_matrix(&labyrinth->h_w, labyrinth->rows, labyrinth->cols);
    if (result) {
      result = load_matrix(f, labyrinth->v_w, labyrinth->rows, labyrinth->cols);
      if (c == 'm') {
        result = result && create_matrix(&labyrinth->pass, labyrinth->rows, labyrinth->cols);
        result = result && load_matrix(f, labyrinth->h_w, labyrinth->rows, labyrinth->cols);
      }
    }
  }
  if (!result) {
    destroy_labyrinth(labyrinth);
  }
  return result;
}

bool load_matrix(FILE *f, int **matrix, int rows, int cols) {
  bool result = true;
  char str[STR_SIZE] = {0};
  int i = 0;

  while (i < rows && get_string(str, f)) {
    if (strlen(str)) {
      const char *ptr = strtok(str, " ");
      for (int j = 0; j < cols && result; ++j) {
        if (!ptr)
          result = false;
        else if (!strcmp(ptr, "0"))
          matrix[i][j] = 0;
        else if (!strcmp(ptr, "1"))
          matrix[i][j] = 1;
        else
          result = false;
        ptr = strtok(NULL, " ");
      }
      ++i;
    }
  }
  return result;
}

double get_chance_value() {
  char str[STR_SIZE] = {0};
  get_string(str, stdin);
  return strtod(str, NULL);
}

bool manual_or_automatic() {
  bool result = false;
  char str[STR_SIZE] = {0};
  get_string(str, stdin);
  if (!strcmp(str, "m")) result = true;
  return result;
}

int get_milliseconds() {
  char str[STR_SIZE] = {0};
  get_string(str, stdin);
  int num = atoi(str);
  if (num < 50 || num > 500) num = 0;
  return num;
}
