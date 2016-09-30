#ifndef _GAME_H_INCLUDED_
	#define _GAME_H_INCLUDED_
	#include "textures.h"
struct soundInitDef
{
	char unsigned loop;
	char *name;
	GAME_SOUND_CHANNEL_ENUM channel;
};

enum GAME_STATE_ENUM
{
	GAME_STATE_LOADING,
	GAME_STATE_TITLE_FMOD,
	GAME_STATE_TITLE_OPENGL,
	GAME_STATE_TITLE_MAIN,
	GAME_STATE_IDLE,
	GAME_STATE_IN_MENU,
	GAME_STATE_LOAD_ROUND,
	GAME_STATE_PLAYING,
	GAME_STATE_PAUSING,
	GAME_STATE_PAUSED,
	GAME_STATE_CONTINUING,
	GAME_STATE_RESTARTING,
	GAME_STATE_DEATH,
	GAME_STATE_LEAVING,
	GAME_STATE_EXIT
};

enum TETRIS_ENTITY_ENUM
{
	TETRIS_ENTITY_NOTHING,
	TETRIS_ENTITY_I,
	TETRIS_ENTITY_T,
	TETRIS_ENTITY_L,
	TETRIS_ENTITY_N_L,
	TETRIS_ENTITY_O,
	TETRIS_ENTITY_S,
	TETRIS_ENTITY_Z,
	TETRIS_ENTITY_SINGLE,
	TETRIS_ENTITY_BOMB
};

enum TETRIMINO_ENUM
{
	TETRIMINO_I,
	TETRIMINO_T,
	TETRIMINO_L,
	TETRIMINO_N_L,
	TETRIMINO_O,
	TETRIMINO_S,
	TETRIMINO_Z,
	TETRIMINO_SINGLE,
	TETRIMINO_DOUBLE,
	TETRIMINO_TILT,
	TETRIMINO_BOMB,
	NUM_TETRIMINOS
};

enum TETRIS_MOVE_ENUM
{
	TETRIS_MOVE_NO,
	TETRIS_MOVE_LEFT,
	TETRIS_MOVE_RIGHT,
	TETRIS_MOVE_DOWN_SOFT,
	TETRIS_MOVE_DOWN_HARD
};

enum TETRIS_ROTATE_ENUM
{
	TETRIS_ROTATE_NO,
	TETRIS_ROTATE_LEFT,
	TETRIS_ROTATE_RIGHT
};

enum GAME_MENU_ENUM
{
	GAME_MENU_MAIN,
	GAME_MENU_PAUSE,
	NUM_GAME_MENUS,
	GAME_MENU_NONE
};

enum GAME_MENU_ITEM_MAIN_ENUM
{
	GAME_MENU_ITEM_MAIN_PARTY,
	GAME_MENU_ITEM_MAIN_CLASSIC,
	GAME_MENU_ITEM_MAIN_OPTIONS,
	GAME_MENU_ITEM_MAIN_CREDITS,
	GAME_MENU_ITEM_MAIN_EXIT,
	NUM_GAME_MENU_ITEMS_MAIN,
	GAME_MENU_ITEM_MAIN_NOTHING
};

enum GAME_MENU_ITEM_PAUSE_ENUM
{
	GAME_MENU_ITEM_PAUSE_CONTINUE,
	GAME_MENU_ITEM_PAUSE_RESTART,
	GAME_MENU_ITEM_PAUSE_OPTIONS,
	GAME_MENU_ITEM_PAUSE_EXIT,
	NUM_GAME_MENU_ITEMS_PAUSE,
	GAME_MENU_ITEM_PAUSE_NOTHING
};

enum GAME_MODE_ENUM
{
	GAME_MODE_PARTY,
	GAME_MODE_CLASSIC
};

enum TETRIS_CLEAR_ENUM
{
	TETRIS_CLEAR_ZERO,
	TETRIS_CLEAR_SINGLE,
	TETRIS_CLEAR_DOUBLE,
	TETRIS_CLEAR_TRIPLE,
	TETRIS_CLEAR_TETRIS,
	TETRIS_CLEAR_BOMB
};

struct TETRIS_REQUEST
{
	bool start, current_start;
	TETRIS_MOVE_ENUM move, current_move;
	TETRIS_ROTATE_ENUM rotate, current_rotate;
};

struct MENU_ITEM
{
	GAME_TEXTURE_ENUM texture_id;
	double pos_x, pos_y, size_x, size_y;
};

enum SHOW_O_C_ENUM
{
	SHOW_NOTHING,
	SHOW_OPTIONS,
	SHOW_CREDITS
};

enum CHANGE_SOUND_ENUM
{
	CHANGE_SOUND_NOTHING,
	CHANGE_SOUND_MUSIC,
	CHANGE_SOUND_SOUNDS
};

struct GAME_MENU
{
	bool selected, show_stats;
	double alpha[NUM_GAME_MENUS], options_alpha;
	FADE_MODE_ENUM fade_mode[NUM_GAME_MENUS], options_fade_mode;
	unsigned int fade_time[NUM_GAME_MENUS];
	MENU_ITEM main[NUM_GAME_MENU_ITEMS_MAIN];
	MENU_ITEM pause[NUM_GAME_MENU_ITEMS_PAUSE];
	GAME_MENU_ENUM show_menu;
	GAME_MENU_ITEM_MAIN_ENUM main_hover;
	GAME_MENU_ITEM_PAUSE_ENUM pause_hover;
	SHOW_O_C_ENUM show_o_c;
	CHANGE_SOUND_ENUM allow_sound_change, apply_change_sound;
	SCREEN_OPTION_ENUM allow_change_resolution;//, apply_change_resolution;
};

struct MOUSE
{
	bool showMouse, clicked;
	unsigned char hover_menu_item_id;
	int texID[2], raw_x, raw_y;
	double pos_x, pos_y;
};

struct PLAYER_STATS
{
	bool update_level;
	unsigned int level, cleared_lines;
	unsigned __int64 score;
	char level_str[11], score_str[20];
	int tick_delay;
};

struct VOLUME_BAR
{
	double obj_size, tex_size;
};

struct VOLUME
{
	float music, sounds;
	VOLUME_BAR music_bar, sounds_bar;
};

	#ifdef _GAME_CPP_
soundInitDef sound_info[NUM_GAME_SOUNDS];
GAME_STATE_ENUM game_state = GAME_STATE_LOADING;
bool allow_title_force = false, no_title_force = true;
TETRIS_ENTITY_ENUM tetris_mydrop[5][5], tetris_nextdrop[5][5], tetris_field[20][10];
unsigned char mydrop_x, mydrop_y;
TETRIS_REQUEST tetris_request;
GAME_MENU menuDat;
MOUSE mouseDat;
GAME_MODE_ENUM game_mode;
PLAYER_STATS player_stats;
VOLUME game_volume;
	#else
extern soundInitDef sound_info[NUM_GAME_SOUNDS];
extern GAME_STATE_ENUM game_state;
extern bool allow_title_force, no_title_force;
extern TETRIS_ENTITY_ENUM tetris_mydrop[5][5], tetris_nextdrop[5][5], tetris_field[20][10];
extern unsigned char mydrop_x, mydrop_y;
extern TETRIS_REQUEST tetris_request;
extern GAME_MENU menuDat;
extern MOUSE mouseDat;
extern GAME_MODE_ENUM game_mode;
extern PLAYER_STATS player_stats;
extern VOLUME game_volume;
	#endif
#endif