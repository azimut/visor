#include "cache.h"
#include "walktree.h"

#include <stdio.h>

int main(void) {
  Pdfs pdfs = pdfs_find();
  for (size_t i = 0; i < pdfs.count; ++i) {
    puts(pdfs.pdfs[i].filepath);
    cache_mkdir_p(pdfs.pdfs[i].filepath);
    puts(cache_image_filepath(pdfs.pdfs[i].filepath, ".png"));
  }
  pdfs_free();
  return 0;
}
