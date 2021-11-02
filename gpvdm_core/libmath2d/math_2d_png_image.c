//
// General-purpose Photovoltaic Device Model gpvdm.com - a drift diffusion
// base/Shockley-Read-Hall model for 1st, 2nd and 3rd generation solarcells.
// The model can simulate OLEDs, Perovskite cells, and OFETs.
// 
// Copyright 2008-2022 Roderick C. I. MacKenzie https://www.gpvdm.com
// r.c.i.mackenzie at googlemail.com
// 
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense, 
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
// SOFTWARE.
// 




/** @file math_2d_malloc.c
	@brief Memory allocation for 2d math structures
*/
#define _FILE_OFFSET_BITS 64
#define _LARGEFILE_SOURCE
#include <enabled_libs.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sim_struct.h>
#include <memory.h>

#include "math_2d.h"
#include "util.h"
#include "cal_path.h"
#include "gpvdm_const.h"
#include <log.h>
#include <png_image.h>

void math_2d_png_load(struct simulation *sim,struct png_image *image,char* file_name)
{
	int y=0;
	FILE *fp;
    char header[8];    // 8 is the maximum size that can be checked

    /* open file and test for it being a png */
    fp = fopen(file_name, "rb");

    if (!fp)
	{
		ewe(sim,"[read_png_file] File %s could not be opened for reading", file_name);
	}

    fread(header, 1, 8, fp);

    if (png_sig_cmp(header, 0, 8))
	{
		ewe(sim,"[read_png_file] File %s is not recognized as a PNG file", file_name);
	}


    image->png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (!image->png_ptr)
	{
		ewe(sim,"[read_png_file] png_create_read_struct failed");
	}

    image->info_ptr = png_create_info_struct(image->png_ptr);

    if (!image->info_ptr)
	{
		ewe(sim,"[read_png_file] png_create_info_struct failed");
	}

    if (setjmp(png_jmpbuf(image->png_ptr)))
	{
		ewe(sim,"[read_png_file] Error during init_io");
	}

    png_init_io(image->png_ptr, fp);
    png_set_sig_bytes(image->png_ptr, 8);

    png_read_info(image->png_ptr, image->info_ptr);

    image->width = png_get_image_width(image->png_ptr, image->info_ptr);
    image->height = png_get_image_height(image->png_ptr, image->info_ptr);
    image->color_type = png_get_color_type(image->png_ptr, image->info_ptr);
    image->bit_depth = png_get_bit_depth(image->png_ptr, image->info_ptr);

	image->number_of_passes = png_set_interlace_handling(image->png_ptr);
    png_read_update_info(image->png_ptr, image->info_ptr);


    /* read file */
    if (setjmp(png_jmpbuf(image->png_ptr)))
	{
		ewe(sim,"[read_png_file] Error during read_image");
	}

    image->row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * image->height);

    for (y=0; y<image->height; y++)
	{
    	image->row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(image->png_ptr,image->info_ptr));
	}

    png_read_image(image->png_ptr, image->row_pointers);

    fclose(fp);
	png_destroy_read_struct(&(image->png_ptr), &(image->info_ptr), png_infopp_NULL);

}


void math_2d_png_import(struct simulation *sim,struct math_2d *data,struct png_image *image)
{

	int x=0;
	int y=0;
	int rgba=3;

    if (image->color_type == PNG_COLOR_TYPE_RGB)
	{
		rgba=3;
	}else
    if (image->color_type == PNG_COLOR_TYPE_RGBA)
	{
		rgba=4;
	}else
	{
		ewe(sim,"Image type not recognized\n");
	}
	
	//printf("here %d %d\n",image->width, image->height);
	math_2d_malloc(data, image->width, image->height);

    for (y=0; y< image->height; y++)
	{
        png_byte* row = image->row_pointers[y];

        for (x=0; x< image->width; x++)
		{
            png_byte* ptr = &(row[x*rgba]);

			data->data[x][y]=(ptr[0]+ptr[1]+ptr[2])/3;
			/*if (image->height/2==y)
			{
				printf("%d %d %d %d %d %d %d %d\n",ptr[0],ptr[1],ptr[2],rgba,png_get_color_type(image->png_ptr,  image->info_ptr),PNG_COLOR_TYPE_RGBA,PNG_COLOR_TYPE_RGB,image->color_type);
			}*/

        }
    }

}

void math_2d_png_free(struct simulation *sim,struct png_image *image)
{

	int y=0;
    for (y=0; y<image->height; y++)
	{
		free(image->row_pointers[y]);
	}

	free(image->row_pointers);
}

void math_2d_png_init(struct simulation *sim,struct png_image *image)
{

	image->width=-1;
	image->height=-1;
	image->color_type=-1;
	image->bit_depth=-1;

	image->png_ptr=NULL;
	image->info_ptr=NULL;
	image->number_of_passes=-1;
	image->row_pointers=NULL;

}


