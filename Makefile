# Compiler and flags
CC = g++
CFLAGS = -Wall -Wextra -pthread -I$(INC_DIR)

# Directories
INC_DIR = src/logger

# Source files
SRCS = src/server/server.cpp src/server/main.cpp
OBJS = $(SRCS:.cpp=.o)

# Executable name
BINS = server

# Dependencies
DEPS = $(INC_DIR)/Logger.hpp

# Default target
all: $(BINS)

# Build target
server: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^
	rm -f $(OBJS)

# Compile source files to object
%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean up
clean:
	rm -f server $(OBJS)

