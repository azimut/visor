#ifndef LIST_H
#define LIST_H

typedef struct List {
  char *filename;
  struct List *next;
} List;

List *list_new(const char *filename);
void list_add(List *, const char *filename);
void list_free(List *);

#endif /* LIST_H */
