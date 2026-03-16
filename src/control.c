#include "control.h"

Control control_new(const int cols, const int rows) {
  return (Control){.cols = cols, .rows = rows};
}

void control_update(Control *control, const Control_Move movement,
                    const size_t count) {
  int new_xpos = control->xpos;
  int new_ypos = control->ypos;
  switch (movement) {
  case CONTROL_MOVE_UP: {
    new_ypos = SDL_max(control->ypos - 1, 0);
    break;
  }
  case CONTROL_MOVE_DOWN: {
    new_ypos = SDL_min(control->ypos + 1, control->rows - 1);
    break;
  }
  case CONTROL_MOVE_LEFT: {
    new_xpos = SDL_max(control->xpos - 1, 0);
    break;
  }
  case CONTROL_MOVE_RIGHT: {
    new_xpos = SDL_min(control->xpos + 1, control->cols - 1);
    break;
  }
  }
  const size_t new_idx = new_ypos * control->rows + new_xpos;
  if (new_idx > count - 1) {
    control->idx = count - 1;
    control->xpos = SDL_fmod(count - 1, control->cols);
    control->ypos = SDL_floor((count - 1) / control->cols);
  } else {
    control->idx = new_idx;
    control->xpos = new_xpos;
    control->ypos = new_ypos;
  }
}
