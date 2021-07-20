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
	struct inp_file inp;
	char full_path[PATH_MAX];
	char temp_path[PATH_MAX];
	char temp[100];

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

					dev->interface_type[z][x][y]=epi->layer[l0].interface_type;

				}
			}
		}
	}


}


