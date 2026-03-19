#include "window.h"
#include "SDL.h"
#include "SDL2_gfxPrimitives.h"
#include "SDL_image.h"
#include "control.h"

#include <stdbool.h>

#define TITLE "visor"
#define WIDTH 640
#define HEIGHT 480
#define POS SDL_WINDOWPOS_UNDEFINED
#define INIT_CAPACITY 10
#define BORDER_THICKNESS 5
#define THUMB_ASPECT 1.1

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

typedef struct Screen {
  int width, height;
  int cols, rows;
  int cellwidth, cellheight;
} Screen;

typedef struct Textures {
  SDL_Texture **texture;
  size_t capacity;
  size_t count;
} Textures;

Textures textures_new(void) {
  SDL_Texture **texture = calloc(INIT_CAPACITY, sizeof(void *));
  return (Textures){.capacity = INIT_CAPACITY, .texture = texture};
}

void textures_add(Textures *textures, const char *imagepath) {
  if (textures->capacity == textures->count) {
    textures->capacity += INIT_CAPACITY;
    textures->texture =
        reallocarray(textures->texture, textures->capacity, sizeof(void *));
  }
  SDL_Texture *new_texture = IMG_LoadTexture(renderer, imagepath);
  textures->texture[textures->count++] = new_texture;
}

Textures textures_from_filepaths(const Filepaths filepaths) {
  Textures textures = textures_new();
  for (size_t i = 0; i < filepaths.count; ++i)
    textures_add(&textures, filepaths.files[i].path);
  return textures;
}

void textures_free(Textures *textures) {
  for (size_t i = 0; i < textures->count; ++i)
    SDL_DestroyTexture(textures->texture[i]);
  free(textures->texture);
  textures->capacity = 0;
  textures->count = 0;
}

int window_init(void) {
  if (SDL_Init(SDL_INIT_VIDEO)) {
    SDL_LogError(SDL_LOG_PRIORITY_ERROR, "Starting SDL...");
    return 1;
  }
  window =
      SDL_CreateWindow(TITLE, POS, POS, -1, -1, SDL_WINDOW_FULLSCREEN_DESKTOP);
  if (!window) {
    SDL_LogError(SDL_LOG_PRIORITY_ERROR, "Creating window");
    return 1;
  }
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer) {
    SDL_LogError(SDL_LOG_PRIORITY_ERROR, "Creating renderer");
    return 1;
  }
  return 0;
}

static SDL_Point texture_size(SDL_Texture *texture) {
  SDL_Point size;
  SDL_QueryTexture(texture, NULL, NULL, &size.x, &size.y);
  return size;
}

static SDL_Point thumbnail_size(SDL_Texture *texture, const Screen screen) {
  SDL_Point result;
  const SDL_Point texsize = texture_size(texture);
  if (texsize.x > texsize.y) {
    const double width = (double)screen.cellwidth / THUMB_ASPECT;
    const double aspect = ((double)texsize.y / texsize.x);
    result.x = width;
    result.y = width * aspect;

  } else {
    const double height = (double)screen.cellheight / THUMB_ASPECT;
    const double aspect = ((double)texsize.x / texsize.y);
    result.x = height * aspect;
    result.y = height;
  }
  return result;
}

static SDL_Rect thumbnail_rect(const size_t idx, const Screen screen,
                               SDL_Texture *texture) {
  const SDL_Point thumbsize = thumbnail_size(texture, screen);
  const double xcoord = SDL_fmod(idx, screen.cols);
  const double ycoord = SDL_floor((double)idx / screen.cols);
  const int xorig = xcoord * screen.cellwidth;
  const int yorig = ycoord * screen.cellheight;
  return (SDL_Rect){.x = xorig + ((screen.cellwidth - thumbsize.x) / 2),
                    .y = yorig + ((screen.cellheight - thumbsize.y) / 2),
                    .w = thumbsize.x,
                    .h = thumbsize.y};
}

static void window_draw_control(SDL_Rect thumb_rect) {
  const int r = 0, g = 255, b = 0;
  const int tlx = thumb_rect.x;
  const int tly = thumb_rect.y;
  const int trx = tlx + thumb_rect.w;
  const int bry = thumb_rect.y + thumb_rect.h;
  thickLineRGBA(renderer, tlx, tly, trx, tly, BORDER_THICKNESS, r, g, b, 255);
  thickLineRGBA(renderer, trx, tly, trx, bry, BORDER_THICKNESS, r, g, b, 255);
  thickLineRGBA(renderer, trx, bry, tlx, bry, BORDER_THICKNESS, r, g, b, 255);
  thickLineRGBA(renderer, tlx, bry, tlx, tly, BORDER_THICKNESS, r, g, b, 255);
}

// Returns the selected index, or -1
int window_draw(const Filepaths filepaths) {
  int selected_idx = -1;
  const int nrows = SDL_ceil(SDL_sqrt(filepaths.count));
  const int ncols = SDL_ceil(SDL_sqrt(filepaths.count));
  SDL_Rect dims;
  SDL_GetDisplayBounds(0, &dims);
  const Screen screen = (Screen){.width = dims.w,
                                 .height = dims.h,
                                 .cols = ncols,
                                 .rows = nrows,
                                 .cellwidth = dims.w / ncols,
                                 .cellheight = dims.h / nrows};
  Control control = control_new(screen.cols, screen.rows);
  Textures textures = textures_from_filepaths(filepaths);
  SDL_Log("screen.cols = %d   screen.rows = %d", screen.cols, screen.rows);
  SDL_Log("filepaths.count = %ld", filepaths.count);
  SDL_Log("textures.count = %ld", textures.count);
  bool quit = false;
  while (!quit) {
    SDL_Event e;
    SDL_WaitEvent(&e);
    if (e.type == SDL_QUIT) {
      quit = true;
    }
    if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
      case SDLK_ESCAPE:
      case SDLK_q: {
        quit = true;
        break;
      }
      case SDLK_h: {
        control_update(&control, CONTROL_MOVE_LEFT, filepaths.count);
        break;
      }
      case SDLK_j: {
        control_update(&control, CONTROL_MOVE_DOWN, filepaths.count);
        break;
      }
      case SDLK_k: {
        control_update(&control, CONTROL_MOVE_UP, filepaths.count);
        break;
      }
      case SDLK_l: {
        control_update(&control, CONTROL_MOVE_RIGHT, filepaths.count);
        break;
      }
      case SDLK_RETURN: {
        if (control.idx > filepaths.count - 1)
          break;
        selected_idx = control.idx;
        quit = true;
        break;
      }
      default:
        break;
      }
    }
    SDL_SetRenderDrawColor(renderer, 0x0A, 0x0A, 0x0A, 0xFF);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
    for (size_t idx = 0; idx < textures.count; ++idx) {
      SDL_Texture *texture = textures.texture[idx];
      SDL_Rect rect = thumbnail_rect(idx, screen, texture);
      SDL_RenderCopy(renderer, texture, NULL, &rect);
      if (idx == control.idx)
        window_draw_control(rect);
    }
    SDL_RenderPresent(renderer);
  }
  textures_free(&textures);
  return selected_idx;
}

void window_free(void) {
  if (!window)
    return;
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  window = NULL, renderer = NULL;
}
