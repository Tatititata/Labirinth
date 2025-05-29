#ifndef Q_LEARNING_H
#define Q_LEARNING_H

#include <math.h>
#include <time.h>

#include "../model/model.h"

#define ACTIONS 4
#define ALPHA 0.1    // learning rate
#define GAMMA 0.95   // discount factor
#define EPSILON 0.1  // exploration rate
#define MAX_STEP_PER_EPISODE 10000
#define REWARD 500.0
#define MAX_EPISODES 100000

typedef struct {
  double q_table[50][50][ACTIONS];
  Point goal;
  int initialized;
} QLearningAgent;

void q_agent_init(QLearningAgent *agent, Point goal);
void q_agent_train(QLearningAgent *agent, Maze *maze);
Point q_agent_choose_action(QLearningAgent *agent, Point cur, Maze *maze,
                            int *action);
void q_agent_update(QLearningAgent *agent, Point current, int action,
                    Point next, double reward);
bool q_agent_find_path(QLearningAgent *agent, Maze *maze, Point start,
                       Pass *pass);
void visualize_q_values(QLearningAgent *agent, Maze *maze);

#endif
