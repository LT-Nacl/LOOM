#include "main.c"
#include "ray.h"

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