PKGS    := MagickWand SDL2_image
CFLAGS  := -Wall -Wextra -pedantic -O0 -ggdb
CFLAGS  += $(shell pkg-config --cflags $(PKGS))
LDFLAGS := $(shell pkg-config --libs   $(PKGS))
CFLAGS  += $(shell sdl2-config --cflags)
LDFLAGS += $(shell sdl2-config --libs)

SRCS := main.c walktree.c cache.c thumbnail.c window.c list.c

main: $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) $(LDFLAGS) -o $@
