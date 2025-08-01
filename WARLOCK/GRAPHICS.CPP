
// I N C L U D E S ///////////////////////////////////////////////////////////


#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <memory.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include "graphics.h"

unsigned char* g_video_buffer;

// G L O B A L S  ////////////////////////////////////////////////////////////

unsigned int sprite_width = SPRITE_WIDTH; // default height and width of sprite
unsigned int sprite_height = SPRITE_HEIGHT;

// F U N C T I O N S /////////////////////////////////////////////////////////


void Blit_Char(int xc,int yc,char c,int color,int trans_flag)
{
	

} // end Blit_Char

//////////////////////////////////////////////////////////////////////////////

void Blit_String(int x,int y,int color, char *string,int trans_flag)
{

} /* end Blit_String */

//////////////////////////////////////////////////////////////////////////////

void Delay(int t)
{

} // end Delay

//////////////////////////////////////////////////////////////////////////////

void Set_Palette_Register(int index, RGB_color_ptr color)
{

} // end Set_Palette_Color

///////////////////////////////////////////////////////////////////////////////

void Get_Palette_Register(int index, RGB_color_ptr color)
{
} // end Get_Palette_Color

//////////////////////////////////////////////////////////////////////////////

void PCX_Init(pcx_picture_ptr image, short int width, short int height)
{
// this function allocates the buffer region needed to load a pcx file

if (!(image->buffer = (char *)malloc(width * height + 1)))

   printf("\ncouldn't allocate screen buffer");

} // end PCX_Init

//////////////////////////////////////////////////////////////////////////////

void Plot_Pixel_Fast(int x,int y,unsigned char color)
{

// plots the pixel in the desired color a little quicker using binary shifting
// to accomplish the multiplications

	int index = Screen_Width * y + x;

	g_video_buffer[index] = color;

} // end Plot_Pixel_Fast

//////////////////////////////////////////////////////////////////////////////

void Plot_Pixel_Fast_D(int x,int y,unsigned char color)
{

// plots the pixel in the desired color a little quicker using binary shifting
// to accomplish the multiplications

} // end Plot_Pixel_Fast_D

//////////////////////////////////////////////////////////////////////////////

unsigned char Get_Pixel(int x,int y)
{

// gets the pixel from the screen

	unsigned int Index = y * Screen_Width + x;

	return g_video_buffer[Index];

} // end Get_Pixel

//////////////////////////////////////////////////////////////////////////////

unsigned char Get_Pixel_D(int x,int y)
{

// gets the pixel from the screen

	return 0;

} // end Get_Pixel_D

//////////////////////////////////////////////////////////////////////////////

void PCX_Delete(pcx_picture_ptr image)
{
// this function de-allocates the buffer region used for the pcx file load

free(image->buffer);

} // end PCX_Delete

//////////////////////////////////////////////////////////////////////////////

void PCX_Load(char *filename, pcx_picture_ptr image, short int enable_palette,
			  short int width, short int height)
{
// this function loads a pcx file into a picture structure, the actual image
// data for the pcx file is decompressed and expanded into a secondary buffer
// within the picture structure, the separate images can be grabbed from this
// buffer later.  also the header and palette are loaded

//FILE *fp, *fopen();
FILE *fp;
int num_bytes,index;
long count;
unsigned char data;
char *temp_buffer;

// open the file

fp = fopen(filename,"rb");

// load the header

temp_buffer = (char *)image;

for (index=0; index<128; index++)
    {
    temp_buffer[index] = getc(fp);
    } // end for index

// load the data and decompress into buffer

count=0;

while(count<=width * height)
     {
     // get the first piece of data

     data = getc(fp);

     // is this a rle?

     if (data>=192 && data<=255)
        {
        // how many bytes in run?

        num_bytes = data-192;

        // get the actual data for the run

        data  = getc(fp);

        // replicate data in buffer num_bytes times

        while(num_bytes-->0)
             {
             image->buffer[count++] = data;

             } // end while

        } // end if rle
     else
        {
        // actual data, just copy it into buffer at next location

        image->buffer[count++] = data;

        } // end else not rle

     } // end while

// move to end of file then back up 768 bytes i.e. to begining of palette

fseek(fp,-768L,SEEK_END);

// load the pallete into the palette

for (index=0; index<256; index++)
    {
    // get the red component

    image->palette[index].red   = (getc(fp));

    // get the green component

    image->palette[index].green = (getc(fp));

    // get the blue component

    image->palette[index].blue  = (getc(fp));

    } // end for index

fclose(fp);

// change the palette to newly loaded palette if commanded to do so

if (enable_palette)
   {

	   for (index=0; index<256; index++)
       {

		   GamePalette[index].r = image->palette[index].red;
		   GamePalette[index].g = image->palette[index].green;
		   GamePalette[index].b = image->palette[index].blue;

       } // end for index

	   Create_Palette();

   } // end if change palette

} // end PCX_Load

//////////////////////////////////////////////////////////////////////////////

void PCX_Show_Buffer(pcx_picture_ptr image)
{
// just copy he pcx buffer into the video buffer

char *data;

data = image->buffer;

memcpy(g_video_buffer, image->buffer, Screen_Width * Screen_Height); 

} // end PCX_Show_Picture

//////////////////////////////////////////////////////////////////////////////

void Show_Double_Buffer(char *buffer)
{

} // end Show_video_buffer

//////////////////////////////////////////////////////////////////////////////

void Init_Double_Buffer(void)
{

} // end Init_Double_Buffer

//////////////////////////////////////////////////////////////////////////////


void Sprite_Init(sprite_ptr sprite,int x,int y,int ac,int as,int mc,int ms)
{
// this function initializes a sprite with the sent data

int index;

sprite->x            = x;
sprite->y            = y;
sprite->x_old        = x;
sprite->y_old        = y;
sprite->width        = sprite_width;
sprite->height       = sprite_height;
sprite->anim_clock   = ac;
sprite->anim_speed   = as;
sprite->motion_clock = mc;
sprite->motion_speed = ms;
sprite->curr_frame   = 0;
sprite->state        = SPRITE_DEAD;
sprite->num_frames   = 0;
sprite->background   = (char *)malloc(sprite_width * sprite_height+1);

// set all bitmap pointers to null

for (index=0; index<MAX_SPRITE_FRAMES; index++)
    sprite->frames[index] = NULL;

} // end Sprite_Init

//////////////////////////////////////////////////////////////////////////////

void Sprite_Delete(sprite_ptr sprite)
{
// this function deletes all the memory associated with a sprire

int index;

free(sprite->background);

// now de-allocate all the animation frames

for (index=0; index<MAX_SPRITE_FRAMES; index++)
    free(sprite->frames[index]);

} // end Sprite_Delete


//////////////////////////////////////////////////////////////////////////////

void PCX_Grap_Bitmap(pcx_picture_ptr image,
                    sprite_ptr sprite,
                    int sprite_frame,
                    int grab_x, int grab_y)

{
// this function will grap a bitmap from the pcx frame buffer. it uses the
// convention that the 320x200 pixel matrix is sub divided into a smaller
// matrix of nxn adjacent squares

//unsigned int x_off,y_off, x,y, index;
unsigned int x_off,y_off, x,y;
char *sprite_data;

// first allocate the memory for the sprite in the sprite structure

sprite->frames[sprite_frame] = (char *)malloc(sprite_width * sprite_height + 1);

// create an alias to the sprite frame for ease of access

sprite_data = sprite->frames[sprite_frame];

// now load the sprite data into the sprite frame array from the pcx picture

x_off = (sprite_width+1)  * grab_x + 1;
y_off = (sprite_height+1) * grab_y + 1;

// compute starting y address

y_off = y_off * 320;

for (y=0; y<sprite_height; y++)
    {

    for (x=0; x<sprite_width; x++)
        {

        // get the next byte of current row and place into next position in
        // sprite frame data buffer

        sprite_data[y*sprite_width + x] = image->buffer[y_off + x_off + x];

        } // end for x

        // move to next line of picture buffer

        y_off+=320;

    } // end for y

// increment number of frames

sprite->num_frames++;

// done!, let's bail!

} // end PCX_Grap_Bitmap

//////////////////////////////////////////////////////////////////////////////

void Behind_Sprite(sprite_ptr sprite)
{

// this function scans the background behind a sprite so that when the sprite
// is draw, the background isnn'y obliterated

char *work_back;
unsigned short int work_offset=0,offset,y;

// alias a pointer to sprite background for ease of access

work_back = sprite->background;

// compute offset of background in video buffer

offset = (sprite->y << 8) + (sprite->y << 6) + sprite->x;

for (y=0; y<sprite_height; y++)
    {
    // copy the next row out off screen buffer into sprite background buffer

    memcpy((char *)&work_back[work_offset],
             (char *)&g_video_buffer[offset],
             sprite_width);

    // move to next line in video buffer and in sprite background buffer

    offset      += Screen_Width;
    work_offset += sprite_width;

    } // end for y

} // end Behind_Sprite

//////////////////////////////////////////////////////////////////////////////

void Erase_Sprite(sprite_ptr sprite)
{
// replace the background that was behind the sprite

// this function replaces the background that was saved from where a sprite
// was going to be placed

char *work_back;
unsigned short int work_offset=0,offset,y;

// alias a pointer to sprite background for ease of access

work_back = sprite->background;

// compute offset of background in video buffer

offset = (sprite->y << 8) + (sprite->y << 6) + sprite->x;

for (y=0; y<sprite_height; y++)
    {
    // copy the next row out off screen buffer into sprite background buffer

    memcpy((char *)&g_video_buffer[offset],
             (char *)&work_back[work_offset],
             sprite_width);

    // move to next line in video buffer and in sprite background buffer

    offset      += Screen_Width;
    work_offset += sprite_width;

    } // end for y


} // end Erase_Sprite

//////////////////////////////////////////////////////////////////////////////

void Draw_Sprite(sprite_ptr sprite)
{

// this function draws a sprite on the screen row by row very quickly
// note the use of shifting to implement multplication

char *work_sprite;
unsigned short int work_offset=0,offset,x,y;
unsigned char data;

// alias a pointer to sprite for ease of access

work_sprite = sprite->frames[sprite->curr_frame];

// compute offset of sprite in video buffer

offset = (sprite->y << 8) + (sprite->y << 6) + sprite->x;

for (y=0; y<sprite_height; y++)
    {
    // copy the next row into the screen buffer using memcpy for speed

    for (x=0; x<sprite_width; x++)
        {

        // test for transparent pixel i.e. 0, if not transparent then draw

        if ((data=work_sprite[work_offset+x]))
             g_video_buffer[offset+x] = data;

        } // end for x

    // move to next line in video buffer and in sprite bitmap buffer

    offset      += Screen_Width;
    work_offset += sprite_width;

    } // end for y

} // end Draw_Sprite

//////////////////////////////////////////////////////////////////////////////

void Behind_Sprite_VB(sprite_ptr sprite)
{

// this function scans the background behind a sprite so that when the sprite
// is draw, the background isnn'y obliterated

char *work_back;
unsigned short int work_offset=0,offset,y;

// alias a pointer to sprite background for ease of access

work_back = sprite->background;

// compute offset of background in video buffer

offset = (sprite->y << 8) + (sprite->y << 6) + sprite->x;

for (y=0; y<sprite_height; y++)
    {
    // copy the next row out off screen buffer into sprite background buffer

    memcpy((char *)&work_back[work_offset],
             (char *)&g_video_buffer[offset],
             sprite_width);

    // move to next line in video buffer and in sprite background buffer

    offset      += Screen_Width;
    work_offset += sprite_width;

    } // end for y

} // end Behind_Sprite_VB

//////////////////////////////////////////////////////////////////////////////

void Erase_Sprite_VB(sprite_ptr sprite)
{
// replace the background that was behind the sprite

// this function replaces the background that was saved from where a sprite
// was going to be placed

char *work_back;
unsigned short int work_offset=0,offset,y;

// alias a pointer to sprite background for ease of access

work_back = sprite->background;

// compute offset of background in video buffer

offset = (sprite->y << 8) + (sprite->y << 6) + sprite->x;

for (y=0; y<sprite_height; y++)
    {
    // copy the next row out off screen buffer into sprite background buffer

    memcpy((char *)&g_video_buffer[offset],
             (char *)&work_back[work_offset],
             sprite_width);

    // move to next line in video buffer and in sprite background buffer

    offset      += Screen_Width;
    work_offset += sprite_width;

    } // end for y


} // end Erase_Sprite_VB

//////////////////////////////////////////////////////////////////////////////

void Draw_Sprite_VB(sprite_ptr sprite)
{

// this function draws a sprite on the screen row by row very quickly
// note the use of shifting to implement multplication

char *work_sprite;
unsigned short int work_offset=0,offset,x,y;
unsigned char data;

// alias a pointer to sprite for ease of access

work_sprite = sprite->frames[sprite->curr_frame];

// compute offset of sprite in video buffer

offset = (sprite->y << 8) + (sprite->y << 6) + sprite->x;

for (y=0; y<sprite_height; y++)
    {
    // copy the next row into the screen buffer using memcpy for speed

    for (x=0; x<sprite_width; x++)
        {

        // test for transparent pixel i.e. 0, if not transparent then draw

        if ((data=work_sprite[work_offset+x]))
             g_video_buffer[offset+x] = data;

        } // end for x

    // move to next line in video buffer and in sprite bitmap buffer

    offset      += Screen_Width;
    work_offset += sprite_width;

    } // end for y

} // end Draw_Sprite_VB

