#ifndef WINDOW_H
#define WINDOW_H

#include "list.h"

int window_init(void);
void window_free(void);
void window_draw(const List);

#endif /* WINDOW_H */
