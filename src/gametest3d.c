/**
 * gametest3d
 * @license The MIT License (MIT)
 *   @copyright Copyright (c) 2015 EngineerOfLies
 *    Permission is hereby granted, free of charge, to any person obtaining a copy
 *    of this software and associated documentation files (the "Software"), to deal
 *    in the Software without restriction, including without limitation the rights
 *    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *    copies of the Software, and to permit persons to whom the Software is
 *    furnished to do so, subject to the following conditions:
 *    The above copyright notice and this permission notice shall be included in all
 *    copies or substantial portions of the Software.
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *    SOFTWARE.
 */
#include "mgl_callback.h"
#include "simple_logger.h"
#include "graphics3d.h"
#include "shader.h"
#include "obj.h"
#include "vector.h"
#include "sprite.h"
#include "entity.h"
#include "space.h"
#include "Boss.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

void set_camera(Vec3D position, Vec3D rotation);
extern Uint32 TIME;

void touch_callback(void *data, void *context)
{
    Entity *me,*other;
    Body *obody;
    if ((!data)||(!context))return;
    me = (Entity *)data;
    obody = (Body *)context;
    if (entity_is_entity(obody->touch.data))
    {
        other = (Entity *)obody->touch.data;
        //slog("%s is ",other->name);
    }
    //slog("touching me.... touching youuuuuuuu");
}

void think(Entity *self)
{
    if (!self)return;
    switch(self->state)
    {
        case 0:
            self->frame = 0;
            break;
        case 1:
            self->frame += 0.3;
            if (self->frame >= 24)self->frame = 0;
            break;
        case 2:
            self->frame -= 0.3;
            if (self->frame < 0)self->frame = 23;
            break;
    }
    self->objModel = self->objAnimation[(int)self->frame];
}


Entity *newCube(Vec3D position,const char *name)
{
    Entity * ent;
    char buffer[255];
    int i;
    ent = entity_new();
    if (!ent)
    {
        return NULL;
    }
    /*for (i = 0; i < 24;i++)
    {
        //sprintf(buffer,"models/robot/walk_bot_%06i.obj",i + 1);
        ent->objAnimation[i] = obj_load(buffer);
    }*/
    ent->objModel = obj_load("models/cube.obj");//ent->objAnimation[0];
	if( !ent->objModel )
		slog( "test" );
    ent->texture = LoadSprite("models/cube_text.png",1024,1024); //LoadSprite("models/robot/robot.png",1024,1024);
    vec3d_cpy(ent->body.position,position);
	vec3d_set(ent->scale,1,1,1);
	vec3d_set(ent->rotation,0,0,0);
	vec4d_set(ent->color,1,1,1,1);
    cube_set(ent->body.bounds,-1,-1,-1,2,2,2);
    ent->rotation.x = 90;
    sprintf(ent->name,"%s",name);
    ent->think = think;
    ent->state = 0;
    mgl_callback_set(&ent->body.touch,touch_callback,ent);
    return ent;
}


int main(int argc, char *argv[])
{
    int i;
    float r = 0;
	int f = -15;
	int g = 0;
	int l = 0;
	int y = 0;
	int u = 15;
	int q = 0;
	int gamestate=0;
	int PlayerHP=15, Bosshealth=30, enemy1health=5, enemy2health=5, Bigtimer=600, CD;
	int SpawnLemon1=0, SpawnLemon2=0, SpawnLemon3=0, SpawnBigLemon=0, enemybullet=0;
	float Lemonx1=0, Lemonx2=0, Lemonx3=0, LemonxBig=0, enemybulletx=0;
	int Lemony1=0, Lemony2=0, Lemony3=0, LemonyBig=0, enemybullety=0;
	int a=0, b=0, c=0, d=0, count, Goomba_attack1=0, Goomba_attack2=0, Goomba_move1, Goomba_move2;
    Space *space;
    Entity *cube1,*cube2, *Actor;
    char bGameLoopRunning = 1;
	Vec3D Bosspos = {u, y, 2};
	Vec3D enemy1pos = {5, 10, 2};
	Vec3D enemy2pos = {5, -10, 2};
    Vec3D cameraPosition = {0,0,70.3};
    Vec3D cameraRotation = {0,0,360};
    SDL_Event e;
    Obj *megaman,*boss, *mapp, *mape, *lemon, *BossHPBar, *PlayerHPBar, *Goomba;
    Sprite *megamantexture, *bosstexture, *maptexture1, *maptexture2, *lemontexture, *BosstextureHP, *PlayertextureHP, *Goombatexture;
    
    init_logger("gametest3d.log");
    if (graphics3d_init(1024,768,1,"gametest3d",33) != 0)
    {
        return -1;
    }
    model_init();
    obj_init();
    entity_init(255);
    
   //load objects, models here, replace cube with megaman
    megaman = obj_load("models/MPU.obj");
	//load his uv map "texture file"
    megamantexture = LoadSprite("models/MegaManBody1.png",1024,1024);

	PlayerHPBar = obj_load("models/cube.obj");
	PlayertextureHP = NULL;


	boss = obj_load("models/Fireman.obj");
	bosstexture = LoadSprite("models/FireManBody1.png",1024,1024);

	BossHPBar = obj_load("models/cube.obj");
	BosstextureHP = NULL;

	Goomba = obj_load("models/goomba.obj");
	Goombatexture = LoadSprite("models/goomba_tex.png",1024,1024);

    mapp = obj_load("models/MidtermMapSingle.obj");
	maptexture1 = LoadSprite("models/PlayerTile(Red).png",1024,1024);

	mape = obj_load("models/MidtermMapSingle.obj");
	maptexture2 = LoadSprite("models/BossTile(Blue).png",1024,1024);
    
    lemon = obj_load("models/cube.obj");
	lemontexture = NULL;//LoadSprite("models/cube_text.png",1024,1024);
    
	    
    while (bGameLoopRunning)
    {
        entity_think_all();

        while ( SDL_PollEvent(&e) ) 
        {
            if (e.type == SDL_QUIT)
            {
                bGameLoopRunning = 0;
            }
            else if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_ESCAPE)
                {
                    bGameLoopRunning = 0;
                }
                else if (e.key.keysym.sym == SDLK_x)
                {
                    cameraPosition.z++;
                }
                else if (e.key.keysym.sym == SDLK_z)
                {
                    cameraPosition.z--;
                }
                else if (e.key.keysym.sym == SDLK_h)
                {
                    vec3d_add(
                        cameraPosition,
                        cameraPosition,
                        vec3d(
                            -sin(cameraRotation.z * DEGTORAD),
                            cos(cameraRotation.z * DEGTORAD),
                            0
                        ));
                }
                else if (e.key.keysym.sym == SDLK_y)
                {
                    vec3d_add(
                        cameraPosition,
                        cameraPosition,
                        vec3d(
                            sin(cameraRotation.z * DEGTORAD),
                            -cos(cameraRotation.z * DEGTORAD),
                            0
                        ));
                }
                else if (e.key.keysym.sym == SDLK_j)
                {
                    vec3d_add(
                        cameraPosition,
                        cameraPosition,
                        vec3d(
                            cos(cameraRotation.z * DEGTORAD),
                            sin(cameraRotation.z * DEGTORAD),
                            0
                        ));
                }
                else if (e.key.keysym.sym == SDLK_g)
                {
                    vec3d_add(
                        cameraPosition,
                        cameraPosition,
                        vec3d(
                            -cos(cameraRotation.z * DEGTORAD),
                            -sin(cameraRotation.z * DEGTORAD),
                            0
                        ));
                }
				else if (e.key.keysym.sym == SDLK_d)
                {
                   
				   
				   if(f >= -5)
				   {
					   f = -5;
				   }
				   else
				   {
					   f += 10;
				   }
                }
				else if (e.key.keysym.sym == SDLK_a)
                {
                   
				   
				   if(f <= -20)
				   {
					   f = -25;
				   }
				   else
				   {
					   f -= 10;
				   }
				  			
                }
				else if (e.key.keysym.sym == SDLK_w)
                {
                   
				   if(g >= 5)
				   {
					   g = 10;
				   }
				   else
				   {
					   g += 10;
				   }
			
                }
				else if (e.key.keysym.sym == SDLK_s)
                {
                   
				   g -= 10;
				    if(g <= -5)
				   {
					   g = -10;
				   }
			
                }
                else if (e.key.keysym.sym == SDLK_LEFT)
                {
                    cameraRotation.z += 1;
                }
                else if (e.key.keysym.sym == SDLK_RIGHT)
                {
                    cameraRotation.z -= 1;
                }
                else if (e.key.keysym.sym == SDLK_UP)
                {
                    cameraRotation.x += 1;
                }
                else if (e.key.keysym.sym == SDLK_DOWN)
                {
                    cameraRotation.x -= 1;
                }
                else if (e.key.keysym.sym == SDLK_SPACE)
                {
                    
					CD = TIME + Bigtimer;					
					
                }
				else if (e.key.keysym.sym == SDLK_n)
                {
                   
				   if (enemy1health <= 0 && enemy2health <= 0)
				   {
					   gamestate=1;
				   }
			
                }
				else if (e.key.keysym.sym == SDLK_n)
                {
                   
				   if (enemy1health <= 0 && enemy2health <= 0)
				   {
					   gamestate=1;
				   }
			
                }
            }
			else if (e.type == SDL_KEYUP)
			{
				if (e.key.keysym.sym == SDLK_SPACE)
				{
					
					if (TIME >= CD && SpawnBigLemon !=1)
					{
						SpawnBigLemon = 1;
						LemonxBig = f;
						LemonyBig = g;
					}
					else if (SpawnLemon1 != 1)
					{
						SpawnLemon1 = 1;
						Lemonx1 = f;
						Lemony1 = g;
					}
					else if (SpawnLemon2 != 1)
					{
						SpawnLemon2 = 1;
						Lemonx2 = f;
						Lemony2 = g;
					}
					else if (SpawnLemon3 != 1)
					{
						SpawnLemon3 = 1;
						Lemonx3 = f;
						Lemony3 = g;
					}
					
				}
			}
				
        }

                
        graphics3d_frame_begin();
        
        glPushMatrix();
        set_camera(
            cameraPosition,
            cameraRotation);
        
        //entity_draw_all();
      
		if (PlayerHP > 0 )
		{
		//Megaman
        obj_draw(
            megaman,
            vec3d(f, g, 2),
            vec3d(90,90,0),
            vec3d(0.5,0.5,0.5),
            vec4d(1,1,1,1),
            megamantexture
        );

		//Megaman HP BAR
		obj_draw(
            PlayerHPBar,
            vec3d(-30+PlayerHP/(2),20,2),
            vec3d(90,-90,0),
            vec3d(.9,.9,PlayerHP/(2)),
            vec4d(0,1,0,1),
			PlayertextureHP
        );
		}

		//Megaman Projectiles
		//Lemon1
		if (SpawnLemon1 != 0)
		{
			obj_draw(
			lemon,
            vec3d(Lemonx1 + a*(.4), Lemony1, 4),
            vec3d(90,90,0),
            vec3d(1,1,2),
            vec4d(.95,.89,0,1),
		    lemontexture
		);
			a++;
				if (Lemonx1 + a*(.4) >=30)
				{
					SpawnLemon1 = 0;
					a=0;
				}
				else if (Lemonx1 + a*(.4) == enemy1pos.x && Lemony1 == enemy1pos.y)
				{
					SpawnLemon1 = 0;
					a=0;
					enemy1health -= 1;
					//slog("Enemy Health %d",enemy1health);
				}
				else if (Lemonx1 + a*(.4) == enemy2pos.x && Lemony1 == enemy2pos.y)
				{
					SpawnLemon1 = 0;
					a=0;
					enemy2health -= 1;
					//slog("Enemy Health %d",enemy2health);
				}
				else if (gamestate == 1 && Lemonx1 + a*(.4) == Bosspos.x && Lemony1 == Bosspos.y)
				{
				SpawnLemon1 = 0;
				a=0;
				Bosshealth -= 1;
				//slog("Boss Health %d",Bosshealth);
				}
		}
		//Lemon2
		if (SpawnLemon2 != 0)
		{
			obj_draw(
			lemon,
            vec3d(Lemonx2 + b*(.4), Lemony2, 4),
            vec3d(90,90,0),
            vec3d(1,1,2),
            vec4d(.95,.89,0,1),
		    lemontexture
		);
			b++;
				if (Lemonx2 + b*(.4) >=30)
				{
					SpawnLemon2 = 0;
					b=0;
				}
				else if (Lemonx2 + b*(.4) == enemy1pos.x && Lemony2 == enemy1pos.y)
				{
					SpawnLemon2 = 0;
					b=0;
					enemy1health -= 1;
					//slog("Enemy Health %d",enemy1health);
				}
				else if (Lemonx2 + b*(.4) == enemy2pos.x && Lemony2 == enemy2pos.y)
				{
					SpawnLemon2 = 0;
					b=0;
					enemy2health -= 1;
					//slog("Enemy Health %d",enemy2health);
				}
				else if (gamestate == 1 && Lemonx2 + b*(.4) == Bosspos.x && Lemony2 == Bosspos.y)
				{
					SpawnLemon2 = 0;
					b=0;
					Bosshealth -= 1;
					//slog("Boss Health %d",Bosshealth);
				}
		}
		if (SpawnLemon3 != 0)
		{
			obj_draw(
			lemon,
            vec3d(Lemonx3 + c*(.4), Lemony3, 4),
            vec3d(90,90,0),
            vec3d(1,1,2),
            vec4d(.95,.89,0,1),
		    lemontexture
		);
			c++;
				//lemons fly off staege
				if (Lemonx3 + c*(.4) >=30)
				{
					SpawnLemon3 = 0;
					c=0;
				}
				//lemons collide with enemies
				else if (Lemonx3 + c*(.4) == enemy1pos.x && Lemony3 == enemy1pos.y)
				{
					SpawnLemon3 = 0;
					c=0;
					enemy1health -= 1;
					slog("Enemy Health %d",enemy1health);
				}
				else if (Lemonx3 + c*(.4) == enemy2pos.x && Lemony3 == enemy2pos.y)
				{
					SpawnLemon3 = 0;
					c=0;
					enemy2health -= 1;
					slog("Enemy Health %d",enemy2health);
				}
				else if (gamestate == 1 && Lemonx3 + c*(.4) == Bosspos.x && Lemony3 == Bosspos.y)
				{
					SpawnLemon3 = 0;
					c=0;
					Bosshealth -= 1;
					//slog("Boss Health %d",Bosshealth);
				}
		}
		if (SpawnBigLemon != 0)
		{
			obj_draw(
			lemon,
            vec3d(LemonxBig + d*(.4), LemonyBig, 4),
            vec3d(90,90,0),
            vec3d(2,2,4),
            vec4d(.9,.3,.1,1),
		    lemontexture
		);
			d++;
				if (LemonxBig + d*(.4) >=30)
				{
					SpawnBigLemon = 0;
					d=0;
				}
				else if (LemonxBig + d*(.4) == enemy1pos.x && LemonyBig == enemy1pos.y)
				{
					SpawnBigLemon = 0;
					d=0;
					enemy1health -= 5;
					//slog("Enemy Health %d",enemy1health);
				}
				else if (LemonxBig + d*(.4) == enemy2pos.x && LemonyBig == enemy2pos.y)
				{
					SpawnBigLemon = 0;
					d=0;
					enemy2health -= 5;
					//slog("Enemy Health %d",enemy2health);
				}
				else if (gamestate == 1 && LemonxBig + d*(.4) == Bosspos.x && LemonyBig == Bosspos.y)
				{
					SpawnBigLemon = 0;
					d=0;
					Bosshealth -= 5;
					slog("Boss Health %d",Bosshealth);
				}
		}

		
		//MAP
		obj_draw(
            mapp,
            vec3d(-15,0,2),
            vec3d(90,90,0),
            vec3d(5,5,5),
            vec4d(1,1,1,1),
		    maptexture1
		);
		obj_draw(
            mape,
            vec3d(15,0,2),
            vec3d(90,90,0),
            vec3d(5,5,5),
            vec4d(1,1,1,1),
		    maptexture2
		);
		

		//Enemies
		if (gamestate == 0)
		{

			if (enemy1health > 0)
			{
			//enemy_move(enemypos.x,enemypos.y, &enemypos);
			//enemy_attack1(Goomba_attack1, &enemy1pos);

			obj_draw(
            Goomba,
            vec3d(enemy1pos.x,enemy1pos.y,2),
            vec3d(90,-90,0),
            vec3d(0.1,0.1,0.1),
            vec4d(1,1,1,1),
            Goombatexture
			);
						
			if (Goomba_attack1 == 0)
			{
				Goomba_attack1 = rand_ranged( 1, 5); //start to (end - 1)
				if (Goomba_attack1 == 2)
				{
					Goomba_attack1 = 1;
				}
		
			};

			if (Goomba_attack1 == 1)
			{
				enemy1pos.x -=.1;
				if (enemy1pos.x <= -30)
				{
					Goomba_attack1 = 0;
					enemy1pos.x = 5;
				}
			}

			}
			else
			{
				enemy1pos.x = 50;
				enemy1pos.y = 50;
			}

			if (enemy2health > 0)
			{
			//enemy_move(enemypos.x,enemypos.y, &enemypos);
			//enemy_attack2(Goomba_attack2, &enemy2pos);
			obj_draw(
            Goomba,
            vec3d(enemy2pos.x,enemy2pos.y,2),
            vec3d(90,-90,0),
            vec3d(0.1,0.1,0.1),
            vec4d(1,1,1,1),
            Goombatexture
			);

			//bottom Goomba won't attack

			if (Goomba_attack2 == 0)
			{
				Goomba_attack2 = rand_ranged( 1, 5); //start to (end - 1)
				if (Goomba_attack2 == 2)
				{
					Goomba_attack2 = 1;
				}
		
			};

			if (Goomba_attack2 == 1)
			{
				enemy2pos.x -=.1;
				if (enemy2pos.x <= -30)
				{
					Goomba_attack2 = 0;
					enemy2pos.x = 5;
				}
			}

			
			}
			else
			{
				enemy2pos.x = 50;
				enemy2pos.y = 50;
			}
			
		}
		if (gamestate != 0)
		{
		if (Bosshealth > 0)
		{
			Boss_move(Bosspos.x,Bosspos.y, &Bosspos);      	    
			//Fire man
			obj_draw(
				boss,
				vec3d(Bosspos.x,Bosspos.y,2),
				vec3d(90,-90,0),
				vec3d(0.5,0.5,0.5),
				vec4d(1,1,1,1),
				bosstexture
				);

				obj_draw(
				BossHPBar,
				vec3d(30-Bosshealth/(2.5),20,2),
				vec3d(90,-90,0),
				vec3d(.9,.9,Bosshealth/(2.5)),
				vec4d(1,0,0,1),
				BosstextureHP
			);

		}
		else
		{
			obj_free(boss);

			Bosspos.x = 50;
			Bosspos.y = 50;
			slog("You WIN!");
		}
		

		}
        
        if (r > 360)r -= 360;
        glPopMatrix();
        /* drawing code above here! */
        graphics3d_next_frame();
	} 
	return 0;
	}


void set_camera(Vec3D position, Vec3D rotation)
{
    glRotatef(-rotation.x, 1.0f, 0.0f, 0.0f);
    glRotatef(-rotation.y, 0.0f, 1.0f, 0.0f);
    glRotatef(-rotation.z, 0.0f, 0.0f, 1.0f);
    glTranslatef(-position.x,
                 -position.y,
                 -position.z);
}

