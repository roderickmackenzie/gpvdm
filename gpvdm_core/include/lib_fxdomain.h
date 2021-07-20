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
