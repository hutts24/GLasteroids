#ifndef _SHIP

#define _SHIP

#include <blah.h>

/* Constant Definitions */

#define SHIP_BULLET_SPEED	25
#define SHIP_BULLET_DISTANCE 1000
#define SHIP_BULLET_DELAY	10

/* Structures */

typedef struct { //ship's projectiles
	int distance;	//Distance bullet travels before dissipating
} Bullet_Data;

typedef struct { //Ship's own data
	unsigned int bullet_delay; //Counts delay before ship can shoot another bullet
} Ship_Data;

/* Function Prototypes */

/* Bullet Functions */

Blah_Entity *bullet_new(Blah_Entity *ship);
void bullet_move(Blah_Entity *bullet);

/* Ship functions */
Blah_Entity *ship_new();
void ship_shoot(Blah_Entity *ship);
void ship_thrust(Blah_Entity *ship);




#endif
