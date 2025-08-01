#include <windows.h>
#include <stdio.h>
#include "backbuffer.h"

extern unsigned char* t_video_buffer;

#define SWAP(a,b,t) {t = a; a = b; b = t;}

struct vector3
{
	float x,y,z;
};

struct tex_coord
{
	float tu, tv;
};

void Blit_String(char * Text, float x, float y, float SizeX, float SizeY);
void Draw_Affine_Textured_Triangle(vector3 v1, tex_coord t1,
							vector3 v2, tex_coord t2,
							vector3 v3, tex_coord t3);
void Draw_Affine_Textured_Poly(int y1, int y2);
void Load_Font_BMP8();
void Delete_Font();

