#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "util/L_term_gfx.h"

int main(){
    struct frame *test_f = frame_create(0, 32, 16);
    set_pixel(test_f, '#', 0, 0);
    set_pixel(test_f, '#', test_f->width-1, test_f->height-1);
    draw_col_sing(test_f, '#', test_f->width/2, 1, test_f->height-2);

    while (1){
        frame_draw(test_f);
        usleep((int)(1000000 * (1.0f/FRAMERATE)));
    }
}