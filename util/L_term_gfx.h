#ifndef L_TERM_GFX_H
#define L_TERM_GFX_H

#define FRAMERATE 60.0f

struct frame{
    int id;
    int width;
    int height;
    char dat[4096]; 
};

struct frame* frame_create(int id, int w, int h);
char get_pixel(struct frame*f, int x, int y);
void set_pixel(struct frame*f, char value, int x, int y);
void frame_draw(struct frame *f);
void draw_col_sing(struct frame *f, char val, int x, int lower, int upper);
void clear_frame(struct frame *f);
#endif