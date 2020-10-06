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


void epi_layer_cpy(struct simulation *sim,struct epi_layer *out,struct epi_layer *in)
{
	int i;

	out->layer_number=in->layer_number;
	out->y_start=in->y_start;
	out->y_stop=in->y_stop;

	//we are no doing shapes
	shape_init(sim,&(out->s));
	for (i=0;i<10;i++)
	{
		shape_init(sim,&(out->shapes[i]));
	}

	out->nshape=in->nshape;
	out->width=in->width;
	strcpy(out->pl_file,in->pl_file);
	out->pl_use_experimental_emission_spectra=in->pl_use_experimental_emission_spectra;
	out->pl_experimental_emission_efficiency=in->pl_experimental_emission_efficiency;
	out->pl_enabled=in->pl_enabled;
	out->pl_fe_fh=in->pl_fe_fh;
	out->pl_fe_te=in->pl_fe_te;
	out->pl_te_fh=in->pl_te_fh;
	out->pl_th_fe=in->pl_th_fe;
	out->pl_fh_th=in->pl_fh_th;
	strcpy(out->pl_spectrum_file,in->pl_spectrum_file);
	inter_copy(&(out->pl_spectrum),&(in->pl_spectrum),TRUE);

	out->photon_extract_eff=NULL;
	out->photon_extract_eff_count=NULL;
	out->avg_photon_extract_eff=in->avg_photon_extract_eff;
	out->peak_wavelength=in->peak_wavelength;

	out->electrical_layer=in->electrical_layer;

	strcpy(out->dos_file,in->dos_file);
	out->layer_type=in->layer_type;

	strcpy(out->interface_file,in->interface_file);
	out->interface_type=in->interface_type;
	out->interface_R=in->interface_R;

	out->rgb[0]=in->rgb[0];
	out->rgb[1]=in->rgb[1];
	out->rgb[2]=in->rgb[2];

	out->G_percent=in->G_percent;

	out->Gnp=in->Gnp;

	out->solve_optical_problem=in->solve_optical_problem;
	out->solve_thermal_problem=in->solve_thermal_problem;
}


void epitaxy_cpy(struct simulation *sim,struct epitaxy *out,struct epitaxy *in)
{
	int i;
	int l;
	int layer_max=20;

	out->layers=in->layers;
	out->device_start=in->device_start;
	out->device_stop=in->device_stop;
	out->electrical_layers=in->electrical_layers;

	//electrical layres including shapes


	for (l=0;l<layer_max;l++)
	{
		strcpy(out->lumo_file[l],in->lumo_file[l]);
		strcpy(out->homo_file[l],in->homo_file[l]);
		strcpy(out->shape_file[l],in->shape_file[l]);
		epi_layer_cpy(sim,&(out->layer[l]),&(in->layer[l]));
	}


	for (l=0;l<out->electrical_layers;l++)
	{
		dos_cpy(&(out->dosn[l]),&(in->dosn[l]));
		dos_cpy(&(out->dosp[l]),&(in->dosp[l]));
	}


}


