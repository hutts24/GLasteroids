/* glast_control.h */

#ifndef _GLAST_CONTROL

#define _GLAST_CONTROL

#include <blah/blah.h>

/* Structure Definitions */

typedef enum Control_Event_Type {CONTROL_PITCH_UP, CONTROL_PITCH_DOWN, CONTROL_YAW_LEFT,
	CONTROL_YAW_RIGHT, CONTROL_THRUST, CONTROL_SHOOT} Control_Event_Type;

typedef struct Control_Event_Data {
	Control_Event_Type type;
} Control_Event_Data;

Blah_Entity_Event *control_event_new(Control_Event_Type type);
blah_bool ship_control(Blah_Entity *ship, Blah_Entity_Event *control_event);

#endif
