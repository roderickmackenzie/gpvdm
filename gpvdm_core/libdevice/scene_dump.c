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

#include <stdio.h>
#include <ray.h>
#include <ray_fun.h>
#include <gpvdm_const.h>
#include <math.h>
#include <stdlib.h>
#include <cal_path.h>
#include <log.h>
#include <device.h>
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
	//int i;

	char temp[200];
	struct dat_file buf;
	struct world *w=&(dev->w);
	dat_file_init(&buf);

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
	buf.y=w->triangles;
	buf.z=1;
	buffer_add_info(sim,&buf);
	struct object *obj;
	//struct triangle *tri;


	int o=0;
	for (o=0;o<w->objects;o++)
	{
		obj=&(w->obj[o]);

		sprintf(temp,"#name %s\n",obj->name);
		buffer_add_string(&buf,temp);

		triangles_to_dat_file(&buf,&(obj->tri));
		//printf("wait %s\n",obj->name);
		//getchar();
	}

	buffer_dump_path(sim,"",file_name,&buf);
	buffer_free(&buf);

}

