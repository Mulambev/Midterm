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
		slog( "fuck" );
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

/*Entity *Player(Vec3D position, const char *name)
	{
		Entity * ent;
		char buffer[255];
		int i;
		ent = entity_new();
		if (!ent)
		{
			return NULL;
		}

		ent->objModel = obj_load("models/MPU.obj");
		ent->texture = LoadSprite("models/MegaManBody1.png",1024,1024);
		vec3d_cpy(ent->body.position,position);
		cube_set(ent->body.bounds,-2.5,-2.5,0,2.5,2.5,2);
		ent->think = think;
		ent->state = 0;
		mgl_callback_set(&ent->body.touch,touch_callback,ent);
		return ent;

		
		
		
		/*1,					//inuse
		1,					// uid
		megaman,			//name[128]
		vec3d(f, g, 2),		//position
		vec3d(90,90,0),		//rotation
		vec3d(0.5,0.5,0.5),	//scale
		vec4d(1,1,1,1),		//color
		megaman,			//model
		megaman[0],			//animation
		100,				//health
		1,					//state
		0,					//frame
		megamantexture,		//texture
		//Body body			//body
		1;					//group
		//void (*think)(struct Entity_S *self); //beh? think function maybe
	}*/


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
    Space *space;
    Entity *cube1,*cube2, *Actor;
    char bGameLoopRunning = 1;
    Vec3D cameraPosition = {0,0,70.3};
    Vec3D cameraRotation = {0,0,360};
    SDL_Event e;
    Obj *megaman,*boss, *mapp, *mape, *lemon;
    Sprite *megamantexture, *bosstexture, *maptexture1, *maptexture2, *lemontexture;
    
    init_logger("gametest3d.log");
    if (graphics3d_init(1024,768,1,"gametest3d",33) != 0)
    {
        return -1;
    }
    model_init();
    obj_init();
    entity_init(255);
    
   //load objects, models here, replace cube with megaman
   // megaman = obj_load("models/MPU.obj");
	//load his uv map "texture file"
    //megamantexture = LoadSprite("models/MegaManBody1.png",1024,1024);

	boss = obj_load("models/Fireman.obj");
	bosstexture = LoadSprite("models/FireManBody1.png",1024,1024);

    mapp = obj_load("models/MidtermMapSingle.obj");
	maptexture1 = LoadSprite("models/PlayerTile(Red).png",1024,1024);

	mape = obj_load("models/MidtermMapSingle.obj");
	maptexture2 = LoadSprite("models/BossTile(Blue).png",1024,1024);
    
    lemon = obj_load("models/cube.obj");
	lemontexture = LoadSprite("models/cube_text.png",1024,1024);
    
	//Actor = Player(vec3d(f,g,2), "Megaman");
	
    cube1 = newCube(vec3d(u,y,2),"Cubert");
    //cube2 = newCube(vec3d(10,0,5),"Hobbes");
    
    //cube2->body.velocity.x = -0.1;
    
    space = space_new();
    space_set_steps(space,100);
    
    //space_add_body(space,&cube1->body);
    //space_add_body(space,&cube2->body);

    while (bGameLoopRunning)
    {
        entity_think_all();

        for (i = 0; i < 100;i++)
        {
            space_do_step(space);
        }
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
                else if (e.key.keysym.sym == SDLK_SPACE)
                {
                    cameraPosition.z++;
                }
                else if (e.key.keysym.sym == SDLK_z)
                {
                    cameraPosition.z--;
                }
                else if (e.key.keysym.sym == SDLK_w)
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
                else if (e.key.keysym.sym == SDLK_s)
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
                else if (e.key.keysym.sym == SDLK_d)
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
                else if (e.key.keysym.sym == SDLK_a)
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
				else if (e.key.keysym.sym == SDLK_j)
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
				else if (e.key.keysym.sym == SDLK_g)
                {
                   
				   f -= 10;
				   if(f <= -20)
				   {
					   f = -25;
				   }
				  			
                }
				else if (e.key.keysym.sym == SDLK_y)
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
				else if (e.key.keysym.sym == SDLK_h)
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
                else if (e.key.keysym.sym == SDLK_n)
                {
                    cube1->state ++;
                    if (cube1->state >= 3)cube1->state = 0;

					/*//Boss_move(u,y);
					q = q % 2;
					switch(q)
					{
					case 0:
							maptexture = LoadSprite("models/PlayerTile(Red).png",1024,1024);
							break;
					case 1:
							maptexture = LoadSprite("models/BossTile(Blue).png",1024,1024);
							break;
					};

					q+=1;*/

                }
            }
        }

                
        graphics3d_frame_begin();
        
        glPushMatrix();
        set_camera(
            cameraPosition,
            cameraRotation);
        
        entity_draw_all();
      
   
		//Megaman
        obj_draw(
            megaman,
            vec3d(f, g, 2),
            vec3d(90,90,0),
            vec3d(0.5,0.5,0.5),
            vec4d(1,1,1,1),
            megamantexture
        );

		//entity_draw(player);


		/*
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
			*/

       	    
		
		/*//Fire man
        obj_draw(
            boss,
            vec3d(u,y,2),
            vec3d(90,-90,0),
            vec3d(0.5,0.5,0.5),
            vec4d(1,1,1,1),
            bosstexture
        );*/
        
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

