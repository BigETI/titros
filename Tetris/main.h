#ifndef _MAIN_H_INCLUDED_
	#define _MAIN_H_INCLUDED_

	#define SCREEN_RESOLUTION_HALF				400
	#define SCREEN_RESOLUTION_THREE_QUARTERS	600
	#define SCREEN_RESOLUTION_DEFAULT			800
	#define SCREEN_RESOLUTION_FIVE_QUARTERS		1000

	#define SPECIAL_KEY_UP		101
	#define SPECIAL_KEY_DOWN	103
	#define SPECIAL_KEY_LEFT	100
	#define SPECIAL_KEY_RIGHT	102

	#define KEY_UP			119
	#define KEY_DOWN		115
	#define KEY_LEFT		97
	#define KEY_RIGHT		100
	#define KEY_ENTER		13
	#define KEY_SPACE		32
	#define KEY_ROTATE_L	101
	#define KEY_ROTATE_R	113
	#define KEY_LOWER_RES	110
	#define KEY_HIGHER_RES	109
	#define KEY_ESCAPE		27

enum FADE_MODE_ENUM
{
	FADE_MODE_NONE,
	FADE_MODE_IN,
	FADE_MODE_OUT
};

enum SCREEN_OPTION_ENUM
{
	SCREEN_OPTION_HALF,
	SCREEN_OPTION_THREE_QUARTERS,
	SCREEN_OPTION_DEFAULT,
	SCREEN_OPTION_FIVE_QUARTERS,
	SCREEN_OPTION_NOTHING
};

#ifndef DWORD
typedef unsigned long DWORD;
#endif

struct SCREEN
{
	FADE_MODE_ENUM fade_mode;
	SCREEN_OPTION_ENUM option;
	unsigned int width_height;
	double alpha;
	DWORD fade_time;
};

	#include "audio.h"
	#ifdef _MAIN_CPP_
audioStream *sounds;
int current_object_2d_pos;
SCREEN screen;
int credits_object;
	#else
extern audioStream *sounds;
extern int current_object_2d_pos;
extern SCREEN screen;
extern void ScreenFadeIn(DWORD fade_time, bool wait);
extern void ScreenFadeOut(DWORD fade_time, bool wait);
extern void LogError(char *str);
extern void DisableOptions();
extern void DisableOptionsAndCredits();
extern void ChangeScreenResolution(SCREEN_OPTION_ENUM option_id);
extern int credits_object;
	#endif
#endif