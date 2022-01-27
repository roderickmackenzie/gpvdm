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
	layer->pl_use_experimental_emission_spectra=-1;
	layer->pl_experimental_emission_efficiency=-1;
	layer->pl_enabled=-1;
	layer->pl_fe_fh=-1;
	layer->pl_fe_te=-1;
	layer->pl_te_fh=-1;
	layer->pl_th_fe=-1;
	layer->pl_fh_th=-1;

	//ray tracing
	layer->theta_steps=-1;
	layer->theta_start=-1.0;
	layer->theta_stop=-1.0;
	layer->phi_steps=-1;
	layer->phi_start=-1.0;
	layer->phi_stop=-1.0;
	layer->emission_source=-1;

	strcpy(layer->pl_spectrum_file,"none");
	inter_init(sim,&(layer->pl_spectrum));
	layer->photon_extract_eff=NULL;
	layer->photon_extract_eff_count=NULL;
	layer->avg_photon_extract_eff=-1;
	layer->peak_wavelength=-1;

	//struct component com;		not initing this one

	layer->electrical_layer=-1;

	//strcpy(layer->dos_file,"none");
	//Interface
	layer->layer_type=-1;
	layer->interface_type=0;
	layer->interface_R=0.0;

	layer->interface_left_doping_enabled=-1;
	layer->interface_left_doping=0.0;

	layer->interface_right_doping_enabled=-1;
	layer->interface_right_doping=0.0;

	//Tunnel
	layer->interface_tunnel_e=-1;
	layer->interface_Ge=0.0;

	layer->interface_tunnel_h=-1;
	layer->interface_Gh=0.0;

	layer->rgb[0]=-1;
	layer->rgb[1]=-1;
	layer->rgb[2]=-1;

	layer->G_percent=-1;

	layer->solve_optical_problem=-1;
	layer->solve_thermal_problem=-1;
}


void epitaxy_init(struct simulation *sim,struct epitaxy *epi)
{
	//int i;
	int l;
	int layer_max=20;

	epi->layers=-1;
	epi->device_start=-1;
	epi->device_stop=-1;

	//electrical layres including shapes


	for (l=0;l<layer_max;l++)
	{
		//strcpy(epi->shape_file[l],"");
		epi_layer_init(sim,&(epi->layer[l]));
	}

}


