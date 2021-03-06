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
#include <dos.h>

void epi_layer_init(struct simulation *sim,struct epi_layer *layer)
{
	int i;

	layer->layer_number=-1;
	layer->y_start=-1;
	layer->y_stop=-1;
	shape_init(sim,&(layer->s));
	for (i=0;i<10;i++)
	{
		shape_init(sim,&(layer->shapes[i]));
	}

	layer->nshape=-1;
	layer->width=-1;
	strcpy(layer->pl_file,"none");
	layer->pl_use_experimental_emission_spectra=-1;
	layer->pl_experimental_emission_efficiency=-1;
	layer->pl_enabled=-1;
	layer->pl_fe_fh=-1;
	layer->pl_fe_te=-1;
	layer->pl_te_fh=-1;
	layer->pl_th_fe=-1;
	layer->pl_fh_th=-1;
	strcpy(layer->pl_spectrum_file,"none");
	inter_init(sim,&(layer->pl_spectrum));
	layer->photon_extract_eff=NULL;
	layer->photon_extract_eff_count=NULL;
	layer->avg_photon_extract_eff=-1;
	layer->peak_wavelength=-1;

	//struct component com;		not initing this one

	layer->electrical_layer=-1;

	strcpy(layer->dos_file,"none");
	layer->layer_type=-1;
	strcpy(layer->interface_file,"none");
	layer->interface_type=0;
	layer->interface_R=0.0;

	layer->rgb[0]=-1;
	layer->rgb[1]=-1;
	layer->rgb[2]=-1;

	layer->G_percent=-1;

	layer->Gnp=-1;

	layer->solve_optical_problem=-1;
	layer->solve_thermal_problem=-1;
}


void epitaxy_init(struct simulation *sim,struct epitaxy *epi)
{
	int i;
	int l;
	int layer_max=20;

	epi->layers=-1;
	epi->device_start=-1;
	epi->device_stop=-1;
	epi->electrical_layers=-1;

	//electrical layres including shapes


	for (l=0;l<layer_max;l++)
	{
		strcpy(epi->lumo_file[l],"");
		strcpy(epi->homo_file[l],"");
		strcpy(epi->shape_file[l],"");
		epi_layer_init(sim,&(epi->layer[l]));
	}

	for (l=0;l<10;l++)
	{
		//printf("here\n");
		//getchar();
		dos_init(&(epi->dosn[l]));
		dos_init(&(epi->dosp[l]));
	}

}


