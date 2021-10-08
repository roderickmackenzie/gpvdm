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




/** @file math_2d_to_triangles_front.c
	@brief Convert a math 2d structure to triangles in a simple way
*/
#define _FILE_OFFSET_BITS 64
#define _LARGEFILE_SOURCE
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
#include <triangles.h>

struct front_line
{
	int start;
	int used;
	void *last;
	void *next;

	int x0;
	int y0;

	int x1;
	int y1;

};

struct front_lines
{
	struct front_line* lines;
	int nlines;
};

void front_lines_init(struct front_lines *lines)
{
	lines->lines=NULL;
	lines->nlines=0;
}

void front_lines_free(struct front_lines *lines)
{
	free(lines->lines);
	lines->lines=NULL;

	lines->nlines=0;
}

void front_lines_dump(char *file_name,struct front_lines *lines)
{
	int i;
	FILE *out;
	struct front_line* line;
	out=fopen(file_name,"w");
	//printf("%ld\n",lines->nlines);
	//getchar();
	for (i=0;i<lines->nlines;i++)
	{
		line=&(lines->lines[i]);
		fprintf(out,"%d %d\n",line->x0,line->y0);
		fprintf(out,"%d %d\n",line->x1,line->y1);
		fprintf(out,"\n");
	}

	fclose(out);
}

struct front_line* front_lines_add(struct front_lines *lines)
{
	int i;
	for (i=0;i<lines->nlines;i++)
	{
		if (lines->lines[i].used==FALSE)
		{
			lines->lines[i].used=TRUE;
			return &(lines->lines[i]);
		}
	}
	lines->lines=realloc(lines->lines,(lines->nlines+1)*sizeof(struct front_line));
	lines->lines[lines->nlines].used=TRUE;
	lines->nlines++;
	return &(lines->lines[lines->nlines-1]);
}

void math_2d_to_triangles_front(struct simulation *sim,struct triangles *tri,struct math_2d *in, int x_steps, int y_steps)
{
	
	int x=0;
	int y=0;
	int dx=0;
	int dy=0;
	int x_pos=0;
	int y_pos=0;

	double x0=0.0;
	double y0=0.0;
	double z0=0.0;

	double x1=0.0;
	double y1=0.0;
	double z1=0.0;

	double x2=0.0;
	double y2=0.0;
	double z2=0.0;

	double d00=0.0;
	double d10=0.0;
	double d01=0.0;
	double d11=0.0;

	dx=in->x_len/x_steps;
	dy=in->y_len/y_steps;

	struct front_line* line;
	struct front_line* line_last;
	struct front_lines lines;

	front_lines_init(&lines);

	triangles_malloc(tri);
	//		y_pos=y*dy;

	y_pos=0;
	line_last=NULL;
	for (x=0;x<x_steps-1;x++)
	{
		x_pos=x*dx;
		line=front_lines_add(&lines);
		line->x0=x_pos;
		line->y0=0;

		line->x1=x_pos+dx;
		line->y1=0;

		line->last=(void*)line_last;

		line_last=line;
	}

	x_pos=x_steps*dx;

	for (y=0;y<y_steps-1;y++)
	{
		y_pos=y*dy;
		line=front_lines_add(&lines);
		line->x0=x_pos;
		line->y0=y_pos;

		line->x1=x_pos;
		line->y1=y_pos+dy;

		line->last=(void*)line_last;

		line_last=line;
	}

	y_pos=y_steps*dy;

	for (x=x_steps-1;x>=0;x--)
	{
		x_pos=x*dx;
		line=front_lines_add(&lines);
		line->x0=x_pos;
		line->y0=y_pos;

		line->x1=x_pos+dx;
		line->y1=y_pos;

		line->last=(void*)line_last;

		line_last=line;
	}

	x_pos=0.0;

	for (y=y_steps-1;y>=0;y--)
	{
		y_pos=y*dy;
		line=front_lines_add(&lines);
		line->x0=0;
		line->y0=y_pos;

		line->x1=0;
		line->y1=y_pos+dy;

		line->last=(void*)line_last;

		line_last=line;
	}

	front_lines_dump("front.dat",&lines);
	//getchar();
	front_lines_free(&lines);

	//triangles_add_triangle(tri, x0,y0,z0,x1,y1,z1,x2,y2,z2,0,0);

}



