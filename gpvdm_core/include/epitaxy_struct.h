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
	double theta_start;
	double theta_stop;
	int phi_steps;
	double phi_start;
	double phi_stop;
	int emission_source;

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

	int interface_tunnel_e;
	long double interface_Ge;

	int interface_tunnel_h;
	long double interface_Gh;

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
