#include "controller.h"

int game_loop() {
  srand(time(0));
  char str[STR_SIZE] = {0};
  while (strcmp(str, "q")) {
    print_start_menu();
    get_string(str, stdin);
    if (!strcmp(str, "m")) {
      proceed_maze('m');
    } else if (!strcmp(str, "c")) {
      proceed_maze('c');
    }
  }
  return 0;
}

void proceed_maze(char c) {
  char str[STR_SIZE] = {0};
  while (strcmp(str, "q")) {
    Maze maze = {0};
    bool result = false;
    print_load_or_create_menu();
    get_string(str, stdin);
    if (!strcmp(str, "c")) {
      result = create_maze(&maze, c);
    } else if (!strcmp(str, "f")) {
      result = load_maze(&maze, c);
    }
    if (result) {
      clear_screen();
      draw_maze(&maze);
      solve_maze(&maze, c);
    }
    destroy_maze(&maze);
  }
}

bool create_maze(Maze *maze, char c) {
  bool result = false;
  int rows = 0, cols = 0;
  while (rows < 1 || cols < 1 || rows > MAX_SIZE || cols > MAX_SIZE) {
    print_input_string();
    get_row_col(&rows, &cols, 'm');
  }
  maze->rows = rows;
  maze->cols = cols;
  if (c == 'm')
    result = generate_eller_maze(maze);
  else
    result = generate_cave(maze, get_chance());
  return result;
}

void solve_maze(Maze *maze, char c) {
  char str[STR_SIZE] = {0};
  while (strcmp(str, "q")) {
    print_find_pass_menu(c);
    get_string(str, stdin);
    if (!strcmp(str, "c")) {
      if (c == 'm')
        find_pass(maze);
      else {
        solve_and_output_cave(maze);
        str[0] = 'q';
      }
    } else if (!strcmp(str, "l")) {
      q_learning_mode(maze);
    } else if (!strcmp(str, "v")) {
      draw_maze_matrix(maze);
    }
  }
}

void find_pass(Maze *maze) {
  int row = -1;
  int col = -1;
  while (row < 1 || col < 1 || row > maze->rows || col > maze->cols) {
    print_point_menu(maze->rows, maze->cols, "start");
    get_row_col(&row, &col, 'm');
  }
  Point start = {row - 1, col - 1};
  row = col = -1;
  while (row < 1 || col < 1 || row > maze->rows || col > maze->cols) {
    print_point_menu(maze->rows, maze->cols, "end");
    get_row_col(&row, &col, 'm');
  }
  Point end = {row - 1, col - 1};
  Pass pass = {0};
  bfs(maze, start, end, &pass);
  output_maze(maze, &pass);
}

void output_maze(Maze *maze, Pass *pass) {
  print_output_menu('m');
  bool m = manual_or_automatic();
  int ms = 0;
  if (!m) {
    while (ms == 0) {
      print_get_millseconds_menu();
      ms = get_milliseconds();
    }
  }
  clear_screen();
  printf("\e[?25l");
  fflush(stdout);
  for (int i = 0; i < pass->length; ++i) {
    draw_maze(maze);
    create_next_step(maze, pass, i);
    if (m) {
      wait_for_any_key();
    } else {
      delay_ms(ms);
    }
  }
  draw_maze(maze);
  printf("\e[?25h");
  fflush(stdout);
  clear_pass(maze, pass);
}

void delay_ms(int milliseconds) {
  struct timespec ts;
  ts.tv_sec = milliseconds / 1000;
  ts.tv_nsec = (milliseconds % 1000) * 1000000;
  nanosleep(&ts, NULL);
}

bool load_maze(Maze *maze, char c) {
  bool result = false;
  print_input_file_menu();
  char str[STR_SIZE] = {0};
  get_string(str, stdin);
  FILE *f = fopen(str, "r");
  if (f) {
    result = parse_maze_file(f, maze, c);
    fclose(f);
    if (!result) {
      printf("Wrong format\n");
    }
  } else {
    printf("Can't load file\n");
  }
  return result;
}

void solve_and_output_cave(Maze *cave) {
  int birth = -1;
  int death = -1;
  while (birth < 0 || death < 0) {
    print_solve_cave_menu();
    get_row_col(&birth, &death, 'c');
  }
  print_output_menu('m');
  bool m = manual_or_automatic();
  int ms = 0;
  if (!m) {
    while (ms == 0) {
      print_get_millseconds_menu();
      ms = get_milliseconds();
    }
  }
  int max_iter = 7 + (cave->rows + cave->cols) / 3;
  clear_screen();
  printf("\e[?25l");
  fflush(stdout);
  draw_maze(cave);
  while (!solve_cave(cave, birth, death) && max_iter) {
    if (m) {
      wait_for_any_key();
    } else {
      delay_ms(ms);
    }
    draw_maze(cave);
    max_iter--;
    if (!max_iter) printf("cycle\n");
  }
  printf("\e[?25h");
  fflush(stdout);
}

double get_chance() {
  double value = -1.;
  while (value <= 0. || value >= 1.) {
    print_chance_menu();
    value = get_chance_value();
  }
  return value;
}

int getch(void) {
  struct termios oldt, newt;
  int ch;
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  ch = getchar();
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  return ch;
}

void wait_for_any_key() {
  printf("Press any key...");
  getch();
}

void q_learning_mode(Maze *maze) {
  QLearningAgent agent;
  Point start, goal;

  int row = -1, col = -1;
  while (row < 1 || col < 1 || row > maze->rows || col > maze->cols) {
    print_point_menu(maze->rows, maze->cols, "goal");
    get_row_col(&row, &col, 'm');
  }
  goal.row = row - 1;
  goal.col = col - 1;

  q_agent_init(&agent, goal);
  printf("Training agent...\e[?25l");
  fflush(stdout);
  q_agent_train(&agent, maze);
  printf("\nTraining complete!\e[?25h\n");
  fflush(stdout);

  char str[STR_SIZE] = {0};
  while (strcmp(str, "q")) {
    print_q_learning_menu();
    get_string(str, stdin);

    if (!strcmp(str, "f")) {
      row = col = -1;
      while (row < 1 || col < 1 || row > maze->rows || col > maze->cols) {
        print_point_menu(maze->rows, maze->cols, "start");
        get_row_col(&row, &col, 'm');
      }
      start.row = row - 1;
      start.col = col - 1;

      Pass pass = {0};
      if (q_agent_find_path(&agent, maze, start, &pass)) {
        printf("Path found! Length: %d\n", pass.length);
        output_maze(maze, &pass);
        clear_pass(maze, &pass);
      } else {
        printf("No path found!\n");
      }
    } else if (!strcmp(str, "v")) {
      visualize_q_values(stdout, &agent, maze);
    }
  }
}
