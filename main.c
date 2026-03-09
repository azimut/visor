#include "walktree.h"
#include <stdio.h>

int main(void) {
  Pdfs pdfs = pdfs_find();
  for (size_t i = 0; i < pdfs.count; ++i) {
    puts(pdfs.pdfs[i].filepath);
  }
  pdfs_free();
  return 0;
}
