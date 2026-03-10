#include "list.h"
#include <limits.h>
#include <stdlib.h>
#include <string.h>

List *list_new(const char *src_filename) {
  size_t strlen_src = strlen(src_filename);
  char *filename = calloc(strlen_src + 1, sizeof(char));
  strcpy(filename, src_filename);
  List *list = calloc(1, sizeof(List));
  list->filename = filename;
  return list;
}

void list_add(List *list, const char *filename) {
  if (list->next)
    list_add(list->next, filename);
  else
    list->next = list_new(filename);
}

void list_free(List *list) {
  free(list->filename);
  list->filename = NULL;
  if (list->next)
    list_free(list->next);
}
