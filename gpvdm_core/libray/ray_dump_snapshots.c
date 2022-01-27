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
#include <inp.h>
#include <util.h>
#include <triangles.h>
#include <memory.h>
#include <epitaxy_struct.h>
#include <epitaxy.h>
#include <device_fun.h>
#include <dat_file.h>
#include <dump.h>

/** @file ray_snapshots.c
	@brief Set up the simulation window for the ray tracer
*/

void ray_dump_shapshots(struct simulation *sim,struct device *dev, struct image *my_image ,struct ray_worker *worker,int layer)
{
	struct dat_file buf;
	char out_dir[PATH_MAX];
	char file_name[200];
	char full_path[PATH_MAX];
	char temp[200];
	//if (my_image->dump_snapshots==TRUE)
	{

		struct snapshots snap;
		snapshots_init(&snap);
		strcpy(snap.type,"snapshots");
		strcpy(snap.plot_type,"3d");
		strcpy(snap.name,"ray_trace");
		strcpy(snap.path,get_output_path(dev));

		dump_make_snapshot_dir(sim,out_dir, worker->l,&snap);


		sprintf(file_name,"ray_%d.dat",layer);

		dat_file_init(&buf);
		buffer_malloc(&buf);

		sprintf(temp,"{\n");
		buffer_add_string(&buf,temp);

		sprintf(temp,"\t\"wavelength\":%Le\n",(long double )my_image->lam[worker->l]);
		buffer_add_string(&buf,temp);

		sprintf(temp,"}");
		buffer_add_string(&buf,temp);

		buffer_dump_path(sim,out_dir,"data.json",&buf);
		buffer_free(&buf);

		join_path(2,full_path,out_dir,file_name);

		dump_plane_to_file(sim,full_path,my_image,dev);
	}

}


