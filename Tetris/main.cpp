#include <windows.h>
#include <iostream>
using namespace std;

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _MAIN_CPP_
#include "main.h"
#include "game.h"
#include "objects.h"

extern DWORD WINAPI threaded_main(__in LPVOID lpParameter);

const double entity_size = 2.0/800.0*30.0, third = 1.0/3.0, twothird = 2.0/3.0, five_tenths = 1.0/15.0;
DWORD last_tick_count, current_tick_count, elapsed_tick_count = 0;

struct SINGLE_ENTITY
{
	unsigned char state;
	double tex_x, tex_y;
	DWORD elapsed_tick_count;
};

SINGLE_ENTITY single_entity;

bool bomb_tex_changed = false;

void DrawEntity(TETRIS_ENTITY_ENUM entity_type)
{
	switch(entity_type)
	{
		case TETRIS_ENTITY_I:
			glTexCoord2d(0.0, 0.75); glVertex2f(0.0, 0.0);
			glTexCoord2d(0.0, 1.0); glVertex2f(0.0, entity_size);
			glTexCoord2d(0.25, 1.0); glVertex2f(entity_size, entity_size);
			glTexCoord2d(0.25, 0.75); glVertex2f(entity_size, 0.0);
			break;
		case TETRIS_ENTITY_T:
			glTexCoord2d(0.75, 0.5); glVertex2f(0.0, 0.0);
			glTexCoord2d(0.75, 0.75); glVertex2f(0.0, entity_size);
			glTexCoord2d(1.0, 0.75); glVertex2f(entity_size, entity_size);
			glTexCoord2d(1.0, 0.5); glVertex2f(entity_size, 0.0);
			break;
		case TETRIS_ENTITY_L:
			glTexCoord2d(0.25, 0.5); glVertex2f(0.0, 0.0);
			glTexCoord2d(0.25, 0.75); glVertex2f(0.0, entity_size);
			glTexCoord2d(0.5, 0.75); glVertex2f(entity_size, entity_size);
			glTexCoord2d(0.5, 0.5); glVertex2f(entity_size, 0.0);
			break;
		case TETRIS_ENTITY_N_L:
			glTexCoord2d(0.0, 0.5); glVertex2f(0.0, 0.0);
			glTexCoord2d(0.0, 0.75); glVertex2f(0.0, entity_size);
			glTexCoord2d(0.25, 0.75); glVertex2f(entity_size, entity_size);
			glTexCoord2d(0.25, 0.5); glVertex2f(entity_size, 0.0);
			break;
		case TETRIS_ENTITY_O:
			glTexCoord2d(0.75, 0.75); glVertex2f(0.0, 0.0);
			glTexCoord2d(0.75, 1.0); glVertex2f(0.0, entity_size);
			glTexCoord2d(1.0, 1.0); glVertex2f(entity_size, entity_size);
			glTexCoord2d(1.0, 0.75); glVertex2f(entity_size, 0.0);
			break;
		case TETRIS_ENTITY_S:
			glTexCoord2d(0.5, 0.75); glVertex2f(0.0, 0.0);
			glTexCoord2d(0.5, 1.0); glVertex2f(0.0, entity_size);
			glTexCoord2d(0.75, 1.0); glVertex2f(entity_size, entity_size);
			glTexCoord2d(0.75, 0.75); glVertex2f(entity_size, 0.0);
			break;
		case TETRIS_ENTITY_Z:
			glTexCoord2d(0.25, 0.75); glVertex2f(0.0, 0.0);
			glTexCoord2d(0.25, 1.0); glVertex2f(0.0, entity_size);
			glTexCoord2d(0.5, 1.0); glVertex2f(entity_size, entity_size);
			glTexCoord2d(0.5, 0.75); glVertex2f(entity_size, 0.0);
			break;
		case TETRIS_ENTITY_SINGLE:
			glTexCoord2d(single_entity.tex_x, single_entity.tex_y-0.25); glVertex2f(0.0, 0.0);
			glTexCoord2d(single_entity.tex_x, single_entity.tex_y); glVertex2f(0.0, entity_size);
			glTexCoord2d(single_entity.tex_x+0.25, single_entity.tex_y); glVertex2f(entity_size, entity_size);
			glTexCoord2d(single_entity.tex_x+0.25, single_entity.tex_y-0.25); glVertex2f(entity_size, 0.0);
			break;
		case TETRIS_ENTITY_BOMB:
			if(bomb_tex_changed)
			{
				glTexCoord2d(0.25, 0.0); glVertex2f(0.0, 0.0);
				glTexCoord2d(0.25, 0.25); glVertex2f(0.0, entity_size);
				glTexCoord2d(0.50, 0.25); glVertex2f(entity_size, entity_size);
				glTexCoord2d(0.50, 0.0); glVertex2f(entity_size, 0.0);
			}
			else
			{
				glTexCoord2d(0.0, 0.0); glVertex2f(0.0, 0.0);
				glTexCoord2d(0.0, 0.25); glVertex2f(0.0, entity_size);
				glTexCoord2d(0.25, 0.25); glVertex2f(entity_size, entity_size);
				glTexCoord2d(0.25, 0.0); glVertex2f(entity_size, 0.0);
			}
			break;
	}
}

void DrawOptions()
{
	// Volume
	glPushMatrix();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTranslated(0.0, 0.8, 0.0);
	glBindTexture(GL_TEXTURE_2D, objtexture[GAME_TEXTURE_OPTIONS].texDat.texID);
	glColor4d(screen.alpha, screen.alpha, screen.alpha, menuDat.options_alpha);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0); glVertex2d(0.0, 0.0);
	glTexCoord2d(0.0, 1.0); glVertex2d(0.0, 0.15);
	glTexCoord2d(1.0, 1.0); glVertex2d(0.75, 0.15);
	glTexCoord2d(1.0, 0.0); glVertex2d(0.75, 0.0);
	glEnd();
	glPopMatrix();


	glPushMatrix();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTranslated(0.0, 0.75, 0.0);
	glBindTexture(GL_TEXTURE_2D, objtexture[GAME_TEXTURE_VOLUME].texDat.texID);
	glColor4d(screen.alpha, screen.alpha, screen.alpha, menuDat.options_alpha);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.75); glVertex2d(0.0, 0.0);
	glTexCoord2d(0.0, 1.0); glVertex2d(0.0, 0.05);
	glTexCoord2d(game_volume.music_bar.tex_size, 1.0); glVertex2d(game_volume.music_bar.obj_size, 0.05);
	glTexCoord2d(game_volume.music_bar.tex_size, 0.75); glVertex2d(game_volume.music_bar.obj_size, 0.0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTranslated(0.0, 0.75, 0.0);
	glColor4d(screen.alpha, screen.alpha, screen.alpha, menuDat.options_alpha);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.5); glVertex2d(0.0, 0.0);
	glTexCoord2d(0.0, 0.75); glVertex2d(0.0, 0.05);
	glTexCoord2d(1.0, 0.75); glVertex2d(0.25, 0.05);
	glTexCoord2d(1.0, 0.5); glVertex2d(0.25, 0.0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTranslated(0.0, 0.75, 0.0);
	glColor4d(screen.alpha, screen.alpha, screen.alpha, menuDat.options_alpha);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.25); glVertex2d(0.0, 0.0);
	glTexCoord2d(0.0, 0.5); glVertex2d(0.0, 0.05);
	glTexCoord2d(1.0, 0.5); glVertex2d(0.25, 0.05);
	glTexCoord2d(1.0, 0.25); glVertex2d(0.25, 0.0);
	glEnd();
	glPopMatrix();


	glPushMatrix();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTranslated(0.5, 0.75, 0.0);
	glColor4d(screen.alpha, screen.alpha, screen.alpha, menuDat.options_alpha);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.75); glVertex2d(0.0, 0.0);
	glTexCoord2d(0.0, 1.0); glVertex2d(0.0, 0.05);
	glTexCoord2d(game_volume.sounds_bar.tex_size, 1.0); glVertex2d(game_volume.sounds_bar.obj_size, 0.05);
	glTexCoord2d(game_volume.sounds_bar.tex_size, 0.75); glVertex2d(game_volume.sounds_bar.obj_size, 0.0);
	glEnd();

	glPopMatrix();
	glPushMatrix();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTranslated(0.5, 0.75, 0.0);
	glColor4d(screen.alpha, screen.alpha, screen.alpha, menuDat.options_alpha);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.5); glVertex2d(0.0, 0.0);
	glTexCoord2d(0.0, 0.75); glVertex2d(0.0, 0.05);
	glTexCoord2d(1.0, 0.75); glVertex2d(0.25, 0.05);
	glTexCoord2d(1.0, 0.5); glVertex2d(0.25, 0.0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTranslated(0.5, 0.75, 0.0);
	glColor4d(screen.alpha, screen.alpha, screen.alpha, menuDat.options_alpha);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0); glVertex2d(0.0, 0.0);
	glTexCoord2d(0.0, 0.25); glVertex2d(0.0, 0.05);
	glTexCoord2d(1.0, 0.25); glVertex2d(0.25, 0.05);
	glTexCoord2d(1.0, 0.0); glVertex2d(0.25, 0.0);
	glEnd();
	glPopMatrix();

	// Resolutions
	glPushMatrix();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTranslated(-0.1, 0.3, 0.0);
	glBindTexture(GL_TEXTURE_2D, objtexture[GAME_TEXTURE_RESOLUTION].texDat.texID);
	glColor4d(screen.alpha, screen.alpha, screen.alpha, menuDat.options_alpha);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.75); glVertex2d(0.0, 0.0);
	glTexCoord2d(0.0, 1.0); glVertex2d(0.0, 0.2);
	glTexCoord2d(1.0, 1.0); glVertex2d(1.0, 0.2);
	glTexCoord2d(1.0, 0.75); glVertex2d(1.0, 0.0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTranslated(-0.1, 0.06, 0.0);
	glColor4d(screen.alpha, screen.alpha, screen.alpha, menuDat.options_alpha);
	glBegin(GL_QUADS);
	if(screen.option == SCREEN_OPTION_HALF)
	{
		glTexCoord2d(0.0, 0.0); glVertex2d(0.0, 0.0);
		glTexCoord2d(0.0, 0.25); glVertex2d(0.0, 0.2);
		glTexCoord2d(0.25, 0.25); glVertex2d(0.25, 0.2);
		glTexCoord2d(0.25, 0.0); glVertex2d(0.25, 0.0);
	}
	else
	{
		if(menuDat.allow_change_resolution == SCREEN_OPTION_HALF)
		{
			glTexCoord2d(0.0, 0.25); glVertex2d(0.0, 0.0);
			glTexCoord2d(0.0, 0.5); glVertex2d(0.0, 0.2);
			glTexCoord2d(0.25, 0.5); glVertex2d(0.25, 0.2);
			glTexCoord2d(0.25, 0.25); glVertex2d(0.25, 0.0);
		}
		else
		{
			glTexCoord2d(0.0, 0.5); glVertex2d(0.0, 0.0);
			glTexCoord2d(0.0, 0.75); glVertex2d(0.0, 0.2);
			glTexCoord2d(0.25, 0.75); glVertex2d(0.25, 0.2);
			glTexCoord2d(0.25, 0.5); glVertex2d(0.25, 0.0);
		}
	}
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTranslated(0.15, -0.04, 0.0);
	glColor4d(screen.alpha, screen.alpha, screen.alpha, menuDat.options_alpha);
	glBegin(GL_QUADS);
	if(screen.option == SCREEN_OPTION_THREE_QUARTERS)
	{
		glTexCoord2d(0.25, 0.0); glVertex2d(0.0, 0.0);
		glTexCoord2d(0.25, 0.25); glVertex2d(0.0, 0.2);
		glTexCoord2d(0.5, 0.25); glVertex2d(0.25, 0.2);
		glTexCoord2d(0.5, 0.0); glVertex2d(0.25, 0.0);
	}
	else
	{
		if(menuDat.allow_change_resolution == SCREEN_OPTION_THREE_QUARTERS)
		{
			glTexCoord2d(0.25, 0.25); glVertex2d(0.0, 0.0);
			glTexCoord2d(0.25, 0.5); glVertex2d(0.0, 0.2);
			glTexCoord2d(0.5, 0.5); glVertex2d(0.25, 0.2);
			glTexCoord2d(0.5, 0.25); glVertex2d(0.25, 0.0);
		}
		else
		{
			glTexCoord2d(0.25, 0.5); glVertex2d(0.0, 0.0);
			glTexCoord2d(0.25, 0.75); glVertex2d(0.0, 0.2);
			glTexCoord2d(0.5, 0.75); glVertex2d(0.25, 0.2);
			glTexCoord2d(0.5, 0.5); glVertex2d(0.25, 0.0);
		}
	}
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTranslated(0.4, 0.06, 0.0);
	glColor4d(screen.alpha, screen.alpha, screen.alpha, menuDat.options_alpha);
	glBegin(GL_QUADS);
	if(screen.option == SCREEN_OPTION_DEFAULT)
	{
		glTexCoord2d(0.5, 0.0); glVertex2d(0.0, 0.0);
		glTexCoord2d(0.5, 0.25); glVertex2d(0.0, 0.2);
		glTexCoord2d(0.75, 0.25); glVertex2d(0.25, 0.2);
		glTexCoord2d(0.75, 0.0); glVertex2d(0.25, 0.0);
	}
	else
	{
		if(menuDat.allow_change_resolution == SCREEN_OPTION_DEFAULT)
		{
			glTexCoord2d(0.5, 0.25); glVertex2d(0.0, 0.0);
			glTexCoord2d(0.5, 0.5); glVertex2d(0.0, 0.2);
			glTexCoord2d(0.75, 0.5); glVertex2d(0.25, 0.2);
			glTexCoord2d(0.75, 0.25); glVertex2d(0.25, 0.0);
		}
		else
		{
			glTexCoord2d(0.5, 0.5); glVertex2d(0.0, 0.0);
			glTexCoord2d(0.5, 0.75); glVertex2d(0.0, 0.2);
			glTexCoord2d(0.75, 0.75); glVertex2d(0.25, 0.2);
			glTexCoord2d(0.75, 0.5); glVertex2d(0.25, 0.0);
		}
	}
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTranslated(0.65, -0.04, 0.0);
	glColor4d(screen.alpha, screen.alpha, screen.alpha, menuDat.options_alpha);
	glBegin(GL_QUADS);
	if(screen.option == SCREEN_OPTION_FIVE_QUARTERS)
	{
		glTexCoord2d(0.75, 0.0); glVertex2d(0.0, 0.0);
		glTexCoord2d(0.75, 0.25); glVertex2d(0.0, 0.2);
		glTexCoord2d(1.0, 0.25); glVertex2d(0.25, 0.2);
		glTexCoord2d(1.0, 0.0); glVertex2d(0.25, 0.0);
	}
	else
	{
		if(menuDat.allow_change_resolution == SCREEN_OPTION_FIVE_QUARTERS)
		{
			glTexCoord2d(0.75, 0.25); glVertex2d(0.0, 0.0);
			glTexCoord2d(0.75, 0.5); glVertex2d(0.0, 0.2);
			glTexCoord2d(1.0, 0.5); glVertex2d(0.25, 0.2);
			glTexCoord2d(1.0, 0.25); glVertex2d(0.25, 0.0);
		}
		else
		{
			glTexCoord2d(0.75, 0.5); glVertex2d(0.0, 0.0);
			glTexCoord2d(0.75, 0.75); glVertex2d(0.0, 0.2);
			glTexCoord2d(1.0, 0.75); glVertex2d(0.25, 0.2);
			glTexCoord2d(1.0, 0.5); glVertex2d(0.25, 0.0);
		}
	}
	glEnd();
	glPopMatrix();

	switch(menuDat.options_fade_mode)
	{
		case FADE_MODE_IN:
			if((menuDat.options_alpha += five_tenths) >= 1.0)
			{
				menuDat.options_alpha = 1.0;
				menuDat.options_fade_mode = FADE_MODE_NONE;
			}
			break;
		case FADE_MODE_OUT:
			if((menuDat.options_alpha -= five_tenths) <= 0.0)
			{
				menuDat.options_alpha = 0.0;
				menuDat.options_fade_mode = FADE_MODE_NONE;
			}
			break;
	}
}

void ToggleOptions()
{
	Delete2DObject(credits_object);
	credits_object = INVALID_OBJECT_ID;
	switch(menuDat.show_o_c)
	{
		case SHOW_NOTHING:
			menuDat.options_fade_mode = FADE_MODE_IN;
			menuDat.show_o_c = SHOW_OPTIONS;
			break;
		case SHOW_OPTIONS:
			menuDat.options_fade_mode = FADE_MODE_OUT;
			menuDat.show_o_c = SHOW_NOTHING;
			break;
		case SHOW_CREDITS:
			Delete2DObject(credits_object);
			credits_object = INVALID_OBJECT_ID;
			menuDat.options_fade_mode = FADE_MODE_IN;
			menuDat.show_o_c = SHOW_OPTIONS;
			break;
	}
}

void ToggleCredits()
{
	switch(menuDat.show_o_c)
	{
		case SHOW_NOTHING:
			Delete2DObject(credits_object);
			credits_object = Create2DObject(GAME_TEXTURE_CREDITS, -0.125, -0.5, 0.0, 1.125, 1.5, 0.0, 0.0, 0.0);
			Object2DFadeIn(credits_object, 500, false);
			menuDat.show_o_c = SHOW_CREDITS;
			break;
		case SHOW_OPTIONS:
			menuDat.options_fade_mode = FADE_MODE_OUT;
			Delete2DObject(credits_object);
			credits_object = Create2DObject(GAME_TEXTURE_CREDITS, -0.125, -0.5, 0.0, 1.125, 1.5, 0.0, 0.0, 0.0);
			Object2DFadeIn(credits_object, 500, false);
			menuDat.show_o_c = SHOW_CREDITS;
			break;
		case SHOW_CREDITS:
			Object2DFadeOut(credits_object, 500, false);
			menuDat.show_o_c = SHOW_NOTHING;
			break;
	}
}

void DisableOptions()
{
	menuDat.options_fade_mode = FADE_MODE_NONE;
	menuDat.options_alpha = 0.0;
	menuDat.allow_sound_change = CHANGE_SOUND_NOTHING;
	menuDat.show_o_c = SHOW_NOTHING;
}

void DisableOptionsAndCredits()
{
	menuDat.options_fade_mode = FADE_MODE_NONE;
	menuDat.options_alpha = 0.0;
	menuDat.allow_sound_change = CHANGE_SOUND_NOTHING;
	Delete2DObject(credits_object);
	credits_object = INVALID_OBJECT_ID;
	menuDat.show_o_c = SHOW_NOTHING;
}

void ChangeScreenResolution(SCREEN_OPTION_ENUM option_id)
{
	if(screen.option == option_id) return;
	switch(screen.option = option_id)
	{
		case SCREEN_OPTION_HALF:
			screen.width_height = SCREEN_RESOLUTION_HALF;
			glutReshapeWindow(SCREEN_RESOLUTION_HALF, SCREEN_RESOLUTION_HALF);
			break;
		case SCREEN_OPTION_THREE_QUARTERS:
			screen.width_height = SCREEN_RESOLUTION_THREE_QUARTERS;
			glutReshapeWindow(SCREEN_RESOLUTION_THREE_QUARTERS, SCREEN_RESOLUTION_THREE_QUARTERS);
			break;
		case SCREEN_OPTION_DEFAULT:
			screen.width_height = SCREEN_RESOLUTION_DEFAULT;
			glutReshapeWindow(SCREEN_RESOLUTION_DEFAULT, SCREEN_RESOLUTION_DEFAULT);
			break;
		case SCREEN_OPTION_FIVE_QUARTERS:
			screen.width_height = SCREEN_RESOLUTION_FIVE_QUARTERS;
			glutReshapeWindow(SCREEN_RESOLUTION_FIVE_QUARTERS, SCREEN_RESOLUTION_FIVE_QUARTERS);
			break;
	}
	glViewport(0, 0, screen.width_height, screen.width_height);
}

void onDraw()
{
	elapsed_tick_count = (current_tick_count = GetTickCount())-last_tick_count;
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	if(object_bg.is)
	{
		glPushMatrix();
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glTranslated(-1.0, -1.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, objtexture[object_bg.texture_id].texDat.texID);
		glColor4d(screen.alpha, screen.alpha, screen.alpha, 1.0);
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 0.0); glVertex2f(0.0f, 0.0f);
		glTexCoord2d(0.0, 1.0); glVertex2f(0.0f, 2.0f);
		glTexCoord2d(1.0, 1.0); glVertex2f(2.0f, 2.0f);
		glTexCoord2d(1.0, 0.0); glVertex2f(2.0f, 0.0f);
		glEnd();
		glPopMatrix();
	}
	unsigned char i, j;
	if(game_state == GAME_STATE_PLAYING || game_state == GAME_STATE_PAUSING || game_state == GAME_STATE_CONTINUING || game_state == GAME_STATE_RESTARTING || game_state == GAME_STATE_PAUSED || game_state == GAME_STATE_DEATH || game_state == GAME_STATE_LOAD_ROUND || game_state == GAME_STATE_LEAVING)
	{
		for(i = 0; i < 5; i++)
		{
			for(j = 0; j < 5; j++)
			{
				if(tetris_nextdrop[i][j] == TETRIS_ENTITY_NOTHING) continue;
				glPushMatrix();
				glTranslated((entity_size*(double)j)+0.25, 0.75-(entity_size*(double)(i+1)), 0.0);
				glBindTexture(GL_TEXTURE_2D, objtexture[GAME_TEXTURE_ENTITIES].texDat.texID);
				glColor4d(screen.alpha, screen.alpha, screen.alpha, 1.0);
				glBegin(GL_QUADS);
				DrawEntity(tetris_nextdrop[i][j]);
				glEnd();
				glPopMatrix();
			}
		}
		for(i = 0; i < 20; i++)
		{
			for(j = 0; j < 10; j++)
			{
				if(tetris_field[i][j] == TETRIS_ENTITY_NOTHING) continue;
				glPushMatrix();
				glTranslated((entity_size*(double)j)-0.75, 0.75-(entity_size*(double)(i+1)), 0.0);
				glBindTexture(GL_TEXTURE_2D, objtexture[GAME_TEXTURE_ENTITIES].texDat.texID);
				glColor4d(screen.alpha, screen.alpha, screen.alpha, 1.0);
				glBegin(GL_QUADS);
				DrawEntity(tetris_field[i][j]);
				glEnd();
				glPopMatrix();
			}
		}
		char temp_x, temp_y;
		for(i = 0; i < 5; i++)
		{
			temp_y = (char)(mydrop_y+(i-2));
			if(temp_y < 0 || temp_y > 19) continue;
			for(j = 0; j < 5; j++)
			{
				temp_x = (char)(mydrop_x+(j-2));
				if(temp_x < 0 || temp_x > 9) continue;
				if(tetris_mydrop[i][j] == TETRIS_ENTITY_NOTHING) continue;
				glPushMatrix();
				glTranslated((entity_size*(double)temp_x)-0.75, 0.75-(entity_size*(double)(temp_y+1)), 0.0);
				glBindTexture(GL_TEXTURE_2D, objtexture[GAME_TEXTURE_ENTITIES].texDat.texID);
				glColor4d(screen.alpha, screen.alpha, screen.alpha, 1.0);
				glBegin(GL_QUADS);
				DrawEntity(tetris_mydrop[i][j]);
				glEnd();
				glPopMatrix();
			}
		}
		if((single_entity.elapsed_tick_count += elapsed_tick_count) >= 83)
		{
			while(single_entity.elapsed_tick_count >= 83)
			{
				single_entity.elapsed_tick_count -= 83;
				if(++single_entity.state >= 11) single_entity.state = 0;
			}
			unsigned char line = 0, col = (unsigned char)single_entity.state;
			while(col >= 4)
			{
				line++;
				col -= 4;
			}
			single_entity.tex_x = 0.25*(double)col;
			single_entity.tex_y = 1.0-(0.25*(double)line);
			bomb_tex_changed = !bomb_tex_changed;
		}
		if(menuDat.show_stats)
		{
			double offset = 0.0;
			for(i = 0; i < 20; i++)
			{
				if(player_stats.score_str[i] == '\0') break;
				if(player_stats.score_str[i] != 11)
				{
					glPushMatrix();
					glTranslated(-0.6+offset, 0.05, 0.0);
					glBindTexture(GL_TEXTURE_2D, objtexture[GAME_TEXTURE_NUMBERS].texDat.texID);
					glColor4d(screen.alpha, screen.alpha, screen.alpha, 1.0);
					glBegin(GL_QUADS);
					glTexCoord2d((double)(player_stats.score_str[i]-1)*0.1, 0.0); glVertex2d(0.0, 0.0);
					glTexCoord2d((double)(player_stats.score_str[i]-1)*0.1, 1.0); glVertex2d(0.0, 0.08);
					glTexCoord2d((double)(player_stats.score_str[i])*0.1, 1.0); glVertex2d(0.08, 0.08);
					glTexCoord2d((double)(player_stats.score_str[i])*0.1, 0.0); glVertex2d(0.08, 0.0);
					glEnd();
					glPopMatrix();
				}
				offset += 0.08;
			}
			offset = 0.0;
			for(i = 0; i < 11; i++)
			{
				if(player_stats.level_str[i] == '\0') break;
				if(player_stats.level_str[i] != 11)
				{
					glPushMatrix();
					glTranslated(0.12+offset, -0.25, 0.0);
					glBindTexture(GL_TEXTURE_2D, objtexture[GAME_TEXTURE_NUMBERS].texDat.texID);
					glColor4d(screen.alpha, screen.alpha, screen.alpha, 1.0);
					glBegin(GL_QUADS);
					glTexCoord2d((double)(player_stats.level_str[i]-1)*0.1, 0.0); glVertex2d(0.0, 0.0);
					glTexCoord2d((double)(player_stats.level_str[i]-1)*0.1, 1.0); glVertex2d(0.0, 0.08);
					glTexCoord2d((double)(player_stats.level_str[i])*0.1, 1.0); glVertex2d(0.08, 0.08);
					glTexCoord2d((double)(player_stats.level_str[i])*0.1, 0.0); glVertex2d(0.08, 0.0);
					glEnd();
					glPopMatrix();
				}
				offset += 0.08;
			}
		}
	}
	for(i = 0; i < MAX_2D_OBJECTS; i++)
	{
		if(!object_2d[i].is) continue;
		glPushMatrix();
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glTranslated(object_2d[i].pos[OBJECT_X], object_2d[i].pos[OBJECT_Y], 0.0);
		glBindTexture(GL_TEXTURE_2D, objtexture[object_2d[i].texture_id].texDat.texID);
		glColor4d(screen.alpha, screen.alpha, screen.alpha, object_2d[i].alpha);
		glRotated(object_2d[i].rotation, 0.0, 0.0, 1.0);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex2d(-object_2d[i].center[OBJECT_X], -object_2d[i].center[OBJECT_Y]);
		glTexCoord2f(0.0f, 1.0); glVertex2d(-object_2d[i].center[OBJECT_X], object_2d[i].size[OBJECT_Y]-object_2d[i].center[OBJECT_Y]);
		glTexCoord2f(1.0f, 1.0f); glVertex2d(object_2d[i].size[OBJECT_X]-object_2d[i].center[OBJECT_X], object_2d[i].size[OBJECT_Y]-object_2d[i].center[OBJECT_Y]);
		glTexCoord2f(1.0f, 0.0f); glVertex2d(object_2d[i].size[OBJECT_X]-object_2d[i].center[OBJECT_X], -object_2d[i].center[OBJECT_Y]);
		glEnd();
		glPopMatrix();
		switch(object_2d[i].fade_mode)
		{
			case FADE_MODE_IN:
				if((object_2d[i].alpha += (double)elapsed_tick_count/(double)object_2d[i].fade_time) >= 1.0)
				{
					object_2d[i].alpha = 1.0;
					object_2d[i].fade_mode = FADE_MODE_NONE;
				}
				break;
			case FADE_MODE_OUT:
				if((object_2d[i].alpha -= (double)elapsed_tick_count/(double)object_2d[i].fade_time) <= 0.0)
				{
					object_2d[i].alpha = 0.0;
					object_2d[i].fade_mode = FADE_MODE_NONE;
				}
				break;
		}
	}
	switch(menuDat.show_menu)
	{
		case GAME_MENU_MAIN:
			for(i = 0; i < NUM_GAME_MENU_ITEMS_MAIN; i++)
			{
				glPushMatrix();
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glTranslated(menuDat.main[i].pos_x, menuDat.main[i].pos_y, 0.0);
				glBindTexture(GL_TEXTURE_2D, objtexture[menuDat.main[i].texture_id].texDat.texID);
				glColor4d(screen.alpha, screen.alpha, screen.alpha, menuDat.alpha[GAME_MENU_MAIN]);
				glBegin(GL_QUADS);
				if(menuDat.main_hover == i)
				{
					if(menuDat.selected)
					{
						glTexCoord2d(0.0, 0.0); glVertex2d(0.0, 0.0);
						glTexCoord2d(0.0, third); glVertex2d(0.0, menuDat.main[i].size_y);
						glTexCoord2d(1.0, third); glVertex2d(menuDat.main[i].size_x, menuDat.main[i].size_y);
						glTexCoord2d(1.0, 0.0); glVertex2d(menuDat.main[i].size_x, 0.0);
					}
					else
					{
						glTexCoord2d(0.0, third); glVertex2d(0.0, 0.0);
						glTexCoord2d(0.0, twothird); glVertex2d(0.0, menuDat.main[i].size_y);
						glTexCoord2d(1.0, twothird); glVertex2d(menuDat.main[i].size_x, menuDat.main[i].size_y);
						glTexCoord2d(1.0, third); glVertex2d(menuDat.main[i].size_x, 0.0);
					}
				}
				else
				{
					glTexCoord2d(0.0, twothird); glVertex2d(0.0, 0.0);
					glTexCoord2d(0.0, 1.0); glVertex2d(0.0, menuDat.main[i].size_y);
					glTexCoord2d(1.0, 1.0); glVertex2d(menuDat.main[i].size_x, menuDat.main[i].size_y);
					glTexCoord2d(1.0, twothird); glVertex2d(menuDat.main[i].size_x, 0.0);
				}
				glEnd();
				glPopMatrix();
			}
			switch(menuDat.fade_mode[GAME_MENU_MAIN])
			{
				case FADE_MODE_IN:
					if((menuDat.alpha[GAME_MENU_MAIN] += (double)elapsed_tick_count/(double)menuDat.fade_time[GAME_MENU_MAIN]) >= 1.0)
					{
						menuDat.alpha[GAME_MENU_MAIN] = 1.0;
						menuDat.fade_mode[GAME_MENU_MAIN] = FADE_MODE_NONE;
					}
					break;
				case FADE_MODE_OUT:
					if((menuDat.alpha[GAME_MENU_MAIN] -= (double)elapsed_tick_count/(double)menuDat.fade_time[GAME_MENU_MAIN]) <= 0.0)
					{
						menuDat.alpha[GAME_MENU_MAIN] = 0.0;
						menuDat.fade_mode[GAME_MENU_MAIN] = FADE_MODE_NONE;
					}
					break;
			}
			DrawOptions();
			break;
		case GAME_MENU_PAUSE:
			glPushMatrix();
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glTranslated(0.0, 0.0, 0.0);
			glBindTexture(GL_TEXTURE_2D, objtexture[GAME_TEXTURE_BLACK].texDat.texID);
			glColor4d(1.0, 1.0, 1.0, 0.5);
			glBegin(GL_QUADS);
			glVertex2d(-1.0, -1.0);
			glVertex2d(-1.0, 1.0);
			glVertex2d(1.0, 1.0);
			glVertex2d(1.0, -1.0);
			glEnd();
			glPopMatrix();
			for(i = 0; i < NUM_GAME_MENU_ITEMS_PAUSE; i++)
			{
				glPushMatrix();
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glTranslated(menuDat.pause[i].pos_x, menuDat.pause[i].pos_y, 0.0);
				glBindTexture(GL_TEXTURE_2D, objtexture[menuDat.pause[i].texture_id].texDat.texID);
				glColor4d(screen.alpha, screen.alpha, screen.alpha, menuDat.alpha[GAME_MENU_PAUSE]);
				glBegin(GL_QUADS);
				if(menuDat.pause_hover == i)
				{
					if(menuDat.selected)
					{
						glTexCoord2d(0.0, 0.0); glVertex2d(0.0, 0.0);
						glTexCoord2d(0.0, third); glVertex2d(0.0, menuDat.pause[i].size_y);
						glTexCoord2d(1.0, third); glVertex2d(menuDat.pause[i].size_x, menuDat.pause[i].size_y);
						glTexCoord2d(1.0, 0.0); glVertex2d(menuDat.pause[i].size_x, 0.0);
					}
					else
					{
						glTexCoord2d(0.0, third); glVertex2d(0.0, 0.0);
						glTexCoord2d(0.0, twothird); glVertex2d(0.0, menuDat.pause[i].size_y);
						glTexCoord2d(1.0, twothird); glVertex2d(menuDat.pause[i].size_x, menuDat.pause[i].size_y);
						glTexCoord2d(1.0, third); glVertex2d(menuDat.pause[i].size_x, 0.0);
					}
				}
				else
				{
					glTexCoord2d(0.0, twothird); glVertex2d(0.0, 0.0);
					glTexCoord2d(0.0, 1.0); glVertex2d(0.0, menuDat.pause[i].size_y);
					glTexCoord2d(1.0, 1.0); glVertex2d(menuDat.pause[i].size_x, menuDat.pause[i].size_y);
					glTexCoord2d(1.0, twothird); glVertex2d(menuDat.pause[i].size_x, 0.0);
				}
				glEnd();
				glPopMatrix();
			}
			DrawOptions();
			switch(menuDat.fade_mode[GAME_MENU_PAUSE])
			{
				case FADE_MODE_IN:
					if((menuDat.alpha[GAME_MENU_PAUSE] += (double)elapsed_tick_count/(double)menuDat.fade_time[GAME_MENU_PAUSE]) >= 1.0)
					{
						menuDat.alpha[GAME_MENU_PAUSE] = 1.0;
						menuDat.fade_mode[GAME_MENU_PAUSE] = FADE_MODE_NONE;
					}
					break;
				case FADE_MODE_OUT:
					if((menuDat.alpha[GAME_MENU_PAUSE] -= (double)elapsed_tick_count/(double)menuDat.fade_time[GAME_MENU_PAUSE]) <= 0.0)
					{
						menuDat.alpha[GAME_MENU_PAUSE] = 0.0;
						menuDat.fade_mode[GAME_MENU_PAUSE] = FADE_MODE_NONE;
					}
					break;
			}
			break;
	}
	glutSwapBuffers();
	if(onLoadTexture.is)
	{
		LoadTGA(onLoadTexture.texture_id, onLoadTexture.file_name, onLoadTexture.smooth);
		onLoadTexture.is = false;
	}
	if(onUnloadTexture.is)
	{
		UnloadTGA(onUnloadTexture.texture_id);
		onUnloadTexture.is = false;
	}
	switch(screen.fade_mode)
	{
		case FADE_MODE_IN:
			if((screen.alpha += (double)elapsed_tick_count/(double)screen.fade_time) >= 1.0)
			{
				screen.alpha = 1.0;
				screen.fade_mode = FADE_MODE_NONE;
			}
			break;
		case FADE_MODE_OUT:
			if((screen.alpha -= (double)elapsed_tick_count/(double)screen.fade_time) <= 0.0)
			{
				screen.alpha = 0.0;
				screen.fade_mode = FADE_MODE_NONE;
			}
			break;
	}
	last_tick_count = current_tick_count;
}

void onChangeSize(int width, int height)
{
	glutReshapeWindow(screen.width_height, screen.width_height);
}

void onPressedKey(unsigned char key, int x, int y)
{
	switch(key = tolower(key))
	{
		case KEY_ESCAPE:
			switch(game_state)
			{
				case GAME_STATE_PLAYING:
					game_state = GAME_STATE_PAUSING;
					break;
				case GAME_STATE_PAUSED:
					game_state = GAME_STATE_CONTINUING;
					break;
				case GAME_STATE_DEATH:
					tetris_request.start = true;
					break;
				case GAME_STATE_IN_MENU:
					game_state = GAME_STATE_EXIT;
					break;
			}
			break;
		case KEY_UP:
			if(game_state == GAME_STATE_PLAYING && tetris_request.move == TETRIS_MOVE_NO) tetris_request.move = TETRIS_MOVE_DOWN_HARD;
			break;
		case KEY_DOWN:
			if(game_state == GAME_STATE_PLAYING && tetris_request.move == TETRIS_MOVE_NO) tetris_request.move = TETRIS_MOVE_DOWN_SOFT;
			break;
		case KEY_LEFT:
			if(game_state == GAME_STATE_PLAYING && tetris_request.move == TETRIS_MOVE_NO) tetris_request.move = TETRIS_MOVE_LEFT;
			break;
		case KEY_RIGHT:
			if(game_state == GAME_STATE_PLAYING && tetris_request.move == TETRIS_MOVE_NO) tetris_request.move = TETRIS_MOVE_RIGHT;
			break;
		case KEY_SPACE:
			if(game_state == GAME_STATE_PLAYING && tetris_request.rotate == TETRIS_ROTATE_NO) tetris_request.rotate = TETRIS_ROTATE_LEFT;
			break;
		case KEY_ENTER:
			if(game_state == GAME_STATE_PLAYING && tetris_request.rotate == TETRIS_ROTATE_NO) tetris_request.rotate = TETRIS_ROTATE_RIGHT;
			break;
		case KEY_ROTATE_L:
			if(game_state == GAME_STATE_PLAYING && tetris_request.rotate == TETRIS_ROTATE_NO) tetris_request.rotate = TETRIS_ROTATE_LEFT;
			break;
		case KEY_ROTATE_R:
			if(game_state == GAME_STATE_PLAYING && tetris_request.rotate == TETRIS_ROTATE_NO) tetris_request.rotate = TETRIS_ROTATE_RIGHT;
			break;
		case KEY_LOWER_RES:
			switch(screen.option)
			{
				case SCREEN_OPTION_THREE_QUARTERS:
					ChangeScreenResolution(SCREEN_OPTION_HALF);
					break;
				case SCREEN_OPTION_DEFAULT:
					ChangeScreenResolution(SCREEN_OPTION_THREE_QUARTERS);
					break;
				case SCREEN_OPTION_FIVE_QUARTERS:
					ChangeScreenResolution(SCREEN_OPTION_DEFAULT);
					break;
			}
			break;
		case KEY_HIGHER_RES:
			switch(screen.option)
			{
				case SCREEN_OPTION_HALF:
					ChangeScreenResolution(SCREEN_OPTION_THREE_QUARTERS);
					break;
				case SCREEN_OPTION_THREE_QUARTERS:
					ChangeScreenResolution(SCREEN_OPTION_DEFAULT);
					break;
				case SCREEN_OPTION_DEFAULT:
					ChangeScreenResolution(SCREEN_OPTION_FIVE_QUARTERS);
					break;
			}
			break;
	}
	if(allow_title_force) if(game_state == GAME_STATE_TITLE_FMOD || game_state == GAME_STATE_TITLE_OPENGL || game_state == GAME_STATE_TITLE_MAIN) no_title_force = false;
	if(game_state == GAME_STATE_IDLE || game_state == GAME_STATE_DEATH) tetris_request.start = true;
}

void onRealeasedKey(unsigned char key, int x, int y)
{
	switch(key)
	{
		case KEY_UP:
			if(game_state == GAME_STATE_PLAYING && tetris_request.move == TETRIS_MOVE_DOWN_HARD) tetris_request.move = TETRIS_MOVE_NO;
			break;
		case KEY_DOWN:
			if(game_state == GAME_STATE_PLAYING && tetris_request.move == TETRIS_MOVE_DOWN_SOFT) tetris_request.move = TETRIS_MOVE_NO;
			break;
		case KEY_LEFT:
			if(game_state == GAME_STATE_PLAYING && tetris_request.move == TETRIS_MOVE_LEFT) tetris_request.move = TETRIS_MOVE_NO;
			break;
		case KEY_RIGHT:
			if(game_state == GAME_STATE_PLAYING && tetris_request.move == TETRIS_MOVE_RIGHT) tetris_request.move = TETRIS_MOVE_NO;
			break;
		case KEY_SPACE:
			if(game_state == GAME_STATE_PLAYING && tetris_request.rotate == TETRIS_ROTATE_LEFT) tetris_request.rotate = TETRIS_ROTATE_NO;
			break;
		case KEY_ENTER:
			if(game_state == GAME_STATE_PLAYING && tetris_request.rotate == TETRIS_ROTATE_RIGHT) tetris_request.rotate = TETRIS_ROTATE_NO;
			break;
		case KEY_ROTATE_L:
			if(game_state == GAME_STATE_PLAYING && tetris_request.rotate == TETRIS_ROTATE_LEFT) tetris_request.rotate = TETRIS_ROTATE_NO;
			break;
		case KEY_ROTATE_R:
			if(game_state == GAME_STATE_PLAYING && tetris_request.rotate == TETRIS_ROTATE_RIGHT) tetris_request.rotate = TETRIS_ROTATE_NO;
			break;
	}
}

void onPressedSpecialKey(int key, int x, int y)
{
	switch(key)
	{
		case SPECIAL_KEY_UP:
			if(game_state == GAME_STATE_PLAYING && tetris_request.move == TETRIS_MOVE_NO) tetris_request.move = TETRIS_MOVE_DOWN_HARD;
			break;
		case SPECIAL_KEY_DOWN:
			if(game_state == GAME_STATE_PLAYING && tetris_request.move == TETRIS_MOVE_NO) tetris_request.move = TETRIS_MOVE_DOWN_SOFT;
			break;
		case SPECIAL_KEY_LEFT:
			if(game_state == GAME_STATE_PLAYING && tetris_request.move == TETRIS_MOVE_NO) tetris_request.move = TETRIS_MOVE_LEFT;
			break;
		case SPECIAL_KEY_RIGHT:
			if(game_state == GAME_STATE_PLAYING && tetris_request.move == TETRIS_MOVE_NO) tetris_request.move = TETRIS_MOVE_RIGHT;
			break;
	}
	if(allow_title_force) if(game_state == GAME_STATE_TITLE_FMOD || game_state == GAME_STATE_TITLE_OPENGL || game_state == GAME_STATE_TITLE_MAIN) no_title_force = false;
	if(game_state == GAME_STATE_IDLE || game_state == GAME_STATE_DEATH) tetris_request.start = true;
}

void onRealeasedSpecialKey(int key, int x, int y)
{
	switch(key)
	{
		case SPECIAL_KEY_UP:
			if(game_state == GAME_STATE_PLAYING && tetris_request.move == TETRIS_MOVE_DOWN_HARD) tetris_request.move = TETRIS_MOVE_NO;
			break;
		case SPECIAL_KEY_DOWN:
			if(game_state == GAME_STATE_PLAYING && tetris_request.move == TETRIS_MOVE_DOWN_SOFT) tetris_request.move = TETRIS_MOVE_NO;
			break;
		case SPECIAL_KEY_LEFT:
			if(game_state == GAME_STATE_PLAYING && tetris_request.move == TETRIS_MOVE_LEFT) tetris_request.move = TETRIS_MOVE_NO;
			break;
		case SPECIAL_KEY_RIGHT:
			if(game_state == GAME_STATE_PLAYING && tetris_request.move == TETRIS_MOVE_RIGHT) tetris_request.move = TETRIS_MOVE_NO;
			break;
	}
}

void ComputeOptions()
{
	SCREEN_OPTION_ENUM resolution_id;
	if((menuDat.apply_change_sound = menuDat.allow_sound_change) == CHANGE_SOUND_MUSIC)
	{
		menuDat.apply_change_sound = CHANGE_SOUND_MUSIC;
		sounds->setVolume(GAME_SOUND_CHANNEL_MUSIC, game_volume.music);
	}
	if((resolution_id = menuDat.allow_change_resolution) != SCREEN_OPTION_NOTHING)
	{
		sounds->playSound(GAME_SOUND_MENU_SELECT);
		ChangeScreenResolution(resolution_id);
	}
}

void onMouseClicked(int button, int state, int x, int y)
{
	switch(state)
	{
		case GLUT_DOWN:
			switch(game_state)
			{
				case GAME_STATE_TITLE_FMOD: case GAME_STATE_TITLE_OPENGL: case GAME_STATE_TITLE_MAIN:
					if(allow_title_force) no_title_force = false;
					break;
				case GAME_STATE_IDLE: case GAME_STATE_DEATH:
					tetris_request.start = true;
					break;
				case GAME_STATE_IN_MENU:
					switch(menuDat.main_hover)
					{
						case GAME_MENU_ITEM_MAIN_PARTY:
							game_mode = GAME_MODE_PARTY;
							game_state = GAME_STATE_LOAD_ROUND;
							break;
						case GAME_MENU_ITEM_MAIN_CLASSIC:
							game_mode = GAME_MODE_CLASSIC;
							game_state = GAME_STATE_LOAD_ROUND;
							break;
						case GAME_MENU_ITEM_MAIN_OPTIONS:
							sounds->playSound(GAME_SOUND_MENU_SELECT);
							ToggleOptions();
							break;
						case GAME_MENU_ITEM_MAIN_CREDITS:
							sounds->playSound(GAME_SOUND_MENU_SELECT);
							ToggleCredits();
							break;
						case GAME_MENU_ITEM_MAIN_EXIT:
							game_state = GAME_STATE_EXIT;
							break;
					}
					ComputeOptions();
					break;
				case GAME_STATE_PAUSED:
					switch(menuDat.pause_hover)
					{
						case GAME_MENU_ITEM_PAUSE_CONTINUE:
							game_state = GAME_STATE_CONTINUING;
							break;
						case GAME_MENU_ITEM_PAUSE_RESTART:
							game_state = GAME_STATE_RESTARTING;
							break;
						case GAME_MENU_ITEM_PAUSE_OPTIONS:
							sounds->playSound(GAME_SOUND_MENU_SELECT);
							ToggleOptions();
							break;
						case GAME_MENU_ITEM_PAUSE_EXIT:
							game_state = GAME_STATE_LEAVING;
							break;
					}
					ComputeOptions();
					break;
			}
			menuDat.selected = true;
			break;
		case GLUT_UP:
			menuDat.selected = false;
			if(menuDat.apply_change_sound == CHANGE_SOUND_SOUNDS) sounds->playSound(GAME_SOUND_MENU_SELECT);
			menuDat.apply_change_sound = CHANGE_SOUND_NOTHING;
			break;
	}
}

void onClickedHoverMouse(int x, int y)
{
	mouseDat.raw_x = x;
	mouseDat.raw_y = y;
}

void onHoverMouse(int x, int y)
{
	mouseDat.raw_x = x;
	mouseDat.raw_y = y;
}

struct threadDef
{
	HANDLE thread_handle;
	DWORD thread_id;
};

threadDef threaded;

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(screen.width_height = SCREEN_RESOLUTION_DEFAULT, SCREEN_RESOLUTION_DEFAULT);
	screen.option = SCREEN_OPTION_DEFAULT;
	glutCreateWindow("Titros v0.5 - A Tetris Clone");
	if(HWND hWnd = WindowFromDC(wglGetCurrentDC()))
	{
		SetWindowLong(hWnd, GWL_STYLE, (DWORD)GetWindowLong(hWnd, GWL_STYLE)&~WS_SIZEBOX);
		SendMessage(hWnd, (UINT)WM_SETICON, ICON_BIG, (LPARAM)LoadImage(NULL, L"icon.ico", IMAGE_ICON, 64, 64, LR_DEFAULTCOLOR|LR_LOADFROMFILE|LR_LOADTRANSPARENT));
	}
	FreeConsole();
	glutDisplayFunc(onDraw);
	glutIdleFunc(onDraw);
	glutReshapeFunc(onChangeSize);
	glutKeyboardFunc(onPressedKey);
	glutKeyboardUpFunc(onRealeasedKey);
	glutSpecialFunc(onPressedSpecialKey);
	glutSpecialUpFunc(onRealeasedSpecialKey);
	glutMouseFunc(onMouseClicked);
	glutMotionFunc(onClickedHoverMouse);
	glutPassiveMotionFunc(onHoverMouse);
	glutIgnoreKeyRepeat(1);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glEnable(GL_BLEND);
	screen.alpha = 1.0;
	LoadTGA(GAME_TEXTURE_LOAD, "data/textures/load.gt", true);
	SetBackground(GAME_TEXTURE_LOAD);
	memset(&menuDat, 0, sizeof(menuDat));
	menuDat.show_menu = GAME_MENU_NONE;
	menuDat.main_hover = GAME_MENU_ITEM_MAIN_NOTHING;
	menuDat.pause_hover = GAME_MENU_ITEM_PAUSE_NOTHING;
	menuDat.allow_change_resolution = SCREEN_OPTION_DEFAULT;
	threaded.thread_handle = CreateThread(NULL, NULL, threaded_main, NULL, NULL, &threaded.thread_id);
	credits_object = INVALID_OBJECT_ID;
	last_tick_count = current_tick_count = GetTickCount();
	glutMainLoop();
	return 0;
}

void ScreenFadeIn(DWORD fade_time, bool wait)
{
	screen.fade_mode = FADE_MODE_NONE;
	screen.alpha = 0.0;
	screen.fade_time = fade_time;
	screen.fade_mode = FADE_MODE_IN;
	if(wait) while(screen.fade_mode != FADE_MODE_NONE){}
}

void ScreenFadeOut(DWORD fade_time, bool wait)
{
	screen.fade_mode = FADE_MODE_NONE;
	screen.alpha = 1.0;
	screen.fade_time = fade_time;
	screen.fade_mode = FADE_MODE_OUT;
	if(wait) while(screen.fade_mode != FADE_MODE_NONE){}
}

void LogError(char *str)
{
	FILE *file;
	fopen_s(&file, "error.txt", "a");
	if(file == NULL) return;
	fprintf(file, str);
	fputc('\n', file);
	fclose(file);
}