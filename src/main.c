#include "cache.h"
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
  printf("[INFO] %ld documents found!\n", documents.count);

  Thumbnails thumbnails = thumbnails_from_docs(documents);

  if (window_init()) {
    fprintf(stderr, "Failed to init window.\n");
    return 1;
  }

  int selected_idx = window_draw(thumbnails);
  window_free();

  if (selected_idx >= 0) {
    printf("Opening %s\n", documents.arr[selected_idx].path);
    char *args[] = {"xdg-open", documents.arr[selected_idx].path, NULL};
    execvp("xdg-open", args);
  }

  documents_free(&documents);
  thumbails_free(&thumbnails);

  return 0;
}
