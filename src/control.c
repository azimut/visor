#include "control.h"

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

Control control_new(const int cols, const int rows) {
  return (Control){.cols = cols, .rows = rows};
}

void control_update(Control *control, const Control_Move movement) {
  switch (movement) {
  case CONTROL_MOVE_UP: {
    control->ypos = MAX(control->ypos - 1, 0);
    break;
  }
  case CONTROL_MOVE_DOWN: {
    control->ypos = MIN(control->ypos + 1, control->rows - 1);
    break;
  }
  case CONTROL_MOVE_LEFT: {
    control->xpos = MAX(control->xpos - 1, 0);
    break;
  }
  case CONTROL_MOVE_RIGHT: {
    control->xpos = MIN(control->xpos + 1, control->cols - 1);
    break;
  }
  }
  control->idx = control->ypos * control->rows + control->xpos;
}
