#ifndef THUMBNAIL_H
#define THUMBNAIL_H

#include "documents.h"

int thumbnail_init(void);
void thumbnail_free(void);
int thumbnail_create(const Document input_file, const char *output_image);

#endif /* THUMBNAIL_H */
