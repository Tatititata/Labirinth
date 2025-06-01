
#include "../q_learning/learning.h"
#include "test_all.h"

Maze create_test_maze() {
  Maze maze = {0};
  maze.rows = 3;
  maze.cols = 3;
  create_matrix(&maze.v_w, 3, 3);
  create_matrix(&maze.h_w, 3, 3);
  create_matrix(&maze.pass, 3, 3);
  return maze;
}

START_TEST(test_q_agent_init) {
  QLearningAgent agent = {0};
  Point goal = {2, 2};

  q_agent_init(&agent, goal);

  ck_assert_int_eq(agent.goal.row, 2);
  ck_assert_int_eq(agent.goal.col, 2);

  for (int i = 0; i < 50; i++) {
    for (int j = 0; j < 50; j++) {
      for (int k = 0; k < ACTIONS; k++) {
        ck_assert_double_eq(agent.q_table[i][j][k], 0.0);
      }
    }
  }
}
END_TEST

START_TEST(test_get_next_point) {
  Point cur = {1, 1};

  Point up = get_next_point(cur, 0);  // up
  ck_assert_int_eq(up.row, 0);
  ck_assert_int_eq(up.col, 1);

  Point down = get_next_point(cur, 1);  // down
  ck_assert_int_eq(down.row, 2);
  ck_assert_int_eq(down.col, 1);

  Point left = get_next_point(cur, 2);  // left
  ck_assert_int_eq(left.row, 1);
  ck_assert_int_eq(left.col, 0);

  Point right = get_next_point(cur, 3);  // right
  ck_assert_int_eq(right.row, 1);
  ck_assert_int_eq(right.col, 2);
}
END_TEST

START_TEST(test_q_agent_choose_action) {
  QLearningAgent agent = {0};
  q_agent_init(&agent, (Point){2, 2});

  Maze maze = create_test_maze();
  Point cur = {1, 1};
  int action = -1;

  Point next = q_agent_choose_action(&agent, cur, &maze, &action);

  ck_assert(action >= 0 && action < ACTIONS);
  ck_assert(
      (next.row == 0 && next.col == 1) || (next.row == 2 && next.col == 1) ||
      (next.row == 1 && next.col == 0) || (next.row == 1 && next.col == 2));

  destroy_maze(&maze);
}
END_TEST

START_TEST(test_q_agent_update) {
  QLearningAgent agent = {0};
  q_agent_init(&agent, (Point){2, 2});

  Point cur = {1, 1};
  int action = 1;  // Вниз
  Point next = {2, 1};
  double reward = 10.0;

  q_agent_update(&agent, cur, action, next, reward);

  double expected_q = ALPHA * (reward + GAMMA * 0.0);  // max_next_q = 0
  ck_assert_double_eq_tol(agent.q_table[cur.row][cur.col][action], expected_q,
                          0.0001);
}
END_TEST

START_TEST(test_q_agent_find_path_simple) {
  QLearningAgent agent = {0};
  q_agent_init(&agent, (Point){2, 2});

  Maze maze = create_test_maze();
  Pass pass = {0};
  Point start = {0, 0};

  q_agent_train(&agent, &maze);

  bool found = q_agent_find_path(&agent, &maze, start, &pass);

  ck_assert(found);
  ck_assert_int_gt(pass.length, 0);
  ck_assert_int_eq(pass.route[0].row, 0);
  ck_assert_int_eq(pass.route[0].col, 0);
  ck_assert_int_eq(pass.route[pass.length - 1].row, 2);
  ck_assert_int_eq(pass.route[pass.length - 1].col, 2);

  clear_pass(&maze, &pass);
  destroy_maze(&maze);
}
END_TEST

START_TEST(test_visualize_q_values_file_output) {
  QLearningAgent agent = {0};
  Maze maze = {.rows = 2, .cols = 2};
  create_matrix(&maze.v_w, 2, 2);
  create_matrix(&maze.h_w, 2, 2);

  agent.q_table[0][0][0] = 1.0;
  agent.q_table[0][1][2] = 2.0;
  agent.q_table[1][0][1] = 3.0;
  agent.q_table[1][1][3] = 4.0;

  FILE* f = fopen(TEMP_FILENAME, "w");
  ck_assert_ptr_nonnull(f);
  visualize_q_values(f, &agent, &maze);
  fclose(f);

  f = fopen(TEMP_FILENAME, "r");
  ck_assert_ptr_nonnull(f);
  char buffer[1024] = {0};
  fread(buffer, sizeof(char), sizeof(buffer) - 1, f);
  fclose(f);

  ck_assert_msg(strstr(buffer, "   1.0") != NULL, "Missing 1.0 in output");
  ck_assert_msg(strstr(buffer, "   2.0") != NULL, "Missing 2.0 in output");
  ck_assert_msg(strstr(buffer, "   3.0") != NULL, "Missing 3.0 in output");
  ck_assert_msg(strstr(buffer, "   4.0") != NULL, "Missing 4.0 in output");

  destroy_maze(&maze);
  remove(TEMP_FILENAME);
}
END_TEST

START_TEST(test_q_agent_find_path_fail) {
  QLearningAgent agent = {0};
  q_agent_init(&agent, (Point){2, 2});

  Maze maze = create_test_maze();
  maze.v_w[0][0] = 1;
  maze.h_w[0][0] = 1;
  Pass pass = {0};
  Point start = {0, 0};

  q_agent_train(&agent, &maze);

  bool found = q_agent_find_path(&agent, &maze, start, &pass);

  ck_assert(!found);
  clear_pass(&maze, &pass);
  destroy_maze(&maze);
}
END_TEST

Suite* q_agent_suite() {
  Suite* s = suite_create("Q-Learning Agent Tests");

  TCase* tc_core = tcase_create("Core");
  tcase_add_test(tc_core, test_q_agent_init);
  tcase_add_test(tc_core, test_get_next_point);
  tcase_add_test(tc_core, test_q_agent_choose_action);
  tcase_add_test(tc_core, test_q_agent_update);
  tcase_add_test(tc_core, test_q_agent_find_path_simple);
  tcase_add_test(tc_core, test_visualize_q_values_file_output);
  tcase_add_test(tc_core, test_q_agent_find_path_fail);

  suite_add_tcase(s, tc_core);

  return s;
}
