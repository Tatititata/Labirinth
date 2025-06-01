#include <check.h>

#include "test_all.h"

START_TEST(test_get_string_basic) {
  write_to_temp_file("Hello world\n");

  FILE *f = fopen(TEMP_FILENAME, "r");
  char buffer[STR_SIZE];
  get_string(buffer, f);
  fclose(f);
  remove(TEMP_FILENAME);

  ck_assert_str_eq(buffer, "Hello world");
}
END_TEST

START_TEST(test_get_row_col_valid_input) {
  write_to_temp_file("5 6\n");

  FILE *f = freopen(TEMP_FILENAME, "r", stdin);
  int rows = 0, cols = 0;
  get_row_col(&rows, &cols, 'r');
  fclose(f);
  remove(TEMP_FILENAME);

  ck_assert_int_eq(rows, 5);
  ck_assert_int_eq(cols, 6);
}
END_TEST

START_TEST(test_get_row_col_invalid_input_text) {
  write_to_temp_file("abc def\n");

  FILE *f = freopen(TEMP_FILENAME, "r", stdin);
  int rows = 0, cols = 0;
  get_row_col(&rows, &cols, 'r');
  fclose(f);
  remove(TEMP_FILENAME);

  ck_assert_int_eq(rows, -1);
  ck_assert_int_eq(cols, -1);
}
END_TEST

START_TEST(test_get_row_col_invalid_bounds) {
  write_to_temp_file("-1 100\n");

  FILE *f = freopen(TEMP_FILENAME, "r", stdin);
  int rows = 0, cols = 0;
  get_row_col(&rows, &cols, 'r');
  fclose(f);
  remove(TEMP_FILENAME);

  ck_assert_int_eq(rows, -1);
  ck_assert_int_eq(cols, -1);
}
END_TEST

START_TEST(test_get_chance_value_valid) {
  write_to_temp_file("0.75\n");

  FILE *f = freopen(TEMP_FILENAME, "r", stdin);
  double value = get_chance_value();
  fclose(f);
  remove(TEMP_FILENAME);

  ck_assert_double_eq_tol(value, 0.75, 1e-6);
}
END_TEST

START_TEST(test_manual_or_automatic_manual) {
  write_to_temp_file("m\n");

  FILE *f = freopen(TEMP_FILENAME, "r", stdin);
  bool is_manual = manual_or_automatic();
  fclose(f);
  remove(TEMP_FILENAME);

  ck_assert(is_manual);
}
END_TEST

START_TEST(test_manual_or_automatic_auto) {
  write_to_temp_file("a\n");

  FILE *f = freopen(TEMP_FILENAME, "r", stdin);
  bool is_manual = manual_or_automatic();
  fclose(f);
  remove(TEMP_FILENAME);

  ck_assert(!is_manual);
}
END_TEST

START_TEST(test_get_milliseconds_valid) {
  write_to_temp_file("500\n");

  FILE *f = freopen(TEMP_FILENAME, "r", stdin);
  int ms = get_milliseconds();
  fclose(f);
  remove(TEMP_FILENAME);

  ck_assert_int_eq(ms, 500);
}
END_TEST

START_TEST(test_get_milliseconds_negative) {
  write_to_temp_file("-100\n");

  FILE *f = freopen(TEMP_FILENAME, "r", stdin);
  int ms = get_milliseconds();
  fclose(f);
  remove(TEMP_FILENAME);

  ck_assert_int_eq(ms, 0);
}
END_TEST

START_TEST(test_get_row_col_c_char_valid_input) {
  write_to_temp_file("0 7\n");

  FILE *f = freopen(TEMP_FILENAME, "r", stdin);
  int rows = -2, cols = -2;
  get_row_col(&rows, &cols, 'c');
  fclose(f);
  remove(TEMP_FILENAME);

  ck_assert_int_eq(rows, 0);
  ck_assert_int_eq(cols, 7);
}
END_TEST

START_TEST(test_get_row_col_c_char_invalid_below_min) {
  write_to_temp_file("-1 2\n");

  FILE *f = freopen(TEMP_FILENAME, "r", stdin);
  int rows = -2, cols = -2;
  get_row_col(&rows, &cols, 'c');
  fclose(f);
  remove(TEMP_FILENAME);

  ck_assert_int_eq(rows, -1);
  ck_assert_int_eq(cols, -1);
}
END_TEST

START_TEST(test_get_row_col_c_char_invalid_above_max) {
  write_to_temp_file("1 8\n");

  FILE *f = freopen(TEMP_FILENAME, "r", stdin);
  int rows = -2, cols = -2;
  get_row_col(&rows, &cols, 'c');
  fclose(f);
  remove(TEMP_FILENAME);

  ck_assert_int_eq(rows, -1);
  ck_assert_int_eq(cols, -1);
}
END_TEST

START_TEST(test_parse_labyrinth_file_basic_manual) {
  const char *labyrinth_data =
      "\n"
      "3 3\n"
      "0 1 0\n"
      "1 0 1\n"
      "0 1 0\n"
      "\n"
      "1 1 1\n"
      "1 1 1\n"
      "1 1 1\n";

  FILE *f = fopen(TEMP_FILENAME, "w");
  fputs(labyrinth_data, f);
  fclose(f);

  f = fopen(TEMP_FILENAME, "r");
  Labyrinth labyrinth = {0};
  bool success = parse_labyrinth_file(f, &labyrinth, 'm');
  fclose(f);
  remove(TEMP_FILENAME);

  ck_assert_msg(success, "Parsing should succeed.");
  ck_assert_int_eq(labyrinth.rows, 3);
  ck_assert_int_eq(labyrinth.cols, 3);
  ck_assert_ptr_nonnull(labyrinth.v_w);
  ck_assert_ptr_nonnull(labyrinth.h_w);
  ck_assert_ptr_nonnull(labyrinth.pass);

  ck_assert_int_eq(labyrinth.v_w[1][0], 1);
  ck_assert_int_eq(labyrinth.h_w[2][2], 1);

  destroy_labyrinth(&labyrinth);
}
END_TEST

START_TEST(test_parse_labyrinth_file_fail_on_wrong_size) {
  const char *labyrinth_data = "a b\n";
  FILE *f = fopen(TEMP_FILENAME, "w");
  fputs(labyrinth_data, f);
  fclose(f);

  f = fopen(TEMP_FILENAME, "r");
  Labyrinth labyrinth = {0};
  bool success = parse_labyrinth_file(f, &labyrinth, 'm');
  fclose(f);
  remove(TEMP_FILENAME);

  ck_assert_msg(!success, "Parsing should fail on invalid size.");
  ck_assert_ptr_null(labyrinth.v_w);
}
END_TEST

START_TEST(test_parse_labyrinth_file_fail_on_wrong_data) {
  const char *labyrinth_data = "3 3\n\n 1 1 0\n 1 1\n";
  FILE *f = fopen(TEMP_FILENAME, "w");
  fputs(labyrinth_data, f);
  fclose(f);

  f = fopen(TEMP_FILENAME, "r");
  Labyrinth labyrinth = {0};
  bool success = parse_labyrinth_file(f, &labyrinth, 'm');
  fclose(f);
  remove(TEMP_FILENAME);

  ck_assert_msg(!success, "Parsing should fail on invalid data.");
  ck_assert_ptr_null(labyrinth.v_w);
}
END_TEST

START_TEST(test_load_matrix_valid) {
  const char *matrix_data =
      "0 1 0\n"
      "1 0 1\n"
      "0 1 0\n";

  FILE *f = fopen(TEMP_FILENAME, "w");
  fputs(matrix_data, f);
  fclose(f);

  f = fopen(TEMP_FILENAME, "r");
  int **matrix = NULL;
  ck_assert(create_matrix(&matrix, 3, 3));
  bool success = load_matrix(f, matrix, 3, 3);
  fclose(f);
  remove(TEMP_FILENAME);

  ck_assert_msg(success, "Matrix should load successfully.");
  ck_assert_int_eq(matrix[1][0], 1);
  free_matrix(&matrix);
}
END_TEST

START_TEST(test_load_matrix_invalid_data) {
  const char *matrix_data =
      "0 1 x\n"
      "1 0 1\n"
      "0 1 0\n";

  FILE *f = fopen(TEMP_FILENAME, "w");
  fputs(matrix_data, f);
  fclose(f);

  f = fopen(TEMP_FILENAME, "r");
  int **matrix = NULL;
  ck_assert(create_matrix(&matrix, 3, 3));
  bool success = load_matrix(f, matrix, 3, 3);
  fclose(f);
  remove(TEMP_FILENAME);

  ck_assert_msg(!success, "Matrix loading should fail on invalid data.");
  free_matrix(&matrix);
}
END_TEST

START_TEST(test_parse_labyrinth_file_with_random_spaces_and_newlines) {
  const int rows = 3, cols = 3;
  const char *v_w_data[3] = {"0 1 0", "1 0 1", "0 1 0"};
  const char *h_w_data[3] = {"1 1 1", "1 1 1", "1 1 1"};

  FILE *f = fopen(TEMP_FILENAME, "w");

  fputs("\n\n", f);
  fprintf(f, "   %d    %d  \n\n", rows, cols);
  for (int i = 0; i < rows; ++i) {
    fputs("\n", f);
    for (const char *p = v_w_data[i]; *p; ++p) {
      fputc(*p, f);
      if (*p == '0' || *p == '1') {
        int spaces = 1 + rand() % 4;
        for (int s = 0; s < spaces; ++s) fputc(' ', f);
      }
    }
    fputc('\n', f);
  }

  fputs("\n\n", f);

  for (int i = 0; i < rows; ++i) {
    fputs("\n", f);
    for (const char *p = h_w_data[i]; *p; ++p) {
      fputc(*p, f);
      if (*p == '0' || *p == '1') {
        int spaces = 1 + rand() % 4;
        for (int s = 0; s < spaces; ++s) fputc(' ', f);
      }
    }
    fputc('\n', f);
  }

  fclose(f);

  f = fopen(TEMP_FILENAME, "r");
  Labyrinth labyrinth = {0};
  bool success = parse_labyrinth_file(f, &labyrinth, 'm');
  fclose(f);
  remove(TEMP_FILENAME);

  ck_assert_msg(success,
                "Parsing with irregular spaces/newlines should succeed.");
  ck_assert_int_eq(labyrinth.rows, 3);
  ck_assert_int_eq(labyrinth.cols, 3);
  ck_assert_ptr_nonnull(labyrinth.v_w);
  ck_assert_ptr_nonnull(labyrinth.h_w);
  ck_assert_ptr_nonnull(labyrinth.pass);

  ck_assert_int_eq(labyrinth.v_w[1][0], 1);
  ck_assert_int_eq(labyrinth.h_w[2][2], 1);

  destroy_labyrinth(&labyrinth);
}
END_TEST

Suite *parser_suite(void) {
  Suite *s = suite_create("Parser");
  TCase *tc = tcase_create("Core");

  tcase_add_test(tc, test_get_string_basic);
  tcase_add_test(tc, test_get_row_col_valid_input);
  tcase_add_test(tc, test_get_row_col_invalid_input_text);
  tcase_add_test(tc, test_get_row_col_invalid_bounds);
  tcase_add_test(tc, test_get_chance_value_valid);
  tcase_add_test(tc, test_manual_or_automatic_manual);
  tcase_add_test(tc, test_manual_or_automatic_auto);
  tcase_add_test(tc, test_get_milliseconds_valid);
  tcase_add_test(tc, test_get_milliseconds_negative);
  tcase_add_test(tc, test_get_row_col_c_char_valid_input);
  tcase_add_test(tc, test_get_row_col_c_char_invalid_below_min);
  tcase_add_test(tc, test_get_row_col_c_char_invalid_above_max);
  tcase_add_test(tc, test_parse_labyrinth_file_basic_manual);
  tcase_add_test(tc, test_parse_labyrinth_file_fail_on_wrong_size);
  tcase_add_test(tc, test_load_matrix_valid);
  tcase_add_test(tc, test_load_matrix_invalid_data);
  tcase_add_test(tc, test_parse_labyrinth_file_with_random_spaces_and_newlines);
  tcase_add_test(tc, test_parse_labyrinth_file_fail_on_wrong_data);
  suite_add_tcase(s, tc);
  return s;
}