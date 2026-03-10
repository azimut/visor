#include "window.h"
#include "SDL.h"
#include "SDL_image.h"
#include "list.h"

#include <assert.h>
#include <stdbool.h>

#define TITLE "pdfeye"
#define WIDTH 320
#define HEIGHT 240
#define POS SDL_WINDOWPOS_UNDEFINED

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

int window_init(void) {
  if (SDL_Init(SDL_INIT_VIDEO)) {
    fprintf(stderr, "[ERROR] Starting SDL...\n");
    return 1;
  }
  window = SDL_CreateWindow(TITLE, POS, POS, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
  if (!window) {
    fprintf(stderr, "[ERROR] Creating window...\n");
    return 1;
  }
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer) {
    fprintf(stderr, "[ERROR] Creating renderer...\n");
    return 1;
  }
  return 0;
}

void window_draw(const List list) {
  bool quit = false;

  /* List *current = list; */
  /* while (current) { */
  /*   puts(current->filename); */
  /*   current = current->next; */
  /* } */

  SDL_Rect rect = {.w = 200, .h = 150, .x = 30, .y = 30};

  SDL_Texture *texture;
  texture = IMG_LoadTexture(renderer, list.filename);
  if (!texture) {
    SDL_Log("Couldn't load %s: %s\n", list.filename, SDL_GetError());
    return;
  }

  while (!quit) {
    SDL_Event e;
    SDL_WaitEvent(&e);
    if (e.type == SDL_QUIT) {
      quit = true;
    }
    SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyTexture(texture);
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
