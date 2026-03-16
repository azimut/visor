PKGS    := MagickWand SDL2_image SDL2_gfx minizip
CFLAGS  := -Wall -Wextra -pedantic -O0 -ggdb
CFLAGS  += $(shell pkg-config --cflags $(PKGS))
LDFLAGS := $(shell pkg-config --libs   $(PKGS))
CFLAGS  += $(shell sdl2-config --cflags)
LDFLAGS += $(shell sdl2-config --libs)
SRCS    := $(wildcard src/*.c)

main: $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) $(LDFLAGS) -o $@

.PHONY: install
install: main
	cp main $(HOME)/bin/visor

.PHONY: deps
deps:; sudo apt install libsdl2-gfx-dev libsdl2-image-dev libmagickwand-dev libminizip-dev
