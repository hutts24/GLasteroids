#include <stdlib.h>
#include <stdio.h>

#include <blah/blah.h>

#include "asteroid.h"
#include "glast.h"
#include "ship.h"

extern Blah_Scene *glast_scene;

extern Blah_List *asteroid_list;
extern Blah_Texture *ast_texture;
extern Blah_Model *sputnik_model;
extern Blah_Model *fighter_model;
extern Blah_Model *alien_model;

/* Private Function Prototypes */

static void asteroid_shrink(Blah_Point *p);
static void asteroid_collision_handler(Blah_Entity *asteroid, Blah_Entity *other);

/* Function Declarations */

void asteroid_change_colour(Blah_Entity *asteroid) {
	Blah_Object_setColour((Blah_Object*)(asteroid->objects.first->data),(float)blah_util_randRangeInt(0,1000)/1000.0,
		(float)blah_util_randRangeInt(0,1000)/1000.0,(float)blah_util_randRangeInt(0,1000)/1000.0, 1);
}

void asteroid_dump(Blah_Entity *asteroid) {
	fprintf(stderr,"asteroid name:%s\n",asteroid->name);
}

blah_bool asteroid_explode(Blah_Entity *asteroid_ptr, Blah_Entity_Event *event) {
	char tempstring[20];
	Blah_Entity *tmp_ptr;
	ASTEROID_DATA *ast_data = ((ASTEROID_DATA*)asteroid_ptr->entityData);
	Blah_Object *obj_ptr = ((Blah_Entity_Object*)asteroid_ptr->objects.first->data)->object;
	
	//update global score status
	blah_score += (unsigned int)((int)2/ast_data->size);
	sprintf(tempstring, "Score: %d",blah_score);
	Blah_Overlay_Text_setText(score_text, tempstring);
	
	ast_data->size/=2; //half current size
	
	if (ast_data->size < 1) 
		Blah_Entity_sendEvent(asteroid_ptr, Blah_Entity_Event_new("ASTEROID DESTROY", asteroid_ptr, asteroid_destroy, 0));
	else {
		Blah_Entity_setVelocity(asteroid_ptr, blah_util_randRangeInt(-5,5),	
			blah_util_randRangeInt(-5,5), blah_util_randRangeInt(-5,5)); //assign random movement vector
		Blah_List_callFunction(&obj_ptr->vertices,(blah_list_element_func)asteroid_shrink);
		Blah_Object_updateBounds(obj_ptr);
		Blah_Object_scale(obj_ptr, 0.5f); //scale object to half current size
		tmp_ptr = asteroid_new(ast_data->size,ast_data->type);
		Blah_Entity_setLocation(tmp_ptr, asteroid_ptr->location.x, asteroid_ptr->location.y, asteroid_ptr->location.z);
		Blah_List_insertElement(asteroid_list, tmp_ptr); 
	}
	return BLAH_TRUE;
}

Blah_Entity *asteroid_new(float size, int type) {
	//All properties are randomly assigned
	Blah_Entity *new_ast;
	Blah_Object *new_object;
	ASTEROID_DATA *ast_data;
//	FILE *tanker_file;
	Blah_Material *new_material;
	
	//fprintf(stderr,"creating new asteroid");
	
//	float radius = size * 20;
	char temp_name[20];
	
	sprintf(temp_name,"Ast#%d",type);
	
	new_ast = Blah_Entity_new(temp_name, GLAST_ENTITY_ASTEROID, sizeof(ASTEROID_DATA));
	ast_data = (ASTEROID_DATA*)new_ast->entityData;
	
	Blah_Entity_setLocation(new_ast, blah_util_randRangeInt(AST_WORLD_LEFT,AST_WORLD_RIGHT),
		blah_util_randRangeInt(AST_WORLD_BOTTOM,AST_WORLD_TOP),
		blah_util_randRangeInt(AST_WORLD_BACK,AST_WORLD_FRONT)
	); 
	
	Blah_Entity_setVelocity(new_ast, blah_util_randRangeInt(-5,5), blah_util_randRangeInt(-5,5), blah_util_randRangeInt(-5,5));
	
	ast_data->size=size;
	ast_data->type=type;
		
	Blah_Entity_setMoveFunction(new_ast, (blah_entity_move_func)asteroid_move);
	
	Blah_Entity_setCollisionFunction(new_ast,(blah_entity_collision_func)asteroid_collision_handler); 
	Blah_Entity_setActiveCollision(new_ast,BLAH_TRUE);  
	
	//new_object = Blah_Object_new(new_ast); //Add all vertices to the asteroid object
	
	//fprintf(stderr,"creating object from sputnik model\n");
	
	switch (type) {
		case 0:
			new_object = Blah_Object_fromModel(sputnik_model);
			Blah_Entity_addObject(new_ast, new_object);
			break;
		case 1:
			new_object = Blah_Object_fromModel(fighter_model);
			Blah_Entity_addObject(new_ast, new_object);
			new_material = Blah_Material_new();
			Blah_Material_setAmbient(new_material,(float)blah_util_randRangeInt(0,1000)/1000.0,
				(float)blah_util_randRangeInt(0,1000)/1000.0,(float)blah_util_randRangeInt(0,1000)/1000.0, (float)blah_util_randRangeInt(0,1000)/1000.0);
			Blah_Material_setDiffuse(new_material,(float)blah_util_randRangeInt(0,1000)/1000.0,
				(float)blah_util_randRangeInt(0,1000)/1000.0,(float)blah_util_randRangeInt(0,1000)/1000.0, (float)blah_util_randRangeInt(0,1000)/1000.0);
			Blah_Material_setSpecular(new_material,(float)blah_util_randRangeInt(0,1000)/1000.0,
				(float)blah_util_randRangeInt(0,1000)/1000.0,(float)blah_util_randRangeInt(0,1000)/1000.0, (float)blah_util_randRangeInt(0,1000)/1000.0);
			Blah_Material_setEmission(new_material,(float)blah_util_randRangeInt(0,1000)/1000.0,
				(float)blah_util_randRangeInt(0,1000)/1000.0,(float)blah_util_randRangeInt(0,1000)/1000.0, (float)blah_util_randRangeInt(0,1000)/1000.0);
			
			Blah_Object_addMaterial(new_object,new_material);
			Blah_Object_setMaterial(new_object,new_material);
			break;
		case 2:
			new_object = Blah_Object_fromModel(alien_model);
			Blah_Entity_addObject(new_ast, new_object);
			break;
	}
	
	//fprintf(stderr,"call to colour\n");
	//Blah_Object_colour(new_object,(float)blah_util_randRangeInt(0,1000)/1000.0,
	//	(float)blah_util_randRangeInt(0,1000)/1000.0,(float)blah_util_randRangeInt(0,1000)/1000.0);
	//Blah_Object_map_texture_auto(new_object, ast_texture);
	//fprintf(stderr,"end colour\n");
	//fprintf(stderr, "set colour to asteroid object from file");
	
	//fprintf(stderr, "added asteroid object to asteroid");
	new_ast->rotationAxisX = .01;
	new_ast->rotationAxisY = .02;
	
	Blah_Scene_addEntity(glast_scene, new_ast);
	
	return new_ast;
}

void asteroid_move(Blah_Entity *astp) {
	
	if ((astp->location.x<AST_WORLD_LEFT) || (astp->location.x>AST_WORLD_RIGHT))
		astp->velocity.x=-astp->velocity.x;
	if ((astp->location.y<AST_WORLD_BOTTOM) || (astp->location.y>AST_WORLD_TOP))
		astp->velocity.y=-astp->velocity.y;
	if ((astp->location.z<AST_WORLD_BACK) || (astp->location.z>AST_WORLD_FRONT))
		astp->velocity.z=-astp->velocity.z; 
}

static void asteroid_shrink(Blah_Point *p) {
	p->x/=2;
	p->y/=2;
	p->z/=2;
}

blah_bool asteroid_destroy(Blah_Entity *asteroid, Blah_Entity_Event *dest_event) {
	//fprintf(stderr,"destroying asteroid\n");
	Blah_Scene_removeEntity(glast_scene, asteroid);
	Blah_List_removeElement(asteroid_list, asteroid); //have to remove it from list of asteroids
	Blah_Entity_destroy(asteroid);
	if (asteroid_list->length < 16)
		Blah_List_insertElement(asteroid_list, asteroid_new(AST_NEW_SIZE, blah_util_randRangeInt(0,2)));
	//fprintf(stderr,"asteroid destroyed\n");
	
	return BLAH_FALSE;	//don't continue processing events
}

static void asteroid_collision_handler(Blah_Entity *asteroid, Blah_Entity *other) {
	if (other->type==GLAST_ENTITY_SHIP_BULLET) // || other->type==GLAST_ENTITY_ASTEROID) 
		Blah_Entity_sendEvent(asteroid, Blah_Entity_Event_new("ASTEROID EXPLODE", asteroid, asteroid_explode, 0));
	
	//else ignore all other collisions for this moment
}
