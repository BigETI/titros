#include <iostream>
#define _OBJECTS_CPP_
#include "main.h"
#include "game.h"
#include "objects.h"

void SetBackground(GAME_TEXTURE_ENUM texture_id)
{
	object_bg.texture_id = texture_id;
	object_bg.is = true;
}

int Create2DObject(GAME_TEXTURE_ENUM texture_id, double x, double y, double rotation, double size_x, double size_y, double center_x, double center_y, double alpha)
{
	for(int i = current_object_2d_pos; i < MAX_2D_OBJECTS; i++)
	{
		if(object_2d[i].is)
		{
			current_object_2d_pos = i+1;
			continue;
		}
		object_2d[i].pos[OBJECT_X] = x;
		object_2d[i].pos[OBJECT_Y] = y;
		object_2d[i].rotation = rotation;
		object_2d[i].size[OBJECT_X] = size_x;
		object_2d[i].size[OBJECT_Y] = size_y;
		object_2d[i].center[OBJECT_X] = center_x;
		object_2d[i].center[OBJECT_Y] = center_y;
		object_2d[i].texture_id = texture_id;
		object_2d[i].alpha = alpha;
		object_2d[i].is = true;
		current_object_2d_pos = i+1;
		return i;
	}
	return INVALID_OBJECT_ID;
}

void Delete2DObject(int object_2d_id)
{
	if(object_2d_id == INVALID_OBJECT_ID) return;
	if(!object_2d[object_2d_id].is) return;
	object_2d[object_2d_id].is = false;
	if(object_2d_id < current_object_2d_pos) current_object_2d_pos = object_2d_id;
}

bool Swap2DObjectTexture(int object_2d_id, GAME_TEXTURE_ENUM texture_id)
{
	if(object_2d_id == INVALID_OBJECT_ID) return false;
	object_2d[object_2d_id].texture_id = texture_id;
	return true;
}

void Object2DFadeIn(int object_id, DWORD fade_time, bool wait)
{
	if(object_id == INVALID_OBJECT_ID) return;
	if(!object_2d[object_id].is) return;
	object_2d[object_id].fade_mode = FADE_MODE_NONE;
	object_2d[object_id].fade_time = fade_time;
	object_2d[object_id].fade_mode = FADE_MODE_IN;
	if(wait) while(object_2d[object_id].fade_mode != FADE_MODE_NONE){}
}

void Object2DFadeOut(int object_id, DWORD fade_time, bool wait)
{
	if(object_id == INVALID_OBJECT_ID) return;
	if(!object_2d[object_id].is) return;
	object_2d[object_id].fade_mode = FADE_MODE_NONE;
	object_2d[object_id].fade_time = fade_time;
	object_2d[object_id].fade_mode = FADE_MODE_OUT;
	if(wait) while(object_2d[object_id].fade_mode != FADE_MODE_NONE){}
}