CC = gcc
CFLAGS = -Wall -g
TARGET = prog
SOURCES = main.c util/L_term_gfx.c util/ray.c

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o  $@ $^ -lm

clean:
	rm -f $(TARGET) *.o
