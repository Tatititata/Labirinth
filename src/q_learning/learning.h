#ifndef Q_LEARNING_H
#define Q_LEARNING_H

#include <math.h>
#include <time.h>

#include "../model/model.h"

#define ACTIONS 4
#define ALPHA 0.1    // learning rate
#define GAMMA 0.95   // discount factor
#define EPSILON 0.1  // exploration rate

#ifdef TESTING

#define MAX_STEP_PER_EPISODE 100
#define MAX_EPISODES 100
#define REWARD 100.0

#else

#define MAX_STEP_PER_EPISODE 100000
#define MAX_EPISODES 100000
#define REWARD 35500.0

#endif

typedef struct {
  double q_table[50][50][ACTIONS];
  Point goal;
} QLearningAgent;

void q_agent_init(QLearningAgent *agent, Point goal);
void q_agent_train(QLearningAgent *agent, Maze *maze);
Point q_agent_choose_action(QLearningAgent *agent, Point cur, Maze *maze,
                            int *action);
void q_agent_update(QLearningAgent *agent, Point cur, int action, Point next,
                    double reward);
bool q_agent_find_path(QLearningAgent *agent, Maze *maze, Point start,
                       Pass *pass);
void visualize_q_values(FILE *out, QLearningAgent *agent, const Maze *maze);
Point get_next_point(Point cur, int action);

#endif
