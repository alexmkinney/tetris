CC = gcc
CFLAGS = -O1 -Iinclude -Wall
LDFLAGS = -mwindows -s -static -static-libgcc
LIBS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lfreetype -lharfbuzz \
		-lgraphite2 -lpng -lz -lbz2 -lwinmm -limm32 -lole32 -loleaut32 \
		-lversion -luuid -lgdi32 -lsetupapi -lbrotlidec -lbrotlicommon \
		-lrpcrt4 -lstdc++ -lpthread

SRCDIR = src
BUILDDIR = build

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SOURCES))
EXECUTABLE = $(BUILDDIR)/tetris

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $^ -o $@ $(LIBS) $(LDFLAGS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILDDIR)
