#include "cache.h"
#include "filepaths.h"
#include "thumbnail.h"
#include "walktree.h"
#include "window.h"

#include <stdio.h>
#include <unistd.h>

int main(void) {
  if (thumbnail_init()) {
    return 1;
  }
  if (window_init()) {
    return 1;
  }
  Filepaths thumbnails = filepaths_new();
  Filepaths pdfs = pdfs_find();
  for (size_t i = 0; i < pdfs.count; ++i) {
    puts("-----");
    puts(pdfs.paths[i]);
    cache_mkdir_p(pdfs.paths[i]);
    puts(cache_image_filepath(pdfs.paths[i], ".jpg"));
    char *thumbnail_path = cache_image_filepath(pdfs.paths[i], ".jpg");

    filepaths_add(&thumbnails, thumbnail_path);

    if (access(thumbnail_path, F_OK)) {
      thumbnail_create(pdfs.paths[i], thumbnail_path, 0);
    }
    free(thumbnail_path);
  }
  window_draw(thumbnails);
  thumbnail_free();
  window_free();
  filepaths_free(&pdfs);
  return 0;
}
