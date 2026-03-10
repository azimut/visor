#include "cache.h"

#include <errno.h>
#include <libgen.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define CACHE_ROOT "/.cache/pdfeye"

static void cache_mkdir(const char *dirpath) {
  const char *homedir = getenv("HOME");
  char *buf = calloc(PATH_MAX, sizeof(char));
  strcat(buf, homedir);
  strcat(buf, CACHE_ROOT);
  strcat(buf, dirpath);
  int err = mkdir(buf, 0755);
  if (!err)
    printf("[INFO] Sucessfully created directory: %s\n", buf);
  else if (errno != EEXIST)
    perror("create_cache_dir");
  free(buf);
}

void cache_mkdir_p(const char *filepath) {
  char *buf = strdup(filepath);
  char *current = buf;
  char *slash;
  while ((slash = strchr(current, '/'))) {
    *slash = '\0';
    cache_mkdir(buf);
    *slash = '/';
    current = slash + 1;
  }
  free(buf);
}

char *cache_image_filepath(const char *filepath, const char *extension) {
  const char *homedir = getenv("HOME");
  char *buf = calloc(PATH_MAX, sizeof(char));
  strcat(buf, homedir);
  strcat(buf, CACHE_ROOT);
  strcat(buf, filepath);
  strcat(buf, extension);
  return buf;
}
