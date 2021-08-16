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

/** @file shape.c
	@brief Load the shape files.
*/

#include <epitaxy.h>
#include <sim_struct.h>
#include <shape.h>
#include <string.h>
#include <inp.h>
#include <util.h>
#include <cal_path.h>
#include <i.h>
#include <triangles.h>
#include <triangle.h>
#include <log.h>
#include <component_fun.h>
#include <enabled_libs.h>
#include <dos.h>
#include <heat_fun.h>

int shape_in_shape(struct simulation *sim,struct shape *s,long double z,long double x,long double y)
{
int ny=0;
int nx=0;

	for (nx=0;nx<s->nx;nx++)
	{

		if ((x>s->x0+(s->dx+s->dx_padding)*nx)&&(x<(s->x0+s->dx+(s->dx+s->dx_padding)*nx)))
		{
			//printf("%s\n",s->name);
			//printf("%Le %Le\n",s->x0,s->dx);
			//printf("y0=%Le %Le %Le\n",s->y0,y,s->y0+s->dy);
			//printf("%Le\n",);
			for (ny=0;ny<s->ny;ny++)
			{
				/*if (s->flip_y==FALSE)
				{
					if ((y>s->y0+(s->dy+s->dy_padding)*ny)&&(y<(s->y0+s->dy+(s->dy+s->dy_padding)*ny)))
					{
						return 0;
					}
				}else
				{*/
					if (y<s->y0-(s->dy+s->dy_padding)*ny)
					{
						if (y>(s->y0-s->dy+(s->dy+s->dy_padding)*ny))
						{
							return 0;
						}
					}
				//}
			}
		}
	}
return -1;
}

/*int shape_get_index(struct simulation *sim,struct epitaxy *in,long double x,long double y,long double z)
{
	int i=0;
	int l=0;
	struct shape *s;

	for (l=0;l<in->layers;l++)
	{
		for (i=0;i<in->layer[l].nshape;i++)
		{
			s=&in->layer[l].shapes[i];
			if (shape_in_shape(sim,s,z,x,y+in->device_start)==0)
			{
				return s->dos_index;
			}
		}
	}
	return -1;

}*/

void shape_free(struct simulation *sim,struct shape *s)
{
	if (strcmp(s->optical_material,"none")!=0)
	{
		inter_free(&(s->n));
		inter_free(&(s->alpha));
	}

	triangles_free((&(s->tri)));

	dos_free(&(s->dosn));
	dos_free(&(s->dosp));
	heat_material_free(&(s->heat));
}


void shape_init(struct simulation *sim,struct shape *s)
{
	s->enabled=-1;

	s->dz=0.0;
	s->dx=0.0;
	s->dy=0.0;

	s->dz_padding=0.0;
	s->dx_padding=0.0;
	s->dy_padding=0.0;

	s->nx=1;
	s->ny=1;
	s->nz=1;

	strcpy(s->name,"");
	strcpy(s->shape_type,"none");
	strcpy(s->optical_material,"none");
	strcpy(s->dos_file,"none");

	s->z0=0.0;
	s->x0=0.0;
	s->y0=0.0;

	s->rotate_x=0.0;
	s->rotate_y=0.0;

	s->Gnp=0.0;

	triangles_init((&(s->tri)));
	#ifdef libcircuit_enabled
		component_init(sim,&(s->com));
	#endif

	dos_init(&(s->dosn));
	dos_init(&(s->dosp));
	heat_material_init(&(s->heat));

	s->color_r=0.5;
	s->color_g=0.5;
	s->color_b=0.5;
}

void shape_cpy(struct simulation *sim,struct shape *out,struct shape *in)
{
	//Not sure why this is not fully populated
	out->rotate_y=in->rotate_y;

	dos_cpy(&(out->dosn),&(in->dosn));
	dos_cpy(&(out->dosp),&(in->dosp));
	heat_material_cpy(&(out->heat),&(in->heat));
	strcpy(out->dos_file,in->dos_file);
	out->Gnp=in->Gnp;


	out->color_r=in->color_r;
	out->color_g=in->color_g;
	out->color_b=in->color_b;
}

void shape_load_materials(struct simulation *sim,struct shape *s)
{
/*	struct triangles tri;
	triangle_load_from_file(sim,(&tri),"/home/rod/gpvdm_local/shape/selen/shape.inp");
	struct dat_file buf;
	buffer_init(&buf);
	buffer_malloc(&buf);
	triangles_to_dat_file(&buf,(&tri));
	buffer_dump_path(sim,"","rod_test.dat",&buf);
	buffer_free(&buf);
	printf("wait2 %s\n",s->name);
	getchar();*/

	struct vec my_min;
	struct vec my_max;

	char file_path[PATH_MAX];

	if (strcmp(s->optical_material,"none")!=0)
	{
		join_path(3, file_path,get_materials_path(sim),s->optical_material,"alpha.gmat");
		if (isfile(file_path)!=0)
		{
			ewe(sim,"File %s not found",file_path);
		}

		inter_load(sim,&(s->alpha),file_path);
		inter_sort(&(s->alpha));

		join_path(3, file_path,get_materials_path(sim),s->optical_material,"n.gmat");
		if (isfile(file_path)!=0)
		{
			ewe(sim,"File %s not found",file_path);
		}

		inter_load(sim,&(s->n),file_path);
		inter_sort(&(s->n));

	}else
	{
		ewe(sim,"No optical material defined for object: %s",s->name);
	}


	if (strcmp(s->shape_type,"none")!=0)
	{

		join_path(3,file_path,get_shape_path(sim),s->shape_type,"shape.inp");
		if (isfile(file_path)!=0)
		{
			printf("Could not load: %s",file_path);
			join_path(3,file_path,get_shape_path(sim),"box","shape.inp");
		}
		triangle_load_from_file(sim,&s->tri,file_path);

	}


}



