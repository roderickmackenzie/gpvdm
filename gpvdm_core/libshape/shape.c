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
				if (s->flip_y==FALSE)
				{
					if ((y>s->y0+(s->dy+s->dy_padding)*ny)&&(y<(s->y0+s->dy+(s->dy+s->dy_padding)*ny)))
					{
						return 0;
					}
				}else
				{
					if (y<s->y0-(s->dy+s->dy_padding)*ny)
					{
						if (y>(s->y0-s->dy+(s->dy+s->dy_padding)*ny))
						{
							return 0;
						}
					}
				}
			}
		}
	}
return -1;
}

int shape_get_index(struct simulation *sim,struct epitaxy *in,long double x,long double y,long double z)
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

}

void shape_free(struct simulation *sim,struct shape *s)
{
	if (strcmp(s->optical_material,"none")!=0)
	{
		inter_free(&(s->n));
		inter_free(&(s->alpha));
	}

	triangles_free((&(s->tri)));
}


void shape_init(struct simulation *sim,struct shape *s)
{
	s->enabled=-1;

	s->dos_index=-1;
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

	s->flip_y=FALSE;
	s->flip_x=FALSE;

	triangles_init((&(s->tri)));

	component_init(sim,&(s->com));
}

void shape_load_materials(struct simulation *sim,struct shape *s)
{
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

	}

	if (strcmp(s->shape_type,"none")!=0)
	{

		join_path(3,file_path,get_shape_path(sim),s->shape_type,"shape.inp");
		triangle_load_from_file(sim,(&s->tri),file_path);
	}


}

struct shape *shape_load_file(struct simulation *sim,struct epitaxy *in,struct shape *s, char *file_name,long double y_pos)
{
	int enabled;
	char full_file_name[PATH_MAX];

	sprintf(full_file_name,"%s.inp",file_name);

	printf_log(sim,"Loading shape file: %s\n",full_file_name);

	struct inp_file inp;
	inp_init(sim,&inp);

	if (inp_load(sim,&inp,full_file_name)==0)
	{
		s->enabled=inp_search_english(sim,&inp,"#shape_enabled");

		s->dos_index=-1;

		inp_search_gdouble(sim,&inp,&(s->dx),"#shape_dx");
		inp_search_gdouble(sim,&inp,&(s->dy),"#shape_dy");
		inp_search_gdouble(sim,&inp,&(s->dz),"#shape_dz");

		inp_search_gdouble(sim,&inp,&(s->dx_padding),"#shape_padding_dx");
		inp_search_gdouble(sim,&inp,&(s->dy_padding),"#shape_padding_dy");
		inp_search_gdouble(sim,&inp,&(s->dz_padding),"#shape_padding_dz");


		inp_search_int(sim,&inp,&(s->nx),"#shape_nx");
		inp_search_int(sim,&inp,&(s->ny),"#shape_ny");
		inp_search_int(sim,&inp,&(s->nz),"#shape_nz");

		inp_search_string(sim,&inp,s->name,"#shape_name");
		inp_search_string(sim,&inp,s->shape_type,"#shape_type");

		inp_search_string(sim,&inp,s->optical_material,"#shape_optical_material");
		assert_platform_path(s->optical_material);

		inp_search_string(sim,&inp,s->dos_file,"#shape_dos");
		inp_search_string(sim,&inp,s->electrical_file,"#shape_electrical");

		#ifdef libcircuit_enabled
			if (s->enabled==TRUE)
			{
				component_load_electrical_file(sim,&(s->com),s->electrical_file);
			}
		#endif

		inp_search_gdouble(sim,&inp,&(s->x0),"#shape_x0");
		inp_search_gdouble(sim,&inp,&(s->z0),"#shape_z0");
		inp_search_gdouble(sim,&inp,&(s->y0),"#shape_y0");

		s->flip_y=inp_search_english(sim,&inp,"#shape_flip_y");
		s->flip_x=inp_search_english(sim,&inp,"#shape_flip_x");

		if (s->flip_y==FALSE)
		{
			s->y0=y_pos+s->y0;
		}else
		{
			s->y0=y_pos-s->y0;
		}

		if (s->enabled==TRUE)
		{
			if (strcmp(s->dos_file,"none")!=0)
			{
				s->dos_index=in->electrical_layers;
				epitaxy_load_dos_files(sim,in, s->dos_file,"none","none");
			}

			shape_load_materials(sim,s);

		}
		inp_free(sim,&inp);
	}else
	{
		ewe(sim,"file %s not found\n",full_file_name);
	}

return s;
}


