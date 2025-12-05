CC = gcc
CFLAGS = -Wall -g
TARGET = prog
SOURCES = main.c util/L_term_gfx.c

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET) *.o
