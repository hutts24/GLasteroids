#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <blah.h>

#include "ship.h"
#include "glast.h"
#include "glast_control.h"

extern Blah_Model *starfury_body, *starfury_cockpit, *starfury_largefin,
	*starfury_smallfin, *starfury_engine, *voyager_model;

extern Blah_Scene *glast_scene;

/* externs - yuck */

extern Blah_Texture *ship_texture, *bullet_texture;

/* Function Declarations */

static void ship_move(Blah_Entity *ship);
static bool bullet_destroy(Blah_Entity *bullet, Blah_Entity_Event *dest_event);
static void bullet_collide(Blah_Entity *bullet, Blah_Entity *other);

Blah_Entity *ship_new() {
	Blah_Entity *ship = Blah_Entity_new("player ship", GLAST_ENTITY_PLAYER_SHIP, sizeof(Ship_Data));

	Blah_Object *ship_obj;

	ship_obj = Blah_Object_fromModel(voyager_model);
	Blah_Entity_addObject(ship, ship_obj);

	Blah_Entity_setMoveFunction(ship, (blah_entity_move_func*)ship_move);

	((Ship_Data*)ship->entityData)->bullet_delay=0;

	return ship;
}

Blah_Entity *bullet_new(Blah_Entity *ship) {
	Blah_Entity *new_bullet = Blah_Entity_new("ship_bullet", GLAST_ENTITY_SHIP_BULLET, sizeof(Bullet_Data));
	Blah_Object *bullet_obj = Blah_Object_new();
	Blah_Material *new_material = Blah_Material_new();

	Blah_Vertex *front = Blah_Vertex_new(0,0,5);
	Blah_Vertex *back = Blah_Vertex_new(0,0,-5);
	Blah_Vertex *top = Blah_Vertex_new(0,5,0);
	Blah_Vertex *bottom = Blah_Vertex_new(0,-5,0);
	Blah_Vertex *left = Blah_Vertex_new(-5,0,0);
	Blah_Vertex *right = Blah_Vertex_new(5,0,0);

	Blah_Vertex *bullet_top[] = {left, front, top, back, right, front, NULL};
	Blah_Vertex *bullet_bottom[] = {left, front, bottom, back, right, front, NULL};

	Blah_Primitive *bullet_prim1 = Blah_Primitive_new(BLAH_PRIMITIVE_TRIANLGE_STRIP, bullet_top, sizeof(bullet_top) / sizeof(Blah_Vertex*));
	Blah_Primitive_mapTextureAuto(bullet_prim1, bullet_texture);
	Blah_Primitive *bullet_prim2 = Blah_Primitive_new(BLAH_PRIMITIVE_TRIANLGE_STRIP, bullet_bottom, sizeof(bullet_bottom) / sizeof(Blah_Vertex*));
	Blah_Primitive_mapTextureAuto(bullet_prim2, bullet_texture);

	Blah_Object_addPrimitive(bullet_obj, bullet_prim1);
	Blah_Object_addPrimitive(bullet_obj, bullet_prim2);
	Blah_Material_setColour(new_material, 1,1,1,1);
	Blah_Object_addMaterial(bullet_obj, new_material);
	Blah_Object_setMaterial(bullet_obj, new_material);

	Blah_Entity_addObject(new_bullet, bullet_obj); //create bullet object and add to entity

	((Bullet_Data*)new_bullet->entityData)->distance = SHIP_BULLET_DISTANCE;  //assign entity data
	Blah_Entity_setVelocity(new_bullet, ship->axisZ.x, ship->axisZ.y, ship->axisZ.z); //assign entity properties
	Blah_Entity_setLocation(new_bullet, ship->location.x, ship->location.y, ship->location.z);
	Blah_Vector_setMagnitude(&new_bullet->velocity,SHIP_BULLET_SPEED);
	Blah_Entity_setMoveFunction(new_bullet, (blah_entity_move_func*)bullet_move);

	new_bullet->rotationAxisX = .1;

	Blah_Entity_setActiveCollision(new_bullet,true);
	Blah_Entity_setCollisionFunction(new_bullet,(blah_entity_collision_func*)bullet_collide);

	Blah_Scene_addEntity(glast_scene, new_bullet);

	return new_bullet;
}

static bool bullet_destroy(Blah_Entity *bullet, Blah_Entity_Event *dest_event) {
	Blah_Scene_removeEntity(glast_scene, bullet);
	Blah_Entity_destroy(bullet);
	return false; //Return false means the entity is destroyed and don't continue
}


static void bullet_collide(Blah_Entity *bullet, Blah_Entity *other) {
	if (other->type == GLAST_ENTITY_ASTEROID) {  //If bullet is colliding with an asteroid
		Blah_Entity_sendEvent(bullet, Blah_Entity_Event_new("DESTROY", NULL, bullet_destroy,0)); //Tell bullet to die
	}
}

void bullet_move(Blah_Entity *bullet) {
	Bullet_Data *bul_dat = (Bullet_Data*)bullet->entityData;


	bul_dat->distance-=(int)Blah_Vector_getMagnitude(&bullet->velocity);
	if (bul_dat->distance < 0) {
		Blah_Entity_sendEvent(bullet, Blah_Entity_Event_new("DESTROY", bullet, bullet_destroy,0)); //Tell bullet to die
	}
}

void ship_shoot(Blah_Entity *ship) {
	bullet_new(ship);
	((Ship_Data*)ship->entityData)->bullet_delay = SHIP_BULLET_DELAY;
}

void ship_thrust(Blah_Entity *ship) {
	Blah_Vector temp_vector;

	memcpy(&temp_vector, &ship->axisZ, sizeof(Blah_Vector));
	Blah_Vector_normalise(&temp_vector);
	Blah_Vector_addVector(&ship->velocity, &temp_vector);
}

static void ship_move(Blah_Entity *ship) {
	Blah_Vector temp_vector;
	Blah_Point new_viewpoint;
	Blah_Point new_focal_point;

	if (((Ship_Data*)ship->entityData)->bullet_delay) //If bullet delay hasn't reached zero yet
		((Ship_Data*)ship->entityData)->bullet_delay--; //decrement count

	if ((ship->location.x + ship->velocity.x <AST_WORLD_LEFT) | (ship->location.x + ship->velocity.x>AST_WORLD_RIGHT))
		ship->velocity.x=-ship->velocity.x * .7;
	if ((ship->location.y + ship->velocity.y <AST_WORLD_BOTTOM) | (ship->location.y + ship->velocity.y >AST_WORLD_TOP))
		ship->velocity.y=-ship->velocity.y * .7;
	if ((ship->location.z + ship->velocity.z <AST_WORLD_BACK) | (ship->location.z + ship->velocity.z >AST_WORLD_FRONT))
		ship->velocity.z=-ship->velocity.z* .7;

	Blah_Vector_scale(&ship->velocity, .98);

	//Update view

	if (cockpit_view) {
		//copy ship's location into to new points for focal and viewpoint
		memcpy(&new_viewpoint, &ship->location, sizeof(Blah_Point));
		memcpy(&new_focal_point, &ship->location, sizeof(Blah_Point));
		//translate viewpoint behind ship
		memcpy(&temp_vector, &ship->axisZ, sizeof(Blah_Vector));
		Blah_Vector_setMagnitude(&temp_vector,100);
		Blah_Vector_invert(&temp_vector);
		Blah_Point_translateByVector(&new_viewpoint, &temp_vector);
		//translate viewpoint above on ship's y axis
		memcpy(&temp_vector, &ship->axisY, sizeof(Blah_Vector));
		Blah_Vector_setMagnitude(&temp_vector,60);
		Blah_Point_translateByVector(&new_focal_point, &temp_vector);
		//translate focal point to above ship on y axis
		Blah_Vector_setMagnitude(&temp_vector,80);
		Blah_Point_translateByVector(&new_viewpoint, &temp_vector);
		//update view parameters
		blah_draw_setFocalPoint(new_focal_point.x, new_focal_point.y, new_focal_point.z); //focus apon origin
		blah_draw_setViewpoint(new_viewpoint.x, new_viewpoint.y, new_viewpoint.z);
		blah_draw_setFieldOfVision(2, 2);
		blah_draw_setDepthOfVision(AST_WORLD_DEPTH+700);
		blah_draw_setViewNormal(ship->axisY.x, ship->axisY.y, ship->axisY.z);
	} else {
		blah_draw_setFocalPoint(0,0,0); //focus apon origin
		blah_draw_setViewpoint(external_viewpoint.x, external_viewpoint.y, external_viewpoint.z);
		blah_draw_setFieldOfVision(1.3, 1.3);
		blah_draw_setDepthOfVision(AST_WORLD_DEPTH+700);
		blah_draw_setViewNormal(0,1,0);
	}
}
