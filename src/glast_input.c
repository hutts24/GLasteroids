/* glast_input.c
	Input handling functions.
	Translates user input into appropriate events
*/

//FIXME - allow different key mapping

#include <stdio.h>
#include <stdlib.h>

#include <blah.h>

#include "glast.h"
#include "glast_input.h"
#include "ship.h"
#include "glast_control.h"

/* Functions */

void glast_input_space(Blah_Input_Key *key) {
	if (!((Ship_Data*)ship->entityData)->bullet_delay) //If bullet delay zero
		Blah_Entity_sendEvent(ship, control_event_new(CONTROL_SHOOT));
}

void glast_input_lshift(Blah_Input_Key *key) {
	Blah_Entity_sendEvent(ship, control_event_new(CONTROL_THRUST));
}

void glast_input_up(Blah_Input_Key *key) {
	Blah_Entity_sendEvent(ship, control_event_new(CONTROL_PITCH_DOWN));
}

void glast_input_down(Blah_Input_Key *key) {
	Blah_Entity_sendEvent(ship, control_event_new(CONTROL_PITCH_UP));
}

void glast_input_left(Blah_Input_Key *key) {
	Blah_Entity_sendEvent(ship, control_event_new(CONTROL_YAW_LEFT));
}

void glast_input_right(Blah_Input_Key *key) {
	Blah_Entity_sendEvent(ship, control_event_new(CONTROL_YAW_RIGHT));
}
