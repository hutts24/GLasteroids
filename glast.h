#ifndef _GLAST

#define _GLAST

#include "glast_entity.h"

/* FIXME - externs */
extern int *global_argc;
extern char** global_argv;
extern blah_bool cockpit_view;
extern Blah_Entity *ship;  //player's ship entity
extern Blah_Point external_viewpoint;
extern Blah_Overlay_Text *score_text;
extern unsigned int blah_score;

/* Constant Definitions */

#define AST_WORLD_LEFT -500
#define AST_WORLD_RIGHT 500
#define AST_WORLD_TOP 500
#define AST_WORLD_BOTTOM -500
#define AST_WORLD_FRONT 500
#define AST_WORLD_BACK -500

#define AST_WORLD_WIDTH (AST_WORLD_RIGHT-AST_WORLD_LEFT)
#define AST_WORLD_HEIGHT (AST_WORLD_TOP-AST_WORLD_BOTTOM)
#define AST_WORLD_DEPTH (AST_WORLD_FRONT-AST_WORLD_BACK)

#define AST_NUM_ASTEROIDS 7

#define AST_NEW_SIZE 4

#define AST_VIEW_ROTATION .03

/* End Constants */

#endif
