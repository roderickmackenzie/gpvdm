// 
// General-purpose Photovoltaic Device Model gpvdm.com - a drift diffusion
// base/Shockley-Read-Hall model for 1st, 2nd and 3rd generation solarcells.
// The model can simulate OLEDs, Perovskite cells, and OFETs.
// 
// Copyright (C) 2008-2020 Roderick C. I. MacKenzie
// 
// https://www.gpvdm.com
// r.c.i.mackenzie at googlemail.com
// 
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the GPVDM nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL Roderick C. I. MacKenzie BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
