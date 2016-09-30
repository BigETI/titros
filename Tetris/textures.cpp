#include <iostream>
#define _TEXTURES_CPP_
#include "main.h"
#include "textures.h"

TGAHeader tgaheader;
TGA tga;
GLubyte uTGAcompare[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};		// Uncompressed TGA Header
GLubyte cTGAcompare[12] = {0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0};	// Compressed TGA Header

bool LoadUncompressedTGA(Texture *texture, FILE *fTGA)
{
	if(fread(tga.header, sizeof(tga.header), 1, fTGA) == 0)
	{
		if(fTGA != NULL) fclose(fTGA);
		return false;
	}
	texture->width  = tga.header[1]*256+tga.header[0];
	texture->height = tga.header[3]*256+tga.header[2];
	texture->bpp	= tga.header[4];
	tga.Width		= texture->width;					
	tga.Height		= texture->height;
	tga.Bpp			= texture->bpp;
	if((texture->width <= 0) || (texture->height <= 0) || ((texture->bpp != 24) && (texture->bpp != 32)))
	{
		if(fTGA != NULL) fclose(fTGA);
		return false;
	}
	if(texture->bpp == 24) texture->type = GL_RGB;
	else texture->type = GL_RGBA;
	tga.bytesPerPixel = (tga.Bpp/8);
	tga.imageSize = (tga.bytesPerPixel*tga.Width*tga.Height);
	texture->imageData = (GLubyte*)malloc(tga.imageSize);
	if(texture->imageData == NULL)
	{
		fclose(fTGA);
		return false;
	}
	if(fread(texture->imageData, 1, tga.imageSize, fTGA) != tga.imageSize)
	{
		if(texture->imageData != NULL) free(texture->imageData);
		fclose(fTGA);
		return false;
	}
	for(GLuint cswap = 0; cswap < (int)tga.imageSize; cswap += tga.bytesPerPixel)
	{
		texture->imageData[cswap] ^= texture->imageData[cswap+2] ^= texture->imageData[cswap] ^= texture->imageData[cswap+2];
	}
	fclose(fTGA);
	return true;
}

bool LoadCompressedTGA(Texture *texture, FILE *fTGA)
{ 
	if(fread(tga.header, sizeof(tga.header), 1, fTGA) == 0)
	{
		if(fTGA != NULL) fclose(fTGA);
		return false;
	}
	texture->width = tga.header[1]*256+tga.header[0];
	texture->height = tga.header[3]*256+tga.header[2];
	texture->bpp = tga.header[4];
	tga.Width = texture->width;
	tga.Height = texture->height;
	tga.Bpp = texture->bpp;
	if((texture->width <= 0) || (texture->height <= 0) || ((texture->bpp != 24) && (texture->bpp != 32)))
	{
		if(fTGA != NULL) fclose(fTGA);
		return false;
	}
	if(texture->bpp == 24) texture->type = GL_RGB;
	else texture->type = GL_RGBA;
	tga.bytesPerPixel = (tga.Bpp/8);
	tga.imageSize = (tga.bytesPerPixel*tga.Width*tga.Height);
	texture->imageData = (GLubyte*)malloc(tga.imageSize);
	if(texture->imageData == NULL)
	{
		fclose(fTGA);
		return false;
	}
	GLuint pixelcount = tga.Height*tga.Width;
	GLuint currentpixel = 0;
	GLuint currentbyte = 0;
	GLubyte *colorbuffer = (GLubyte*)malloc(tga.bytesPerPixel);
	do
	{
		GLubyte chunkheader = 0;
		if(fread(&chunkheader, sizeof(GLubyte), 1, fTGA) == 0)
		{
			if(fTGA != NULL) fclose(fTGA);
			if(texture->imageData != NULL) free(texture->imageData);
			return false;
		}
		if(chunkheader < 128)
		{
			chunkheader++;
			for(short counter = 0; counter < chunkheader; counter++)
			{
				if(fread(colorbuffer, 1, tga.bytesPerPixel, fTGA) != tga.bytesPerPixel)
				{
					if(fTGA != NULL) fclose(fTGA);
					if(colorbuffer != NULL) free(colorbuffer);
					if(texture->imageData != NULL) free(texture->imageData);
					return false;
				}
				texture->imageData[currentbyte] = colorbuffer[2];
				texture->imageData[currentbyte+1] = colorbuffer[1];
				texture->imageData[currentbyte+2] = colorbuffer[0];
				if(tga.bytesPerPixel == 4) texture->imageData[currentbyte+3] = colorbuffer[3];
				currentbyte += tga.bytesPerPixel;
				currentpixel++;
				if(currentpixel > pixelcount)
				{
					if(fTGA != NULL) fclose(fTGA);
					if(colorbuffer != NULL) free(colorbuffer);
					if(texture->imageData != NULL) free(texture->imageData);
					return false;
				}
			}
		}
		else
		{
			chunkheader -= 127;
			if(fread(colorbuffer, 1, tga.bytesPerPixel, fTGA) != tga.bytesPerPixel)
			{
				if(fTGA != NULL) fclose(fTGA);
				if(colorbuffer != NULL) free(colorbuffer);
				if(texture->imageData != NULL) free(texture->imageData);
				return false;
			}
			for(short counter = 0; counter < chunkheader; counter++)
			{
				texture->imageData[currentbyte] = colorbuffer[2];
				texture->imageData[currentbyte+1] = colorbuffer[1];
				texture->imageData[currentbyte+2] = colorbuffer[0];
				if(tga.bytesPerPixel == 4) texture->imageData[currentbyte+3] = colorbuffer[3];
				currentbyte += tga.bytesPerPixel;
				currentpixel++;
				if(currentpixel > pixelcount)
				{
					if(fTGA != NULL) fclose(fTGA);
					if(colorbuffer != NULL) free(colorbuffer);
					if(texture->imageData != NULL) free(texture->imageData);
					return false;
				}
			}
		}
	}
	while(currentpixel < pixelcount);
	fclose(fTGA);
	return true;
}

void LoadTGA(GAME_TEXTURE_ENUM texture_id, char *file_name, bool smooth)
{
	char errormsg[128];
	if(objtexture[texture_id].is) return;
	FILE *fTGA;
	fopen_s(&fTGA, file_name, "rb");
	if(fTGA == NULL)
	{
		LogError(errormsg);
		exit(0);
		return;
	}
	if(fread(&tgaheader, sizeof(TGAHeader), 1, fTGA) == 0)
	{
		if(fTGA != NULL) fclose(fTGA);
		return;
	}
	if(memcmp(uTGAcompare, &tgaheader, sizeof(tgaheader)) == 0)
	{
		if(LoadUncompressedTGA(&objtexture[texture_id].texDat, fTGA))
		{
			glGenTextures(1, &objtexture[texture_id].texDat.texID);
			glBindTexture(GL_TEXTURE_2D, objtexture[texture_id].texDat.texID);
			glTexImage2D(GL_TEXTURE_2D, 0, objtexture[texture_id].texDat.bpp/8, objtexture[texture_id].texDat.width, objtexture[texture_id].texDat.height, 0, objtexture[texture_id].texDat.type, GL_UNSIGNED_BYTE, objtexture[texture_id].texDat.imageData);
			if(smooth)
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
			else
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			}
			glEnable(GL_TEXTURE_2D);
			if(objtexture[texture_id].texDat.imageData) free(objtexture[texture_id].texDat.imageData);
			objtexture[texture_id].is = true;
			return;
		}
		else
		{
			LogError(errormsg);
			exit(0);
			return;
		}
	}
	else if(memcmp(cTGAcompare, &tgaheader, sizeof(tgaheader)) == 0)
	{
		if(LoadCompressedTGA(&objtexture[texture_id].texDat, fTGA))
		{
			glGenTextures(1, &objtexture[texture_id].texDat.texID);
			glBindTexture(GL_TEXTURE_2D, objtexture[texture_id].texDat.texID);
			glTexImage2D(GL_TEXTURE_2D, 0, objtexture[texture_id].texDat.bpp/8, objtexture[texture_id].texDat.width, objtexture[texture_id].texDat.height, 0, objtexture[texture_id].texDat.type, GL_UNSIGNED_BYTE, objtexture[texture_id].texDat.imageData);
			if(smooth)
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
			else
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			}
			glEnable(GL_TEXTURE_2D);
			if(objtexture[texture_id].texDat.imageData) free(objtexture[texture_id].texDat.imageData);
			objtexture[texture_id].is = true;
			return;
		}
		else
		{
			LogError(errormsg);
			exit(0);
			return;
		}
	}
	fclose(fTGA);
	LogError(errormsg);
	exit(0);
}

void LoadTexture(GAME_TEXTURE_ENUM texture_id, char *file_name, bool smooth)
{
	if(onLoadTexture.is) return;
	onLoadTexture.texture_id = texture_id;
	onLoadTexture.file_name = file_name;
	onLoadTexture.smooth = smooth;
	onLoadTexture.is = true;
	while(onLoadTexture.is){}
}

void UnloadTGA(GAME_TEXTURE_ENUM texture_id)
{
	if(!objtexture[texture_id].is) return;
	objtexture[texture_id].is = false;
	glDeleteTextures(1, &objtexture[texture_id].texDat.texID);
}

void UnloadTexture(GAME_TEXTURE_ENUM texture_id)
{
	if(onUnloadTexture.is) return;
	onUnloadTexture.texture_id = texture_id;
	onUnloadTexture.is = true;
	while(onUnloadTexture.is){}
}