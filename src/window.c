#include "window.h"
#include "SDL.h"
#include "SDL_image.h"

#include <stdbool.h>

#define TITLE "pdfeye"
#define WIDTH 320
#define HEIGHT 240
#define COLS 2
#define ROWS 2
#define POS SDL_WINDOWPOS_UNDEFINED
#define INIT_CAPACITY 10

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

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
    textures_add(&textures, filepaths.paths[i]);
  return textures;
}

void textures_free(Textures *textures) {
  for (size_t i = 0; i < textures->count; ++i)
    SDL_DestroyTexture(textures->texture[i]);
  free(textures->texture);
  textures->capacity = 0;
  textures->count = 0;
}

static SDL_Rect thumbnail_size(size_t idx) {
  const int w = WIDTH / COLS;
  const int h = HEIGHT / ROWS;
  const double xcoord = SDL_fmod(idx, COLS);
  const double ycoord = SDL_floor(idx / COLS);
  const int x = xcoord * w;
  const int y = ycoord * h;
  return (SDL_Rect){.x = x, .y = y, .w = w, .h = h};
}

int window_init(void) {
  if (SDL_Init(SDL_INIT_VIDEO)) {
    SDL_LogError(SDL_LOG_PRIORITY_ERROR, "Starting SDL...");
    return 1;
  }
  window = SDL_CreateWindow(TITLE, POS, POS, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
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

void window_draw(const Filepaths filepaths) {
  Textures textures = textures_from_filepaths(filepaths);
  bool quit = false;
  while (!quit) {
    SDL_Event e;
    SDL_WaitEvent(&e);
    if (e.type == SDL_QUIT) {
      quit = true;
    }
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
    for (size_t i = 0; i < textures.count; ++i) {
      SDL_Rect rect = thumbnail_size(i);
      SDL_Texture *texture = textures.texture[i];
      SDL_RenderCopy(renderer, texture, NULL, &rect);
    }
    SDL_RenderPresent(renderer);
  }

  textures_free(&textures);
  return;
}

void window_free(void) {
  if (!window)
    return;
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  window = NULL, renderer = NULL;
}
