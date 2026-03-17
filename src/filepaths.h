#ifndef FILEPATHS_H
#define FILEPATHS_H

#include <stdlib.h>

typedef enum {
  EXTENSION_PDF = 0,
  EXTENSION_EPUB,
  EXTENSION_IMAGE,
  EXTENSION_UNKNOWN
} Extension;

typedef struct File {
  char *path;
  Extension extension;
} File;

typedef struct Files {
  File *files;
  size_t capacity;
  size_t count;
} Filepaths;

Filepaths filepaths_new(void);
void filepaths_add(Filepaths *, const char *);
void filepaths_free(Filepaths *);

#endif /* FILEPATHS_H */
