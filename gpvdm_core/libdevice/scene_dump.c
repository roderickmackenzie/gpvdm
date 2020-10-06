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

#include <stdio.h>
#include <ray.h>
#include <ray_fun.h>
#include <gpvdm_const.h>
#include <math.h>
#include <stdlib.h>
#include <cal_path.h>
#include <log.h>
#include <device.h>
#include <inp.h>
#include <util.h>
#include <triangles.h>
#include <memory.h>
#include <epitaxy_struct.h>
#include <epitaxy.h>
#include <dat_file.h>

/** @file scene_dump.c
	@brief Dump the scene to file
*/


void device_dump_world_to_file(struct simulation *sim,struct device *dev,char *file_name)
{
	int i;

	char temp[200];

	//printf("file dump\n");
	struct dat_file buf;
	buffer_init(&buf);

	buffer_malloc(&buf);
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,"Ray trace triange file");
	strcpy(buf.type,"poly");
	strcpy(buf.y_label,"Position");
	strcpy(buf.x_label,"Position");
	strcpy(buf.data_label,"Position");

	strcpy(buf.y_units,"m");
	strcpy(buf.x_units,"m");
	strcpy(buf.data_units,"m");
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.x=1;
	buf.y=dev->triangles;
	buf.z=1;
	buffer_add_info(sim,&buf);
	struct object *obj;
	struct triangle *tri;


	int o=0;
	for (o=0;o<dev->objects;o++)
	{
		obj=&(dev->obj[o]);

		sprintf(temp,"#name %s\n",obj->name);
		buffer_add_string(&buf,temp);

		triangles_to_dat_file(&buf,&(obj->tri));

	}

	buffer_dump_path(sim,"",file_name,&buf);
	buffer_free(&buf);
}

