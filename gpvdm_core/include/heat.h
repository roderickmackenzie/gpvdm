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
	struct dimensions dim;
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
