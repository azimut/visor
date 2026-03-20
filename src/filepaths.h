#ifndef FILEPATHS_H
#define FILEPATHS_H

#include <stdlib.h>

typedef struct File {
  char *path;
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
