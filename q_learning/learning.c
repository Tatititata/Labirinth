#include "learning.h"

void q_agent_init(QLearningAgent *agent, Point goal) {
  agent->goal = goal;
  for (int i = 0; i < 50; i++) {
    for (int j = 0; j < 50; j++) {
      for (int k = 0; k < ACTIONS; k++) {
        agent->q_table[i][j][k] = 0.0;
      }
    }
  }
}

Point get_next_point(Point cur, int action) {
  Point next = cur;
  if (action == 0)
    next.row -= 1;
  else if (action == 1)
    next.row += 1;
  else if (action == 2)
    next.col -= 1;
  else
    next.col += 1;
  return next;
}

Point q_agent_choose_action(QLearningAgent *agent, Point cur, Labyrinth *labyrinth,
                            int *action) {
  Point next = cur;
  if ((double)rand() / RAND_MAX < EPSILON) {
    bool cango = false;
    int steps = 0;
    while (!cango && steps < ACTIONS) {
      *action = rand() % ACTIONS;
      next = get_next_point(cur, *action);
      cango = can_go(labyrinth, cur.row, cur.col, next.row, next.col);
      ++steps;
    }
    if (!cango) next = cur;
  } else {
    double max_q = -INFINITY;
    for (int a = 0; a < ACTIONS; a++) {
      Point tmp = get_next_point(cur, a);
      if (can_go(labyrinth, cur.row, cur.col, tmp.row, tmp.col)) {
        if (agent->q_table[cur.row][cur.col][a] > max_q) {
          max_q = agent->q_table[cur.row][cur.col][a];
          next = tmp;
          *action = a;
        }
      }
    }
  }
  return next;
}

void q_agent_update(QLearningAgent *agent, Point cur, int action, Point next,
                    double reward) {
  double max_next_q = -INFINITY;
  for (int a = 0; a < ACTIONS; a++) {
    if (agent->q_table[next.row][next.col][a] > max_next_q) {
      max_next_q = agent->q_table[next.row][next.col][a];
    }
  }
  agent->q_table[cur.row][cur.col][action] =
      (1 - ALPHA) * agent->q_table[cur.row][cur.col][action] +
      ALPHA * (reward + GAMMA * max_next_q);
}

void q_agent_train(QLearningAgent *agent, Labyrinth *labyrinth) {
  double penalty = (double)REWARD / 7.0 / (double)MAX_STEP_PER_EPISODE;

  for (int episode = 0; episode < MAX_EPISODES; episode++) {
#ifndef TESTING
    if (episode % 1000 == 0) {
      printf(".");
      fflush(stdout);
    }
#endif
    Point current = {.row = rand() % labyrinth->rows, .col = rand() % labyrinth->cols};
    int steps = 0;
    while (
        !(current.row == agent->goal.row && current.col == agent->goal.col) &&
        steps < MAX_STEP_PER_EPISODE) {
      int action;
      Point next = q_agent_choose_action(agent, current, labyrinth, &action);
      double reward = penalty;
      if (next.row == agent->goal.row && next.col == agent->goal.col) {
        reward = REWARD;
      }
      q_agent_update(agent, current, action, next, reward);
      steps++;
      current = next;
    }
  }
}

bool q_agent_find_path(QLearningAgent *agent, Labyrinth *labyrinth, Point start,
                       Pass *pass) {
  bool result = false;
  clear_pass(labyrinth, pass);
  int max_steps = labyrinth->rows * labyrinth->cols;
  result = create_array(&(pass->route), max_steps);

  if (result) {
    pass->length = 0;
    Point cur = start;

    pass->route[pass->length] = cur;
    pass->length++;

    while (!(cur.row == agent->goal.row && cur.col == agent->goal.col) &&
           pass->length < max_steps) {
      double max_q = -INFINITY;
      Point next = cur;

      for (int a = 0; a < ACTIONS; a++) {
        Point tmp = get_next_point(cur, a);
        if (can_go(labyrinth, cur.row, cur.col, tmp.row, tmp.col)) {
          if (agent->q_table[cur.row][cur.col][a] > max_q) {
            max_q = agent->q_table[cur.row][cur.col][a];
            next = tmp;
          }
        }
      }
      cur = next;
      pass->route[pass->length] = cur;
      pass->length++;
    }

    if (cur.row == agent->goal.row && cur.col == agent->goal.col) {
      result = true;
    } else {
      clear_pass(labyrinth, pass);
      result = false;
    }
  }
  return result;
}

void visualize_q_values(FILE *out, QLearningAgent *agent, const Labyrinth *labyrinth) {
  fprintf(out, "\nQ-values visualization:\n");
  for (int i = 0; i < labyrinth->rows; i++) {
    for (int j = 0; j < labyrinth->cols; j++) {
      double max_q = -INFINITY;
      for (int a = 0; a < ACTIONS; a++) {
        if (agent->q_table[i][j][a] > max_q) {
          max_q = agent->q_table[i][j][a];
        }
      }
      fprintf(out, "%6.1f ", max_q);
    }
    fprintf(out, "\n");
  }
}