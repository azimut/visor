#include "walktree.h"

#include <ftw.h>
#include <libgen.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define INIT_SIZE_PDFS 10
#define INIT_SIZE_FILENAME 100

Pdfs pdfs = {0};

void pdfs_free(void) {
  if (pdfs.pdfs == NULL) {
    return;
  }
  for (size_t i = 0; i < pdfs.count; ++i) {
    free(pdfs.pdfs[i].filepath);
  }
  free(pdfs.pdfs);
  pdfs.pdfs = NULL;
}

static void pdfs_init(void) {
  if (pdfs.pdfs != NULL) {
    return;
  }
  pdfs.pdfs = malloc(sizeof(Pdf) * INIT_SIZE_PDFS);
  pdfs.capacity = INIT_SIZE_PDFS;
}

static void pdfs_add(const char *filepath, long filesize, long mtime) {
  if (pdfs.capacity < pdfs.count + 1) {
    size_t new_capacity = pdfs.capacity + INIT_SIZE_PDFS;
    pdfs.pdfs = reallocarray(pdfs.pdfs, sizeof(Pdf), new_capacity);
    pdfs.capacity = new_capacity;
  }
  pdfs.pdfs[pdfs.count].filepath = malloc(sizeof(char) * INIT_SIZE_FILENAME);
  strcpy(pdfs.pdfs[pdfs.count].filepath, filepath);
  pdfs.pdfs[pdfs.count].filesize = filesize;
  pdfs.pdfs[pdfs.count].mtime = mtime;
  pdfs.count++;
}

static char *file_extension(const char *filepath) {
  char *buf1 = strndup(filepath, 999);
  char *filename = basename(buf1);
  char *extension = strrchr(filename, '.');
  return extension == filename ? NULL : extension;
}

static int callback(const char *filepath, const struct stat *sb, int tflag) {
  const bool is_directory = tflag == FTW_D;
  const char *extension = file_extension(filepath);

  if (is_directory && strcmp(filepath, ".")) {
    return 1;
  }
  if (is_directory || !extension || strcmp(extension, ".pdf")) {
    return 0;
  }
  pdfs_add(filepath, sb->st_size, sb->st_mtim.tv_sec);
  return 0;
}

Pdfs pdfs_find(void) {
  pdfs_init();
  ftw(".", callback, 10);
  return pdfs;
}
