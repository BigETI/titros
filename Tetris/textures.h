#ifndef _TEXTURES_H_INCLUDED_
	#define _TEXTURES_H_INCLUDED_
	#include <windows.h>
	#include <stdio.h>
	#include <gl\GL.h>

struct Texture
{
	GLubyte *imageData;
	GLuint bpp;
	GLuint width;
	GLuint height;
	GLuint texID;
	GLuint type;
};	

struct TGAHeader
{
	GLubyte Header[12];
};

struct TGA
{
	GLubyte		header[6];								// First 6 Useful Bytes From The Header
	GLuint		bytesPerPixel;							// Holds Number Of Bytes Per Pixel Used In The TGA File
	GLuint		imageSize;								// Used To Store The Image Size When Setting Aside Ram
	GLuint		temp;									// Temporary Variable
	GLuint		type;	
	GLuint		Height;									//Height of Image
	GLuint		Width;									//Width ofImage
	GLuint		Bpp;									// Bits Per Pixel
};

struct TEXTURE
{
	bool is;
	Texture texDat;
};

enum GAME_TEXTURE_ENUM
{
	GAME_TEXTURE_LOAD,
	GAME_TEXTURE_BLACK,
	GAME_TEXTURE_FMOD,
	GAME_TEXTURE_OPENGL,
	GAME_TEXTURE_TITLE,
	GAME_TEXTURE_BUTTON_START,
	GAME_TEXTURE_MAIN_MENU,
	GAME_TEXTURE_PARTY_BUTTON,
	GAME_TEXTURE_CLASSIC_BUTTON,
	GAME_TEXTURE_OPTIONS_BUTTON,
	GAME_TEXTURE_OPTIONS,
	GAME_TEXTURE_VOLUME,
	GAME_TEXTURE_RESOLUTION,
	GAME_TEXTURE_CREDITS_BUTTON,
	GAME_TEXTURE_CREDITS,
	GAME_TEXTURE_EXIT_BUTTON,
	GAME_TEXTURE_FIELD,
	GAME_TEXTURE_READY,
	GAME_TEXTURE_GO,
	GAME_TEXTURE_PAUSE,
	GAME_TEXTURE_CONTINUE_BUTTON,
	GAME_TEXTURE_RESTART_BUTTON,
	GAME_TEXTURE_GAME_OVER,
	GAME_TEXTURE_ENTITIES,
	GAME_TEXTURE_SCORE,
	GAME_TEXTURE_LEVEL,
	GAME_TEXTURE_NUMBERS,
	NUM_GAME_TEXTURES
};

struct onLoad
{
	bool smooth, is, finished;
	char *file_name;
	GAME_TEXTURE_ENUM texture_id;
};

struct onUnload
{
	bool is, finished;
	GAME_TEXTURE_ENUM texture_id;
};

	#ifdef _TEXTURES_CPP_
TEXTURE objtexture[NUM_GAME_TEXTURES];
onLoad onLoadTexture;
onUnload onUnloadTexture;
	#else
extern void LoadTGA(GAME_TEXTURE_ENUM texture_id, char *file_name, bool smooth);
extern void LoadTexture(GAME_TEXTURE_ENUM texture_id, char *file_name, bool smooth);
extern void UnloadTGA(GAME_TEXTURE_ENUM texture_id);
extern void UnloadTexture(GAME_TEXTURE_ENUM texture_id);
extern TEXTURE objtexture[NUM_GAME_TEXTURES];
extern onLoad onLoadTexture;
extern onUnload onUnloadTexture;
	#endif
#endif