#include "boss.h"
#include "vector.h"
#include "graphics3d.h"

extern Uint32 TIME;

Uint32 CD;
Uint32 CD_move = 1000;
Uint32 CD_attack1;
Uint32 CD_attack2;


//int move = 5000;


				//boss position
Vec3D Boss_move(int u, int y)
{	
	Vec3D Position;
	int i;


	if (TIME >= CD_move)
	{
		i = rand_ranged( 1, 4 ); //start to end - 1 
		switch(i)
			{
			
				case 1:
					u += 0;
				break;

				case 2:
					u += 10;
				break;

				case 3:
					u -= 10;
				break;
			};

		i = rand_ranged( 1, 4 ); //start to end - 1
		switch(i)
			{
			
				case 1:
					y += 0;
				break;

				case 2:
					y += 10;
				break;

				case 3:
					y -= 10;
				break;
			
					
			};

		CD_move = TIME + CD_move;
		vec3d_set(Position, u, y, 2);
		return Position;
	}
	else
	{
		vec3d_set(Position, u, y, 2);
		return Position;
	}

} 
