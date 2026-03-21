#include "documents.h"

#include <string.h>

#define INIT_CAPACITY 10

Document *document_new(const char *rawpath) {
  const char *extension = strrchr(rawpath, '.');
  if (!extension)
    return NULL;

  Format format;
  if (!strcasecmp(extension, ".pdf"))
    format = FORMAT_PDF;
  else if (!strcasecmp(extension, ".epub"))
    format = FORMAT_EPUB;
  else if (!strcasecmp(extension, ".ppt"))
    format = FORMAT_PPT;
  else if (!strcasecmp(extension, ".doc"))
    format = FORMAT_DOC;
  else
    return NULL;

  char *path = realpath(rawpath, NULL);
  if (!path)
    return NULL;

  Document *document = calloc(1, sizeof(Document));
  document->format = format;
  document->path = path;
  return document;
}

void document_free(Document *doc) {
  free(doc->path);
  free(doc);
  doc = NULL;
}

Documents documents_new(void) {
  return (Documents){.arr = calloc(INIT_CAPACITY, sizeof(Document)),
                     .capacity = INIT_CAPACITY};
}

void documents_add(Documents *docs, const Document doc) {
  if (docs->capacity == docs->count) {
    docs->capacity += INIT_CAPACITY;
    docs->arr = reallocarray(docs->arr, docs->capacity, sizeof(Document));
  }
  docs->arr[docs->count].format = doc.format;
  docs->arr[docs->count].path = strdup(doc.path);
  docs->count++;
}

void documents_free(Documents *docs) {
  for (size_t i = 0; i < docs->capacity; ++i) {
    free(docs->arr[i].path);
  }
  free(docs->arr);
  docs = NULL;
}
