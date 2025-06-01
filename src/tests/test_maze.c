#include <check.h>

#include "test_all.h"

START_TEST(test_default_maze_generation) {
  Maze maze = {0};
  maze.rows = 50;
  maze.cols = 50;

  bool gen_result = generate_eller_maze(&maze);
  ck_assert(gen_result);

  destroy_maze(&maze);
}
END_TEST

START_TEST(test_single_row_maze_generation) {
  Maze maze = {0};
  maze.rows = 1;
  maze.cols = 5;

  bool gen_result = generate_eller_maze(&maze);
  ck_assert(gen_result);

  for (int j = 0; j < maze.cols - 1; j++) {
    ck_assert(maze.v_w[0][j] == 0);
  }

  destroy_maze(&maze);
}
END_TEST

START_TEST(test_single_column_maze_generation) {
  Maze maze = {0};
  maze.rows = 5;
  maze.cols = 1;

  bool gen_result = generate_eller_maze(&maze);
  ck_assert(gen_result);

  for (int i = 0; i < maze.rows - 1; i++) {
    ck_assert(maze.h_w[i][0] == 0);
  }
  ck_assert(maze.h_w[maze.rows - 1][0] == 1);

  destroy_maze(&maze);
}
END_TEST

START_TEST(test_bfs_single_row) {
  Maze maze = {0};
  maze.rows = 1;
  maze.cols = 3;
  generate_eller_maze(&maze);

  Point start = {0, 0};
  Point end = {0, 2};
  Pass pass = {0};

  bfs(&maze, end, start, &pass);
  ck_assert_int_eq(pass.length, 3);

  clear_pass(&maze, &pass);
  destroy_maze(&maze);
}
END_TEST

START_TEST(test_create_next_step_valid) {
  Maze maze = {0};
  maze.rows = 3;
  maze.cols = 3;
  create_matrix(&maze.pass, maze.rows, maze.cols);

  Pass pass = {0};
  pass.length = maze.rows * maze.cols;
  create_array(&(pass.route), pass.length);
  pass.route[0] = (Point){0, 0};
  pass.route[1] = (Point){1, 1};

  ck_assert_int_eq(maze.pass[0][0], 0);
  ck_assert_int_eq(maze.pass[1][1], 0);

  create_next_step(&maze, &pass, 0);
  create_next_step(&maze, &pass, 1);

  ck_assert_int_eq(maze.pass[0][0], 1);
  ck_assert_int_eq(maze.pass[1][1], 1);
  ck_assert_int_eq(maze.pass[0][1], 0);

  clear_pass(&maze, &pass);
  destroy_maze(&maze);
}
END_TEST

TCase* create_maze_tcase() {
  TCase* tc = tcase_create("Maze Generation");
  tcase_add_test(tc, test_default_maze_generation);
  tcase_add_test(tc, test_single_row_maze_generation);
  tcase_add_test(tc, test_single_column_maze_generation);
  tcase_add_test(tc, test_bfs_single_row);
  tcase_add_test(tc, test_create_next_step_valid);
  return tc;
}

Suite* maze_suite() {
  Suite* s = suite_create("Maze Tests");
  suite_add_tcase(s, create_maze_tcase());
  return s;
}