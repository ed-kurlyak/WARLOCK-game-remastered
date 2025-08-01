#include "text.h"

unsigned char* t_video_buffer;

unsigned char *Res = NULL;

float g_xl, g_tul, g_tvl;
float g_xr, g_tur, g_tvr;
float g_dxdyl, g_dtudyl, g_dtvdyl;
float g_dxdyr, g_dtudyr, g_dtvdyr;

UINT g_TexWidth = 255;
UINT g_TexHeight = 255;

void Load_Font_BMP8()
{

	FILE* fp = NULL;
	fopen_s(&fp, "FONT.BMP", "rb");
	if (fp == NULL) printf("Error Open File");

	// read the BMP file header (14 bytes)
	BITMAPFILEHEADER bfh;
	fread(&bfh, sizeof(BITMAPFILEHEADER), 1, fp);

	// read the BMP info header (40 bytes)
	BITMAPINFOHEADER bih;
	fread(&bih, sizeof(BITMAPINFOHEADER), 1, fp);

	// each palette entry takes 4 bytes: 3 bytes for R, G, B and 1 reserved byte
	RGBQUAD RgbPal[256];
	fread(&RgbPal, 256 * sizeof(RGBQUAD), 1, fp);

	// move the file pointer to the start of pixel data
	fseek(fp, bfh.bfOffBits, SEEK_SET);

	// allocate memory for the image pixel indices
	unsigned char* ResTemp = new unsigned char[bih.biHeight * bih.biWidth];

	// read the image's pixel index data from file
	fread(ResTemp, bih.biHeight * bih.biWidth, 1, fp);

	// texture loaded, close the file
	fclose(fp);

	g_TexWidth = bih.biWidth;
	g_TexHeight = bih.biHeight;

	// pointer to the array of bytes read from the BMP file
	Res = new unsigned char[bih.biHeight * bih.biWidth];

	for (int h = 0; h < bih.biHeight; h++)
	{
		for (int w = 0; w < bih.biWidth; w++)
		{
			int IndexFrom = (h * bih.biWidth + w);
			int IndexTo = ((bih.biHeight - 1 - h) * bih.biWidth + w);

			// flip the BMP image vertically
			Res[IndexTo] = ResTemp[IndexFrom];
		}
	}
}

void Draw_Affine_Textured_Poly(int y1, int y2)
{
	for ( int y = y1; y < y2; y++ )
	{
		float itu, itv;
		float dtu, dtv;

		int dx = int(g_xr - g_xl);

		if(dx > 0)
		{
			dtu = (g_tur - g_tul) / (g_xr - g_xl);
			dtv = (g_tvr - g_tvl) / (g_xr - g_xl);
		}
		else
		{
			dtu = 0.0f;
			dtv = 0.0f;
		}

		int xln = (int)g_xl;

		float dxt = 1 - (g_xl - xln);

		itu = g_tul + dxt * dtu;
		itv = g_tvl + dxt * dtv;
		
		for ( int x = (int)g_xl; x < (int)g_xr; x++ )
		{
			float tu = itu;
			float tv = itv;

			int t = (int)tu + (int)tv * g_TexWidth;
			
			unsigned char Color = Res[t];

			unsigned int Index = Screen_Width * y + x;

			// replace color in palette
			if (Color == 255)
			{
				Color = 10;
				t_video_buffer[Index] = Color;
			}

			itu += dtu;
			itv += dtv;
		}

		g_xl += g_dxdyl;
		g_tul += g_dtudyl;
		g_tvl += g_dtvdyl;

		g_xr += g_dxdyr;
		g_tur += g_dtudyr;
		g_tvr += g_dtvdyr;
	}
}

void Draw_Affine_Textured_Triangle(vector3 v1, tex_coord t1,
							vector3 v2, tex_coord t2,
							vector3 v3, tex_coord t3)
{
	int Side = 0;
	float TempFloat;
	int TempInt;

	float x1 = v1.x;
	float y1 = v1.y;
	float tu1 = t1.tu * (g_TexWidth - 1);
	float tv1 = t1.tv * (g_TexHeight - 1);

	float x2 = v2.x;
	float y2 = v2.y;
	float tu2 = t2.tu * (g_TexWidth - 1);
	float tv2 = t2.tv * (g_TexHeight - 1);

	float x3 = v3.x;
	float y3 = v3.y;
	float tu3 = t3.tu * (g_TexWidth - 1);
	float tv3 = t3.tv * (g_TexHeight - 1);

	int y1i = (int) y1;
	int y2i = (int) y2;
	int y3i = (int) y3;

	if(y2i < y1i)
	{
		SWAP(x2, x1, TempFloat);
		SWAP(y2, y1, TempFloat);

		SWAP(tu2, tu1, TempFloat);
		SWAP(tv2, tv1, TempFloat);

		SWAP(y2i, y1i, TempInt);
	}

	if(y3i < y1i)
	{
		SWAP(x3, x1, TempFloat);
		SWAP(y3, y1, TempFloat);

		SWAP(tu3, tu1, TempFloat);
		SWAP(tv3, tv1, TempFloat);

		SWAP(y3i, y1i, TempInt);

	}

	if(y3i < y2i)
	{
		SWAP(x3, x2, TempFloat);
		SWAP(y3, y2, TempFloat);

		SWAP(tu3, tu2, TempFloat);
		SWAP(tv3, tv2, TempFloat);

		SWAP(y3i, y2i, TempInt);

	}

	if( y1i < y2i && y2i < y3i )
	{
		float dxdy1 = (x2 - x1) / (y2 - y1);
		float dxdy2 = (x3 - x1) / (y3 - y1);
		Side = dxdy2 > dxdy1;
	}

	if(y1i == y2i)
		Side = x1 > x2;

	if(y2i == y3i)
		Side = x3 > x2;

	if(!Side)
	{

		g_dxdyl = (x3 - x1) / (y3 - y1);
		g_dtudyl = (tu3 - tu1) / (y3 - y1);
		g_dtvdyl = (tv3 - tv1) / (y3 - y1);

		g_xl = x1;
		g_tul = tu1;
		g_tvl = tv1;
		
		if(y1i < y2i)
		{
			g_dxdyr = (x2 - x1) / (y2 - y1);
			g_dtudyr = (tu2 - tu1) / (y2 - y1);
			g_dtvdyr = (tv2 - tv1) / (y2 - y1);

			g_xr = x1;
			g_tur = tu1;
			g_tvr = tv1;


			Draw_Affine_Textured_Poly(y1i, y2i);
		}
		
		if(y2i < y3i)
		{
			g_dxdyr = (x3 - x2) / (y3 - y2);
			g_dtudyr = (tu3 - tu2) / (y3 - y2);
			g_dtvdyr = (tv3 - tv2) / (y3 - y2);

			g_xr = x2;
			g_tur = tu2;
			g_tvr = tv2;

			Draw_Affine_Textured_Poly(y2i, y3i);
		}
	}
	else
	{
		g_dxdyr = (x3 - x1) / (y3 - y1);
		g_dtudyr = (tu3 - tu1) / (y3 - y1);
		g_dtvdyr = (tv3 - tv1) / (y3 - y1);

		g_xr = x1;
		g_tur = tu1;
		g_tvr = tv1;

		if(y1i < y2i)
		{
			g_dxdyl = (x2 - x1) / (y2 - y1);
			g_dtudyl = (tu2 - tu1) / (y2 - y1);
			g_dtvdyl = (tv2 - tv1) / (y2 - y1);

			g_xl = x1;
			g_tul = tu1;
			g_tvl = tv1;

			Draw_Affine_Textured_Poly(y1i, y2i);
		}
		
		if(y2i < y3i)
		{
			g_dxdyl = (x3 - x2) / (y3 - y2);
			g_dtudyl = (tu3 - tu2) / (y3 - y2);
			g_dtvdyl = (tv3 - tv2) / (y3 - y2);
			
			g_xl = x2;
			g_tul = tu2;
			g_tvl = tv2;

			Draw_Affine_Textured_Poly(y2i, y3i);
		}
	}
}

void Blit_String(char * Text, float x, float y, float SizeX, float SizeY)
{
	
	UINT Length = (UINT)strlen(Text);

	//каждая буква два треугольника 6 вершин
	UINT BuffSize = Length * 6;

	vector3 * VertBuff = new vector3[BuffSize];
	tex_coord * TexBuff = new tex_coord[BuffSize];

	UINT Index = 0;

	for ( unsigned int i=0 ; i<Length ; i++ )
	{
		vector3 VertexUpLeft    = { x+i*SizeX,			y,			0};
		vector3 VertexUpRight   = { x+i*SizeX+SizeX,	y,			0};
		vector3 VertexDownRight = { x+i*SizeX+SizeX,	y+SizeY,	0};
		vector3 VertexDownLeft  = { x+i*SizeX,			y+SizeY,	0};

		VertBuff[Index + 0] = VertexDownLeft;
		VertBuff[Index + 1] = VertexUpLeft;
		VertBuff[Index + 2] = VertexUpRight;

		VertBuff[Index + 3] = VertexDownLeft;
		VertBuff[Index + 4] = VertexUpRight;
		VertBuff[Index + 5] = VertexDownRight;
		
		char character = Text[i];
		float uv_x = (character%16)/16.0f;
		float uv_y = (character/16)/16.0f;

		//отступ перед буквой в клетке 3 пикселя
		float Tw1 = 3.0f / g_TexWidth;
		//ширина буквы 9 пикселей
		float Tw2 = 9.0f / g_TexWidth;
		//отступ по высоте на 1 пиксель (убираем искажения текстуры)
		float TwY = 1.0f / g_TexHeight;

		//ширина клетки 16 пикселей

		/*
		//код без учета начала и ширины символа
		tex_coord UvUpLeft  = { uv_x           , uv_y };
		tex_coord UvUpRight = { uv_x+1.0f/16.0f, uv_y };
		tex_coord UvDownRight = { uv_x+1.0f/16.0f, (uv_y + 1.0f/16.0f) };
		tex_coord UvDownLeft  = { uv_x           , (uv_y + 1.0f/16.0f) };
		*/

		//код учитывает начало и ширину символа
		tex_coord UvUpLeft    =	{ uv_x + Tw1,		uv_y + TwY };
		tex_coord UvUpRight	  =	{ uv_x + Tw1 + Tw2,	uv_y + TwY };
		tex_coord UvDownRight =	{ uv_x + Tw1 + Tw2,	uv_y + 1.0f/16.0f};
		tex_coord UvDownLeft  =	{ uv_x + Tw1,		uv_y + 1.0f/16.0f };

		TexBuff[Index + 0] = UvDownLeft;
		TexBuff[Index + 1] = UvUpLeft;
		TexBuff[Index + 2] = UvUpRight;

		TexBuff[Index + 3] = UvDownLeft;
		TexBuff[Index + 4] = UvUpRight;
		TexBuff[Index + 5] = UvDownRight;

		Index += 6;

	}

	//каждая буква 2 треугольника
	UINT TriangleCount = Length * 2;

	for ( UINT i = 0; i < TriangleCount; i++ )
	{
		vector3 v1 = VertBuff[i * 3 + 0];
		vector3 v2 = VertBuff[i * 3 + 1];
		vector3 v3 = VertBuff[i * 3 + 2];

		tex_coord t1 = TexBuff[i * 3 + 0];
		tex_coord t2 = TexBuff[i * 3 + 1];
		tex_coord t3 = TexBuff[i * 3 + 2];

		Draw_Affine_Textured_Triangle(v1, t1, v2, t2, v3, t3);
	}

	delete [] VertBuff;
	delete [] TexBuff;
}

void Delete_Font()
{
		delete [] Res;
}