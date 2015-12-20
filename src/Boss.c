#include "boss.h"
#include "vector.h"
#include "graphics3d.h"
#include "simple_logger.h"
#include <SDL.h>
#include <SDL_main.h>

extern Uint32 TIME;


Uint32 CD_move = 2000;
Uint32 CD_attack1;
Uint32 CD_attack2;
Uint32 CD_enemy;
Uint32 CD = 0; 

int i;

				//boss position
void Boss_move(int u, int y, Vec3D *Bosspos)
{	
	int i;
	
	if (CD < TIME)
	{
		 
		 i = rand_ranged( 1, 20)%3; //start to (end - 1) 
		 switch(i)
			{
			
				case 0:
					if (u <= 6)
					{
						u = 5;
					}
					else
					{
						u -= 10;
					}
				break;

				case 1:
					
					if (u >= 20)
					{
						u = 25;
					}
					else
					{
						u += 10;
					}

				break;

				case 2:
					if (u = 5)
					{
						u += 10;
					}
					if (u = 25)
					{
						u -= 10;
					}
				};
	
		i = rand_ranged( 1, 20)%2 ; //start to (end - 1)
		switch(i)
			{
			
				case 0:
					if (y <= -9)
					{
						y = -10;
					}
					else
					{
						y -= 10;
					}
				break;

				case 1:
					if (y >= 9)
					{
						y = 10;
					}
					else
					{
						y += 10;
					}
				break;
								
			};

		Bosspos->x = u;
		Bosspos->y = y;
		CD = CD_move + TIME;
	}
}

/*void Boss_attack1(int a, int b, Vec3D *Bosspos, Vec3D *Volcano)
{
	int j;

	if (CD_attack1 <= TIME)
	{
		j = rand_ranged( 1, 10)%4; //start to (end - 1)
		switch(i)
			{
			
				case 0:
					b = 10;
				break;

				case 1:
					b= 20;
				break;

				case 2:
					b = 30;
				break;
								
			};

		CD_attack1 = TIME + 1000;
		Volcano->x = Bosspos->x - b;
		Volcano->y = Bosspos->y;
	}
}*/

/*void enemy_attack1(int u,  Vec3D *enemy1pos)
{
	int i;	
	if (CD_enemy < TIME)
	{
		i = rand_ranged( 1, 10) ;

		if (i == 5)
		{
			u -=1;
			if (u == -30)
			{
				u = 5;
				i = 0;
			}
		}

	enemy1pos->x = u;
	CD_enemy = TIME + 2000;
		
	}
	

}*/

/*void enemy_attack2(int u,  Vec3D *enemy2pos)
{
	int i;	
	if (CD_enemy < TIME)
	{
		i = rand_ranged( 1, 10) ;

		if (i == 5)
		{
			u -=1;
			if (u == -30)
			{
				u = 5;
				i = 0;
			}
		}

	enemy2pos->x = u;
	CD_enemy = TIME + 2000;
		
	}
	

}*/