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


/** @file shape_struct.h
	@brief A structure to hold shapes
*/


#ifndef shape_struct_h
#define shape_struct_h
#include "advmath.h"
#include <sim_struct.h>
#include <triangle.h>
#include <component.h>
#include <enabled_libs.h>
#include <dos_struct.h>
#include <heat_material.h>

struct shape
{
	int enabled;
	long double dx;
	long double dy;
	long double dz;
	long double dx_padding;
	long double dy_padding;
	long double dz_padding;
	int nx;
	int ny;
	int nz;
	char name[100];
	char shape_type[20];
	char optical_material[100];
	long double x0;
	long double y0;
	long double z0;
	int epi_index;
	struct math_xy alpha;
	struct math_xy n;
	struct triangles tri;
	long double rotate_x;
	long double rotate_y;
	#ifdef libcircuit_enabled
		struct component com;
	#endif

	char dos_file[100];
	char id[100];
	struct dos dosn;
	struct dos dosp;
	struct heat_material heat;
	long double Gnp;

	long double color_r;
	long double color_g;
	long double color_b;
};

#endif
