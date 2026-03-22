#include "control.h"

Control
control_new(const int ncols, const int nrows)
{
  return (Control){.ncols = ncols, .nrows = nrows};
}

void
control_update(Control *control, const Control_Move movement, const size_t total_size)
{
  int new_xpos = control->xpos;
  int new_ypos = control->ypos;
  switch (movement) {
  case CONTROL_MOVE_UP: {
    new_ypos = SDL_max(control->ypos - 1, 0);
    break;
  }
  case CONTROL_MOVE_DOWN: {
    new_ypos = SDL_min(control->ypos + 1, control->nrows - 1);
    break;
  }
  case CONTROL_MOVE_LEFT: {
    new_xpos = control->xpos - 1;
    if (new_xpos < 0) {
      if (new_ypos == 0) {
        new_xpos = 0;
      } else {
        new_xpos = control->ncols - 1;
        new_ypos -= 1;
      }
    }
    break;
  }
  case CONTROL_MOVE_RIGHT: {
    new_xpos = control->xpos + 1;
    if (new_xpos > control->ncols - 1) {
      new_xpos = 0;
      new_ypos += 1;
    }
    break;
  }
  }
  const size_t new_idx = new_ypos * control->ncols + new_xpos;
  const size_t max_idx = total_size - 1;
  if (new_idx > max_idx) {
    control->idx = max_idx;
    control->xpos = SDL_fmod(max_idx, control->ncols);
    control->ypos = SDL_floor((double)max_idx / control->ncols);
  } else {
    control->idx = new_idx;
    control->xpos = new_xpos;
    control->ypos = new_ypos;
  }
}
