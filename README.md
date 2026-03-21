# visor

A poor's man file explorer. But only for pdf documents. Keyboard centric. NOT a pdf reader, just a file explorer and launcher.

## raison d'etre

- File explorers are too bloated, or too slow to startup
- I still mainly explore directories through the console

## Features

- supports `.pdf`, `.epub`, `.doc`, and `.ppt`
- shows thumbnails of pdfs on current dir in a grid
- thumbnails cached in `~/.cache/visor/`
- files opened with `xdg-open`

## Depends on

- SDL2
- SDL2_gfx
- SDL2_image
- MagickWand
- minizip

## Keybindings

| **key** | **action** | **key** | **action** |
|:-------:|------------|:-------:|------------|
| ↑ / k   | move up    | q       | exit       |
| ↓ / j   | move down  | intro   | select     |
| ← / h   | move left  | i       | preview    |
| → / l   | move right |         |            |
