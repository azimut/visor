#include "cache.h"
#include "filepaths.h"
#include "thumbnail.h"
#include "walktree.h"
#include "window.h"

#include <stdio.h>
#include <unistd.h>

int main(void) {

  Documents documents = find_documents(0);
  if (documents.count == 0) {
    fprintf(stderr, "No documents found in directory :/\n");
    return 1;
  }

  if (thumbnail_init()) {
    fprintf(stderr, "Failed to initialize MagickWand.\n");
    return 1;
  }

  Filepaths thumbnails = filepaths_new();
  for (size_t i = 0; i < documents.count; ++i) {
    const char *document = documents.arr[i].path;
    cache_mkdir_p(document);
    char *thumbnail_path = cache_image_filepath(document, ".jpg");
    filepaths_add(&thumbnails, thumbnail_path);
    if (access(thumbnail_path, F_OK))
      thumbnail_create(documents.arr[i], thumbnail_path);
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
    printf("Opening %s\n", documents.arr[selected_idx].path);
    char *args[] = {"xdg-open", documents.arr[selected_idx].path, NULL};
    execvp("xdg-open", args);
  }

  documents_free(&documents);
  return 0;
}
