# Wrath Blast Makefile
# Faction Folklore Volume I

CC           := cc
CFLAGS       := -std=c17 -Wall -Wextra -Wpedantic -O2
INCLUDES     := -Iinclude

FLOPPY_SIZE  := 1474560

SRC          := $(wildcard src/*.c)
OBJ          := $(SRC:.c=.o)

TARGET       := build/wrath-blast

.PHONY: all clean debug release run size floppy

all: release

release: CFLAGS += -DNDEBUG
release: $(TARGET)

debug: CFLAGS += -g -O0
debug: TARGET := build/wrath-blast-debug
debug: $(TARGET)

$(TARGET): $(OBJ) | build
	$(CC) $(CFLAGS) $(OBJ) -o $@

src/%.o: src/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

build:
	mkdir -p build

run: release
	./$(TARGET)

size: release
	@echo "Executable Size:"
	@wc -c $(TARGET)

floppy: release
	@bytes=$$(wc -c < $(TARGET)); \
	echo "Executable: $$bytes bytes"; \
	echo "Remaining: $$(( $(FLOPPY_SIZE) - $$bytes )) bytes"

clean:
	rm -rf build
	rm -f src/*.o src/*.d

-include $(OBJ:.o=.d)
