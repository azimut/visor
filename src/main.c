#include "cache.h"
#include "list.h"
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
  List *thumbnails = NULL;
  Pdfs pdfs = pdfs_find();
  for (size_t i = 0; i < pdfs.count; ++i) {
    puts(pdfs.pdfs[i].filepath);
    cache_mkdir_p(pdfs.pdfs[i].filepath);
    puts(cache_image_filepath(pdfs.pdfs[i].filepath, ".png"));
    char *thumbnail_path = cache_image_filepath(pdfs.pdfs[i].filepath, ".png");

    if (!thumbnails)
      thumbnails = list_new(thumbnail_path);
    else
      list_add(thumbnails, thumbnail_path);

    if (access(thumbnail_path, F_OK)) {
      thumbnail_create(pdfs.pdfs[i].filepath, thumbnail_path, 0);
    }
    free(thumbnail_path);
  }
  window_draw(*thumbnails);
  pdfs_free();
  thumbnail_free();
  window_free();
  return 0;
}
