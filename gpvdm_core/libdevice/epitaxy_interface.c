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

/** @file epitaxy_interface.c
	@brief Epitaxy interface
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
#include <lock.h>

void epitaxy_load_interface_file(struct simulation *sim, struct epi_layer *layer,struct json_obj *interface_json)
{
	if (interface_json==NULL)
	{
		ewe(sim,"interface file does not exist\n");
	}

	json_get_english(sim,interface_json, &(layer->interface_type),"interface_model");

	json_get_long_double(sim,interface_json, &(layer->interface_R),"interface_eh_tau");

	if (layer->interface_type==INTERFACE_NONE)
	{
		layer->interface_R=0.0;
	}

	//Tuneling
	json_get_english(sim,interface_json, &(layer->interface_tunnel_e),"interface_tunnel_e");
	json_get_long_double(sim,interface_json, &(layer->interface_Ge),"interface_Ge");
	layer->interface_Ge=fabsl(layer->interface_Ge);

	if (layer->interface_tunnel_e==FALSE)
	{
		layer->interface_Ge=0.0;
	}

	json_get_english(sim,interface_json, &(layer->interface_tunnel_h),"interface_tunnel_h");
	json_get_long_double(sim,interface_json, &(layer->interface_Gh),"interface_Gh");
	layer->interface_Gh=fabsl(layer->interface_Gh);

	if (layer->interface_tunnel_h==FALSE)
	{
		layer->interface_Gh=0.0;
	}

	//Doping
	json_get_english(sim,interface_json, &(layer->interface_left_doping_enabled),"interface_left_doping_enabled");
	json_get_long_double(sim,interface_json, &(layer->interface_left_doping),"interface_left_doping");

	json_get_english(sim,interface_json, &(layer->interface_right_doping_enabled),"interface_right_doping_enabled");
	json_get_long_double(sim,interface_json, &(layer->interface_right_doping),"interface_right_doping");

}

void epitaxy_setup_interfaces(struct simulation *sim,struct device *dev)
{
int x=0;
int y=0;
int z=0;
int l0=0;
int l1=0;

if (dev->drift_diffision_simulations_enabled==FALSE)
{
	return;
}

struct dimensions *dim=&(dev->ns.dim);
struct epitaxy *epi=&(dev->my_epitaxy);
dev->interfaces_n=0;
dev->interfaces_n_srh=0;
dev->interfaces_tunnels_e=0;
dev->interfaces_tunnels_h=0;

	if (dim->ylen==0)
	{
		return;
	}

	for (z=0;z<dim->zlen;z++)
	{
		for (x=0;x<dim->xlen;x++)
		{
			for (y=0;y<dim->ylen-1;y++)
			{
				l0=dev->imat_epitaxy[z][x][y];
				l1=dev->imat_epitaxy[z][x][y+1];
				

				if (l0!=l1)
				{
					dev->interface_B[z][x][y]=0.0;
					dev->interface_Bt[z][x][y]=0.0;

					if (epi->layer[l0].interface_type!=INTERFACE_NONE)
					{
						if (epi->layer[l0].interface_type==INTERFACE_RECOMBINATION)
						{
							dev->interface_B[z][x][y]=epi->layer[l0].interface_R;
							dev->interfaces_n++;
						}else
						if (epi->layer[l0].interface_type==INTERFACE_RECOMBINATION_SRH)
						{
							dev->interface_Bt[z][x][y]=epi->layer[l0].interface_R;
							dev->interfaces_n_srh++;
						}


					}

					if (epi->layer[l0].interface_tunnel_e==TRUE)
					{
						dev->interface_Ge[z][x][y]=epi->layer[l0].interface_Ge;
						dev->interfaces_tunnels_e++;
					}

					if (epi->layer[l0].interface_tunnel_h==TRUE)
					{
						dev->interface_Gh[z][x][y]=epi->layer[l0].interface_Gh;
						dev->interfaces_tunnels_h++;
					}

					dev->interface_type[z][x][y]=epi->layer[l0].interface_type;

				}
			}
		}
	}


}


