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

/** @file light.h
@brief light functions from liblight
*/

#ifndef h_light
#define h_light
#include <complex.h>
#include "advmath.h"
#include "i.h"
#include <sim_struct.h>
#include <epitaxy_struct.h>
#include <ray.h>
#include <matrix.h>
#include <matrix_solver_memory.h>
#include <object.h>
#include <dat_file_struct.h>
#include <dim.h>

struct light_sources
{
	int nlight_sources;
	struct light_src *light_sources;
	long double lstart;
	long double lstop;
	int llen;

	//for EQE
	int use_flat_sepctrum;			
};

struct light_src
{
	int nspectra;
	struct math_xy spectra_tot;
	struct math_xy spectra_tot_no_filter;
	struct math_xy *spectra;
	double light_multiplyer[20];
	//for EQE
	int use_flat_sepctrum;			


	//filter
	int filter_enabled;
	char filter_path[PATH_MAX];
	struct math_xy filter_read;
	long double filter_dB;
	int filter_invert;
	long double local_ground_view_factor;

	//external interface
	long double n;
	int external_interface_enabled;
	char id[100];
	long double lstart;
	long double lstop;

	//config
	char illuminate_from[20];
	long double x0;
	long double y0;
	long double z0;

	int theta_steps;
	long double theta_start;
	long double theta_stop;

	int phi_steps;
	long double phi_start;
	long double phi_stop;


};

struct light
{
	//output files
	char dump_dir[PATH_MAX];
	struct dimensions dim;

	//long double zxyl
	float ****Ep;
	float ****Epz;
	float ****En;
	float ****Enz;
	float ****n;
	float ****alpha0;
	float ****alpha;
	double ****photons;
	double ****photons_asb;
	//float ****pointing_vector;
	//float ****E_tot_r;
	//float ****E_tot_i;
	float ****H;

	//complex zxyl
	float complex ****t;
	float complex ****r;
	float complex ****nbar;

	//zxy_p_object
	struct object ****obj;

	//3D arrrays
	long double ***Gn;
	long double ***Gp;
	long double ***Htot;
	long double ***photons_tot;


	//1D arrays
	long double *reflect;
	long double *transmit;

	//for EQE
	int use_flat_sepctrum;			

	//Input spectra
	struct light_src light_src_y0;
	struct light_src light_src_y1;

	long double *sun_y0;
	long double *sun_y1;
	long double *sun_photons_y0;
	long double *sun_photons_y1;

	//with no filter
	long double *sun_y0_no_filter;
	long double *sun_y1_no_filter;
	long double *sun_photons_y0_no_filter;
	long double *sun_photons_y1_no_filter;

	//electricl field
	long double *sun_E_y0;
	long double *sun_E_y1;
	char suns_spectrum_file[200];
	char light_file_generation[300];

	//matrix
	int worker_max;
	struct matrix *mx;
	struct matrix_solver_memory *msm;

	//laser
	long double laser_wavelength;
	int laser_pos;
	long double ND;
	long double spotx;
	long double spoty;
	long double pulseJ;
	long double pulse_width;

	//long double device_ylen;
	long double Psun;
	long double laser_eff;
	long double simplephotondensity;
	long double simple_alpha;
	long double Dphotoneff;

	//Dll section
	void (*fn_init)();
	void (*fn_solve_and_update)();
	int (*fn_solve_lam_slice)();
	long double (*fn_cal_photon_density)();
	void (*light_ver)();
	void *lib_handle;

	//config
	long double lstart;
	long double lstop;
	char mode[20];
	long double electron_eff;
	long double hole_eff;
	int force_update;
	int light_wavelength_auto_mesh;
	long double *extract_eff;

	//Config values
	int align_mesh;
	int disable_transfer_to_electrical_mesh;
	int disable_cal_photon_density;
	long double light_file_generation_shift;

	int print_wavlengths;

	int finished_solveing;

	long double last_Psun;
	long double last_laser_eff;
	long double last_wavelength_laser;


	struct epitaxy *epi;

	char light_profile[200];
	struct triangles light_profile_tri;

	char snapshot_path[PATH_MAX];
	int dump_verbosity;
};

#endif
