#ifndef WINDOW_H
#define WINDOW_H

#include "documents.h"
#include "thumbnail.h"

int window_init(void);
void window_free(void);
int window_draw(Documents, Thumbnails);

#endif /* WINDOW_H */
