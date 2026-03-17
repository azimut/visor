#define _GNU_SOURCE

#include "filepaths.h"
#include <stdio.h>
#include <string.h>

#define INIT_CAPACITY 10

Filepaths filepaths_new(void) {
  return (Filepaths){.capacity = INIT_CAPACITY,
                     .files = calloc(INIT_CAPACITY, sizeof(File))};
}

void filepaths_add(Filepaths *filepaths, const char *path) {
  if (filepaths->capacity == filepaths->count) {
    filepaths->capacity += INIT_CAPACITY;
    filepaths->files =
        reallocarray(filepaths->files, filepaths->capacity, sizeof(File));
  }
  const size_t idx = filepaths->count;
  filepaths->files[idx].path = strdup(path);
  if (strcasestr(path, ".pdf"))
    filepaths->files[idx].extension = EXTENSION_PDF;
  else if (strcasestr(path, ".epub"))
    filepaths->files[idx].extension = EXTENSION_EPUB;
  else if (strcasestr(path, ".jpg") || strcasestr(path, ".png"))
    filepaths->files[idx].extension = EXTENSION_IMAGE;
  else
    filepaths->files[idx].extension = EXTENSION_UNKNOWN;
  filepaths->count++;
}

void filepaths_free(Filepaths *filepaths) {
  if (!filepaths || !filepaths->files)
    return;
  for (size_t i = 0; i < filepaths->capacity; ++i)
    free(filepaths->files[i].path);
  filepaths->capacity = 0;
  filepaths->count = 0;
  filepaths->files = NULL;
}
