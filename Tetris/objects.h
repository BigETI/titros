#ifndef _OBJECTS_H_INCLUDED_
	#define _OBJECTS_H_INCLUDED_

	#define MAX_2D_OBJECTS		5
	#define INVALID_OBJECT_ID	-1

enum OBJECT_ENUM
{
	OBJECT_X,
	OBJECT_Y,
	OBJECT_2D_ROT,
	OBJECT_3D_ROT_X,
	OBJECT_3D_ROT_Y,
	OBJECT_3D_ROT_Z
};

enum OBJECT_MOVE_ENUM
{
	OBJECT_NO_MOVE,
	OBJECT_MOVE_DEFAULT,
	OBJECT_MOVE_DEFAULT_LOOP,
	OBJECT_MOVE_COS,
	OBJECT_MOVE_COS_LOOP,
	OBJECT_MOVE_SIN,
	OBJECT_MOVE_SIN_LOOP
};

enum OBJECT_SCALE_ENUM
{
	OBJECT_NO_SCALE,
	OBJECT_SCALE_DEFAULT,
	OBJECT_SCALE_DEFAULT_LOOP,
	OBJECT_SCALE_COS,
	OBJECT_SCALE_COS_LOOP,
	OBJECT_SCALE_SIN,
	OBJECT_SCALE_SIN_LOOP
};

enum OBJECT_ROTATE_ENUM
{
	OBJECT_NO_ROTATE,
	OBJECT_ROTATE_DEFAULT,
	OBJECT_ROTATE_DEFAULT_LOOP,
	OBJECT_ROTATE_COS,
	OBJECT_ROTATE_COS_LOOP,
	OBJECT_ROTATE_SIN,
	OBJECT_ROTATE_SIN_LOOP
};

struct OBJECT_2D_BG
{
	bool is;
	GAME_TEXTURE_ENUM texture_id;
};

struct OBJECT_2D
{
	bool is;
	double pos[6], rotation, size[2], center[2], alpha;
	GAME_TEXTURE_ENUM texture_id;
	FADE_MODE_ENUM fade_mode;
	DWORD fade_time;
};

	#ifdef _OBJECTS_CPP_
OBJECT_2D_BG object_bg;
OBJECT_2D object_2d[MAX_2D_OBJECTS];
	#else
extern void SetBackground(GAME_TEXTURE_ENUM texture_id);
extern int Create2DObject(GAME_TEXTURE_ENUM texture_id, double x, double y, double rotation, double size_x, double size_y, double center_x, double center_y, double alpha);
extern void Delete2DObject(int object_2d_id);
extern bool Swap2DObjectTexture(int object_2d_id, GAME_TEXTURE_ENUM texture_id);
extern void Object2DFadeIn(int object_id, DWORD fade_time, bool wait);
extern void Object2DFadeOut(int object_id, DWORD fade_time, bool wait);
extern OBJECT_2D_BG object_bg;
extern OBJECT_2D object_2d[MAX_2D_OBJECTS];
	#endif
#endif