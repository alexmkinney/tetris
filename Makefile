CC = gcc
CFLAGS = -O1 -Iinclude -Wall
LDFLAGS = -s
LDLIBS =
 
# Platform detection
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
  PLATFORM = linux
  LDLIBS += $(shell pkg-config --libs sdl2 SDL2_ttf)
endif
ifeq ($(UNAME_S), Darwin)
  PLATFORM = macos
  LDLIBS += $(shell pkg-config --libs sdl2 SDL2_ttf)
  LDLIBS += -L/usr/local/lib # For Homebrew-installed libraries
endif
ifeq ($(OS), Windows_NT)
  PLATFORM = windows
  LDFLAGS += -static -static-libgcc
  LDLIBS += $(shell pkg-config --libs --static sdl2 SDL2_ttf)
  LDLIBS += -lstdc++
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
