#ifndef CONTROL_H
#define CONTROL_H

#include "SDL.h"

typedef struct Control {
  int xpos, ypos;
  int cols, rows;
} Control;

typedef enum {
  CONTROL_MOVE_UP = 0,
  CONTROL_MOVE_RIGHT,
  CONTROL_MOVE_DOWN,
  CONTROL_MOVE_LEFT,
} Control_Move;

Control control_new(const int cols, const int rows);
void control_update(Control *, const Control_Move);

#endif /* CONTROL_H */
