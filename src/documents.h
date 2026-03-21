#ifndef DOCUMENTS_H
#define DOCUMENTS_H

#include <stdlib.h>

typedef enum {
  FORMAT_PDF = 0,
  FORMAT_EPUB,
  FORMAT_PPT,
  FORMAT_DOC,
} Format;

typedef struct Document {
  char *path;
  Format format;
} Document;

typedef struct Documents {
  Document *arr;
  size_t capacity;
  size_t count;
} Documents;

Document *document_new(const char *path);
void document_free(Document *);

Documents documents_new(void);
void documents_add(Documents *, const Document);
void documents_free(Documents *);

#endif /* DOCUMENTS_H */
