#ifndef WINDOW_H
#define WINDOW_H

#include "thumbnail.h"

int window_init(void);
void window_free(void);
int window_draw(Thumbnails);

#endif /* WINDOW_H */
