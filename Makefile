CFLAGS  := -Wall -Wextra -pedantic -O0 -ggdb
CFLAGS  += $(shell pkg-config --cflags MagickWand)
LDFLAGS := $(shell pkg-config --libs   MagickWand)

SRCS := main.c walktree.c cache.c thumbnail.c

main: $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) $(LDFLAGS) -o $@
