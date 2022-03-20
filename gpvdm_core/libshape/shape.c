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
#include <exciton_fun.h>
#include <singlet_fun.h>

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
	exciton_material_free(&(s->ex));
	#ifdef libsinglet_enabled
		singlet_material_free(&(s->sing));
	#endif
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
	exciton_material_init(&(s->ex));
	#ifdef libsinglet_enabled
		singlet_material_init(&(s->sing));
	#endif
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
	exciton_material_cpy(&(out->ex),&(in->ex));
	#ifdef libsinglet_enabled
		singlet_material_cpy(&(out->sing),&(in->sing));
	#endif
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
	dat_file_init(&buf);
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

void shape_cal_min_max(struct simulation *sim,struct vec *min,struct vec *max,struct shape *s)
{
		if (s->x0<min->x)
		{
			min->x=s->x0;
		}

		if (s->x0+s->dx>max->x)
		{
			max->x=s->x0+s->dx;
		}

		if (s->y0<min->y)
		{
			min->y=s->y0;
		}

		if (s->y0+s->dy>max->y)
		{
			max->y=s->y0+s->dy;
		}

		if (s->z0<min->z)
		{
			min->z=s->z0;
		}

		if (s->z0+s->dz>max->z)
		{
			max->z=s->z0+s->dz;
		}
}

