CFLAGS  := -Wall -Wextra -pedantic -O0 -ggdb
CFLAGS  += $(shell pkg-config --cflags MagickWand)
LDFLAGS := $(shell pkg-config --libs   MagickWand)

SRCS := walktree.c thumbnail.c

main: main.c walktree.c
	$(CC) $(CFLAGS) $@.c walktree.c $(LDFLAGS) -o $@
