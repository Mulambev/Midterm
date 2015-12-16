#ifndef __BSP_H__
#define __BSP_H__

#include "vector.h"
#include "obj.h"

#define BSP_L 0
#define BSP_R 1

typedef struct BSP_S
{
	Vec3D triangle[3];
	Vec3D normal;
	float d;
	struct BSP_S *children[2];
}BSP;


BSP *bsp_convert_from_obj(Obj *object);


void bsp_save_to_file(BSP *bsp, char *filename);


BSP *bsp_load_from_file(BSP *bsp, char *filename);


int bsp_cube_collision(BSP *bsp, Cube *cube);


float bsp_ray_collision(BSP *bsp, Vec3D start, Vec3D end);

#endif