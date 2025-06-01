#include <check.h>

#include "test_all.h"

START_TEST(test_default_labyrinth_generation) {
  Labyrinth labyrinth = {0};
  labyrinth.rows = 50;
  labyrinth.cols = 50;

  bool gen_result = generate_eller_labyrinth(&labyrinth);
  ck_assert(gen_result);

  destroy_labyrinth(&labyrinth);
}
END_TEST

START_TEST(test_single_row_labyrinth_generation) {
  Labyrinth labyrinth = {0};
  labyrinth.rows = 1;
  labyrinth.cols = 5;

  bool gen_result = generate_eller_labyrinth(&labyrinth);
  ck_assert(gen_result);

  for (int j = 0; j < labyrinth.cols - 1; j++) {
    ck_assert(labyrinth.v_w[0][j] == 0);
  }

  destroy_labyrinth(&labyrinth);
}
END_TEST

START_TEST(test_single_column_labyrinth_generation) {
  Labyrinth labyrinth = {0};
  labyrinth.rows = 5;
  labyrinth.cols = 1;

  bool gen_result = generate_eller_labyrinth(&labyrinth);
  ck_assert(gen_result);

  for (int i = 0; i < labyrinth.rows - 1; i++) {
    ck_assert(labyrinth.h_w[i][0] == 0);
  }
  ck_assert(labyrinth.h_w[labyrinth.rows - 1][0] == 1);

  destroy_labyrinth(&labyrinth);
}
END_TEST

START_TEST(test_bfs_single_row) {
  Labyrinth labyrinth = {0};
  labyrinth.rows = 1;
  labyrinth.cols = 3;
  generate_eller_labyrinth(&labyrinth);

  Point start = {0, 0};
  Point end = {0, 2};
  Pass pass = {0};

  bfs(&labyrinth, end, start, &pass);
  ck_assert_int_eq(pass.length, 3);

  clear_pass(&labyrinth, &pass);
  destroy_labyrinth(&labyrinth);
}
END_TEST

START_TEST(test_create_next_step_valid) {
  Labyrinth labyrinth = {0};
  labyrinth.rows = 3;
  labyrinth.cols = 3;
  create_matrix(&labyrinth.pass, labyrinth.rows, labyrinth.cols);

  Pass pass = {0};
  pass.length = labyrinth.rows * labyrinth.cols;
  create_array(&(pass.route), pass.length);
  pass.route[0] = (Point){0, 0};
  pass.route[1] = (Point){1, 1};

  ck_assert_int_eq(labyrinth.pass[0][0], 0);
  ck_assert_int_eq(labyrinth.pass[1][1], 0);

  create_next_step(&labyrinth, &pass, 0);
  create_next_step(&labyrinth, &pass, 1);

  ck_assert_int_eq(labyrinth.pass[0][0], 1);
  ck_assert_int_eq(labyrinth.pass[1][1], 1);
  ck_assert_int_eq(labyrinth.pass[0][1], 0);

  clear_pass(&labyrinth, &pass);
  destroy_labyrinth(&labyrinth);
}
END_TEST

TCase* create_labyrinth_tcase() {
  TCase* tc = tcase_create("Labyrinth Generation");
  tcase_add_test(tc, test_default_labyrinth_generation);
  tcase_add_test(tc, test_single_row_labyrinth_generation);
  tcase_add_test(tc, test_single_column_labyrinth_generation);
  tcase_add_test(tc, test_bfs_single_row);
  tcase_add_test(tc, test_create_next_step_valid);
  return tc;
}

Suite* labyrinth_suite() {
  Suite* s = suite_create("Labyrinth Tests");
  suite_add_tcase(s, create_labyrinth_tcase());
  return s;
}