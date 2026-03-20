#ifndef CONTROL_H
#define CONTROL_H

#include "SDL.h"

typedef struct Control {
  int xpos, ypos;
  const int ncols, nrows;
  size_t idx;
} Control;

typedef enum {
  CONTROL_MOVE_UP = 0,
  CONTROL_MOVE_RIGHT,
  CONTROL_MOVE_DOWN,
  CONTROL_MOVE_LEFT,
} Control_Move;

Control control_new(const int ncols, const int nrows);
void control_update(Control *, const Control_Move, const size_t total_size);

#endif /* CONTROL_H */
