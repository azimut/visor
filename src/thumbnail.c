#include <string.h>
#include <unzip.h>
#include <wand/MagickWand.h>

#include "thumbnail.h"

#define BUF_SIZE 8192
#define IMG_SIZE 500

MagickWand *magick_wand = NULL;

int thumbnail_init(void) {
  if (magick_wand) {
    fprintf(stderr, "[ERROR] Already created wand!\n");
    return 1;
  }
  MagickWandGenesis();
  magick_wand = NewMagickWand();
  if (!magick_wand) {
    fprintf(stderr, "[ERROR] Failed to create MagickWand\n");
    return 1;
  }
  MagickSetImageResolution(magick_wand, 300, 300);
  return 0;
}

void thumbnail_free(void) {
  if (!magick_wand)
    return;
  DestroyMagickWand(magick_wand);
  magick_wand = NULL;
  MagickWandTerminus();
}

static int thumbnail_create_pdf(const char *input_pdf, const char *output_image,
                                unsigned int page_number) {
  printf("[INFO] Converting page %d of %s ... ", page_number, input_pdf);
  fflush(stdout);
  MagickBooleanType status;

  // 4. Read the specific page from the PDF file.
  // The format "filename[page_number]" is used to specify the page index.
  // Note: MagickReadImage might set the pointer to the last page if not handled
  // properly in older versions but specifying the index in the filename
  // argument generally works for single page reads.
  char filename_with_page[256];
  sprintf(filename_with_page, "%s[%d]", input_pdf, page_number);
  MagickSetOption(magick_wand, "pdf:use-cropbox", "true");
  status = MagickReadImage(magick_wand, filename_with_page);

  if (status == MagickFalse) {
    fprintf(stderr, "[ERROR] Failed to read image from PDF page %d\n",
            page_number);
    // Error handling code here to get the exception description if needed
    DestroyMagickWand(magick_wand);
    MagickWandTerminus();
    return 1;
  }

  MagickTransformImageColorspace(magick_wand, sRGBColorspace);
  MagickSetImageAlphaChannel(magick_wand, RemoveAlphaChannel);

  // 5. Set the output image format
  MagickSetImageFormat(magick_wand, "jpg");

  // 6. Set image compression quality (optional, for formats like JPEG)
  MagickSetImageCompressionQuality(magick_wand, 90);

  const size_t orig_width = MagickGetImageWidth(magick_wand);
  const size_t orig_height = MagickGetImageHeight(magick_wand);
  size_t width, height;
  if (orig_width > orig_height) {
    width = IMG_SIZE;
    height = IMG_SIZE * (double)orig_height / orig_width;
  } else {
    width = IMG_SIZE * (double)orig_width / orig_height;
    height = IMG_SIZE;
  }
  MagickAdaptiveResizeImage(magick_wand, width, height);

  // 7. Write the image to a file
  status = MagickWriteImage(magick_wand, output_image);
  if (status == MagickFalse) {
    fprintf(stderr, "[ERROR] Failed to write image to file %s\n", output_image);
  } else {
    puts("DONE!");
  }

  return 0;
}

static int thumbnail_create_epub(const char *input_epub,
                                 const char *output_image) {
  unzFile zip = unzOpen(input_epub);
  if (!zip) {
    fprintf(stderr, "Cannot open epub: %s", input_epub);
    return -1;
  }
  if (unzGoToFirstFile(zip) != UNZ_OK) {
    fprintf(stderr, "Empty epub?: %s", input_epub);
    unzClose(zip);
    return -1;
  }

  do {
    char filename[256];
    unz_file_info info;

    if (unzGetCurrentFileInfo(zip, &info, filename, sizeof(filename), NULL, 0,
                              NULL, 0) != UNZ_OK)
      break;

    if (strstr(filename, "cover") &&
        (strstr(filename, ".jpg") || strstr(filename, ".jpeg") ||
         strstr(filename, ".png"))) {
      if (unzOpenCurrentFile(zip) != UNZ_OK)
        break;

      FILE *out = fopen(output_image, "wb");
      if (!out) {
        unzCloseCurrentFile(zip);
        break;
      }

      char buf[BUF_SIZE];
      int bytes;

      while ((bytes = unzReadCurrentFile(zip, buf, BUF_SIZE)) > 0)
        fwrite(buf, 1, bytes, out);

      fclose(out);
      unzCloseCurrentFile(zip);
      unzClose(zip);

      return 0;
    }

  } while (unzGoToNextFile(zip) == UNZ_OK);

  unzClose(zip);
  return -1;
}

int thumbnail_create(const Document input_file, const char *output_image) {
  switch (input_file.format) {
  case FORMAT_PDF:
    return thumbnail_create_pdf(input_file.path, output_image, 0);
  case FORMAT_EPUB:
    return thumbnail_create_epub(input_file.path, output_image);
  }
  return -1;
}
