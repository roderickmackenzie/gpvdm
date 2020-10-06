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


	long double *ymesh;
	long double *xmesh;
	long double *zmesh;

	long double *dy;
	long double *dx;
	long double *dz;

	int srh_bands;

};

struct dim_zx_epitaxy
{
	int zlen;
	int xlen;
	int ylen;
};

struct dim_light
{
	int zlen;
	int xlen;
	int ylen;
	int llen;

	long double *y;
	long double *x;
	long double *z;
	long double *l;

	long double dy;
	long double dx;
	long double dz;
	long double dl;

};

struct dim_heat
{
	int zlen;
	int xlen;
	int ylen;

	long double *y;
	long double *x;
	long double *z;

	long double *dy;
	long double *dx;
	long double *dz;
};


void dim_set_simple_mesh_x(struct dimensions *dim, long double start, long double stop);
void dim_set_simple_mesh_z(struct dimensions *dim, long double start, long double stop);

//dimension
void dim_init(struct dimensions *dim);
void dim_free(struct dimensions *dim);
void dim_alloc(struct dimensions *dim);
void dim_cpy(struct dimensions *out,struct dimensions *in);
void dim_alloc_xyz(struct dimensions *dim,char xyz);
void dim_free_xyz(struct dimensions *dim,char xyz);
void dim_swap(struct dimensions *out,struct dimensions *in);
void dim_info_to_buf(struct dat_file *buf,struct dimensions *dim);
long double dim_dl_xyz(struct dimensions *dim, int x0, int y0, int z0,int x1,int y1, int z1);

//dim_epitaxy
void dim_init_zx_epitaxy(struct dim_zx_epitaxy *dim);
void dim_free_zx_epitaxy(struct dim_zx_epitaxy *dim);

//dim_light
void dim_light_init_xyzl(struct dim_light *dim,char xyzl);
void dim_light_init(struct dim_light *dim);
void dim_light_free_xyzl(struct dim_light *dim,char xyzl);
void dim_light_free(struct dim_light *dim);
void dim_light_malloc_xyzl(struct dim_light *dim,char xyzl);
void dim_light_malloc(struct dim_light *dim);
void dim_light_cpy(struct dim_light *out,struct dim_light *in);
void dim_light_info_to_buf(struct dat_file *buf,struct dim_light *dim);

//dim_heat
void dim_heat_init_xyz(struct dim_heat *dim,char xyz);
void dim_heat_init(struct dim_heat *dim);
void dim_heat_free_xyz(struct dim_heat *dim,char xyz);
void dim_heat_free(struct dim_heat *dim);
void dim_heat_malloc_xyz(struct dim_heat *dim,char xyz);
void dim_heat_malloc(struct dim_heat *dim);
void dim_heat_info_to_buf(struct dat_file *buf,struct dim_heat *dim);


#endif
