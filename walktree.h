#ifndef WALKTREE_H
#define WALKTREE_H

#include <stdlib.h>

typedef struct Pdf {
  char *filepath;
  long filesize;
  long mtime;
} Pdf;

typedef struct Pdfs {
  Pdf *pdfs;
  size_t count;
  size_t capacity;
} Pdfs;

Pdfs pdfs_find(void);
void pdfs_free(void);

#endif /* WALKTREE_H */
