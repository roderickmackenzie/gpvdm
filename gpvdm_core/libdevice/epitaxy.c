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

/** @file epitaxy.c
	@brief Load the epitaxy structure.
*/

#include <string.h>
#include "epitaxy.h"
#include "inp.h"
#include "util.h"
#include "gpvdm_const.h"
#include <cal_path.h>
#include <shape.h>
#include <contacts.h>
#include <component.h>
#include <component_fun.h>
#include <log.h>

void epitaxy_load_pl_file(struct simulation *sim, struct epi_layer *layer,struct json_obj *pl_json)
{
	struct inp_file inp;
	char full_path[PATH_MAX];
	char temp_path[PATH_MAX];
	char spectrum_file[100];
	char temp[100];
	int count=0;
	if (pl_json==NULL)
	{
		ewe(sim,"pl_json is NULL in epitaxy_load_pl_file\n");
	}

	json_get_long_double(sim,pl_json, &(layer->pl_fe_fh),"pl_fe_fh");

	json_get_long_double(sim,pl_json, &(layer->pl_fe_te),"pl_fe_te");

	json_get_long_double(sim,pl_json, &(layer->pl_te_fh),"pl_te_fh");

	json_get_long_double(sim,pl_json, &(layer->pl_th_fe),"pl_th_fe");

	json_get_long_double(sim,pl_json, &(layer->pl_fh_th),"pl_fh_th");

	json_get_english(sim,pl_json, &(layer->pl_enabled),"pl_emission_enabled");

	json_get_english(sim,pl_json, &(layer->pl_use_experimental_emission_spectra),"pl_use_experimental_emission_spectra");

	json_get_long_double(sim,pl_json, &(layer->pl_experimental_emission_efficiency),"pl_experimental_emission_efficiency");

	json_get_string(sim, pl_json, spectrum_file,"pl_input_spectrum");

	if (layer->pl_enabled==TRUE)
	{
		if (strcmp(spectrum_file,"none")==0)
		{

			strcpy(layer->pl_spectrum_file,"none");
			if (layer->pl_use_experimental_emission_spectra==TRUE)
			{
				ewe(sim,"You have not specified an emisison spectra");
			}
		}else
		{
			join_path(3, layer->pl_spectrum_file, sim->emission_path, spectrum_file,"spectra.inp");

			epitaxy_load_emission(sim,layer);

		}
	}else
	{
		layer->pl_fe_fh=0.0;
		layer->pl_fe_te=0.0;
		layer->pl_te_fh=0.0;
		layer->pl_th_fe=0.0;
		layer->pl_fh_th=0.0;
		layer->pl_enabled=FALSE;
		strcpy(layer->pl_spectrum_file,"none");
	}
	layer->photon_extract_eff=NULL;
	layer->avg_photon_extract_eff=0.0;

}



void epitaxy_load(struct simulation *sim,struct epitaxy *in, struct json_obj *obj_epi)
{
	int i;
	char temp[100];
	char interface_file[20];
	char shape_file[20];

	long double y_pos=0.0;
	struct shape *s;

	in->device_start=-1.0;
	in->device_stop=-1.0;
	char layer_id[20];
	char *test;
	struct json_obj *obj_layer;
	struct json_obj *obj_pl;
	struct json_obj *obj_interface;
	//struct json_obj *obj_shape;

	json_get_int(sim, obj_epi, &(in->layers),"layers");

	if (in->layers>20)
	{
		ewe(sim,"Too many material layers\n");
	}

	if (in->layers<1)
	{
		ewe(sim,"No material layers\n");
	}

	for (i=0;i<in->layers;i++)
	{
		in->layer[i].layer_number=i;
		s=&(in->layer[i].s);
		shape_init(sim,s);

		sprintf(layer_id,"layer%d",i);
		obj_layer=json_obj_find(obj_epi, layer_id);

		if (obj_layer==NULL)
		{
			ewe(sim,"Object %s not found\n",layer_id);
		}

		json_get_english(sim,obj_layer, &(in->layer[i].layer_type),"layer_type");

		if (shape_load_from_json(sim,in,s, obj_layer ,0.0)==FALSE)
		{
			printf_log(sim,"Warning shape %s disabled\n", shape_file);
		}
		
		in->layer[i].s.epi_index=i;

		obj_pl=json_obj_find(obj_layer, "shape_pl");
		epitaxy_load_pl_file(sim,&(in->layer[i]),obj_pl);

		in->layer[i].width=fabs(s->dy);
		s->y0=y_pos;

		//interface file

		//json_dump_obj(obj_layer);
		obj_interface=json_obj_find(obj_layer, "layer_interface");
		epitaxy_load_interface_file(sim,&(in->layer[i]),obj_interface);

		json_get_english(sim,obj_layer, &(in->layer[i].solve_optical_problem),"solve_optical_problem");

		json_get_english(sim,obj_layer, &(in->layer[i].solve_thermal_problem),"solve_thermal_problem");

		//json_get_long_double(sim,obj_layer, &(in->layer[i].Gnp),"Gnp");
		char temp[20];
		char full_path[PATH_MAX];
		if (in->layer[i].layer_type==LAYER_ACTIVE)
		{
			if (in->device_start==-1.0)
			{
				in->device_start=y_pos;
			}

			in->layer[i].electrical_layer=TRUE;
		}else
		{
			in->layer[i].electrical_layer=FALSE;
		}

		in->layer[i].y_start=y_pos;
		y_pos+=in->layer[i].width;
		in->layer[i].y_stop=y_pos;
	}

	in->device_stop=epitaxy_get_device_stop(in);


	epitaxy_shapes_load(sim,in,obj_epi);



}

/**
 * @brief Get the height of the layers
 *
 */
long double epitaxy_get_total_length_y(struct epitaxy *in)
{
int i=0;
gdouble tot=0.0;

for (i=0;i<in->layers;i++)
{
	tot+=in->layer[i].width;
}

return tot;
}

/**
 * @brief Get the height of the layers which are electrically active.
 *
 */
gdouble epitaxy_get_electrical_length(struct epitaxy *in)
{
int i=0;
gdouble tot=0.0;

for (i=0;i<in->layers;i++)
{
	if (in->layer[i].electrical_layer==TRUE)
	{
		tot+=in->layer[i].width;
	}
}

return tot;
}

/**
 * @brief Return the index of a layer in the device.
 *
 */
int epitaxy_get_layer(struct epitaxy *in,gdouble pos)
{
int i=0;
long double layer_end=0.0;//-in->device_start;
for (i=0;i<in->layers;i++)
{
	layer_end+=in->layer[i].width;

	if (pos<=layer_end)
	{
		return i;
	}

}

return -1;
}


/**
 * @brief Return the electrical layer number of a layer from a y cordinate.
 *
 */
/*int epitaxy_get_electrical_material_layer(struct epitaxy *in,gdouble pos)
{
int i=0;
gdouble layer_end=0.0;
int electrical_layer=0;
for (i=0;i<in->layers;i++)
{
	if (in->layer[i].electrical_layer==TRUE)
	{
		layer_end+=in->layer[i].width;

		if (pos<layer_end)
		{
			return electrical_layer;
		}
		electrical_layer++;
	}

}


return -1;
}*/

/**
 * @brief Return the layer number of a layer from a y cordinate.
 *
 */
int epitaxy_get_epitaxy_layer_using_electrical_pos(struct epitaxy *in,gdouble pos)
{
int i=0;
gdouble layer_end=0.0;
int electrical_layer=0;
for (i=0;i<in->layers;i++)
{
	if (in->layer[i].electrical_layer==TRUE)
	{
		layer_end+=in->layer[i].width;

		if (pos<layer_end)
		{
			return i;
		}
		electrical_layer++;
	}

}

return -1;
}

/**
 * @brief Return the position of the start of the first electrical layer in the device.
 *
 */
gdouble epitaxy_get_device_start(struct epitaxy *in)
{
int i=0;
gdouble pos=0.0;
for (i=0;i<in->layers;i++)
{

	if (in->layer[i].electrical_layer==TRUE)
	{
		return pos;
	}
	pos+=in->layer[i].width;

}

return -1;
}

/**
 * @brief Return the position of the end of the electrical layers in the device.
 *
 */
long double epitaxy_get_device_stop(struct epitaxy *in)
{
int i=0;
gdouble pos=0.0;
int found=FALSE;
for (i=0;i<in->layers;i++)
{

	if (in->layer[i].electrical_layer==TRUE)
	{
		found=TRUE;
	}

	if ((in->layer[i].electrical_layer==FALSE)&&(found==TRUE))
	{
		return pos;
	}

	pos+=in->layer[i].width;
}

if (found==TRUE)
{
	return pos;
}


return -1;
}

/**
 * @brief Return the index of the layer where the device starts.
 *
 */
gdouble epitaxy_get_device_start_i(struct epitaxy *in)
{
int i=0;
for (i=0;i<in->layers;i++)
{

	if (in->layer[i].electrical_layer==TRUE)
	{
		return i;
	}

}

return -1;
}


void epitaxy_mask(struct simulation *sim,struct device *dev)
{
int x=0;
int y=0;
int z=0;
int c=0;
int l=0;
long double y_pos=0.0;
int in_shape;

if (dev->electrical_simulation_enabled==FALSE)
{
	return;
}

struct dimensions *dim=&(dev->ns.dim);
struct epitaxy *epi=&(dev->my_epitaxy);
struct contact *cont;
struct epi_layer *layer;

if (dim->ylen==0)
{
	return;
}

if ((dim->xlen==1)&&(dim->zlen==1))
{
	for (z=0;z<dim->zlen;z++)
	{
		for (x=0;x<dim->xlen;x++)
		{
			for (y=0;y<dim->ylen;y++)
			{
				dev->mask[z][x][y]=TRUE;
			}

			for (l=0;l<epi->layers;l++)
			{	
				dev->mask_epitaxy[z][x][l]=TRUE;
			}
		}
	}
	return;
}

	for (z=0;z<dim->zlen;z++)
	{
		for (x=0;x<dim->xlen;x++)
		{

			//for (y=0;y<dim->ylen;y++)
			//{

			for (l=0;l<epi->layers;l++)
			{
				layer=&(dev->my_epitaxy.layer[l]);
				y_pos=layer->y_start+(layer->y_stop-layer->y_start)/2.0;
				//l=dev->imat_epitaxy[z][x][y];
				dev->mask_epitaxy[z][x][l]=TRUE;

				if (layer->layer_type==LAYER_CONTACT)
				{
					dev->mask_epitaxy[z][x][l]=FALSE;

					for (c=0;c<dev->ncontacts;c++)
					{
						cont=&(dev->contacts[c]);
						in_shape=shape_in_shape(sim,&(cont->shape),dim->zmesh[z],dim->xmesh[x],y_pos);
						if (in_shape==0)
						{
							//dev->mask[z][x][y]=TRUE;
							dev->mask_epitaxy[z][x][l]=TRUE;
						}

						//printf("%d %s %d\n",c,cont->name,in_shape);
					}
				}

				//printf("%d %d\n",l,dev->mask_epitaxy[z][x][l]);
				//getchar();

			}

			//printf("fin\n");
		}
	}

return;
}

