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

/** @file world_cpy.c
	@brief Copy the world
*/

#include <epitaxy.h>
#include <sim_struct.h>
#include <shape.h>
#include <cal_path.h>
#include <world.h>
#include <world_struct.h>
#include <triangles.h>
#include <triangle.h>
#include <detector.h>

void world_cpy(struct simulation *sim,struct world *out,struct world *in)
{
	int i;

	out->shapes=(struct shape *)malloc(sizeof(struct shape)*in->items);
	out->items=in->items;

	for (i=0;i<in->items;i++)
	{
		shape_init(sim,&(out->shapes[i]));
		shape_cpy(sim,&(out->shapes[i]),&(in->shapes[i]));
	}

	out->world_automatic_size=in->world_automatic_size;

	out->x0=in->x0;
	out->x1=in->x1;
	out->y0=in->y0;
	out->y1=in->y1;
	out->z0=in->z0;
	out->z1=in->z1;

	out->obj=NULL;			//We are not going to copy this
	out->objects=0;
	out->triangles=0;

	detectors_cpy(sim,out,in);
}


