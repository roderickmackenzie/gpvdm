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

/** @file heat.h
@brief a structure for the heat model
*/

#ifndef h_heat
#define h_heat
#include <complex.h>
#include "advmath.h"
#include "i.h"
#include <sim_struct.h>
#include <epitaxy_struct.h>
#include <ray.h>
#include <matrix.h>
#include <object.h>
#include <dat_file_struct.h>
#include <dim.h>
#include <mesh_struct.h>
#include <heat_material.h>


struct heat
{
	char dump_dir[PATH_MAX];
	struct dim_heat dim;
	int thermal_model_type;
	int dump_verbosity;

	//zxy
	long double ***Tl;
	long double ***Te;
	long double ***Th;

	long double ***Hl;

	long double ***H_optical;
	long double ***H_joule;
	long double ***H_parasitic;
	long double ***H_recombination;

	long double ***He;
	long double ***Hh;

	long double ***kl;
	long double ***ke;
	long double ***kh;

	//objects
	struct object ****obj;


	struct matrix mx;

	//boundry temperatures
	long double Ty0;
	long double Ty1;
	long double Tx0;
	long double Tx1;
	long double Tz0;
	long double Tz1;

	//Boundry type
	int Ty0_boundry;
	int Ty1_boundry;
	int Tx0_boundry;
	int Tx1_boundry;
	int Tz0_boundry;
	int Tz1_boundry;

	//heat sink
	long double heatsink_y0;
	long double heatsink_y1;
	long double heatsink_x0;
	long double heatsink_x1;
	long double heatsink_z0;
	long double heatsink_z1;

	long double heatsink_length_y0;
	long double heatsink_length_y1;
	long double heatsink_length_x0;
	long double heatsink_length_x1;
	long double heatsink_length_z0;
	long double heatsink_length_z1;

	int Tliso;
	int Triso;
	int nofluxl;

	//convergance
	int thermal_conv;
	long double min_error;
	int newton_enable_external_thermal;
	int thermal_l;
	int thermal_e;
	int thermal_h;
	int thermal_max_ittr;
	int thermal_couple_to_electrical_solver;
	struct mesh_obj mesh_data;

	//Options
	int joule_heating;
	int parasitic_heating;
	int recombination_heating;
	int optical_heating;

	int solver_verbosity;

};

#endif
