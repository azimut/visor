#include "cache.h"
#include "thumbnail.h"
#include "walktree.h"

#include <stdio.h>
#include <unistd.h>

int main(void) {
  if (thumbnail_init()) {
    return 1;
  }
  Pdfs pdfs = pdfs_find();
  for (size_t i = 0; i < pdfs.count; ++i) {
    puts(pdfs.pdfs[i].filepath);
    cache_mkdir_p(pdfs.pdfs[i].filepath);
    puts(cache_image_filepath(pdfs.pdfs[i].filepath, ".png"));
    char *thumbnail_path = cache_image_filepath(pdfs.pdfs[i].filepath, ".png");
    if (access(thumbnail_path, F_OK)) {
      thumbnail_create(pdfs.pdfs[i].filepath, thumbnail_path, 0);
    }
    free(thumbnail_path);
  }
  pdfs_free();
  thumbnail_free();
  return 0;
}
