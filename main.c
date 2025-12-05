#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "util/L_term_gfx.h"
#define _USE_MATH_DEFINES //google says i need ts
#include <math.h>


#define LEVEL_HEIGHT 64
#define LEVEL_WIDTH 64
#define HALF_FOV 2 * M_PI
int test_arr[LEVEL_WIDTH][LEVEL_HEIGHT]; //the (test) world
struct player{
    int x;
    int y;
    char * inv; // for later
    float state; //0 - 2PI.

};

struct player * init_player(unsigned int x, unsigned int y, int * world[64][64]){
    struct player * p = (struct player * )malloc(sizeof(struct player));
    if(x<64 && y<64){ 
        *world[x][y] = 99;
        p->x = x;
        p->y = y;
    }else{
        *world[LEVEL_WIDTH/2][LEVEL_HEIGHT/2] = p->state;
        p->x = LEVEL_WIDTH/2;
        p->y = LEVEL_HEIGHT/2;
    }
    return p;
}

//trig time

//need to check walls from state-HALF_FOV to state+half_FOV
//idea 1 naive check is to iterate through the world and check if the trig aligns
//idea 2 sweep by angle and check from player out to the first non 0, works because distance is needed for drawing anyways





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