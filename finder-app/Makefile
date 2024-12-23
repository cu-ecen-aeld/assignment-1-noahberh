
# Cross compiler option
CROSS_COMPILE ?= 

# Compiler flag
CC = $(CROSS_COMPILE)gcc
CFLAGS = -Wall -Wextra -g

# Target
TARGET = writer

# Source
SRC = writer.c

# Rule
$(TARGET): $(SRS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Clean
clean:
	rm -f $(TARGET)



