#include <windows.h>
#include <time.h>
#include <iostream>
using namespace std;

#define _GAME_CPP_
#include "main.h"
#include "game.h"
#include "objects.h"
#include "tetris.h"

TETRIS_ENTITY_ENUM temp_drop[5][5];
TETRIMINO_ENUM entity_id;
unsigned char count_till_bomb = 0;
const double twelfth = 1.0/12.0;
bool mouse_update = false;
int temp_mouse_x = 0, temp_mouse_y = 0;
double options_x, options_y;
bool no_hover;
DWORD gametick;

void InitMainMenuItem(GAME_MENU_ITEM_MAIN_ENUM menu_item_id, GAME_TEXTURE_ENUM texture_id, double pos_x, double pos_y, double size_x, double size_y)
{
	menuDat.main[menu_item_id].texture_id = texture_id;
	menuDat.main[menu_item_id].pos_x = pos_x;
	menuDat.main[menu_item_id].pos_y = pos_y;
	menuDat.main[menu_item_id].size_x = size_x;
	menuDat.main[menu_item_id].size_y = size_y;
}

void InitPauseMenuItem(GAME_MENU_ITEM_PAUSE_ENUM menu_item_id, GAME_TEXTURE_ENUM texture_id, double pos_x, double pos_y, double size_x, double size_y)
{
	menuDat.pause[menu_item_id].texture_id = texture_id;
	menuDat.pause[menu_item_id].pos_x = pos_x;
	menuDat.pause[menu_item_id].pos_y = pos_y;
	menuDat.pause[menu_item_id].size_x = size_x;
	menuDat.pause[menu_item_id].size_y = size_y;
}

void InitSound(GAME_SOUND_ENUM sound_id, char *file_name, unsigned char loop, GAME_SOUND_CHANNEL_ENUM channel)
{
	sound_info[sound_id].name = file_name;
	sound_info[sound_id].loop = loop;
	sound_info[sound_id].channel = channel;
}

void MenuFadeIn(GAME_MENU_ENUM menu_id, DWORD fade_time, bool wait)
{
	menuDat.fade_mode[menu_id] = FADE_MODE_NONE;
	menuDat.fade_time[menu_id] = fade_time;
	menuDat.fade_mode[menu_id] = FADE_MODE_IN;
	if(wait) while(menuDat.fade_mode[menu_id] != FADE_MODE_NONE){}
}

/*void MenuFadeOut(GAME_MENU_ENUM menu_id, DWORD fade_time, bool wait)
{
	menuDat.fade_mode[menu_id] = FADE_MODE_NONE;
	menuDat.fade_time[menu_id] = fade_time;
	menuDat.fade_mode[menu_id] = FADE_MODE_OUT;
	if(wait) while(menuDat.fade_mode[menu_id] != FADE_MODE_NONE){}
}*/

unsigned __int64 easypow(unsigned char x, unsigned char y)
{
	if(y == 0) return 1;
	unsigned __int64 result = 1;
	for(unsigned char i = 1; i < y; i++) result *= x;
	return result;
};

void SetMusicVolume(float volume)
{
	if(volume > 1.0f) volume = 1.0f;
	else if(volume < 0.0f) volume = 0.0f;
	sounds->setVolume(GAME_SOUND_CHANNEL_MUSIC, volume);
	game_volume.music = volume;
	game_volume.music_bar.obj_size = (game_volume.music_bar.tex_size = (double)volume)*0.25;
}

void SetSoundsVolume(float volume)
{
	if(volume > 1.0f) volume = 1.0f;
	else if(volume < 0.0f) volume = 0.0f;
	sounds->setVolume(GAME_SOUND_CHANNEL_EFFECT_PRI, volume);
	sounds->setVolume(GAME_SOUND_CHANNEL_EFFECT_SEC, volume);
	game_volume.sounds = volume;
	game_volume.sounds_bar.obj_size = (game_volume.sounds_bar.tex_size = (double)volume)*0.25;
}

void GiveScore(TETRIS_CLEAR_ENUM clears)
{
	switch(clears)
	{
		case TETRIS_CLEAR_SINGLE:
			player_stats.score += 40*(player_stats.level+1);
			break;
		case TETRIS_CLEAR_DOUBLE:
			player_stats.score += 100*(player_stats.level+1);
			break;
		case TETRIS_CLEAR_TRIPLE:
			player_stats.score += 300*(player_stats.level+1);
			break;
		case TETRIS_CLEAR_TETRIS:
			player_stats.score += 1200*(player_stats.level+1);
			break;
	}
	unsigned __int64 temp_score = player_stats.score, powered = 0;
	unsigned char count = 0;
	bool in_number = false;
	for(unsigned char i = 0; i < 19; i++)
	{
		while(temp_score >= (powered = easypow(10, 19-i)))
		{
			temp_score -= powered;
			count++;
		}
		if(count != 0)
		{
			player_stats.score_str[i] = count+1;
			in_number = true;
			count = 0;
		}
		else if(in_number) player_stats.score_str[i] = 1;
		else
		{
			if(i == 18) player_stats.score_str[i] = 1;
			else player_stats.score_str[i] = 11;
		}
	}
	player_stats.score_str[19] = '\0';
}

void ResetPlayerStats()
{
	memset(&player_stats, 0, sizeof(player_stats));
	player_stats.tick_delay = 525;
	memset(&player_stats.score_str, 11, sizeof(player_stats.score_str));
	player_stats.score_str[18] = 1;
	player_stats.score_str[19] = '\0';
	memset(&player_stats.level_str, 11, sizeof(player_stats.level_str));
	player_stats.level_str[9] = 1;
	player_stats.level_str[10] = '\0';
}

void ComputeMouse(GAME_MENU_ENUM menu_id)
{
	if(temp_mouse_x != mouseDat.raw_x)
	{
		options_x = mouseDat.pos_x = (2.0/(double)screen.width_height*(double)(temp_mouse_x = mouseDat.raw_x))-1.0;
		mouse_update = true;
	}
	if(temp_mouse_y != mouseDat.raw_y)
	{
		mouseDat.pos_y = (options_y = 1.0-(2.0/(double)screen.width_height*(double)(temp_mouse_y = mouseDat.raw_y)))-twelfth;
		mouse_update = true;
	}
	if(mouse_update)
	{
		mouse_update = false;
		switch(menu_id)
		{
			case GAME_MENU_MAIN:
				for(unsigned char i = 0; i < NUM_GAME_MENU_ITEMS_MAIN; i++)
				{
					if((mouseDat.pos_x >= menuDat.main[i].pos_x && mouseDat.pos_x <= (menuDat.main[i].pos_x+menuDat.main[i].size_x)) && (mouseDat.pos_y+twelfth >= menuDat.main[i].pos_y && mouseDat.pos_y+twelfth <= (menuDat.main[i].pos_y+menuDat.main[i].size_y)))
					{
						if(menuDat.main_hover != i)
						{
							menuDat.main_hover = (GAME_MENU_ITEM_MAIN_ENUM)i;
							sounds->playSound(GAME_SOUND_MENU_HOVER);
						}
						no_hover = false;
					}
				}
				if(no_hover) menuDat.main_hover = GAME_MENU_ITEM_MAIN_NOTHING;
				break;
			case GAME_MENU_PAUSE:
				for(unsigned char i = 0; i < NUM_GAME_MENU_ITEMS_PAUSE; i++)
				{
					if((mouseDat.pos_x >= menuDat.pause[i].pos_x && mouseDat.pos_x <= (menuDat.pause[i].pos_x+menuDat.pause[i].size_x)) && (mouseDat.pos_y+twelfth >= menuDat.pause[i].pos_y && mouseDat.pos_y+twelfth <= (menuDat.pause[i].pos_y+menuDat.pause[i].size_y)))
					{
						if(menuDat.pause_hover != i)
						{
							menuDat.pause_hover = (GAME_MENU_ITEM_PAUSE_ENUM)i;
							sounds->playSound(GAME_SOUND_MENU_HOVER);
						}
						no_hover = false;
					}
				}
				if(no_hover) menuDat.pause_hover = GAME_MENU_ITEM_PAUSE_NOTHING;
				break;
		}
		if(menuDat.show_o_c == SHOW_OPTIONS)
		{
			if((options_x >= 0.0 && options_x <= 0.25) && (options_y >= 0.75 && options_y <= 0.8)) menuDat.allow_sound_change = CHANGE_SOUND_MUSIC;
			else if((options_x >= 0.5 && options_x <= 0.75) && (options_y >= 0.75 && options_y <= 0.8)) menuDat.allow_sound_change = CHANGE_SOUND_SOUNDS;
			else menuDat.allow_sound_change = CHANGE_SOUND_NOTHING;
			switch(menuDat.apply_change_sound)
			{
				case CHANGE_SOUND_MUSIC:
					if(options_x < 0.0) SetMusicVolume(0.0f);
					else if(options_x > 0.25) SetMusicVolume(1.0f);
					else SetMusicVolume((float)options_x*4.0f);
					break;
				case CHANGE_SOUND_SOUNDS:
					if(options_x < 0.5) SetSoundsVolume(0.0f);
					else if(options_x > 0.75) SetSoundsVolume(1.0f);
					else SetSoundsVolume(((float)options_x-0.5f)*4.0f);
					break;
			}
			if((options_x >= -0.1 && options_x <= 0.15) && (options_y >= 0.06 && options_y <= 0.26)) menuDat.allow_change_resolution = SCREEN_OPTION_HALF;
			else if((options_x >= 0.15 && options_x <= 0.4) && (options_y >= -0.04 && options_y <= 0.16)) menuDat.allow_change_resolution = SCREEN_OPTION_THREE_QUARTERS;
			else if((options_x >= 0.4 && options_x <= 0.65) && (options_y >= 0.06 && options_y <= 0.26)) menuDat.allow_change_resolution = SCREEN_OPTION_DEFAULT;
			else if((options_x >= 0.65 && options_x <= 0.9) && (options_y >= -0.04 && options_y <= 0.16)) menuDat.allow_change_resolution = SCREEN_OPTION_FIVE_QUARTERS;
			else menuDat.allow_change_resolution = SCREEN_OPTION_NOTHING;
		}
	}
}

void ClearTetrisField()
{
	unsigned char i, j;
	for(i = 0; i < 20; i++) for(j = 0; j < 10; j++) tetris_field[i][j] = TETRIS_ENTITY_NOTHING;
	for(i = 0; i < 5; i++)
	{
		for(j = 0; j < 5; j++)
		{
			tetris_mydrop[i][j] = TETRIS_ENTITY_NOTHING;
			tetris_nextdrop[i][j] = TETRIS_ENTITY_NOTHING;
		}
	}
}

void GenerateNextDrop()
{
	if(game_mode == GAME_MODE_PARTY)
	{
		if(++count_till_bomb >= 50)
		{
			count_till_bomb = 0;
			entity_id = TETRIMINO_BOMB;
		}
		else
		{
			switch(rand()%30)
			{
				case 17:
					entity_id = TETRIMINO_SINGLE;
					break;
				case 18:
					entity_id = TETRIMINO_DOUBLE;
					break;
				case 19:
					entity_id = TETRIMINO_TILT;
					break;
				default:
					entity_id = (TETRIMINO_ENUM)(rand()%(NUM_TETRIMINOS-4));
					break;
			}
		}
	}
	else entity_id = (TETRIMINO_ENUM)(rand()%(NUM_TETRIMINOS-4));
	for(unsigned char i = 0; i < 5; i++) for(unsigned char j = 0; j < 5; j++) tetris_nextdrop[i][j] = tetris_drop[entity_id][i][j];
}

bool ValidFieldAndMyDrop(unsigned char x, unsigned char y, bool autofill)
{
	char temp_x, temp_y;
	unsigned char i, j;
	if(autofill) for(i = 0; i < 5; i++) for(j = 0; j < 5; j++) temp_drop[i][j] = tetris_mydrop[i][j];
	for(i = 0; i < 5; i++)
	{
		temp_y = (char)(y+(i-2));
		if(temp_y > 19) {for(j = 0; j < 5; j++) {if(temp_drop[i][j] != TETRIS_ENTITY_NOTHING) return false;}}
		else
		{
			for(j = 0; j < 5; j++)
			{
				temp_x = (char)(x+(j-2));
				if(temp_x < 0 || temp_x > 9) {if(temp_drop[i][j] != TETRIS_ENTITY_NOTHING) return false;}
				else
				{
					if(temp_y < 0) continue;
					if(temp_drop[i][j] != TETRIS_ENTITY_NOTHING && tetris_field[temp_y][temp_x] != TETRIS_ENTITY_NOTHING) return false;
				}
			}
		}
	}
	return true;
}

int game_over_object = INVALID_OBJECT_ID;

void DropNextBlock()
{
	for(unsigned char i = 0; i < 5; i++) for(unsigned char j = 0; j < 5; j++) tetris_mydrop[i][j] = tetris_nextdrop[i][j];
	if(ValidFieldAndMyDrop(mydrop_x = 4, mydrop_y = 0, true))
	{
		if(entity_id == TETRIMINO_BOMB) sounds->playSound(GAME_SOUND_BOMB);
		GenerateNextDrop();
		return;
	}
	sounds->playSound(GAME_SOUND_SWOOSH);
	if(game_over_object == INVALID_OBJECT_ID) game_over_object = Create2DObject(GAME_TEXTURE_GAME_OVER, 0.0, 0.5, 0.0, 1.5, 0.5, 0.75, 0.25, 0.0);
	else Swap2DObjectTexture(game_over_object, GAME_TEXTURE_GAME_OVER);
	Object2DFadeIn(game_over_object, 1000, true);
	sounds->playSound(GAME_SOUND_GAME_OVER);
	game_state = GAME_STATE_DEATH;
}

bool MoveMyDropLeft()
{
	unsigned char temp_x = mydrop_x-1;
	if(!ValidFieldAndMyDrop(temp_x, mydrop_y, true)) return false;
	mydrop_x = temp_x;
	return true;
}

bool MoveMyDropRight()
{
	unsigned char temp_x = mydrop_x+1;
	if(!ValidFieldAndMyDrop(temp_x, mydrop_y, true)) return false;
	mydrop_x = temp_x;
	return true;
}

bool MoveMyDropDown()
{
	unsigned char temp_y = mydrop_y+1;
	if(!ValidFieldAndMyDrop(mydrop_x, temp_y, true)) return false;
	mydrop_y = temp_y;
	return true;
}

bool RotateMyDropClockwise()
{
	unsigned char i, j;
	for(j = 0; j < 5; j++) for(i = 0; i < 5; i++) temp_drop[i][j] = tetris_mydrop[j][4-i];
	if(!ValidFieldAndMyDrop(mydrop_x, mydrop_y, false)) return false;
	for(i = 0; i < 5; i++) for(j = 0; j < 5; j++) tetris_mydrop[i][j] = temp_drop[i][j];
	return true;
}

bool RotateMyDropAntiClockwise()
{
	unsigned char i, j;
	for(j = 0; j < 5; j++) for(i = 0; i < 5; i++) temp_drop[i][j] = tetris_mydrop[4-j][i];
	if(!ValidFieldAndMyDrop(mydrop_x, mydrop_y, false)) return false;
	for(i = 0; i < 5; i++) for(j = 0; j < 5; j++) tetris_mydrop[i][j] = temp_drop[i][j];
	return true;
}

TETRIS_CLEAR_ENUM PlaceMyDrop()
{
	char temp_x, temp_y;
	unsigned char i, j;
	bool is_bomb = false;
	for(i = 0; i < 5; i++)
	{
		temp_y = ((char)mydrop_y+((char)i-2));
		if(temp_y < 0 || temp_y > 19) continue;
		for(j = 0; j < 5; j++)
		{
			temp_x = ((char)mydrop_x+((char)j-2));
			if(temp_x < 0 || temp_x > 9) continue;
			if(tetris_mydrop[i][j] == TETRIS_ENTITY_NOTHING) continue;
			if(tetris_mydrop[i][j] == TETRIS_ENTITY_BOMB)
			{
				for(i = 0; i < 5; i++)
				{
					temp_y = ((char)mydrop_y+((char)i-2));
					if(temp_y < 0 || temp_y > 19) continue;
					for(j = 0; j < 5; j++)
					{
						temp_x = ((char)mydrop_x+((char)j-2));
						if(temp_x < 0 || temp_x > 9) continue;
						tetris_field[temp_y][temp_x] = TETRIS_ENTITY_NOTHING;
					}
				}
				is_bomb = true;
				break;
			}
			tetris_field[temp_y][temp_x] = tetris_mydrop[i][j];
		}
		if(is_bomb) break;
	}
	for(i = 0; i < 5; i++) for(j = 0; j < 5; j++) tetris_mydrop[i][j] = TETRIS_ENTITY_NOTHING;
	if(is_bomb) return TETRIS_CLEAR_BOMB;
	bool no_void = true;
	unsigned char count_lines = TETRIS_CLEAR_ZERO;
	for(i = 19; i < 20; i--)
	{
		for(j = 9; j < 10; j--)
		{
			if(tetris_field[i][j] != TETRIS_ENTITY_NOTHING) continue;
			no_void = false;
		}
		if(no_void)
		{
			if(count_lines > TETRIS_CLEAR_TETRIS) count_lines = TETRIS_CLEAR_TETRIS;
			else count_lines++;
			for(unsigned char k = i; k > 0; k--) for(j = 9; j < 10; j--) tetris_field[k][j] = tetris_field[k-1][j];
			i++;
		}
		no_void = true;
	}
	if(count_lines > 0)
	{
		GiveScore((TETRIS_CLEAR_ENUM)count_lines);
		player_stats.cleared_lines += count_lines;
		while(player_stats.cleared_lines >= 10)
		{
			player_stats.cleared_lines -= 10;
			player_stats.level++;
			player_stats.update_level = true;
		}
		if(player_stats.update_level)
		{
			player_stats.tick_delay = ((10-(player_stats.level > 10 ? 10 : player_stats.level))*50)+25;
			unsigned int temp_level = player_stats.level, powered = 0;
			unsigned char count = 0;
			bool in_number = false;
			for(unsigned char i = 0; i < 10; i++)
			{
				while(temp_level >= (powered = (int)easypow(10, 10-i)))
				{
					temp_level -= powered;
					count++;
				}
				if(count != 0)
				{
					player_stats.level_str[i] = count+1;
					in_number = true;
					count = 0;
				}
				else if(in_number) player_stats.level_str[i] = 1;
				else
				{
					if(i == 9) player_stats.level_str[i] = 1;
					else player_stats.level_str[i] = 11;
				}
			}
			player_stats.level_str[10] = '\0';
		}
	}
	return (TETRIS_CLEAR_ENUM)count_lines;
}

void SmashMyDropDown()
{
	while(MoveMyDropDown()){}
	switch(PlaceMyDrop())
	{
		case TETRIS_CLEAR_ZERO:
			sounds->playSound(GAME_SOUND_DROP_FAST);
			break;
		case TETRIS_CLEAR_SINGLE:
			sounds->playSound(GAME_SOUND_CLEAR_LINES);
			Sleep(500);
			if(player_stats.update_level) sounds->playSound(GAME_SOUND_LEVEL_UP);
			else sounds->playSound(GAME_SOUND_SINGLE);
			break;
		case TETRIS_CLEAR_DOUBLE:
			sounds->playSound(GAME_SOUND_CLEAR_LINES);
			Sleep(500);
			if(player_stats.update_level) sounds->playSound(GAME_SOUND_LEVEL_UP);
			else sounds->playSound(GAME_SOUND_DOUBLE);
			break;
		case TETRIS_CLEAR_TRIPLE:
			sounds->playSound(GAME_SOUND_CLEAR_LINES);
			Sleep(500);
			if(player_stats.update_level) sounds->playSound(GAME_SOUND_LEVEL_UP);
			else sounds->playSound(GAME_SOUND_TRIPLE);
			break;
		case TETRIS_CLEAR_TETRIS:
			sounds->playSound(GAME_SOUND_EXPLOSION);
			Sleep(500);
			if(player_stats.update_level) sounds->playSound(GAME_SOUND_LEVEL_UP);
			else sounds->playSound(GAME_SOUND_TETRIS);
			break;
		case TETRIS_CLEAR_BOMB:
			sounds->playSound(GAME_SOUND_EXPLOSION);
			Sleep(500);
			break;
	}
	player_stats.update_level = false;
	DropNextBlock();
}

DWORD WINAPI threaded_main(__in LPVOID lpParameter)
{
	srand((unsigned int)time(NULL));
	sounds = new audioStream;
	sounds->initializeSound();
	InitSound(GAME_MUSIC_INTRO, "data/sounds/intro_music.gs", FMOD_LOOP_NORMAL, GAME_SOUND_CHANNEL_MUSIC);
	InitSound(GAME_MUSIC_PARTY, "data/sounds/party_music.gs", FMOD_LOOP_NORMAL, GAME_SOUND_CHANNEL_MUSIC);
	InitSound(GAME_MUSIC_CLASSIC, "data/sounds/classic_music.gs", FMOD_LOOP_NORMAL, GAME_SOUND_CHANNEL_MUSIC);
	InitSound(GAME_SOUND_WELCOME, "data/sounds/welcome.gs", FMOD_LOOP_OFF, GAME_SOUND_CHANNEL_EFFECT_PRI);
	InitSound(GAME_SOUND_PARTY, "data/sounds/party.gs", FMOD_LOOP_OFF, GAME_SOUND_CHANNEL_EFFECT_PRI);
	InitSound(GAME_SOUND_CLASSIC, "data/sounds/classic.gs", FMOD_LOOP_OFF, GAME_SOUND_CHANNEL_EFFECT_PRI);
	InitSound(GAME_SOUND_SWOOSH, "data/sounds/swoosh.gs", FMOD_LOOP_OFF, GAME_SOUND_CHANNEL_EFFECT_SEC);
	InitSound(GAME_SOUND_MENU_HOVER, "data/sounds/hover.gs", FMOD_LOOP_OFF, GAME_SOUND_CHANNEL_EFFECT_SEC);
	InitSound(GAME_SOUND_MENU_SELECT, "data/sounds/select.gs", FMOD_LOOP_OFF, GAME_SOUND_CHANNEL_EFFECT_SEC);
	InitSound(GAME_SOUND_READY, "data/sounds/ready.gs", FMOD_LOOP_OFF, GAME_SOUND_CHANNEL_EFFECT_PRI);
	InitSound(GAME_SOUND_GO, "data/sounds/go.gs", FMOD_LOOP_OFF, GAME_SOUND_CHANNEL_EFFECT_PRI);
	InitSound(GAME_SOUND_PAUSE, "data/sounds/pause.gs", FMOD_LOOP_OFF, GAME_SOUND_CHANNEL_EFFECT_SEC);
	InitSound(GAME_SOUND_GAME_OVER, "data/sounds/game_over.gs", FMOD_LOOP_OFF, GAME_SOUND_CHANNEL_EFFECT_PRI);
	InitSound(GAME_SOUND_MOVE, "data/sounds/move.gs", FMOD_LOOP_OFF, GAME_SOUND_CHANNEL_EFFECT_SEC);
	InitSound(GAME_SOUND_ROTATE, "data/sounds/rotate.gs", FMOD_LOOP_OFF, GAME_SOUND_CHANNEL_EFFECT_SEC);
	InitSound(GAME_SOUND_DROP, "data/sounds/drop.gs", FMOD_LOOP_OFF, GAME_SOUND_CHANNEL_EFFECT_SEC);
	InitSound(GAME_SOUND_DROP_FAST, "data/sounds/drop_fast.gs", FMOD_LOOP_OFF, GAME_SOUND_CHANNEL_EFFECT_SEC);
	InitSound(GAME_SOUND_CLEAR_LINES, "data/sounds/clear_lines.gs", FMOD_LOOP_OFF, GAME_SOUND_CHANNEL_EFFECT_SEC);
	InitSound(GAME_SOUND_BOMB, "data/sounds/bomb.gs", FMOD_LOOP_OFF, GAME_SOUND_CHANNEL_EFFECT_PRI);
	InitSound(GAME_SOUND_EXPLOSION, "data/sounds/explosion.gs", FMOD_LOOP_OFF, GAME_SOUND_CHANNEL_EFFECT_SEC);
	InitSound(GAME_SOUND_SINGLE, "data/sounds/single.gs", FMOD_LOOP_OFF, GAME_SOUND_CHANNEL_EFFECT_PRI);
	InitSound(GAME_SOUND_DOUBLE, "data/sounds/double.gs", FMOD_LOOP_OFF, GAME_SOUND_CHANNEL_EFFECT_PRI);
	InitSound(GAME_SOUND_TRIPLE, "data/sounds/triple.gs", FMOD_LOOP_OFF, GAME_SOUND_CHANNEL_EFFECT_PRI);
	InitSound(GAME_SOUND_TETRIS, "data/sounds/tetris.gs", FMOD_LOOP_OFF, GAME_SOUND_CHANNEL_EFFECT_PRI);
	InitSound(GAME_SOUND_LEVEL_UP, "data/sounds/lvl_up.gs", FMOD_LOOP_OFF, GAME_SOUND_CHANNEL_EFFECT_PRI);
	sounds->loadSounds();
	SetMusicVolume(0.8f);
	SetSoundsVolume(1.0f);
	LoadTexture(GAME_TEXTURE_BLACK, "data/textures/black.gt", false);
	LoadTexture(GAME_TEXTURE_FMOD, "data/textures/fmod.gt", true);
	LoadTexture(GAME_TEXTURE_OPENGL, "data/textures/opengl.gt", true);
	LoadTexture(GAME_TEXTURE_TITLE, "data/textures/title.gt", true);
	LoadTexture(GAME_TEXTURE_BUTTON_START, "data/textures/button_start.gt", true);
	LoadTexture(GAME_TEXTURE_MAIN_MENU, "data/textures/main_menu.gt", true);
	LoadTexture(GAME_TEXTURE_PARTY_BUTTON, "data/textures/party_button.gt", true);
	LoadTexture(GAME_TEXTURE_CLASSIC_BUTTON, "data/textures/classic_button.gt", true);
	LoadTexture(GAME_TEXTURE_OPTIONS_BUTTON, "data/textures/options_button.gt", true);
	LoadTexture(GAME_TEXTURE_OPTIONS, "data/textures/options.gt", true);
	LoadTexture(GAME_TEXTURE_VOLUME, "data/textures/volume.gt", true);
	LoadTexture(GAME_TEXTURE_RESOLUTION, "data/textures/resolution.gt", true);
	LoadTexture(GAME_TEXTURE_CREDITS_BUTTON, "data/textures/credits_button.gt", true);
	LoadTexture(GAME_TEXTURE_CREDITS, "data/textures/credits.gt", true);
	LoadTexture(GAME_TEXTURE_EXIT_BUTTON, "data/textures/exit_button.gt", true);
	LoadTexture(GAME_TEXTURE_FIELD, "data/textures/field.gt", true);
	LoadTexture(GAME_TEXTURE_READY, "data/textures/ready.gt", true);
	LoadTexture(GAME_TEXTURE_GO, "data/textures/go.gt", true);
	LoadTexture(GAME_TEXTURE_PAUSE, "data/textures/pause.gt", true);
	LoadTexture(GAME_TEXTURE_CONTINUE_BUTTON, "data/textures/continue_button.gt", true);
	LoadTexture(GAME_TEXTURE_RESTART_BUTTON, "data/textures/restart_button.gt", true);
	LoadTexture(GAME_TEXTURE_GAME_OVER, "data/textures/game_over.gt", true);
	LoadTexture(GAME_TEXTURE_ENTITIES, "data/textures/entities.gt", true);
	LoadTexture(GAME_TEXTURE_SCORE, "data/textures/score.gt", true);
	LoadTexture(GAME_TEXTURE_LEVEL, "data/textures/level.gt", true);
	LoadTexture(GAME_TEXTURE_NUMBERS, "data/textures/numbers.gt", true);
	menuDat.alpha[GAME_MENU_MAIN] = 0.0;
	InitMainMenuItem(GAME_MENU_ITEM_MAIN_PARTY, GAME_TEXTURE_PARTY_BUTTON, -1.0, 0.75, 0.75, 0.25);
	InitMainMenuItem(GAME_MENU_ITEM_MAIN_CLASSIC, GAME_TEXTURE_CLASSIC_BUTTON, -1.0, 0.35, 0.75, 0.25);
	InitMainMenuItem(GAME_MENU_ITEM_MAIN_OPTIONS, GAME_TEXTURE_OPTIONS_BUTTON, -1.0, -0.05, 0.75, 0.25);
	InitMainMenuItem(GAME_MENU_ITEM_MAIN_CREDITS, GAME_TEXTURE_CREDITS_BUTTON, -1.0, -0.45, 0.75, 0.25);
	InitMainMenuItem(GAME_MENU_ITEM_MAIN_EXIT, GAME_TEXTURE_EXIT_BUTTON, 0.25, -0.85, 0.75, 0.25);
	menuDat.alpha[GAME_MENU_PAUSE] = 1.0;
	InitPauseMenuItem(GAME_MENU_ITEM_PAUSE_CONTINUE, GAME_TEXTURE_CONTINUE_BUTTON, -1.0, 0.75, 0.75, 0.25);
	InitPauseMenuItem(GAME_MENU_ITEM_PAUSE_RESTART, GAME_TEXTURE_RESTART_BUTTON, -1.0, 0.35, 0.75, 0.25);
	InitPauseMenuItem(GAME_MENU_ITEM_PAUSE_OPTIONS, GAME_TEXTURE_OPTIONS_BUTTON, -1.0, -0.05, 0.75, 0.25);
	InitPauseMenuItem(GAME_MENU_ITEM_PAUSE_EXIT, GAME_TEXTURE_EXIT_BUTTON, 0.25, -0.85, 0.75, 0.25);
	gametick = GetTickCount();
	int press_button_object = INVALID_OBJECT_ID, stats_object[2] = {INVALID_OBJECT_ID, INVALID_OBJECT_ID};
	bool start_game;
	unsigned char i, j;
	while(true)
	{
		no_hover = true;
		bool not_smashed = true;
		switch(game_state)
		{
			case GAME_STATE_LOADING:
				ScreenFadeOut(500, true);
				SetBackground(GAME_TEXTURE_FMOD);
				ScreenFadeIn(500, true);
				game_state = GAME_STATE_TITLE_FMOD;
				break;
			case GAME_STATE_TITLE_FMOD:
				allow_title_force = true;
				while(gametick+1500 >= GetTickCount() && no_title_force){}
				allow_title_force = false;
				ScreenFadeOut(500, true);
				SetBackground(GAME_TEXTURE_OPENGL);
				ScreenFadeIn(500, true);
				no_title_force = true;
				game_state = GAME_STATE_TITLE_OPENGL;
				break;
			case GAME_STATE_TITLE_OPENGL:
				allow_title_force = true;
				while(gametick+1500 >= GetTickCount() && no_title_force){}
				allow_title_force = false;
				ScreenFadeOut(500, true);
				SetBackground(GAME_TEXTURE_TITLE);
				sounds->playSound(GAME_MUSIC_INTRO);
				ScreenFadeIn(500, true);
				sounds->playSound(GAME_SOUND_WELCOME);
				no_title_force = true;
				game_state = GAME_STATE_TITLE_MAIN;
				break;
			case GAME_STATE_TITLE_MAIN:
				allow_title_force = true;
				while(gametick+1500 >= GetTickCount() && no_title_force){}
				allow_title_force = false;
				SetBackground(GAME_TEXTURE_TITLE);
				UnloadTexture(GAME_TEXTURE_LOAD);
				UnloadTexture(GAME_TEXTURE_FMOD);
				UnloadTexture(GAME_TEXTURE_OPENGL);
				no_title_force = true;
				press_button_object = Create2DObject(GAME_TEXTURE_BUTTON_START, 0.0, -0.3, 0.0, 1.0, 0.5, 0.5, 0.25, 0.0);
				Object2DFadeIn(press_button_object, 1000, false);
				game_state = GAME_STATE_IDLE;
				break;
			case GAME_STATE_IDLE:
				start_game = tetris_request.start;
				if(start_game != tetris_request.current_start)
				{
					tetris_request.current_start = start_game;
					sounds->playSound(GAME_SOUND_SWOOSH);
					ScreenFadeOut(1000, true);
					Delete2DObject(press_button_object);
					press_button_object = INVALID_OBJECT_ID;
					SetBackground(GAME_TEXTURE_MAIN_MENU);
					menuDat.show_menu = GAME_MENU_MAIN;
					MenuFadeIn(GAME_MENU_MAIN, 2000, false);
					ScreenFadeIn(1000, true);
					game_state = GAME_STATE_IN_MENU;
					tetris_request.start = false;
					tetris_request.current_start = false;
				}
				break;
			case GAME_STATE_IN_MENU:
				ComputeMouse(GAME_MENU_MAIN);
				break;
			case GAME_STATE_LOAD_ROUND:
				ResetPlayerStats();
				switch(game_mode)
				{
					case GAME_MODE_PARTY:
						sounds->playSound(GAME_SOUND_PARTY);
						Sleep(500);
						sounds->playSound(GAME_SOUND_SWOOSH);
						ScreenFadeOut(1000, true);
						DisableOptionsAndCredits();
						menuDat.show_stats = true;
						for(i = 0; i < 2; i++) Delete2DObject(stats_object[i]);
						stats_object[0] = Create2DObject(GAME_TEXTURE_SCORE, 0.25, 0.15, 0.0, 0.75, 0.15, 0.0, 0.0, 1.0);
						stats_object[1] = Create2DObject(GAME_TEXTURE_LEVEL, 0.25, -0.15, 0.0, 0.75, 0.15, 0.0, 0.0, 1.0);
						menuDat.show_menu = GAME_MENU_NONE;
						menuDat.main_hover = GAME_MENU_ITEM_MAIN_NOTHING;
						ClearTetrisField();
						GenerateNextDrop();
						SetBackground(GAME_TEXTURE_FIELD);
						sounds->playSound(GAME_MUSIC_PARTY);
						ScreenFadeIn(1000, true);
						break;
					case GAME_MODE_CLASSIC:
						sounds->playSound(GAME_SOUND_CLASSIC);
						Sleep(500);
						sounds->playSound(GAME_SOUND_SWOOSH);
						ScreenFadeOut(1000, true);
						DisableOptionsAndCredits();
						menuDat.show_stats = true;
						for(i = 0; i < 2; i++) Delete2DObject(stats_object[i]);
						stats_object[0] = Create2DObject(GAME_TEXTURE_SCORE, 0.25, 0.15, 0.0, 0.75, 0.15, 0.0, 0.0, 1.0);
						stats_object[1] = Create2DObject(GAME_TEXTURE_LEVEL, 0.25, -0.15, 0.0, 0.75, 0.15, 0.0, 0.0, 1.0);
						menuDat.show_menu = GAME_MENU_NONE;
						menuDat.main_hover = GAME_MENU_ITEM_MAIN_NOTHING;
						ClearTetrisField();
						GenerateNextDrop();
						SetBackground(GAME_TEXTURE_FIELD);
						sounds->playSound(GAME_MUSIC_CLASSIC);
						ScreenFadeIn(1000, true);
						break;
				}
				Sleep(500);
				Delete2DObject(press_button_object);
				press_button_object = Create2DObject(GAME_TEXTURE_READY, 0.0, 0.0, 0.0, 1.0, 0.5, 0.5, 0.25, 0.0);
				Object2DFadeIn(press_button_object, 166, true);
				sounds->playSound(GAME_SOUND_READY);
				for(i = 0; i < 165; i++)
				{
					object_2d[press_button_object].size[OBJECT_X] += 0.002;
					object_2d[press_button_object].size[OBJECT_Y] += 0.001;
					for(j = 0; j < 2; j++) object_2d[press_button_object].center[j] = object_2d[press_button_object].size[j]/2;
					Sleep(8);
				}
				Object2DFadeOut(press_button_object, 500, false);
				for(i = 0; i < 60; i++)
				{
					object_2d[press_button_object].size[OBJECT_X] += 0.002;
					object_2d[press_button_object].size[OBJECT_Y] += 0.001;
					for(j = 0; j < 2; j++) object_2d[press_button_object].center[j] = object_2d[press_button_object].size[j]/2;
					Sleep(8);
				}
				Delete2DObject(press_button_object);
				press_button_object = Create2DObject(GAME_TEXTURE_GO, 0.0, 0.0, 0.0, 1.0, 0.5, 0.5, 0.25, 0.0);
				Object2DFadeIn(press_button_object, 250, false);
				sounds->playSound(GAME_SOUND_GO);
				for(i = 0; i < 30; i++)
				{
					object_2d[press_button_object].size[OBJECT_X] += 0.002;
					object_2d[press_button_object].size[OBJECT_Y] += 0.001;
					for(j = 0; j < 2; j++) object_2d[press_button_object].center[j] = object_2d[press_button_object].size[j]/2;
					Sleep(8);
				}
				Object2DFadeOut(press_button_object, 250, false);
				for(i = 0; i < 30; i++)
				{
					object_2d[press_button_object].size[OBJECT_X] += 0.002;
					object_2d[press_button_object].size[OBJECT_Y] += 0.001;
					for(j = 0; j < 2; j++) object_2d[press_button_object].center[j] = object_2d[press_button_object].size[j]/2;
					Sleep(8);
				}
				Delete2DObject(press_button_object);
				press_button_object = INVALID_OBJECT_ID;
				DropNextBlock();
				game_state = GAME_STATE_PLAYING;
				break;
			case GAME_STATE_PLAYING:
				gametick = GetTickCount();
				while(gametick+(DWORD)player_stats.tick_delay >= GetTickCount())
				{
					TETRIS_MOVE_ENUM temp_move = tetris_request.move;
					TETRIS_ROTATE_ENUM temp_rotate = tetris_request.rotate;
					if(temp_move != tetris_request.current_move)
					{
						switch(temp_move)
						{
							case TETRIS_MOVE_LEFT:
								if(MoveMyDropLeft()) sounds->playSound(GAME_SOUND_MOVE);
								break;
							case TETRIS_MOVE_RIGHT:
								if(MoveMyDropRight()) sounds->playSound(GAME_SOUND_MOVE);
								break;
							case TETRIS_MOVE_DOWN_SOFT:
								if(MoveMyDropDown()) sounds->playSound(GAME_SOUND_MOVE);
								break;
							case TETRIS_MOVE_DOWN_HARD:
								SmashMyDropDown();
								not_smashed = false;
								break;
						}
						tetris_request.current_move = temp_move;
					}
					if(temp_rotate != tetris_request.current_rotate)
					{
						switch(temp_rotate)
						{
							case TETRIS_ROTATE_LEFT:
								if(RotateMyDropAntiClockwise()) sounds->playSound(GAME_SOUND_ROTATE);
								break;
							case TETRIS_ROTATE_RIGHT:
								if(RotateMyDropClockwise()) sounds->playSound(GAME_SOUND_ROTATE);
								break;
						}
						tetris_request.current_rotate = temp_rotate;
					}
				}
				if(not_smashed)
				{
					if(!MoveMyDropDown())
					{
						switch(PlaceMyDrop())
						{
							case TETRIS_CLEAR_ZERO:
								sounds->playSound(GAME_SOUND_DROP);
								break;
							case TETRIS_CLEAR_SINGLE:
								sounds->playSound(GAME_SOUND_CLEAR_LINES);
								Sleep(500);
								if(player_stats.update_level) sounds->playSound(GAME_SOUND_LEVEL_UP);
								else sounds->playSound(GAME_SOUND_SINGLE);
								break;
							case TETRIS_CLEAR_DOUBLE:
								sounds->playSound(GAME_SOUND_CLEAR_LINES);
								Sleep(500);
								if(player_stats.update_level) sounds->playSound(GAME_SOUND_LEVEL_UP);
								else sounds->playSound(GAME_SOUND_DOUBLE);
								break;
							case TETRIS_CLEAR_TRIPLE:
								sounds->playSound(GAME_SOUND_CLEAR_LINES);
								Sleep(500);
								if(player_stats.update_level) sounds->playSound(GAME_SOUND_LEVEL_UP);
								else sounds->playSound(GAME_SOUND_TRIPLE);
								break;
							case TETRIS_CLEAR_TETRIS:
								sounds->playSound(GAME_SOUND_EXPLOSION);
								Sleep(500);
								if(player_stats.update_level) sounds->playSound(GAME_SOUND_LEVEL_UP);
								else sounds->playSound(GAME_SOUND_TETRIS);
								break;
							case TETRIS_CLEAR_BOMB:
								sounds->playSound(GAME_SOUND_EXPLOSION);
								Sleep(500);
								break;
						}
						player_stats.update_level = false;
						DropNextBlock();
					}
				}
				break;
			case GAME_STATE_PAUSING:
				sounds->setVolume(GAME_SOUND_CHANNEL_MUSIC, game_volume.music/5.0f);
				sounds->playSound(GAME_SOUND_PAUSE);
				Delete2DObject(press_button_object);
				press_button_object = Create2DObject(GAME_TEXTURE_PAUSE, 0.0, -0.5, 0.0, 1.5, 0.5, 0.75, 0.25, 0.0);
				Object2DFadeIn(press_button_object, 500, false);
				menuDat.show_menu = GAME_MENU_PAUSE;
				game_state = GAME_STATE_PAUSED;
				break;
			case GAME_STATE_PAUSED:
				ComputeMouse(GAME_MENU_PAUSE);
				break;
			case GAME_STATE_CONTINUING:
				sounds->setVolume(GAME_SOUND_CHANNEL_MUSIC, game_volume.music);
				sounds->playSound(GAME_SOUND_PAUSE);
				DisableOptions();
				Delete2DObject(press_button_object);
				press_button_object = INVALID_OBJECT_ID;
				menuDat.show_menu = GAME_MENU_NONE;
				menuDat.pause_hover = GAME_MENU_ITEM_PAUSE_NOTHING;
				Sleep(500);
				game_state = GAME_STATE_PLAYING;
				break;
			case GAME_STATE_RESTARTING:
				sounds->pauseChannel(GAME_SOUND_CHANNEL_MUSIC);
				sounds->playSound(GAME_SOUND_SWOOSH);
				ScreenFadeOut(500, true);
				DisableOptions();
				ResetPlayerStats();
				Delete2DObject(press_button_object);
				press_button_object = INVALID_OBJECT_ID;
				menuDat.show_menu = GAME_MENU_NONE;
				ClearTetrisField();
				count_till_bomb = 0;
				GenerateNextDrop();
				switch(game_mode)
				{
					case GAME_MODE_PARTY:
						sounds->playSound(GAME_MUSIC_PARTY);
						break;
					case GAME_MODE_CLASSIC:
						sounds->playSound(GAME_MUSIC_CLASSIC);
						break;
				}
				ScreenFadeIn(500, true);
				Sleep(500);
				Delete2DObject(press_button_object);
				press_button_object = Create2DObject(GAME_TEXTURE_READY, 0.0, 0.0, 0.0, 1.0, 0.5, 0.5, 0.25, 0.0);
				Object2DFadeIn(press_button_object, 166, true);
				sounds->playSound(GAME_SOUND_READY);
				for(i = 0; i < 165; i++)
				{
					object_2d[press_button_object].size[OBJECT_X] += 0.002;
					object_2d[press_button_object].size[OBJECT_Y] += 0.001;
					for(j = 0; j < 2; j++) object_2d[press_button_object].center[j] = object_2d[press_button_object].size[j]/2;
					Sleep(8);
				}
				Object2DFadeOut(press_button_object, 500, false);
				for(i = 0; i < 60; i++)
				{
					object_2d[press_button_object].size[OBJECT_X] += 0.002;
					object_2d[press_button_object].size[OBJECT_Y] += 0.001;
					for(j = 0; j < 2; j++) object_2d[press_button_object].center[j] = object_2d[press_button_object].size[j]/2;
					Sleep(8);
				}
				Delete2DObject(press_button_object);
				press_button_object = Create2DObject(GAME_TEXTURE_GO, 0.0, 0.0, 0.0, 1.0, 0.5, 0.5, 0.25, 0.0);
				Object2DFadeIn(press_button_object, 250, false);
				sounds->playSound(GAME_SOUND_GO);
				for(i = 0; i < 30; i++)
				{
					object_2d[press_button_object].size[OBJECT_X] += 0.002;
					object_2d[press_button_object].size[OBJECT_Y] += 0.001;
					for(j = 0; j < 2; j++) object_2d[press_button_object].center[j] = object_2d[press_button_object].size[j]/2;
					Sleep(8);
				}
				Object2DFadeOut(press_button_object, 250, false);
				for(i = 0; i < 30; i++)
				{
					object_2d[press_button_object].size[OBJECT_X] += 0.002;
					object_2d[press_button_object].size[OBJECT_Y] += 0.001;
					for(j = 0; j < 2; j++) object_2d[press_button_object].center[j] = object_2d[press_button_object].size[j]/2;
					Sleep(8);
				}
				Delete2DObject(press_button_object);
				press_button_object = INVALID_OBJECT_ID;
				DropNextBlock();
				game_state = GAME_STATE_PLAYING;
				break;
			case GAME_STATE_DEATH:
				start_game = tetris_request.start;
				if(start_game != tetris_request.current_start)
				{
					sounds->playSound(GAME_SOUND_SWOOSH);
					ScreenFadeOut(1000, true);
					Delete2DObject(game_over_object);
					game_over_object = INVALID_OBJECT_ID;
					ClearTetrisField();
					memset(&tetris_request, 0, sizeof(tetris_request));
					SetBackground(GAME_TEXTURE_MAIN_MENU);
					menuDat.show_stats = false;
					for(i = 0; i < 2; i++)
					{
						Delete2DObject(stats_object[i]);
						stats_object[i] = INVALID_OBJECT_ID;
					}
					menuDat.show_menu = GAME_MENU_MAIN;
					sounds->playSound(GAME_MUSIC_INTRO);
					MenuFadeIn(GAME_MENU_MAIN, 2000, false);
					ScreenFadeIn(500, true);
					count_till_bomb = 0;
					game_state = GAME_STATE_IN_MENU;
				}
				break;
			case GAME_STATE_LEAVING:
				sounds->pauseChannel(GAME_SOUND_CHANNEL_MUSIC);
				sounds->playSound(GAME_SOUND_MENU_SELECT);
				ScreenFadeOut(1000, true);
				DisableOptions();
				Delete2DObject(press_button_object);
				press_button_object = INVALID_OBJECT_ID;
				menuDat.show_stats = false;
				for(i = 0; i < 2; i++)
				{
					Delete2DObject(stats_object[i]);
					stats_object[i] = INVALID_OBJECT_ID;
				}
				menuDat.show_menu = GAME_MENU_MAIN;
				ClearTetrisField();
				ResetPlayerStats();
				SetBackground(GAME_TEXTURE_MAIN_MENU);
				MenuFadeIn(GAME_MENU_MAIN, 2000, false);
				sounds->playSound(GAME_MUSIC_INTRO);
				ScreenFadeIn(1000, true);
				count_till_bomb = 0;
				game_state = GAME_STATE_IN_MENU;
				break;
			case GAME_STATE_EXIT:
				sounds->playSound(GAME_SOUND_MENU_SELECT);
				ScreenFadeOut(500, true);
				exit(0);
				break;
		}
		sounds->updateSound();
		while(gametick+8 >= GetTickCount()){}
		gametick = GetTickCount();
	}
	return 0;
}