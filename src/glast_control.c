/* glast_control.c */

#include <stdio.h>
#include <stdlib.h>

#include <blah.h>

#include "glast.h"
#include "glast_control.h"
#include "ship.h"

/* Functions */

bool ship_control(Blah_Entity *ship, Blah_Entity_Event *control_event) {
	//Event handling function for ship when receiving a control event
	//fprintf(stderr,"enter ship_control\n");


	switch (((Control_Event_Data*)control_event->eventData)->type) {
		case CONTROL_YAW_LEFT :
			Blah_Entity_rotateEuler(ship, 0, .1, 0);
			break;
		case CONTROL_YAW_RIGHT :
			Blah_Entity_rotateEuler(ship, 0, -.1, 0);
			break;
		case CONTROL_PITCH_DOWN :
			Blah_Entity_rotateEuler(ship, .1, 0, 0);
			break;
		case CONTROL_PITCH_UP :
			Blah_Entity_rotateEuler(ship, -.1, 0, 0);
			break;
		case CONTROL_SHOOT :
			ship_shoot(ship);
			break;
		case CONTROL_THRUST : //Add velocity to ship in direction facing
			ship_thrust(ship);
			break;
		default:
			break;
	}

	//fprintf(stderr,"exit ship_control\n");
	return true;
}

Blah_Entity_Event *control_event_new(Control_Event_Type type) {
	Blah_Entity_Event *new_event = Blah_Entity_Event_new("ship control", NULL,
		ship_control, sizeof(Control_Event_Data));
	((Control_Event_Data*)new_event->eventData)->type = type;
	//fprintf(stderr,"made new ship control event\n");
	return new_event;
}
