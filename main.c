#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "util/L_term_gfx.h"
#include <math.h>




#define LEVEL_HEIGHT 64
#define LEVEL_WIDTH 64
#define HALF_FOV 0.78539816339f //pi/4
int test_arr[LEVEL_HEIGHT][LEVEL_WIDTH]; //the (test) world
struct player{
	int x;
	int y;
	char * inv; // for later
	float state; //0 - 2PI.

};



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
struct hit * cast_rays(struct player *p, int world[LEVEL_HEIGHT][LEVEL_WIDTH], int num_rays){
	struct hit *results = malloc(sizeof(struct hit) * num_rays);

	float start_angle = p->state - HALF_FOV;
	float end_angle = p->state + HALF_FOV;
	float step = (end_angle - start_angle) / num_rays;
	for(int i = 0; i < num_rays; i++){
		float ang = start_angle + i * step;
		float dx = cosf(ang);
		float dy = sinf(ang);

		float working_dist = 0.0f; // for the tuple once the ray hits it
		int val = 0;

		while(working_dist < SCAN_MAX){
			int scanx = (int)(p->x + dx *working_dist); //ray properties
			int scany = (int)(p->y + dy *working_dist);
			if (scanx < 0 || scany < 0 || scanx >= LEVEL_WIDTH || scany >= LEVEL_HEIGHT) { //cond 1 out of bounds
				val = -1;
				break;
			}

			if (world[scany][scanx] && !(world[scany][scanx] == 99)){ //cond 2 hit
				val = world[scany][scanx];
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
void test_world_and_rays()
{
	int simple_world[8][8] = {
		{1, 1, 1, 1, 1, 1, 1, 1},
		{1, 0, 0, 0, 0, 0, 2, 1},
		{1, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 2, 1},
		{1, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 2, 1},
		{1, 1, 1, 1, 1, 1, 1, 1}
	};


	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			test_arr[y][x] = simple_world[y][x];
		}
	}

	struct player p;
	p.x = 1;
	p.y = 4;
	p.state = 0.0f;
	simple_world[p.y][p.x] = 9;

	/*
	   printf("=== WORLD (8x8) ===\n");
	   for (int y = 0; y < 8; y++) {
	   for (int x = 0; x < 8; x++) {
	   printf("%d ", simple_world[y][x]);
	   }
	   printf("\n");
	   }
	   printf("\n");




	   printf("Player: pos(%d,%d) angle=%.2f rad (%.0f deg)\n\n", 
	   p.x, p.y, p.state, p.state * 180.0f / M_PI);



	   printf("=== %d RAYS (FOV %.0f deg) ===\n", num_rays, 2*HALF_FOV*180/M_PI);
	   */
	float x_max = 2.5f;


	/*
	   for (int i = 0; i < num_rays; i++) {
	   printf("Ray %d: dist=%.2f  hit=%d  ang=%.2f\n", i, scan[i].distance, scan[i].value, p.state + i * 2 * HALF_FOV / num_rays-HALF_FOV);
	   }
	   */
	float offset = 0.01f;
	p.state = 0;
	struct frame f;
	f.id = 0;
	f.width = 64;
	f.height = 32;
    clear_frame(&f);
	struct frame * test_f = &f;
	while(1){

		p.state += offset;
		p.state = fmod(p.state, 2 * M_PI);
		int num_rays = 256;
		struct hit *scan = cast_rays(&p, test_arr, num_rays);
		for (int i = 0; i < num_rays; i++) {
			float dy =  (f.height / 2.0f) * x_max / fmaxf(scan[i].distance, 0.1f);
			int low = f.height/2 - (int)dy;
			int high = f.height/2 + (int)dy;
			// int dx = (i * 2 * HALF_FOV / num_rays-HALF_FOV)/(2*HALF_FOV) * f.width;
			int x = (i * f.width) / num_rays;
			printf("i=%d dist=%.2f dy=%.2f low=%d high=%d\n", i, scan[i].distance, dy, low, high);
			if (low < 0) low = 0;
			if (high >= f.height) high = f.height - 1;
			if (x >= 0 && x < f.width && low <= high) {
				draw_col_sing(test_f, (scan[i].value == 2) ? '#' : '@', x, low, high);
			}
		}
		frame_draw(test_f);
		free(scan);

		usleep((int)(1000000 * (1.0f/FRAMERATE)));
	}
	free(test_f);

	usleep(1*1000000);


}


int main(){
	test_world_and_rays();
	return 0;
	/*
	   struct frame *test_f = frame_create(0, 32, 16);


	   set_pixel(test_f, '#', 0, 0);
	   set_pixel(test_f, '#', f.width-1, f.height-1);
	   draw_col_sing(test_f, '#', f.width/2, 1, f.height-2);

	   while (1){
	   frame_draw(test_f);
	   usleep((int)(1000000 * (1.0f/FRAMERATE)));
	   }
	   */
}
