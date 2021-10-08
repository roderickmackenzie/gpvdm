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


/** @file math_2d.h
	@brief Header file for math_2d.c
*/
#ifndef math_2d_h
#define math_2d_h
#include "advmath.h"
#include <sim_struct.h>
#include <png_image.h>
#include <triangle.h>

struct math_2d
{
	long double **data;
	int x_len;
	int y_len;
	long double *y_mesh;
	long double *x_mesh;

};

void math_2d_init(struct math_2d *in);
void math_2d_malloc(struct math_2d *in, int x_len, int y_len);
void math_2d_set_value(struct math_2d *in,long double value);
void math_2d_free(struct math_2d *in);
void math_2d_init_y_mesh(struct math_2d *in,long double start, long double stop);
void math_2d_save(struct simulation *sim,char *file_name,struct math_2d *data);
void math_2d_to_triangles_simple(struct simulation *sim,struct triangles *tri,struct math_2d *in, int x_steps, int y_steps);
void math_2d_to_triangles_front(struct simulation *sim,struct triangles *tri,struct math_2d *in, int x_steps, int y_steps);

//math
long double math_2d_y_min(struct simulation *sim,struct math_2d *in);
long double math_2d_y_max(struct simulation *sim,struct math_2d *in);
void math_2d_norm(struct simulation *sim,struct math_2d *in);
void math_2d_add_long_double(struct simulation *sim,struct math_2d *in,long double val);

//png
void math_2d_png_load(struct simulation *sim,struct png_image *image,char* file_name);
void math_2d_png_import(struct simulation *sim,struct math_2d *data,struct png_image *image);
void math_2d_png_free(struct simulation *sim,struct png_image *image);
void math_2d_png_init(struct simulation *sim,struct png_image *image);
#endif
