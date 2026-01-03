# Compiler and tools
CC = gcc
LD = ld
OBJCOPY = objcopy

# Directory structure
SRC_DIR = src
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)/bin

# Compiler flags
CFLAGS = -I/usr/include/efi \
         -fpic \
         -ffreestanding \
         -fno-stack-protector \
         -fno-stack-check \
         -fshort-wchar \
         -mno-red-zone \
         -maccumulate-outgoing-args

# Linker flags
LDFLAGS = -shared \
          -Bsymbolic \
          -L/usr/lib \
          -T/usr/lib/elf_x86_64_efi.lds

# Libraries
LIBS = -lgnuefi -lefi

# Auto-detect all .c files in src directory
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))

# Output files
SHARED_OBJ = $(OBJ_DIR)/main.so
TARGET = $(BIN_DIR)/wordle.efi

# Default target
all: $(TARGET)

# Create necessary directories
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files to shared object
$(SHARED_OBJ): $(OBJECTS)
	$(LD) $(LDFLAGS) /usr/lib/crt0-efi-x86_64.o $(OBJECTS) -o $@ $(LIBS)

# Convert shared object to EFI executable
$(TARGET): $(SHARED_OBJ) | $(BIN_DIR)
	$(OBJCOPY) \
		-j .text \
		-j .sdata \
		-j .data \
		-j .rodata \
		-j .dynamic \
		-j .dynsym \
		-j .rel \
		-j .rela \
		-j .rel.* \
		-j .rela.* \
		-j .reloc \
		--target efi-app-x86_64 \
		--subsystem=10 \
		$< $@

# Clean up generated files
clean:
	rm -rf $(BUILD_DIR)
run: $(TARGET)
	./run
# Phony targets
.PHONY: all clean
