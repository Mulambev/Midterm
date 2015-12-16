#ifndef __MAP_H__
#define __MAP_H__

#include "collision.h"
#include "bsp.h"
#include "spawn.h"

typedef struct
{
	char mapname[128];
	Cube bounds; //absolute world bounds
	BSP *bsp; //static world geomytry
	Obj *obj; //Object to draw
	Sprite *skybox[6]; //the world skybox (if needed)
	int spawnCount; //how many spawn items
	Spawn *spawnList; // the list of spawn items
}Map;


Map * map_load(char *filename, char *id);
void map_draw(Map *map);
void map_close(Map *map);

#endif
