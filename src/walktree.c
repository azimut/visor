#define _GNU_SOURCE 1
#define _XOPEN_SOURCE 500

#include "walktree.h"

#include <dirent.h>
#include <ftw.h>
#include <stdbool.h>
#include <string.h>

Documents pdfs = {0};

static int
callback(const char *filepath, __attribute__((unused)) const struct stat *sb, int tflag, struct FTW *ftwbuf)
{
  const bool is_directory = tflag == FTW_D;

  if (is_directory && strcmp(filepath, ".")) {
    return 1;
  }

  if (is_directory) {
    return 0;
  }

  if (ftwbuf->level > 1) {
    return 0;
  }

  Document *doc = document_new(filepath);
  if (doc) {
    documents_add(&pdfs, *doc);
  }
  free(doc);

  return 0;
}

Documents
find_documents(const unsigned int depth)
{
  pdfs = documents_new();
  if (depth == 0) {
    DIR *dir = opendir(".");
    struct dirent *current;
    while ((current = readdir(dir))) {
      if (!(current->d_type == DT_REG || current->d_type == DT_LNK)) {
        continue;
      }
      struct stat st_target;
      if (stat(current->d_name, &st_target) == -1) {
        continue;
      }
      Document *doc = document_new(current->d_name);
      if (doc) {
        documents_add(&pdfs, *doc);
      }
      free(doc);
    }
    closedir(dir);
  } else {
    nftw(".", callback, 10, FTW_DEPTH);
  }
  documents_sort(&pdfs);
  return pdfs;
}
