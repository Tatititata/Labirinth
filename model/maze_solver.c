#include "model.h"

bool queue_init(Queue *q, int size) {
  q->head = q->tail = 0;
  return create_array(&q->data, size);
}

bool queue_empty(const Queue *q) { return q->head == q->tail; }

void enqueue(Queue *q, Point p) { q->data[q->tail++] = p; }

Point dequeue(Queue *q) { return q->data[q->head++]; }

bool can_go(Labyrinth *labyrinth, int r1, int c1, int r2, int c2) {
  bool result = false;
  if (r2 < 0 || r2 >= labyrinth->rows || c2 < 0 || c2 >= labyrinth->cols)
    result = false;
  else if (r1 == r2 && c1 == c2 + 1)  // left
    result = !labyrinth->v_w[r2][c2];
  else if (r1 == r2 && c1 + 1 == c2)  // right
    result = !labyrinth->v_w[r1][c1];
  else if (r1 + 1 == r2 && c1 == c2)  // down
    result = !labyrinth->h_w[r1][c1];
  else if (r1 == r2 + 1 && c1 == c2)  // up
    result = !labyrinth->h_w[r2][c2];
  return result;
}

bool empty_point(const Point *point) {
  return point->row == -1 && point->col == -1;
}

void bfs(Labyrinth *labyrinth, Point end, Point start, Pass *pass) {
  Point **prev;
  Queue q;
  int size = labyrinth->rows * labyrinth->cols;
  bool result = create_prev_tmp_matrix(&prev, labyrinth->rows, labyrinth->cols);
  result = result && queue_init(&q, size * 2);
  result = result && create_array(&(pass->route), size);
  if (result) {
    enqueue(&q, start);
    prev[start.row][start.col] = start;

    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};
    bool found = false;

    while (!queue_empty(&q) && !found) {
      Point cur = dequeue(&q);
      if (cur.row == end.row && cur.col == end.col) {
        found = true;
      } else {
        for (int d = 0; d < 4; ++d) {
          int nr = cur.row + dr[d], nc = cur.col + dc[d];
          if (can_go(labyrinth, cur.row, cur.col, nr, nc) &&
              empty_point(&prev[nr][nc])) {
            prev[nr][nc] = cur;
            enqueue(&q, (Point){nr, nc});
          }
        }
      }
    }
    pass->length = 0;

    if (found) {
      Point p = end;
      while (!(p.row == start.row && p.col == start.col)) {
        pass->route[pass->length] = p;
        pass->length++;
        p = prev[p.row][p.col];
      }
      pass->route[pass->length] = p;
      pass->length++;
    }
  }
  if (q.data) free(q.data);
  if (prev) free(prev);
}

bool create_prev_tmp_matrix(Point ***prev, int rows, int cols) {
  bool result = false;
  *prev =
      (Point **)malloc(rows * sizeof(Point *) + rows * cols * sizeof(Point));
  if ((*prev)) {
    result = true;
    Point *ptr = (Point *)((*prev) + rows);
    for (int i = 0; i < rows; ++i) {
      (*prev)[i] = ptr + i * cols;
      for (int j = 0; j < cols; ++j) {
        (*prev)[i][j] = (Point){-1, -1};
      }
    }
  }
  return result;
}

bool create_array(Point **ptr, int size) {
  *ptr = (Point *)malloc(size * sizeof(Point));
  return *ptr != NULL;
}

void clear_pass(Labyrinth *labyrinth, Pass *pass) {
  if (pass->route) {
    free(pass->route);
    pass->route = NULL;
  }

  if (labyrinth->pass)
    memset(labyrinth->pass[0], 0, labyrinth->rows * labyrinth->cols * sizeof(int));
}

void create_next_step(Labyrinth *labyrinth, Pass *pass, int i) {
  Point p = pass->route[i];
  labyrinth->pass[p.row][p.col] = 1;
}
