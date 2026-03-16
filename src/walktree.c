#define _GNU_SOURCE 1
#define _XOPEN_SOURCE 500

#include "walktree.h"

#include <ftw.h>
#include <libgen.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Filepaths pdfs = {0};

static char *file_extension(const char *filepath) {
  char *buf1 = strndup(filepath, 999);
  char *filename = basename(buf1);
  char *extension = strrchr(filename, '.');
  return extension == filename ? NULL : extension;
}

static int callback(const char *filepath,
                    __attribute__((unused)) const struct stat *sb, int tflag,
                    struct FTW *ftwbuf) {
  const bool is_directory = tflag == FTW_D;
  const char *extension = file_extension(filepath);

  if (is_directory && strcmp(filepath, ".")) {
    return 1;
  }

  if (is_directory || !extension) {
    return 0;
  }

  const int is_pdf = !strcasecmp(extension, ".pdf");
  const int is_epub = !strcasecmp(extension, ".epub");
  if ((!is_pdf && !is_epub)) {
    return 0;
  }

  if (ftwbuf->level > 1) {
    return 0;
  }

  char *buf = realpath(filepath, NULL);
  filepaths_add(&pdfs, buf);
  free(buf);
  return 0;
}

Filepaths pdfs_find(void) {
  pdfs = filepaths_new();
  nftw(".", callback, 10, FTW_DEPTH);
  return pdfs;
}
