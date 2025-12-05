#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define FRAMERATE 60.0f

struct frame{
    int id;
    int width;
    int height;
    char dat[4096]; 
};

struct frame* frame_create(int id, int w, int h) {
    struct frame *f = malloc(sizeof(struct frame));
    if (!f) return NULL;

    f->id = id;
    f->width = w;
    f->height = h;

    for (int i = 0; i < 4096; i++){
        f->dat[i] = 0;
    }
    return f;
}

char get_pixel(struct frame*f, int x, int y){
    return f->dat[x + y * f->width];
}

void set_pixel(struct frame*f, char value, int x, int y){
    f->dat[x + y * f->width] = value;
}

void clear_frame(struct frame *f){
    for(int i = 0; i < f->width * f->height && i < 4096; i++){
        f->dat[i] = 0;
    }
}
void frame_draw(struct frame *f) {
    printf("\033[2J\033[H");
    for (int y = 0; y < f->height; y++) {
        const char *row = &f->dat[y * f->width];
        for (int x = 0; x < f->width; x++) {
            char v = row[x];
            putchar(v ? v : ' ');


        }
        putchar('\n');
    }
    clear_frame(f);
}


void draw_col_sing(struct frame *f, char val, int x, int lower, int upper) {
    for (int y = f->height-upper; y < (f->height-lower); y++){
        f->dat[y * f->width + x] = val;
    }
}




