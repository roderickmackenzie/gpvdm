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

/** @file exciton.h
@brief a structure for the heat model
*/

#ifndef h_exciton
#define h_exciton
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
#include <exciton_material.h>


struct exciton
{
	char dump_dir[PATH_MAX];
	struct dimensions dim;
	int dump_verbosity;

	//zxy
	long double ***n;		//Exciton density
	long double ***G;		//Generation
	long double ***Gn;		//Generation
	long double ***Gp;		//Generation
	long double ***D;		//Diffusion constant
	long double ***L;		//Diffusion length
	long double ***tau;		//lifetime of exciton
	long double ***k_pl;	//radiative decay rate in absence of quencher sites
	long double ***k_fret;	//denotes the rate of Förster resonance energy transfer (FRET) in the presence of a neighboring material.
	long double ***alpha;	//exciton–exciton annihilation rate constant
	long double ***k_dis;	//Dissociation

	//Saved rates
	long double ***Rk_pl;
	long double ***Rk_fret;
	long double ***Ralpha;
	long double ***Rk_dis;

	//objects
	struct object ****obj;


	struct matrix mx;

	//boundry temperatures
	long double n_y0;
	long double n_y1;
	long double n_x0;
	long double n_x1;
	long double n_z0;
	long double n_z1;

	//Boundry type
	int y0_boundry;
	int y1_boundry;
	int x0_boundry;
	int x1_boundry;
	int z0_boundry;
	int z1_boundry;

	//heat sink
	long double excitonsink_y0;
	long double excitonsink_y1;
	long double excitonsink_x0;
	long double excitonsink_x1;
	long double excitonsink_z0;
	long double excitonsink_z1;

	long double excitonsink_length_y0;
	long double excitonsink_length_y1;
	long double excitonsink_length_x0;
	long double excitonsink_length_x1;
	long double excitonsink_length_z0;
	long double excitonsink_length_z1;

	//convergance
	int ex_conv;
	long double min_error;
	int exciton_enabled;
	int exciton_max_ittr;
	int exciton_couple_to_electrical_solver;
	int solver_verbosity;
	struct mesh_obj mesh_data;

};

#endif
