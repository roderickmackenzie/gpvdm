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
	char spectrum_file[100];
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

	//ray tracing
	json_get_int(sim,pl_json, &(layer->theta_steps),"ray_theta_steps");
	json_get_double(sim,pl_json, &(layer->theta_start),"ray_theta_start");
	json_get_double(sim,pl_json, &(layer->theta_stop),"ray_theta_stop");

	json_get_int(sim,pl_json, &(layer->phi_steps),"ray_phi_steps");
	json_get_double(sim,pl_json, &(layer->phi_start),"ray_phi_start");
	json_get_double(sim,pl_json, &(layer->phi_stop),"ray_phi_stop");
	json_get_english(sim, pl_json, &(layer->emission_source),"ray_emission_source");

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
	layer->avg_photon_extract_eff=1.0;

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
						in_shape=shape_in_shape(sim,&(cont->shape),dim->z[z],dim->x[x],y_pos);
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

