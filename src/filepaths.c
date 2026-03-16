#include "filepaths.h"
#include <string.h>

#define INIT_CAPACITY 10

Filepaths filepaths_new(void) {
  return ((Filepaths){.capacity = INIT_CAPACITY,
                      .paths = calloc(INIT_CAPACITY, sizeof(void *))});
}

void filepaths_add(Filepaths *filepaths, const char *path) {
  if (filepaths->capacity == filepaths->count) {
    filepaths->capacity += INIT_CAPACITY;
    filepaths->paths =
        reallocarray(filepaths->paths, filepaths->capacity, sizeof(void *));
  }
  filepaths->paths[filepaths->count++] = strdup(path);
}

void filepaths_free(Filepaths *filepaths) {
  if (!filepaths || !filepaths->paths)
    return;
  for (size_t i = 0; i < filepaths->capacity; ++i)
    free(filepaths->paths[i]);
  filepaths->capacity = 0;
  filepaths->count = 0;
  filepaths->paths = NULL;
}
