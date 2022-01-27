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

/** @file dim.h
	@brief Dimention file
*/

#ifndef dim_h
#define dim_h

#include <dat_file_struct.h>

struct dimensions
{
	int zlen;
	int xlen;
	int ylen;
	int llen;

	long double *y;
	long double *x;
	long double *z;
	long double *l;

	long double *dY;
	long double *dX;
	long double *dZ;

	long double dy;
	long double dx;
	long double dz;
	long double dl;

	int srh_bands;
};

struct dim_zx_epitaxy
{
	int zlen;
	int xlen;
	int ylen;
};



void dim_set_simple_mesh_x(struct dimensions *dim, long double start, long double stop);
void dim_set_simple_mesh_z(struct dimensions *dim, long double start, long double stop);

//dimension
void dim_init(struct dimensions *dim);
void dim_free(struct dimensions *dim);
void dim_malloc(struct dimensions *dim);
void dim_cpy(struct dimensions *out,struct dimensions *in);
void dim_malloc_xyz(struct dimensions *dim,char xyz);
void dim_free_xyz(struct dimensions *dim,char xyz);
void dim_swap(struct dimensions *out,struct dimensions *in);
void dim_info_to_buf(struct dat_file *buf,struct dimensions *dim);
long double dim_dl_xyz(struct dimensions *dim, int x0, int y0, int z0,int x1,int y1, int z1);

//dim_epitaxy
void dim_init_zx_epitaxy(struct dim_zx_epitaxy *dim);
void dim_free_zx_epitaxy(struct dim_zx_epitaxy *dim);

//dim_light
void dim_light_init_xyzl(struct dimensions *dim,char xyzl);
void dim_light_init(struct dimensions *dim);
void dim_light_free_xyzl(struct dimensions *dim,char xyzl);
void dim_light_free(struct dimensions *dim);
void dim_light_malloc_xyzl(struct dimensions *dim,char xyzl);
void dim_light_malloc(struct dimensions *dim);
void dim_light_cpy(struct dimensions *out,struct dimensions *in);

//dim_heat
void dim_heat_malloc(struct dimensions *dim);


#endif
