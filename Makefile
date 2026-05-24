# Makefile — Shpenzime GUI
# Builds on Linux, macOS, and Windows (MinGW).
# funksione.c is compiled as-is — no changes needed.

CC      = gcc
CFLAGS  = -std=c99 -Wall -Wextra -Wno-unused-parameter -I.
LDFLAGS = -L.
TARGET  = shpenzime

SRC     = main.c funksione.c raylib_mingw_compat.c

# ── Platform detection ────────────────────────────────────────────────────
ifeq ($(OS),Windows_NT)
    UNAME := Windows
else
    UNAME := $(shell uname)
endif

ifeq ($(UNAME), Linux)
    LIBS    = -lraylib -lm -lpthread -ldl -lGL
    TARGET := shpenzime
endif

ifeq ($(UNAME), Darwin)
    LIBS    = -lraylib -lm -framework OpenGL -framework Cocoa -framework IOKit
    TARGET := shpenzime
endif

ifeq ($(UNAME), Windows)
    LIBS    = -lraylib -lgdi32 -lwinmm -mwindows
    TARGET := shpenzime.exe
endif

# ── Targets ───────────────────────────────────────────────────────────────
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
