#ifndef THUMBNAIL_H
#define THUMBNAIL_H

#include "documents.h"

typedef struct Thumbnail {
  char *path;
} Thumbnail;

typedef struct Thumbnails {
  Thumbnail *arr;
  size_t capacity;
  size_t count;
} Thumbnails;

Thumbnails thumbnails_from_docs(const Documents);
void thumbails_free(Thumbnails *);

#endif /* THUMBNAIL_H */
