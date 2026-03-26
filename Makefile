PKGS    := MagickWand SDL2_image SDL2_gfx SDL2_ttf minizip
CFLAGS  := -Wall -Wextra -pedantic
CFLAGS  += $(shell pkg-config --cflags $(PKGS))
LDFLAGS := $(shell pkg-config --libs   $(PKGS))
CFLAGS  += $(shell sdl2-config --cflags)
LDFLAGS += $(shell sdl2-config --libs)
SRCS    := $(wildcard src/*.c)

ifdef DEBUG
CFLAGS  += -ggdb3 -O0
endif

main: $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) $(LDFLAGS) -o $@

.PHONY: install
install: main
	cp main $(HOME)/bin/visor

.PHONY: deps
deps:; sudo apt install libsdl2-ttf-dev libsdl2-gfx-dev libsdl2-image-dev libmagickwand-dev libminizip-dev
