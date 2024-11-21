# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
LDFLAGS =

# Directories
SRC_DIR = src
DATA_DIR = data
BUILD_DIR = build

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

# Executable name
EXEC = atm

# Default target
all: $(BUILD_DIR) $(DATA_DIR) $(EXEC)

# Create necessary directories
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(DATA_DIR):
	mkdir -p $(DATA_DIR)
	touch $(DATA_DIR)/users.txt
	touch $(DATA_DIR)/records.txt

# Linking
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compilation rule
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(SRC_DIR)/header.h
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -rf $(BUILD_DIR) $(EXEC)

# Rebuild
re: clean all

# Run the program
run: $(EXEC)
	./$(EXEC)

# Debug target
debug: $(EXEC)
	gdb ./$(EXEC)

# Valgrind memory check
memcheck: $(EXEC)
	valgrind --leak-check=full --show-leak-kinds=all ./$(EXEC)

# Phony targets
.PHONY: all clean re run debug memcheck

# Additional targets for different build configurations
# Release build with optimizations
release: CFLAGS += -O2
release: all

# Sanitizer build for debugging
sanitize: CFLAGS += -fsanitize=address -fsanitize=undefined
sanitize: LDFLAGS += -fsanitize=address -fsanitize=undefined
sanitize: all