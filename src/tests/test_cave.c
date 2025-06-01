#include "test_all.h"

int count_alive_cells(const Maze* cave) {
  int count = 0;
  for (int i = 0; i < cave->rows; ++i) {
    for (int j = 0; j < cave->cols; ++j) {
      if (cave->v_w[i][j]) count++;
    }
  }
  return count;
}

START_TEST(test_generate_cave_basic) {
  Maze cave = {0};
  cave.rows = 10;
  cave.cols = 10;

  bool result = generate_cave(&cave, 0.5);

  ck_assert(result);
  ck_assert_ptr_nonnull(cave.v_w);
  ck_assert_ptr_nonnull(cave.h_w);

  bool has_zero = false, has_one = false;
  for (int i = 0; i < cave.rows; ++i) {
    for (int j = 0; j < cave.cols; ++j) {
      if (cave.v_w[i][j] == 0) has_zero = true;
      if (cave.v_w[i][j] == 1) has_one = true;
    }
  }
  ck_assert(has_zero && has_one);

  destroy_maze(&cave);
}
END_TEST

START_TEST(test_generate_cave_extreme_chance) {
  Maze cave = {0};
  cave.rows = 5;
  cave.cols = 5;

  bool result = generate_cave(&cave, 0.0);
  ck_assert(result);
  ck_assert_int_eq(count_alive_cells(&cave), 0);
  destroy_maze(&cave);

  cave.rows = 5;
  cave.cols = 5;
  result = generate_cave(&cave, 1.0);
  ck_assert(result);
  ck_assert_int_eq(count_alive_cells(&cave), 25);
  destroy_maze(&cave);
}
END_TEST

START_TEST(test_proceed_alive) {
  Maze cave = {0};
  cave.rows = 3;
  cave.cols = 3;
  create_matrix(&cave.v_w, 3, 3);
  create_matrix(&cave.h_w, 3, 3);

  int test_grid[3][3] = {{1, 0, 1}, {0, 1, 0}, {1, 0, 1}};

  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      cave.v_w[i][j] = test_grid[i][j];
    }
  }

  proceed_alive(&cave, 1, 1, 2);  // Должна выжить (3 соседа >= 2)
  ck_assert_int_eq(cave.h_w[1][1], 1);

  proceed_alive(&cave, 1, 1, 3);  // должна выжить (4 < 3 → false, !false = 1)
  ck_assert_int_eq(cave.h_w[1][1], 1);

  proceed_alive(&cave, 1, 1, 4);  // должна выжить (4 < 4 → false, !false = 1)
  ck_assert_int_eq(cave.h_w[1][1], 1);

  proceed_alive(&cave, 1, 1, 5);  // death=5, sum=4 < 5 → true, !true=0 → умерла
  ck_assert_int_eq(cave.h_w[1][1], 0);

  destroy_maze(&cave);
}
END_TEST

START_TEST(test_proceed_dead) {
  Maze cave = {0};
  cave.rows = 3;
  cave.cols = 3;
  create_matrix(&cave.v_w, 3, 3);
  create_matrix(&cave.h_w, 3, 3);

  int test_grid[3][3] = {{1, 0, 1}, {0, 0, 0}, {1, 0, 1}};

  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      cave.v_w[i][j] = test_grid[i][j];
    }
  }

  proceed_dead(&cave, 1, 1, 3);  // Должна ожить (4 соседа > 3)
  ck_assert_int_eq(cave.h_w[1][1], 1);

  proceed_dead(&cave, 1, 1, 4);  // Должна остаться мертвой (4 соседа <= 4)
  ck_assert_int_eq(cave.h_w[1][1], 0);

  destroy_maze(&cave);
}
END_TEST

START_TEST(test_solve_cave) {
  Maze cave = {0};
  cave.rows = 3;
  cave.cols = 3;
  create_matrix(&cave.v_w, 3, 3);
  create_matrix(&cave.h_w, 3, 3);

  int test_grid[3][3] = {{1, 0, 1}, {0, 1, 0}, {1, 0, 1}};

  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      cave.v_w[i][j] = test_grid[i][j];
      cave.h_w[i][j] = 0;
    }
  }

  bool result = solve_cave(&cave, 3, 2);

  ck_assert(!result);

  ck_assert_int_eq(cave.v_w[0][0], 1);
  ck_assert_int_eq(cave.v_w[0][1], 1);
  ck_assert_int_eq(cave.v_w[1][0], 1);
  ck_assert_int_eq(cave.v_w[1][1], 1);
  ck_assert_int_eq(cave.v_w[2][2], 1);

  destroy_maze(&cave);
}
END_TEST

START_TEST(test_swap_matrix) {
  int **a, **b;
  create_matrix(&a, 2, 2);
  create_matrix(&b, 2, 2);

  a[0][0] = 1;
  a[0][1] = 2;
  a[1][0] = 3;
  a[1][1] = 4;

  b[0][0] = 5;
  b[0][1] = 6;
  b[1][0] = 7;
  b[1][1] = 8;

  swap_matrix(&a, &b);

  ck_assert_int_eq(b[0][0], 1);
  ck_assert_int_eq(a[0][0], 5);

  free_matrix(&a);
  free_matrix(&b);
}
END_TEST

Suite* cave_suite() {
  Suite* s = suite_create("Cave");
  TCase* tc = tcase_create("Core");

  tcase_add_test(tc, test_generate_cave_basic);
  tcase_add_test(tc, test_generate_cave_extreme_chance);
  tcase_add_test(tc, test_proceed_alive);
  tcase_add_test(tc, test_proceed_dead);
  tcase_add_test(tc, test_solve_cave);
  tcase_add_test(tc, test_swap_matrix);

  suite_add_tcase(s, tc);
  return s;
}
