#ifndef THUMBNAIL_H
#define THUMBNAIL_H
#include <wand/MagickWand.h>

int thumbnail_init(void);
void thumbnail_free(void);
int thumbnail_create(const char *input_file, const char *output_image);

#endif /* THUMBNAIL_H */
