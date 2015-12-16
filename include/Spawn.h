
#ifndef __SPAWN_H__
#define __SPAWN_H__

#include "vector.h"

typedef struct
{
	Vec3D position; //position of the spawn
	Vec3D rotation; //rotation of the spawn
	char id[3];		//unique id for item in the map
	char state[128]; //state for the entity to spawn with
	char target[128]; //Target to patrol/next position in race
	char description[512]; 
	char tags[512]; //space seperated tags for dynamic use
	char classname[128]; //name of entity to spawn
}Spawn;

#endif