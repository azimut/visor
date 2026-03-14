#include "window.h"
#include "SDL.h"
#include "SDL2_gfxPrimitives.h"
#include "SDL_image.h"
#include "control.h"

#include <stdbool.h>

#define TITLE "pdfeye"
#define WIDTH 640
#define HEIGHT 480
#define COLS 5
#define ROWS 5
#define POS SDL_WINDOWPOS_UNDEFINED
#define INIT_CAPACITY 10
#define BORDER_THICKNESS 10

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

static SDL_Rect thumbnail_rect(size_t idx) {
  const int w = WIDTH / COLS;
  const int h = HEIGHT / ROWS;
  const double xcoord = SDL_fmod(idx, COLS);
  const double ycoord = SDL_floor(idx / COLS);
  const int x = xcoord * w;
  const int y = ycoord * h;
  return (SDL_Rect){.x = x, .y = y, .w = w, .h = h};
}

static void window_draw_control(const Control control) {
  const int w = WIDTH / COLS;
  const int h = HEIGHT / ROWS;
  const int xtl = control.xpos * w, ytl = control.ypos * h;
  const int xtr = xtl + w, ytr = ytl;
  const int xbr = xtr, ybr = ytr + h;
  const int xbl = xtl, ybl = ybr;
  const int r = 200, b = 20, g = 100;
  thickLineRGBA(renderer, xtl, ytl, xtr, ytr, BORDER_THICKNESS, r, g, b, 255);
  thickLineRGBA(renderer, xtr, ytr, xbr, ybr, BORDER_THICKNESS, r, g, b, 255);
  thickLineRGBA(renderer, xbr, ybr, xbl, ybl, BORDER_THICKNESS, r, g, b, 255);
  thickLineRGBA(renderer, xbl, ybl, xtl, ytl, BORDER_THICKNESS, r, g, b, 255);
}

// Returns the selected index, or -1
int window_draw(const Filepaths filepaths) {
  int selected_idx = -1;
  Control control = control_new(COLS, ROWS);
  Textures textures = textures_from_filepaths(filepaths);
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
        control_update(&control, CONTROL_MOVE_LEFT);
        break;
      }
      case SDLK_j: {
        control_update(&control, CONTROL_MOVE_DOWN);
        break;
      }
      case SDLK_k: {
        control_update(&control, CONTROL_MOVE_UP);
        break;
      }
      case SDLK_l: {
        control_update(&control, CONTROL_MOVE_RIGHT);
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
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0xFF, 0xFF);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
    for (size_t i = 0; i < textures.count; ++i) {
      SDL_Rect rect = thumbnail_rect(i);
      SDL_Texture *texture = textures.texture[i];
      SDL_RenderCopy(renderer, texture, NULL, &rect);
    }
    window_draw_control(control);
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
