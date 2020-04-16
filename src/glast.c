#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <signal.h>

#include <blah.h>

#include "asteroid.h"
#include "glast.h"
#include "ship.h"
#include "glast_control.h"
#include "glast_input.h"

/* Globals - yucky */

int *global_argc;
char** global_argv;

bool cockpit_view = false;

Blah_Font *glast_font;
Blah_Font *tex_font;
Blah_Entity *ship;  //player's ship entity
Blah_Image *temp_image;
Blah_Image *ast_image;
Blah_Image *ship_image;
Blah_Image *bullet_image;
Blah_Image *copy_image;
Blah_List *asteroid_list=NULL;
Blah_Scene_Object *axes,*walls;
Blah_Texture *temp_texture;
Blah_Texture *ast_texture;
Blah_Texture *ship_texture;
Blah_Texture *bullet_texture;
Blah_Model *sputnik_model;
Blah_Model *fighter_model;
Blah_Model *alien_model;
Blah_Model *starfury_body, *starfury_cockpit, *starfury_engine, *starfury_largefin, *starfury_smallfin;
Blah_Model *voyager_model;
Blah_Model *fighter_model;
Blah_Point external_viewpoint = {150,150,+750};
Blah_Scene *glast_scene;
Blah_Overlay *help_overlay;
Blah_Overlay *score_overlay;
Blah_Overlay_Text *score_text;
unsigned int blah_score = 0;
Blah_Light *light_one, *light_two;

/* end globals */

/* Function Declarations */

static void toggle_full_screen() {
	blah_video_setFullScreen(!blah_video_isFullScreen());
}

/* static void toggle_double_buffer() {
	blah_video_set_double_buffered(!blah_video_is_double_buffered());
} */

static void toggle_help() {
	Blah_Overlay_setVisible(help_overlay, !Blah_Overlay_isVisible(help_overlay));
}

static void toggle_axes() {
	Blah_Scene_Object_setVisible(axes, !Blah_Scene_Object_isVisible(axes));
}

static void toggle_view() {
	cockpit_view = !cockpit_view;
}

static Blah_Font *load_raster_font() {
	Blah_Image *test_font_image;
	Blah_Font *test_font;

	unsigned int font_map[256];
	memset(font_map, 0, sizeof(font_map));
    // TODO - use advanced initialisation here
	font_map['A']=91;font_map['B']=92;font_map['C']=93;font_map['D']=94;
	font_map['E']=95;font_map['F']=96;font_map['G']=97;font_map['H']=98;
	font_map['I']=99;font_map['J']=100;font_map['K']=81;font_map['L']=82;
	font_map['M']=83;font_map['N']=84;font_map['O']=85;font_map['P']=86;
	font_map['Q']=87;font_map['R']=88;font_map['S']=89;font_map['T']=90;
	font_map['U']=71;font_map['V']=72;font_map['W']=73;font_map['X']=74;
	font_map['Y']=75;font_map['Z']=76;font_map['a']=77;font_map['b']=78;
	font_map['c']=79;font_map['d']=80;font_map['e']=61;font_map['f']=62;
	font_map['g']=63;font_map['h']=64;font_map['i']=65;font_map['j']=66;
	font_map['k']=67;font_map['l']=68;font_map['m']=69;font_map['n']=70;
	font_map['o']=51;font_map['p']=52;font_map['q']=53;font_map['r']=54;
	font_map['s']=55;font_map['t']=56;font_map['u']=57;font_map['v']=58;
	font_map['w']=59;font_map['x']=60;font_map['y']=41;font_map['z']=42;
	font_map['1']=43;font_map['2']=44;font_map['3']=45;font_map['4']=46;
	font_map['5']=47;font_map['6']=48;font_map['7']=49;font_map['8']=50;
	font_map['9']=31;font_map['0']=32;font_map['!']=33;font_map['@']=34;
	font_map['#']=35;font_map['$']=36;font_map['%']=37;font_map['^']=38;
	font_map['&']=39;font_map['*']=40;font_map['(']=21;font_map[')']=22;
	font_map['~']=23;font_map['`']=24;font_map['-']=25;font_map['_']=26;
	font_map['=']=27;font_map['+']=28;font_map['[']=29;font_map[']']=30;
	font_map['{']=11;font_map['}']=12;font_map[':']=13;font_map[';']=14;
	font_map['"']=15;font_map['\'']=16;font_map['<']=17;font_map['>']=18;
	font_map[',']=19;font_map['?']=20;font_map['!']=1;font_map['.']=2;
	font_map['|']=3;font_map['\\']=4;font_map[' ']=5;

	test_font_image = Blah_Image_fromFile("glastfont12x16.tga");
	test_font = Blah_Font_new(BLAH_FONT_RASTER, "glastfont", test_font_image, font_map, 12, 16);
	Blah_Image_destroy(test_font_image);

	return test_font;
}



 static Blah_Font *load_texture_font() {
	Blah_Image *test_font_image;
	Blah_Font *test_font;

	unsigned int font_map[256];
	memset(font_map, 0, sizeof(font_map));

	font_map['A']=91;font_map['B']=92;font_map['C']=93;font_map['D']=94;
	font_map['E']=95;font_map['F']=96;font_map['G']=97;font_map['H']=98;
	font_map['I']=99;font_map['J']=100;font_map['K']=81;font_map['L']=82;
	font_map['M']=83;font_map['N']=84;font_map['O']=85;font_map['P']=86;
	font_map['Q']=87;font_map['R']=88;font_map['S']=89;font_map['T']=90;
	font_map['U']=71;font_map['V']=72;font_map['W']=73;font_map['X']=74;
	font_map['Y']=75;font_map['Z']=76;font_map['a']=77;font_map['b']=78;
	font_map['c']=79;font_map['d']=80;font_map['e']=61;font_map['f']=62;
	font_map['g']=63;font_map['h']=64;font_map['i']=65;font_map['j']=66;
	font_map['k']=67;font_map['l']=68;font_map['m']=69;font_map['n']=70;
	font_map['o']=51;font_map['p']=52;font_map['q']=53;font_map['r']=54;
	font_map['s']=55;font_map['t']=56;font_map['u']=57;font_map['v']=58;
	font_map['w']=59;font_map['x']=60;font_map['y']=41;font_map['z']=42;
	font_map['1']=43;font_map['2']=44;font_map['3']=45;font_map['4']=46;
	font_map['5']=47;font_map['6']=48;font_map['7']=49;font_map['8']=50;
	font_map['9']=31;font_map['0']=32;font_map['!']=33;font_map['@']=34;
	font_map['#']=35;font_map['$']=36;font_map['%']=37;font_map['^']=38;
	font_map['&']=39;font_map['*']=40;font_map['(']=21;font_map[')']=22;
	font_map['~']=23;font_map['`']=24;font_map['-']=25;font_map['_']=26;
	font_map['=']=27;font_map['+']=28;font_map['[']=29;font_map[']']=30;
	font_map['{']=11;font_map['}']=12;font_map[':']=13;font_map[';']=14;
	font_map['"']=15;font_map['\'']=16;font_map['<']=17;font_map['>']=18;
	font_map[',']=19;font_map['?']=20;font_map['!']=1;font_map['.']=2;
	font_map['|']=3;font_map['\\']=4;font_map[' ']=5;

	test_font_image = Blah_Image_fromFile("glastfont16x16.tga");
	//fprintf(stderr,"loaded glastfont.tga");
	test_font = Blah_Font_new(BLAH_FONT_TEXTURE, "texfont", test_font_image, font_map, 16, 16);
	//fprintf(stderr,"made font from glastfont.tga");
	Blah_Image_destroy(test_font_image);

	return test_font;
}


static Blah_Overlay *help_overlay_new() {
	 char *help_string = "left + right arrow keys - yaw the ship left and right.\n\n\
up + down arrow keys - pitch up and down.\n\n\
left shift - thrust the ship in the direction it is facing.\n\n\
spacebar - fire shots.\n\n\
't' - toggle the display of the world axes.\n\n\
'v' - toggle the between external world view and ship.\n\n\
'z' and 'x' - rotate the external viewpoint about y axis.\n\n\
'h' - toggle this help display.\n\n\
escape - quit the demo.";

	//char *help_string = "A";

	Blah_Overlay *new_overlay = Blah_Overlay_new(1, "test_overlay", 300,300);
	Blah_Overlay_addText(new_overlay, "help text", help_string, tex_font, 0,260);

	return new_overlay;
}

static Blah_Overlay *score_overlay_new() {
	Blah_Overlay *new_overlay = Blah_Overlay_new(2, "score overlay", 100,20);
	score_text = Blah_Overlay_addText(new_overlay, "score text",
		"Score: 0         ", tex_font, 0,0);

	return new_overlay;
}

static Blah_Scene_Object *axes_new() {
	Blah_Object *axes = Blah_Object_new(NULL);
	Blah_Material *red, *blue, *green;

	Blah_Primitive *axis_x, *axis_y, *axis_z;

	Blah_Vertex *origin = Blah_Object_addVertex(axes,0,0,0);

	Blah_Vertex *far_x = Blah_Object_addVertex(axes,1000,0,0);
	Blah_Vertex *x1 = Blah_Object_addVertex(axes,AST_WORLD_RIGHT,20,20);
	Blah_Vertex *x2 = Blah_Object_addVertex(axes,AST_WORLD_RIGHT,-20,-20);
	Blah_Vertex *x3 = Blah_Object_addVertex(axes,AST_WORLD_RIGHT,20,-20);
	Blah_Vertex *x4 = Blah_Object_addVertex(axes,AST_WORLD_RIGHT,-20,20);
	Blah_Vertex *x_axis[] = {origin,far_x,x1,x2,x3,x4};

	Blah_Vertex *far_y = Blah_Object_addVertex(axes,0,1000,0);
	Blah_Vertex *y1 = Blah_Object_addVertex(axes,20,AST_WORLD_TOP,20);
	Blah_Vertex *y2 = Blah_Object_addVertex(axes,-20,AST_WORLD_TOP,-20);
	Blah_Vertex *y3 = Blah_Object_addVertex(axes,20,AST_WORLD_TOP,-20);
	Blah_Vertex *y4 = Blah_Object_addVertex(axes,-20,AST_WORLD_TOP,20);
	Blah_Vertex *y_axis[] = {origin,far_y,y1,y2,y3,y4};

	Blah_Vertex *far_z = Blah_Object_addVertex(axes,0,0,1000);
	Blah_Vertex *z1 = Blah_Object_addVertex(axes,20,20,AST_WORLD_FRONT);
	Blah_Vertex *z2 = Blah_Object_addVertex(axes,-20,-20,AST_WORLD_FRONT);
	Blah_Vertex *z3 = Blah_Object_addVertex(axes,-20,20,AST_WORLD_FRONT);
	Blah_Vertex *z4 = Blah_Object_addVertex(axes,20,-20,AST_WORLD_FRONT);
	Blah_Vertex *z_axis[] = {origin,far_z,z1,z2,z3,z4};

	axis_x = Blah_Primitive_new(BLAH_PRIMITIVE_LINE, x_axis, blah_countof(x_axis));
	//Blah_Primitive_set_colour(axis_x,1, 0, 0, 1);
	red = Blah_Material_new();Blah_Material_setColour(red, 1, 0, 0, 1);
	Blah_Object_addMaterial(axes, red);
	Blah_Primitive_setMaterial(axis_x, red);
	Blah_Object_addPrimitive(axes, axis_x);

	axis_y = Blah_Primitive_new(BLAH_PRIMITIVE_LINE, y_axis, blah_countof(y_axis));
	//Blah_Primitive_set_colour(axis_y, 0, 1 , 0, 1);
	green = Blah_Material_new();Blah_Material_setColour(green, 0, 1, 0, 1);
	Blah_Object_addMaterial(axes, green);
	Blah_Primitive_setMaterial(axis_y, green);
	Blah_Object_addPrimitive(axes, axis_y);

	axis_z = Blah_Primitive_new(BLAH_PRIMITIVE_LINE, z_axis, blah_countof(z_axis));
	//Blah_Primitive_set_colour(axis_z, 0, 0, 1, 1);
	blue = Blah_Material_new();Blah_Material_setColour(blue, 0, 0, 1, 1);
	Blah_Object_addMaterial(axes, blue);
	Blah_Primitive_setMaterial(axis_z, blue);
	Blah_Object_addPrimitive(axes, axis_z);

	return Blah_Scene_addObject(glast_scene,axes);
}

static Blah_Scene_Object *walls_new() {
	Blah_Object *walls = Blah_Object_new(NULL);
	Blah_Material *new_material = Blah_Material_new();

	Blah_Vertex *left_top_back = Blah_Object_addVertex(walls, AST_WORLD_LEFT, AST_WORLD_TOP, AST_WORLD_BACK);
	Blah_Vertex *right_top_back = Blah_Object_addVertex(walls, AST_WORLD_RIGHT, AST_WORLD_TOP, AST_WORLD_BACK);
	Blah_Vertex *left_bottom_back = Blah_Object_addVertex(walls, AST_WORLD_LEFT, AST_WORLD_BOTTOM, AST_WORLD_BACK);
	Blah_Vertex *right_bottom_back = Blah_Object_addVertex(walls, AST_WORLD_RIGHT, AST_WORLD_BOTTOM, AST_WORLD_BACK);
	Blah_Vertex *left_top_front = Blah_Object_addVertex(walls, AST_WORLD_LEFT, AST_WORLD_TOP, AST_WORLD_FRONT);
	Blah_Vertex *left_bottom_front = Blah_Object_addVertex(walls, AST_WORLD_LEFT, AST_WORLD_BOTTOM, AST_WORLD_FRONT);
	Blah_Vertex *right_bottom_front = Blah_Object_addVertex(walls, AST_WORLD_RIGHT, AST_WORLD_BOTTOM, AST_WORLD_FRONT);

	Blah_Vertex *back_wall_seq[] = {left_top_back, right_top_back, right_bottom_back, left_bottom_back};
	Blah_Vertex *left_wall_seq[] = {left_top_front, left_top_back, left_bottom_back, left_bottom_front};
	Blah_Vertex *bottom_wall_seq[] = {left_bottom_back, right_bottom_back, right_bottom_front, left_bottom_front};

	Blah_Point map_top_left = {0,1,0}; Blah_Point map_top_right = {1,1,0};
	Blah_Point map_bottom_left = {0,0,0}; Blah_Point map_bottom_right = {1,0,0};
	const Blah_Point *back_mapping[] = {&map_top_right, &map_top_left, &map_bottom_left, &map_bottom_right, NULL};

	Blah_Primitive *back_wall = Blah_Primitive_new(BLAH_PRIMITIVE_POLYGON, back_wall_seq, blah_countof(back_wall_seq));
	Blah_Primitive_mapTexture(back_wall, temp_texture, back_mapping);
	Blah_Object_addPrimitive(walls, back_wall);

	Blah_Primitive *left_wall = Blah_Primitive_new(BLAH_PRIMITIVE_POLYGON, left_wall_seq, blah_countof(left_wall_seq));
	Blah_Primitive_mapTextureAuto(left_wall, temp_texture);
	Blah_Object_addPrimitive(walls, left_wall);

	Blah_Primitive *bottom_wall = Blah_Primitive_new(BLAH_PRIMITIVE_POLYGON, bottom_wall_seq, blah_countof(bottom_wall_seq));
	Blah_Primitive_mapTextureAuto(bottom_wall, temp_texture);
	Blah_Object_addPrimitive(walls, bottom_wall);


	Blah_Vertex *f_left_top_back = Blah_Object_addVertex(walls, AST_WORLD_LEFT+10, AST_WORLD_TOP-10, AST_WORLD_BACK+10);
	Blah_Vertex *f_right_top_back = Blah_Object_addVertex(walls, AST_WORLD_RIGHT-10, AST_WORLD_TOP-10, AST_WORLD_BACK+10);
	Blah_Vertex *f_right_top_front = Blah_Object_addVertex(walls, AST_WORLD_RIGHT-10, AST_WORLD_TOP-10, AST_WORLD_FRONT-10);
	Blah_Vertex *f_left_bottom_back = Blah_Object_addVertex(walls, AST_WORLD_LEFT+10, AST_WORLD_BOTTOM+10, AST_WORLD_BACK+10);
	Blah_Vertex *f_right_bottom_back = Blah_Object_addVertex(walls, AST_WORLD_RIGHT-10, AST_WORLD_BOTTOM+10, AST_WORLD_BACK+10);
	Blah_Vertex *f_left_top_front = Blah_Object_addVertex(walls, AST_WORLD_LEFT+10, AST_WORLD_TOP-10, AST_WORLD_FRONT-10);
	Blah_Vertex *f_left_bottom_front = Blah_Object_addVertex(walls, AST_WORLD_LEFT+10, AST_WORLD_BOTTOM+10, AST_WORLD_FRONT-10);
	Blah_Vertex *f_right_bottom_front = Blah_Object_addVertex(walls, AST_WORLD_RIGHT-10, AST_WORLD_BOTTOM+10, AST_WORLD_FRONT-10);

	Blah_Vertex *back_frame_seq[] = {f_left_top_back, f_right_top_back, f_right_bottom_back, f_left_bottom_back, f_left_top_back};
	Blah_Vertex *left_frame_seq[] = {f_left_top_front, f_left_top_back, f_left_bottom_back, f_left_bottom_front, f_left_top_front};
	Blah_Vertex *bottom_frame_seq[] = {f_left_bottom_back, f_right_bottom_back, f_right_bottom_front, f_left_bottom_front, f_left_bottom_back};
	Blah_Vertex *front_edge_seq[] = {f_right_bottom_front, f_right_top_front, f_left_top_front, f_right_top_front, f_right_top_front, f_right_top_back};

	Blah_Primitive *back_frame = Blah_Primitive_new(BLAH_PRIMITIVE_LINE_STRIP, back_frame_seq, blah_countof(back_frame_seq));
	Blah_Object_addPrimitive(walls, back_frame);

	Blah_Primitive *left_frame = Blah_Primitive_new(BLAH_PRIMITIVE_LINE_STRIP, left_frame_seq, blah_countof(left_frame_seq));
	Blah_Object_addPrimitive(walls, left_frame);

	Blah_Primitive *bottom_frame = Blah_Primitive_new(BLAH_PRIMITIVE_LINE_STRIP, bottom_frame_seq, blah_countof(bottom_wall_seq));
	Blah_Object_addPrimitive(walls, bottom_frame);

	Blah_Primitive *front_edge = Blah_Primitive_new(BLAH_PRIMITIVE_LINE, front_edge_seq, blah_countof(front_edge_seq));
	Blah_Object_addPrimitive(walls, front_edge);

	Blah_Material_setColour(new_material, 1, 1, 1, 1);
	Blah_Object_addMaterial(walls,new_material);
	Blah_Object_setMaterial(walls,new_material);

	return Blah_Scene_addObject(glast_scene,walls);
}

static Blah_List *make_asteroid_list() {
	Blah_List *new_list;
	int count;

	new_list=Blah_List_new("ASTEROIDS");
	new_list->destroyElementFunction = (blah_list_element_dest_func*)asteroid_destroy;

	for (count=0;count<AST_NUM_ASTEROIDS;count++)
		Blah_List_appendElement(new_list, asteroid_new(AST_NEW_SIZE, blah_util_randRangeInt(0,2)));  //alloc a new asteroid

	return new_list;
}

static void free_asteroid_list(Blah_List *list) {
	Blah_List_destroy(list);
}

/* static void explode_asteroid_list(Blah_List *list) {
	Blah_List_call_function(list, asteroid_explode);
} */

static void glast_exit() {
	// The asteroid list is the only structure owned by Glasteroids,

	free_asteroid_list(asteroid_list);
	fprintf(stderr,"freed asteroids\n");
	// Let engine garbage collection take care of other structures

	Blah_Scene_destroy(glast_scene);
	fprintf(stderr,"destroyed scene\n");

	// blah_engine_exit();
	fprintf(stderr,"exited blah\n");

	exit(0);
}

static void rotate_view_clockwise() {
	Blah_Vector world_y = {0,1,0};

	Blah_Point_rotateAxis(&external_viewpoint, &world_y, AST_VIEW_ROTATION);
}

static void rotate_view_anticlockwise() {
	Blah_Vector world_y = {0,1,0};

	Blah_Point_rotateAxis(&external_viewpoint, &world_y, -AST_VIEW_ROTATION);
}

static void video_up() {
	const Blah_Video_Mode* cur_mode = blah_video_getCurrentMode();

	if (cur_mode) {
		const Blah_Video_Mode* new_mode = blah_video_getNextMode(cur_mode);
		if (new_mode) {	blah_video_setMode(new_mode); }
	}
}

static void video_down() {
	const Blah_Video_Mode* cur_mode = blah_video_getCurrentMode();

	if (cur_mode) {
		const Blah_Video_Mode* new_mode = blah_video_getPrevMode(cur_mode);
		if (new_mode) {	blah_video_setMode(new_mode); }
	}
}

int main(int argc, char** argv) {
	Blah_Debug_Log *glast_log = Blah_Debug_Log_new("glast_log"); // Set up main log file

	global_argc = &argc;
	global_argv = argv;

	blah_engine_init(argc, argv);  //initialise engine components

	const Blah_Video_Mode* vm = blah_video_getIdealMode(1600, 1200, 24);
	blah_console_message("Got ideal video mode - %dx%dx%d\n", vm->width, vm->height, vm->colourDepth);

	if (!blah_video_setMode(vm)) { Blah_Debug_Log_error(glast_log, 0, "Set video mode failed"); }

	//Set up drawing environment
	blah_draw_setFocalPoint(0,0,0); //focus apon origin
	blah_draw_setViewpoint((AST_WORLD_RIGHT+100),(AST_WORLD_TOP-200),AST_WORLD_DEPTH/2);
	blah_draw_setFieldOfVision(1.6, 1.6);
	blah_draw_setDepthOfVision(AST_WORLD_DEPTH+1000);
	blah_draw_setViewNormal(0,1,0);

	temp_image = Blah_Image_fromFile("tadmap.tga");
	blah_console_message("opened tadmap.tga\n");

	temp_texture = Blah_Texture_fromImage(temp_image);

	blah_console_message("Made texture from image.\n");

	bullet_image = Blah_Image_fromFile("bullet.tga");
	copy_image = Blah_Image_fromImage(bullet_image, "bullet_copy", 0, bullet_image->width/2-1, 0, bullet_image->height/2-1);
	blah_console_message("Made copy of image\n");

	bullet_texture = Blah_Texture_fromImage(bullet_image);
	blah_console_message("Made bullet texture from image");

	// Load models into memory
	voyager_model = Blah_Model_load("voyager.lwo");
	Blah_Model_scale(voyager_model, 1);
	fighter_model = Blah_Model_load("storm.lwo");
	Blah_Model_scale(fighter_model, 30);
	sputnik_model = Blah_Model_load("sputnik1.lwo");
	Blah_Model_scale(sputnik_model, 50);
	alien_model = Blah_Model_load("alien.lwo");
	Blah_Model_scale(alien_model, 70);

	blah_console_message("loaded all models\n");

	unsigned int time_seed = time(NULL);
	srand(time_seed);	//seed random generator

	//Load the font
	glast_font = load_raster_font();
	tex_font = load_texture_font();

	//Set up the scene
	glast_scene = Blah_Scene_new();
	Blah_Scene_setAmbientLight(glast_scene, 1,1,1,1);
	//Set up light one
	light_one = Blah_Light_new();
	Blah_Light_setAmbient(light_one, 0,.5,.5,1);
	Blah_Light_setLocation(light_one, 500,500,500);
	Blah_Light_setDirection(light_one, -.25,-.5,1);
	Blah_Light_setSpread(light_one, 50);
	Blah_Scene_addLight(glast_scene, light_one);
	//Set up light two
	light_two = Blah_Light_new();
	Blah_Light_setAmbient(light_two, .5,0,0,1);
	Blah_Light_setLocation(light_two, -500,-500,-500);
	Blah_Light_setDirection(light_two, .25,.5,1);
	Blah_Light_setSpread(light_two, 50);
	Blah_Scene_addLight(glast_scene, light_two);

	//Create overlays
	help_overlay = help_overlay_new();
	score_overlay = score_overlay_new();

	axes = axes_new();
	walls = walls_new();

	ship = ship_new();
	asteroid_list=make_asteroid_list();

	Blah_Scene_addEntity(glast_scene, ship);
	blah_draw_setCurrentScene(glast_scene);
	Blah_Scene_addOverlay(glast_scene, help_overlay);
	Blah_Overlay_setPosition(help_overlay, 20,20);
	Blah_Scene_addOverlay(glast_scene, score_overlay);
	Blah_Overlay_setPosition(score_overlay, vm->width-(16*12), vm->height-20);


	//Set up general keyboard input handlers

	blah_input_keyboard_setDepressFunction(BLAH_INPUT_KEY_ESCAPE, glast_exit);
	blah_input_keyboard_setDepressFunction(BLAH_INPUT_KEY_RETURN, toggle_full_screen);
	blah_input_keyboard_setDepressFunction(BLAH_INPUT_KEY_T, toggle_axes);
	blah_input_keyboard_setDepressFunction(BLAH_INPUT_KEY_V, toggle_view);
	blah_input_keyboard_setDepressFunction(BLAH_INPUT_KEY_EQUALS, video_up);
	blah_input_keyboard_setDepressFunction(BLAH_INPUT_KEY_MINUS, video_down);
	blah_input_keyboard_setHoldFunction(BLAH_INPUT_KEY_X, rotate_view_anticlockwise);
	blah_input_keyboard_setHoldFunction(BLAH_INPUT_KEY_Z, rotate_view_clockwise);
	blah_input_keyboard_setDepressFunction(BLAH_INPUT_KEY_H, toggle_help);


	//Set up ship control handlers
	blah_input_keyboard_setHoldFunction(BLAH_INPUT_KEY_LEFT, glast_input_left);
	blah_input_keyboard_setHoldFunction(BLAH_INPUT_KEY_RIGHT, glast_input_right);
	blah_input_keyboard_setHoldFunction(BLAH_INPUT_KEY_UP,	glast_input_up);
	blah_input_keyboard_setHoldFunction(BLAH_INPUT_KEY_DOWN, glast_input_down);
	blah_input_keyboard_setHoldFunction(BLAH_INPUT_KEY_LEFT_SHIFT, glast_input_lshift);
	blah_input_keyboard_setHoldFunction(BLAH_INPUT_KEY_SPACE, glast_input_space);

	fprintf(stderr,"entering main loop\n");

	while (1) {
		blah_engine_main();
	}

	Blah_Debug_Log_destroy(glast_log);

	return 0;
}
