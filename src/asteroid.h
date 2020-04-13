#ifndef _ASTEROID

#define _ASTEROID

#include <blah.h>

/* Structure definitions */

typedef struct Asteroid_Data {
	float size;
	int type;
} ASTEROID_DATA;

/* Asteroid function prototypes */

bool asteroid_explode(Blah_Entity *ast_ptr, Blah_Entity_Event *event);
Blah_Entity *asteroid_new(float size, int type);
void animate_asteroid(Blah_Entity *astp);
void asteroid_change_colour(Blah_Entity *astp);
void asteroid_move(Blah_Entity *astp);
void asteroid_dump(Blah_Entity *astp);
bool asteroid_destroy(Blah_Entity *asteroid, Blah_Entity_Event *dest_event);

#endif
