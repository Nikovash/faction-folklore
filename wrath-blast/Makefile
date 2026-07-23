# Wrath Blast Makefile
# Faction Folklore Volume I

CC             := cc
WINDOWS_CC     := x86_64-w64-mingw32-gcc

CPPFLAGS       := -Iinclude
COMMON_FLAGS   := -std=c11 -Wall -Wextra -Wpedantic

RELEASE_FLAGS  := $(COMMON_FLAGS) -O2 -DNDEBUG
DEBUG_FLAGS    := $(COMMON_FLAGS) -O0 -g3
WINDOWS_FLAGS  := $(COMMON_FLAGS) -Os -DNDEBUG \
                  -ffunction-sections -fdata-sections

FLOPPY_SIZE    := 1474560

COMMON_SRC     := src/floor.c src/game.c src/main.c
POSIX_SRC      := $(COMMON_SRC) src/platform_posix.c
WINDOWS_SRC    := $(COMMON_SRC) src/platform_windows.c

RELEASE_DIR    := build/release
DEBUG_DIR      := build/debug
WINDOWS_DIR    := build/windows

RELEASE_OBJ    := $(patsubst src/%.c,$(RELEASE_DIR)/%.o,$(POSIX_SRC))
DEBUG_OBJ      := $(patsubst src/%.c,$(DEBUG_DIR)/%.o,$(POSIX_SRC))
WINDOWS_OBJ    := $(patsubst src/%.c,$(WINDOWS_DIR)/%.o,$(WINDOWS_SRC))

TARGET         := build/wrath-blast
DEBUG_TARGET   := build/wrath-blast-debug
WINDOWS_TARGET := build/wrath-blast.exe

.PHONY: all release debug windows check-windows-compiler \
        run run-debug size floppy windows-size windows-floppy clean

all: release

release: $(TARGET)

debug: $(DEBUG_TARGET)

$(TARGET): $(RELEASE_OBJ) | build
	$(CC) $(RELEASE_FLAGS) $(RELEASE_OBJ) -o $@

$(DEBUG_TARGET): $(DEBUG_OBJ) | build
	$(CC) $(DEBUG_FLAGS) $(DEBUG_OBJ) -o $@

$(WINDOWS_TARGET): $(WINDOWS_OBJ) | build
	$(WINDOWS_CC) $(WINDOWS_FLAGS) \
	    -Wl,--gc-sections -s \
	    $(WINDOWS_OBJ) -o $@

$(RELEASE_DIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(RELEASE_FLAGS) -MMD -MP -c $< -o $@

$(DEBUG_DIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(DEBUG_FLAGS) -MMD -MP -c $< -o $@

$(WINDOWS_DIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(WINDOWS_CC) $(CPPFLAGS) $(WINDOWS_FLAGS) -MMD -MP -c $< -o $@

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

check-windows-compiler:
	@command -v $(WINDOWS_CC) >/dev/null 2>&1 || { \
		echo "Error: $(WINDOWS_CC) is not installed."; \
		echo "Install the MinGW-w64 cross-compiler first."; \
		exit 1; \
	}

windows: check-windows-compiler
	$(MAKE) $(WINDOWS_TARGET)

windows-size: windows
	@bytes=$$(wc -c < $(WINDOWS_TARGET)); \
	echo "Windows executable: $$bytes bytes"

windows-floppy: windows
	@bytes=$$(wc -c < $(WINDOWS_TARGET)); \
	remaining=$$(( $(FLOPPY_SIZE) - $$bytes )); \
	echo "Windows executable: $$bytes bytes"; \
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
-include $(WINDOWS_OBJ:.o=.d)
