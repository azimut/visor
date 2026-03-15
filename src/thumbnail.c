#include "thumbnail.h"

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

int thumbnail_create(const char *input_pdf, const char *output_image,
                     unsigned int page_number) {

  printf("[INFO] Converting page %d of %s ... ", page_number, input_pdf);
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

  size_t width = MagickGetImageWidth(magick_wand);
  size_t height = MagickGetImageHeight(magick_wand);
  float factor = 0.5;

  MagickAdaptiveResizeImage(magick_wand, width * factor, height * factor);

  // 7. Write the image to a file
  status = MagickWriteImage(magick_wand, output_image);
  if (status == MagickFalse) {
    fprintf(stderr, "[ERROR] Failed to write image to file %s\n", output_image);
  } else {
    puts("DONE!");
  }

  return 0;
}
