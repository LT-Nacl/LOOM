#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "util/L_term_gfx.h"
#define _USE_MATH_DEFINES //google says i need ts
#include <math.h>


#define LEVEL_HEIGHT 64
#define LEVEL_WIDTH 64
#define HALF_FOV 0.78539816339f //pi/4
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

struct hit { //adhoc tuple
    float distance; 
    int value; 
};

#define SCAN_MAX 200.0f
#define RAY_STEP 0.5f
struct hit * cast_rays(struct player *p, int world[LEVEL_WIDTH][LEVEL_HEIGHT], int num_rays){
    struct hit *results = malloc(sizeof(struct hit) * num_rays);

    float start_angle = p->state - HALF_FOV;
    float end_angle = p->state + HALF_FOV;
    float step = (end_angle - start_angle) / num_rays;
    for(int i = 0; i < num_rays; i++){
        float ang = start_angle + i * step;
        float dx = cosf(ang);
        float dy = sinf(ang);

        float working_dist; // for the tuple once the ray hits it
        int val;

        while(working_dist < SCAN_MAX){
            int scanx = (int)(p->x + dx *working_dist); //ray properties
            int scany = (int)(p->y + dy *working_dist);
            if (scanx < 0 || scany < 0 || scanx >= LEVEL_WIDTH || scany >= LEVEL_HEIGHT) { //cond 1 out of bounds
                val = -1;
                break;
            }

            if (world[scanx][scany]){ //cond 2 hit
                val = world[scanx][scany];
                break;
            }

            working_dist += RAY_STEP;
        }
        if(working_dist > SCAN_MAX){ val = 0;}
        results[i].value = val;
        results[i].distance = working_dist;
    }
    return results;
}



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