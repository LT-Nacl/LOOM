#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define FRAMERATE 60.0f

void putchar_rgb(int color, char c) {
    int r = (color >> 16) & 0xFF;
    int g = (color >> 8) & 0xFF;
    int b = color & 0xFF;
    if (r < 0){ r = 0;} if (r > 255) {r = 255;}
    if (g < 0) {g = 0;} if (g > 255) {g = 255;}
    if (b < 0) {b = 0;} if (b > 255) {b = 255;}
    printf("\x1b[38;2;%d;%d;%dm", r, g, b);
    putchar(c);
}

void reset_color() {
    printf("\x1b[0m");
}

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
            if(v=='@'){
                putchar_rgb(0x16161d,v);
            }else if(v=='#'){
                putchar_rgb(0x00ff00,v);   
            }else{
            putchar(v ? v : ' ');
            }

            reset_color();

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

void draw_text(struct frame *f, const char *text, int x, int y, int color) {
    int r = (color >> 16) & 0xFF;
    int g = (color >> 8) & 0xFF;
    int b = color & 0xFF;
    printf("\x1b[38;2;%d;%d;%dm", r, g, b);
    int i = 0;
    while (text[i] != '\0' && (x + i) < f->width && y < f->height) {
        f->dat[y * f->width + (x + i)] = text[i];
        i++;
    }
    reset_color();
}




