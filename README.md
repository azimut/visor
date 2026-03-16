# visor

A poor's man file explorer. But only for pdfs. Keyboard centric. NOT a pdf reader, just a file explorer and launcher for your pdf viewer.

## Depends on

- SDL2
- SDL2_gfx
- SDL2_image
- MagickWand

## Planned Features

- [x] cache thumbnails
- [x] xdg-open pdf
  - [ ] then exit
- [x] Grid size based on number of files
  - [ ] non square grids
- [x] respect aspect ratio on thumbnail
- [x] hover only above valid cells
- [ ] sort by filename & depth
- [ ] fullscreen
- [ ] lazy image rendering (no render offscreen)
- [ ] dynamic resize of thumbnails (fake zoom)
- [ ] dynamic resize grid based on window size
- [ ] interactive filter by
  - filename
  - pdftitle
- [ ] iterate over sudirs (OPTIONAL)
