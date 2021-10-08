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

/** @file time_mesh.h
	@brief The main structure which holds information about the device.
*/

#ifndef time_mesh_h
#define time_mesh_h
#include <stdio.h>
#include "code_ctrl.h"
#include "light.h"
#include <epitaxy_struct.h>
#include "advmath.h"
#include <dos_struct.h>
#include <contact_struct.h>
#include <perovskite_struct.h>
#include <circuit_struct.h>
#include <dim.h>
#include <matrix.h>
#include <shape_struct.h>
#include <heat.h>
#include <mesh_struct.h>
#include <lib_fxdomain.h>

struct time_mesh
{
	long double *tm_time_mesh;
	long double *tm_dt;
	long double *tm_sun;
	long double *tm_voltage;
	long double *tm_laser;
	long double *tm_fs_laser;

	int tm_mesh_len;
	int tm_use_mesh;
	int tm_mesh_pos;
};


#endif
