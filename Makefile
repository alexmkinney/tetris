CC = gcc
CFLAGS = -O1 -Iinclude -Wall
LDFLAGS = -s
LDLIBS =
 
# Platform detection
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
  PLATFORM = linux
  CFLAGS += $(shell pkg-config --cflags sdl2 SDL2_ttf)
  LDLIBS += $(shell pkg-config --libs sdl2 SDL2_ttf)
endif
ifeq ($(UNAME_S), Darwin)
  PLATFORM = macos
  CFLAGS += $(shell pkg-config --cflags sdl2 SDL2_ttf)
  LDLIBS += $(shell pkg-config --libs sdl2 SDL2_ttf)
  LDLIBS += -L/usr/local/lib # For Homebrew-installed libraries
endif
ifeq ($(OS), Windows_NT)
  PLATFORM = windows
  LDFLAGS += -mwindows -static -static-libgcc
  LDLIBS += -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lfreetype -lharfbuzz \
			-lgraphite2 -lpng -lz -lbz2 -lwinmm -limm32 -lole32 -loleaut32 \
			-lversion -luuid -lgdi32 -lsetupapi -lbrotlidec -lbrotlicommon \
			-lrpcrt4 -lstdc++ -lpthread
endif

SRCDIR = src
BUILDDIR = build

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SOURCES))
EXECUTABLE = $(BUILDDIR)/tetris

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS) $(LDLIBS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILDDIR)
