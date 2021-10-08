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

/** @file lib_fxdomain.h
@brief Code to read input files.
*/

#ifndef lib_fxdomain_h
#define lib_fxdomain_h
#include "advmath.h"
#include "inp_struct.h"
#include <sim_struct.h>
#include "list_struct.h"


struct fxdomain
{
	//output data
	struct math_xy out_modulation_cut;
	struct math_xy out_modulation;
	struct math_xy out_j_cut;
	struct math_xy out_j;
	struct math_xy out_v_cut;
	struct math_xy out_v;


	//Overall results
	struct math_xy real_imag;
	struct math_xy fx_real;
	struct math_xy fx_imag;
	struct math_xy j_fit_error;
	struct math_xy mod_fit_error;
	struct math_xy phi;

	int fxdomain_sim_mode;
	int fxdomain_points;
	int fxdomain_n;
	long double fxdomain_Vexternal;
	long double fxdomain_voltage_modulation_max;
	long double fxdomain_light_modulation_depth;
	char fxdomain_modulation_type[100];
	int fxdomain_measure;
	long double fxdomain_L;

	//roll off
	int fxdomain_modulation_rolloff_enable;
	long double fxdomain_modulation_rolloff_start_fx;
	long double fxdomain_modulation_rolloff_speed;

	//imps
	int fxdomain_norm_tx_function;

	//dump verbosity
	int fxdomain_dump_verbocity;
	int fxdomain_screen_verbocity;
	char snapshot_path[PATH_MAX];
	char prefix_result[20];
	char prefix_modulation[20];

	long double fx;
	int modulate_voltage;
	int total_steps;

	//fitting
	int fxdomain_do_fit;
	int periods_to_fit;
	long double last_real;
	long double last_imag;
	long double last_mag;
	long double last_phi;
	long double last_j_error;
	long double last_v_error;
	long double last_mod_error;
	int simulation_type;


	//mesh
	long double *fx_mesh;
	int mesh_len;
	int mesh_pos;


};

#endif
