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


/** @file epitaxy.h
	@brief Read the epitaxy from the epitaxy.inp file.
*/


#ifndef epitaxy_struct_h
#define epitaxy_struct_h
#include "advmath.h"
#include <sim_struct.h>
#include <shape_struct.h>
#include <component.h>
#include <dos_struct.h>

struct epi_layer
{
	int layer_number;
	long double y_start;
	long double y_stop;
	struct shape s;		//this shape
	struct shape shapes[10];
	int nshape;
	long double width;
	int pl_use_experimental_emission_spectra;
	long double pl_experimental_emission_efficiency;

	int pl_enabled;
	long double pl_fe_fh;
	long double pl_fe_te;
	long double pl_te_fh;
	long double pl_th_fe;
	long double pl_fh_th;

	//ray tracing
	int theta_steps;
	double ray_theta_start;
	double ray_theta_stop;

	int phi_steps;
	double ray_phi_start;
	double ray_phi_stop;

	char pl_spectrum_file[PATH_MAX];
	struct math_xy pl_spectrum;
	double *photon_extract_eff;
	double *photon_extract_eff_count;
	long double avg_photon_extract_eff;
	long double peak_wavelength;

	int electrical_layer;

	int layer_type;

	int interface_type;
	long double interface_R;

	int interface_left_doping_enabled;
	long double interface_left_doping;

	int interface_right_doping_enabled;
	long double interface_right_doping;

	long double rgb[3];

	//Generation
	long double G_percent;		//Percentage of light absorbed in each layer

	int solve_optical_problem;
	int solve_thermal_problem;

};

struct epitaxy
{
	int layers;
	struct epi_layer layer[20];
	long double device_start;
	long double device_stop;

};

#endif
