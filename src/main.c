#include "cache.h"
#include "filepaths.h"
#include "thumbnail.h"
#include "walktree.h"
#include "window.h"

#include <stdio.h>
#include <unistd.h>

int main(void) {

  Filepaths pdfs = pdfs_find();
  if (pdfs.count == 0) {
    fprintf(stderr, "No .pdf files found in directory :/\n");
    return 1;
  }

  if (thumbnail_init()) {
    fprintf(stderr, "Failed to initialize MagickWand.\n");
    return 1;
  }

  Filepaths thumbnails = filepaths_new();
  for (size_t i = 0; i < pdfs.count; ++i) {
    cache_mkdir_p(pdfs.paths[i]);
    char *thumbnail_path = cache_image_filepath(pdfs.paths[i], ".jpg");
    filepaths_add(&thumbnails, thumbnail_path);
    if (access(thumbnail_path, F_OK))
      thumbnail_create(pdfs.paths[i], thumbnail_path, 0);
    free(thumbnail_path);
  }

  if (window_init()) {
    fprintf(stderr, "Failed to init window.\n");
    return 1;
  }
  int selected_idx = window_draw(thumbnails);
  window_free();
  thumbnail_free();

  if (selected_idx >= 0) {
    printf("Opening %s\n", pdfs.paths[selected_idx]);
    char *args[] = {"xdg-open", pdfs.paths[selected_idx], NULL};
    execvp("xdg-open", args);
  }

  filepaths_free(&pdfs);
  return 0;
}
