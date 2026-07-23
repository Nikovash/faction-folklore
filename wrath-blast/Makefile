# Wrath Blast Makefile
# Faction Folklore Volume I

CC            := cc
CPPFLAGS      := -Iinclude
COMMON_FLAGS  := -std=c17 -Wall -Wextra -Wpedantic

RELEASE_FLAGS := $(COMMON_FLAGS) -O2 -DNDEBUG
DEBUG_FLAGS   := $(COMMON_FLAGS) -O0 -g3

FLOPPY_SIZE   := 1474560

SRC           := $(wildcard src/*.c)

RELEASE_DIR   := build/release
DEBUG_DIR     := build/debug

RELEASE_OBJ   := $(patsubst src/%.c,$(RELEASE_DIR)/%.o,$(SRC))
DEBUG_OBJ     := $(patsubst src/%.c,$(DEBUG_DIR)/%.o,$(SRC))

TARGET        := build/wrath-blast
DEBUG_TARGET  := build/wrath-blast-debug

.PHONY: all release debug run run-debug size floppy clean

all: release

release: $(TARGET)

debug: $(DEBUG_TARGET)

$(TARGET): $(RELEASE_OBJ) | build
	$(CC) $(RELEASE_FLAGS) $(RELEASE_OBJ) -o $@

$(DEBUG_TARGET): $(DEBUG_OBJ) | build
	$(CC) $(DEBUG_FLAGS) $(DEBUG_OBJ) -o $@

$(RELEASE_DIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(RELEASE_FLAGS) -MMD -MP -c $< -o $@

$(DEBUG_DIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(DEBUG_FLAGS) -MMD -MP -c $< -o $@

build:
	mkdir -p build

run: release
	./$(TARGET)

run-debug: debug
	./$(DEBUG_TARGET)

size: release
	@bytes=$$(wc -c < $(TARGET)); \
	echo "Executable: $$bytes bytes"

floppy: release
	@bytes=$$(wc -c < $(TARGET)); \
	remaining=$$(( $(FLOPPY_SIZE) - $$bytes )); \
	echo "Executable: $$bytes bytes"; \
	echo "Floppy capacity: $(FLOPPY_SIZE) bytes"; \
	echo "Remaining: $$remaining bytes"; \
	if [ $$remaining -lt 0 ]; then \
		echo "Status: TOO LARGE"; \
		exit 1; \
	else \
		echo "Status: FITS"; \
	fi

clean:
	rm -rf build

-include $(RELEASE_OBJ:.o=.d)
-include $(DEBUG_OBJ:.o=.d)
