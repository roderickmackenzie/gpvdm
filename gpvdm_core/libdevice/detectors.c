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

/** @file world_init.c
	@brief Initalize the world
*/

#include <epitaxy.h>
#include <sim_struct.h>
#include <shape.h>
#include <cal_path.h>
#include <world.h>
#include <world_struct.h>
#include <util.h>
#include <ray.h>
#include <ray_fun.h>
#include <detector.h>

void detector_init(struct simulation *sim,struct detector *d)
{
	d->viewpoint_enabled=FALSE;
	d->viewpoint_x0=-1.0;
	d->viewpoint_y0=-1.0;
	d->viewpoint_z0=-1.0;
	d->viewpoint_dx=-1.0;
	d->viewpoint_dz=-1.0;

	d->viewpoint_nx=-1.0;
	d->viewpoint_nz=-1.0;

}

void detector_free(struct simulation *sim,struct detector *d)
{
	detector_init(sim,d);
}

void detector_cpy(struct simulation *sim,struct detector *out,struct detector *in)
{
	out->viewpoint_enabled=in->viewpoint_enabled;
	out->viewpoint_x0=in->viewpoint_x0;
	out->viewpoint_y0=in->viewpoint_y0;
	out->viewpoint_z0=in->viewpoint_z0;
	out->viewpoint_dx=in->viewpoint_dx;
	out->viewpoint_dz=in->viewpoint_dz;

	out->viewpoint_nx=in->viewpoint_nx;
	out->viewpoint_nz=in->viewpoint_nz;
}

void detectors_init(struct simulation *sim,struct world *w)
{
		w->det=NULL;
		w->detectors=-1;
}

void detectors_load(struct simulation *sim,struct world *w, struct json_obj *json_detectors)
{
	int d=0;
	char temp[100];
	struct detector *det;
	struct json_obj *json_segment;
	struct json_obj *json_viewpoint;

	json_get_int(sim, json_detectors, &(w->detectors),"segments");

	if (w->detectors>0)
	{
		w->det=malloc(sizeof(struct detector)*w->detectors);
		for (d=0;d<w->detectors;d++)
		{
			sprintf(temp,"segment%d",d);
			json_segment=json_obj_find(json_detectors, temp);

			if (json_segment==NULL)
			{
				ewe(sim,"%s not found in detectors\n",temp);
			}

			det=&(w->det[d]);
			detector_init(sim,det);


			json_viewpoint=json_obj_find(json_segment, "config");

			if (json_viewpoint==NULL)
			{
				ewe(sim,"viewpoint not found\n");
			}

			json_get_english(sim, json_viewpoint, &(det->viewpoint_enabled),"viewpoint_enabled");

			json_get_double(sim, json_viewpoint, &(det->viewpoint_x0),"viewpoint_x0");
			json_get_double(sim, json_viewpoint, &(det->viewpoint_y0),"viewpoint_y0");
			json_get_double(sim, json_viewpoint, &(det->viewpoint_z0),"viewpoint_z0");

			json_get_double(sim, json_viewpoint, &(det->viewpoint_dx),"viewpoint_dx");
			json_get_double(sim, json_viewpoint, &(det->viewpoint_dz),"viewpoint_dz");

			json_get_int(sim, json_viewpoint, &(det->viewpoint_nx),"viewpoint_nx");
			json_get_int(sim, json_viewpoint, &(det->viewpoint_nz),"viewpoint_nz");


		}

	}

}

void detectors_cpy(struct simulation *sim,struct world *out,struct world *in)
{
	int i;
	out->detectors=in->detectors;

	if (in->detectors>0)
	{
		out->det=malloc(sizeof(struct detector)*out->detectors);
		for (i=0;i<out->detectors;i++)
		{
			detector_init(sim,&(out->det[i]));
			detector_cpy(sim,&(out->det[i]),&(in->det[i]));
		}
	}
}

void detectors_free(struct simulation *sim,struct world *w)
{
	int i;
	for (i=0;i<w->detectors;i++)
	{
		detector_free(sim,&(w->det[i]));
	}

	if (w->det!=NULL)
	{
		free(w->det);
		w->det=NULL;
	}

	detectors_init(sim,w);
}

void dectors_add_to_scene(struct simulation *sim,struct world *w,struct image *my_image,struct vec *min, struct vec *max)
{
	int d;

	double camera_dx=0.0;
	double camera_dz=0.0;

	double camera_y=0.0;
	double camera_x=0.0;
	double camera_z=0.0;
	struct object *obj;

	struct detector *det;
	for (d=0;d<w->detectors;d++)
	{
		det=&(w->det[d]);

		if (det->viewpoint_enabled==TRUE)
		{
			camera_dx=(max->x-min->x)*det->viewpoint_dx;
			camera_dz=(max->z-min->z)*det->viewpoint_dz;

			camera_x=min->x+det->viewpoint_x0*(max->x-min->x);
			camera_y=min->y+det->viewpoint_y0*(max->y-min->y);
			camera_z=min->z+det->viewpoint_z0*(max->z-min->z);

			obj=add_plane(w,camera_x,camera_y, camera_z, camera_dx,camera_dz,RAY_VIEWPOINT);

			obj->epi_layer=-1;
			strcpy(obj->name,"viewpoint");

			dim_malloc_xyz(&(my_image->viewpoint_dim),'x');
			dim_malloc_xyz(&(my_image->viewpoint_dim),'z');
			dim_set_simple_mesh_z(&(my_image->viewpoint_dim), camera_z, camera_z+camera_dz);
			dim_set_simple_mesh_x(&(my_image->viewpoint_dim), camera_x, camera_x+camera_dx);
			if (d>1)
			{
				break;
			}
		}
	}
}
