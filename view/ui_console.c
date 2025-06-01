#include "ui_console.h"

#define RED "\033[1;31m"
#define RESET "\033[0m"

void print_start_menu() {
  printf("To create a labyrinth press " RED "'m'" RESET " button. ");
  printf("To create a cave press " RED "'c'" RESET " button. ");
  printf("To quit press " RED "'q'" RESET " button. ");
}

void print_load_or_create_menu() {
  printf("To load from file press " RED "'f'" RESET " button. ");
  printf("To create press " RED "'c'" RESET " button. ");
  printf(RED "q" RESET " - Quit to previous menu. ");
}

void print_find_pass_menu(char c) {
  if (c == 'm') {
    printf("To find pass between 2 cells press " RED "'c'" RESET " button. ");
    printf("For Q-learning press " RED "'l'" RESET " button. ");
  } else {
    printf("To run cave generation press " RED "'c'" RESET " button. ");
  }
  printf(RED "q" RESET " - Quit to previous menu. ");
}

void print_point_menu(int row, int col, const char* c) {
  printf("Input %s row and column separated by space - ", c);
  printf("row from 1 to %d, column from 1 to %d: ", row, col);
}

void print_input_string() {
  printf("Input natural number of rows and columns ");
  printf("(<= 50) separated by space: ");
}

void print_output_menu(char c) {
  printf("For manual ");
  if (c == 'm')
    printf("pass drawing");
  else
    printf("cave creating");
  printf(" press " RED "'m'" RESET ", for automatic drawing press any key. ");
}

void print_input_file_menu() {
  printf("Input " RED "*.txt" RESET " file name: ");
}

void print_chance_menu() {
  printf("Input real/double (0, 1) value of chance for starting cell init: ");
}

void print_solve_cave_menu() {
  printf("Input integer " RED "'birth'" RESET " > " RED "'death'" RESET);
  printf(" limits [0, 7] separated by space: ");
}

void print_get_millseconds_menu() {
  printf("Input number of milliseconds for algorithm frequency (50 - 500): ");
}

void print_q_learning_menu() {
  printf("Q-learning mode: ");
  printf(RED "f" RESET " - Find path from start point; ");
  printf(RED "q" RESET " - Quit to previous menu. ");
}

void draw_labyrinth_matrix(const Labyrinth* labyrinth) {
  printf("Vertical walls\n");
  for (int i = 0; i < labyrinth->rows; ++i) {
    for (int j = 0; j < labyrinth->cols; ++j) {
      printf("%d ", labyrinth->v_w[i][j]);
    }
    printf("\n");
  }

  printf("\nHorizontal walls\n");
  for (int i = 0; i < labyrinth->rows; ++i) {
    for (int j = 0; j < labyrinth->cols; ++j) {
      printf("%d ", labyrinth->h_w[i][j]);
    }
    printf("\n");
  }
}

void draw_labyrinth(const Labyrinth* labyrinth) {
  if (labyrinth->pass)
    print_labyrinth(labyrinth);
  else
    print_cave(labyrinth);
}

int get_console_size(int* rows, int* cols) {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  *rows = w.ws_row - 1;
  *cols = w.ws_col;
  return 0;
}

void print_cave(const Labyrinth* cave) {
  char buffer[MAX_BUF_SIZE];
  int pos = 0;

  pos += sprintf(buffer + pos, "\033[H");

  int console_rows, console_cols;
  get_console_size(&console_rows, &console_cols);

  int cave_rows = cave->rows;
  int cave_cols = cave->cols;

  int cell_height = console_rows / cave_rows;
  int cell_width = console_cols / cave_cols;
  if (cell_height < 1) cell_height = 1;
  if (cell_width < 1) cell_width = 1;

  for (int i = 0; i < cave_rows; ++i) {
    for (int h = 0; h < cell_height; ++h) {
      for (int j = 0; j < cave_cols; ++j) {
        for (int k = 0; k < cell_width; ++k) {
          pos += sprintf(buffer + pos, cave->v_w[i][j] ? "█" : " ");
        }
      }
      pos += sprintf(buffer + pos, "\n");
    }
  }

  printf("%s", buffer);
  fflush(stdout);
}

void print_labyrinth(const Labyrinth* labyrinth) {
  char buffer[MAX_BUF_SIZE];
  int pos = 0;

  pos += sprintf(buffer + pos, "\033[?7l\033[H");

  int console_rows, console_cols;
  get_console_size(&console_rows, &console_cols);

  int labyrinth_rows = labyrinth->rows;
  int labyrinth_cols = labyrinth->cols;

  int total_walls = 2 + (labyrinth_cols - 1);
  int cell_width = (console_cols - total_walls) / labyrinth_cols;
  if (cell_width < 1) cell_width = 1;

  int total_h_walls = 2 + (labyrinth_rows - 1);
  int cell_height = (console_rows - total_h_walls) / labyrinth_rows;
  if (cell_height < 1) cell_height = 1;

  pos += sprintf(buffer + pos, "┌");
  for (int j = 0; j < labyrinth_cols; ++j) {
    for (int k = 0; k < cell_width; ++k) pos += sprintf(buffer + pos, "─");
    if (j < labyrinth_cols - 1) pos += sprintf(buffer + pos, "┬");
  }
  pos += sprintf(buffer + pos, "┐\n");

  for (int i = 0; i < labyrinth_rows; ++i) {
    for (int h = 0; h < cell_height; ++h) {
      pos += sprintf(buffer + pos, "│");
      for (int j = 0; j < labyrinth_cols; ++j) {
        for (int k = 0; k < cell_width; ++k) {
          if (labyrinth->pass[i][j] && h == cell_height / 2 && k == cell_width / 2)
            pos += sprintf(buffer + pos, "\033[91m█\x1b[0m");
          else
            pos += sprintf(buffer + pos, " ");
        }
        if (j < labyrinth_cols - 1)
          pos += sprintf(buffer + pos, labyrinth->v_w[i][j] ? "│" : " ");
      }
      pos += sprintf(buffer + pos, "│\n");
    }

    if (i < labyrinth_rows - 1) {
      pos += sprintf(buffer + pos, "├");
      for (int j = 0; j < labyrinth_cols; ++j) {
        for (int k = 0; k < cell_width; ++k)
          pos += sprintf(buffer + pos, labyrinth->h_w[i][j] ? "─" : " ");
        if (j < labyrinth_cols - 1) pos += sprintf(buffer + pos, "┼");
      }
      pos += sprintf(buffer + pos, "┤\n");
    }
  }

  pos += sprintf(buffer + pos, "└");
  for (int j = 0; j < labyrinth_cols; ++j) {
    for (int k = 0; k < cell_width; ++k) pos += sprintf(buffer + pos, "─");
    if (j < labyrinth_cols - 1) pos += sprintf(buffer + pos, "┴");
  }
  pos += sprintf(buffer + pos, "┘\n");

  sprintf(buffer + pos, "\033[?7h");

  printf("%s", buffer);
  fflush(stdout);
}

void clear_screen() {
  printf("\033[2J\033[H");
  fflush(stdout);
}
