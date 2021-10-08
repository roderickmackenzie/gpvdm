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
#include <dos.h>


void epi_layer_cpy(struct simulation *sim,struct epi_layer *out,struct epi_layer *in)
{
	int i;

	out->layer_number=in->layer_number;
	out->y_start=in->y_start;
	out->y_stop=in->y_stop;

	//we are no doing shapes
	shape_cpy(sim,&(out->s),&(in->s));

	for (i=0;i<10;i++)
	{
		shape_cpy(sim,&(out->shapes[i]),&(in->shapes[i]));
	}

	out->nshape=in->nshape;
	out->width=in->width;
	out->pl_use_experimental_emission_spectra=in->pl_use_experimental_emission_spectra;
	out->pl_experimental_emission_efficiency=in->pl_experimental_emission_efficiency;
	out->pl_enabled=in->pl_enabled;
	out->pl_fe_fh=in->pl_fe_fh;
	out->pl_fe_te=in->pl_fe_te;
	out->pl_te_fh=in->pl_te_fh;
	out->pl_th_fe=in->pl_th_fe;
	out->pl_fh_th=in->pl_fh_th;

	//ray tracing
	out->theta_steps=in->theta_steps;
	out->theta_start=in->theta_start;
	out->theta_stop=in->theta_stop;
	out->phi_steps=in->phi_steps;
	out->phi_start=in->phi_start;
	out->phi_stop=in->phi_stop;
	out->emission_source=in->emission_source;
	strcpy(out->pl_spectrum_file,in->pl_spectrum_file);
	inter_copy(&(out->pl_spectrum),&(in->pl_spectrum),TRUE);

	out->photon_extract_eff=NULL;
	out->photon_extract_eff_count=NULL;
	out->avg_photon_extract_eff=in->avg_photon_extract_eff;
	out->peak_wavelength=in->peak_wavelength;

	out->electrical_layer=in->electrical_layer;

	//strcpy(out->dos_file,in->dos_file);
	out->layer_type=in->layer_type;

	out->interface_type=in->interface_type;
	out->interface_R=in->interface_R;

	out->interface_left_doping_enabled=in->interface_left_doping_enabled;
	out->interface_left_doping=in->interface_left_doping;

	out->interface_right_doping_enabled=in->interface_right_doping_enabled;
	out->interface_right_doping=in->interface_right_doping;

	out->rgb[0]=in->rgb[0];
	out->rgb[1]=in->rgb[1];
	out->rgb[2]=in->rgb[2];

	out->G_percent=in->G_percent;


	out->solve_optical_problem=in->solve_optical_problem;
	out->solve_thermal_problem=in->solve_thermal_problem;
}


void epitaxy_cpy(struct simulation *sim,struct epitaxy *out,struct epitaxy *in)
{
	//int i;
	int l;
	int layer_max=20;

	out->layers=in->layers;
	out->device_start=in->device_start;
	out->device_stop=in->device_stop;

	//electrical layres including shapes


	for (l=0;l<layer_max;l++)
	{
		//strcpy(out->shape_file[l],in->shape_file[l]);
		epi_layer_cpy(sim,&(out->layer[l]),&(in->layer[l]));
	}


}


