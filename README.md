# visor

A poor's man file explorer. But only for pdfs. Keyboard centric. NOT a pdf reader, just a file explorer and launcher for your pdf viewer.

## raison d'etre

- File explorers are too bloated, or too slow to startup
- I still mainly explore directories through the console

## Features

- supports .pdf and .epub
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

| key   | action     |
|:-----:|------------|
| h     | move left  |
| j     | move down  |
| k     | move up    |
| l     | move right |
| INTRO | select     |
