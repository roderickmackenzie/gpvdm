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

/** @file gpvdm_const.h
	@brief Physical constants.
*/

#ifndef h_gpvdm_const
#define h_gpvdm_const

#include <enabled_libs.h>

//Physical constants
#define STR_MAX	1024
#define epsilon0 (long double)8.85418782e-12			// m-3 kg-1 s4 A2;
#define epsilon0f (float)8.85418782e-12			// m-3 kg-1 s4 A2;

#define mu0f (float)1.25663706e-6			//

#define hp (long double)6.62606896e-34			//J S Wikipeda
#define PI (long double)3.14159265358979323846

#define PIf (float)3.14159265358979323846

#define hbar (long double)(6.62606896e-34/(2.0*PI))	//Calculated
#define kb (long double)1.3806504e-23			//J K-1 Wiki
#define Qe (long double)1.602176487e-19			//C Wikipeda
#define m0 (long double)9.10938215e-31 			//Kg Wikipeda
#define cl  (long double)2.99792458e8			//m/s Wikipieda
#define clf  (float)2.99792458e8			//m/s Wikipieda


//SRH constants
#define srh_1	1
#define srh_2	2
#define srh_3	3
#define srh_4	4
#define interface_schottky	 1

//Interfaces
#define INTERFACE_NONE				0
#define INTERFACE_RECOMBINATION		1
#define INTERFACE_RECOMBINATION_SRH	2
//TRUE/FALSE
#define TRUE 1
#define FALSE 0



#define TOP 0
#define BOTTOM 1
#define RIGHT 2
#define LEFT 3

#define ELECTRON 0
#define HOLE 1

#define FIT_SIMPLEX 0
#define FIT_NEWTON 1
#define FIT_BFGS 2

#define FIT_NOT_FITTING 0
#define FIT_SINGLE_FIT 1
#define FIT_RUN_FIT 2

//tpv light
#define tpv_set_light 0
#define tpv_set_voltage 1
#define tpv_mode_laser	0
#define tpv_mode_sun 1

//sim modes
#define IDEAL_DIODE_IDEAL_LOAD 2
#define LOAD 1
#define OPEN_CIRCUIT 0


//dump control
#define dump_newton 2
#define dump_plot 3
#define dump_stop_plot 4
#define dump_opt_for_fit 5
#define dump_write_converge 6
#define dump_print_text 7
#define dump_exit_on_dos_error 8
#define dump_lock 11
#define dump_norm_time_to_one 12
#define dump_band_structure 14
#define dump_first_guess 17
#define dump_1d_slices 18
#define dump_print_pos_error 19
#define dump_dynamic 22
#define dump_norm_y_axis 24
#define dump_write_out_band_structure 25
#define dump_optical_probe 27
#define dump_info_text 28
#define dump_optical_probe_spectrum 29
#define dump_ray_trace_map 31
#define dump_use_cache 34
#define dump_write_headers 37
#define dump_remove_dos_cache 38

//Atempt to put output in files
#define dump_nothing				-1
#define dump_verbosity_key_results	0	
#define dump_verbosity_everything	1

//energy space dump
#define ENERGY_SPACE_MAP 1
#define SINGLE_MESH_POINT 2

//dos types
#define dos_exp		0
#define dos_an		1
#define dos_fd		2
#define dos_exp_fd 	3
#define dos_read 	5

//fx_domain signal_types
#define LARGE_SIGNAL	0
#define SMALL_SIGNAL	1
#define FOURIER			2

//free dos types
#define mb_equation 0
#define mb_look_up_table 1
#define fd_look_up_table 2
#define mb_look_up_table_analytic 3

//contact types
#define contact_ohmic 0
#define contact_schottky 1

//Ray tracer
#define ray_run_never	0
#define ray_run_once    1
#define ray_run_step	2
#define RAY_SIM_EDGE	0
#define RAY_VIEWPOINT	1
#define RAY_OBJECT		2
#define ray_emission_single_point 0
#define ray_emission_electrical_mesh 1

#define CONSTANT 0
#define INTERPOLATE 2
#define INTERPOLATE2 3

//Heat model
#define THERMAL_HYDRODYNAMIC 	0
#define THERMAL_LATTICE 			1

#define ISOTHERMAL	0
#define DIRICHLET	0
#define NEUMANN		1
#define HEATSINK	2

//Heat fdtd
#define FDTD_SIN 	0
#define FDTD_PULSE 	0


#define measure_voltage		0
#define measure_current		1

#define LAYER_ACTIVE 	0
#define LAYER_CONTACT 	1
#define LAYER_OTHER		2

	#include <linux/limits.h>

#define WAIT 0
#define READY 1
#define DONE 2

#define TRUE 1
#define FALSE 0

	#define THREAD_NULL	-1
	#define THREAD_FUNCTION void *


#define SOLVER_VERBOSITY_NOTHING 0
#define SOLVER_VERBOSITY_AT_END 1
#define SOLVER_VERBOSITY_EVERY_STEP 2

//spm
#define SPM_WHOLE_DEVICE 0
#define SPM_BOX 1
#define SPM_X_CUT 2

#define GPVDM_FLOAT 0
#define GPVDM_DOUBLE 1
#define GPVDM_LONG_DOUBLE 2


#endif
