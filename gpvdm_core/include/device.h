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

/** @file device.h
	@brief The main structure which holds information about the device.
*/

#ifndef device_h
#define device_h
#include <stdio.h>
#include "code_ctrl.h"
#include "light.h"
#include <epitaxy_struct.h>
#include "advmath.h"
#include <contact_struct.h>
#include <perovskite_struct.h>
#include <circuit_struct.h>
#include <dim.h>
#include <matrix.h>
#include <shape_struct.h>
#include <heat.h>
#include <exciton.h>
#include <singlet.h>
#include <mesh_struct.h>
#include <lib_fxdomain.h>
#include <time_mesh.h>
#include <matrix_solver_memory.h>
#include <json.h>
#include <world_struct.h>
#include <optical_mode.h>

struct solver_cache
{
	char hash[100];
	int length;
	int enabled;
};


struct newton_state
{
	struct dimensions dim;

	int last_ittr;
	long double last_error;
	double last_time;
	long double ***phi;
	long double ***x_Nion;

	//Singlet
	long double ***x_Ns;
	long double ***x_Nt;
	long double ***x_Nsd;
	long double ***x_Ntd;
	long double x_Nho;

	long double ***x;
	long double ***xp;

	long double ****xt;
	long double ****xpt;

	//Fermi levels for schottky contact
	long double **x_y0;
	long double **xp_y0;
	long double **x_y1;
	long double **xp_y1;

	//for clever exiting of solver
	long double last_errors[10];
	int last_error_pos;

	//What so solver for
	int Nion_enabled;
	int singlet_enabled;

	//for accelerating the Bernoulli fucntion
	long double ***By_xi_plus;
	long double ***By_xi_neg;
	long double ***By_xip_plus;
	long double ***By_xip_neg;

	long double ***Bx_xi_plus;
	long double ***Bx_xi_neg;
	long double ***Bx_xip_plus;
	long double ***Bx_xip_neg;

	long double ***dBy_xi_plus;
	long double ***dBy_xi_neg;
	long double ***dBy_xip_plus;
	long double ***dBy_xip_neg;

	long double ***dBx_xi_plus;
	long double ***dBx_xi_neg;
	long double ***dBx_xip_plus;
	long double ***dBx_xip_neg;


};

struct newton_state_complex
{
	struct dimensions dim;

	long double complex ***phi;
	long double complex ***x;
	long double complex ***xp;

	long double complex ****xt;
	long double complex ****xpt;

};

struct device
{

	int sim_number;

	//Dimensions
		long double xlen;
		long double ylen;
		long double zlen;

		long double A;			//Area
		long double Vol;
		long double area;

	//Meshing
		int remesh;
		int newmeshsize;
		int dynamic_mesh;

	//Current at contacts
		long double **Jn_y0;
		long double **Jn_y1;
		long double **Jp_y0;
		long double **Jp_y1;

	//Charge density at contacts
		int **n_contact_y0;
		int **n_contact_y1;
		int **n_contact_x0;
		int **n_contact_x1;

	//Contact fermi levels
		long double **Fi0_y0;		//This is the equilibrium fermi level of the contact were it in free space, i.e. with no phi subtracted
		long double **Fi0_y1;		
		long double **Fi0_x0;
		long double **Fi0_x1;
	
	//Built in potentials
		long double **V_y0;		//Diference between the equlibrium fermi level and the fermilevel at in->Fi0_y0[0][0]
		long double **V_y1;		//This is referenced to Fi0_y0[0][0], and is the difference between Fi0_y0[0][0] and Fi0_y0[z][x/y]
		long double **V_x0;		//, this difference must be equal to the built in potential on the contact
		long double **V_x1;

		long double Vbi;		//I have no idea why there are two

	//Charge densities on surfaces even away from contacts
		long double **electrons_y0;
		long double **holes_y0;

		long double **electrons_y1;
		long double **holes_y1;

		long double **electrons_x0;
		long double **holes_x0;

		long double **electrons_x1;
		long double **holes_x1;

	//Ions
		long double ***Nad;
		long double ***Nion;
		long double ***dNion;
		long double ***dNiondphi;
		long double ***Nion_last;

	//Singlet
		long double ***Ns;
		long double ***Nt;
		long double ***Nsd;
		long double ***Ntd;
		long double Nho;

		long double ***dNs;
		long double ***dNt;
		long double ***dNsd;
		long double ***dNtd;
		long double dNho;

		long double ***Ns_last;
		long double ***Nt_last;
		long double ***Nsd_last;
		long double ***Ntd_last;
		long double Nho_last;

		struct optical_mode mode;

	//Generation
		long double ***G;
		long double ***Gn;
		long double ***Gp;

	//Free charges
		long double ***n;
		long double ***p;
		long double ***dn;
		long double ***dndphi;
		long double ***dp;
		long double ***dpdphi;
		long double ***Dn;
		long double ***Dp;

		long double ***Fn;
		long double ***Fp;
		long double ***Nc;
		long double ***Nv;

		long double ***nf_save;
		long double ***pf_save;

		long double ***nfequlib;
		long double ***pfequlib;

		long double ***nlast;
		long double ***plast;

		long double ***wn;
		long double ***wp;

		long double ***n_orig;
		long double ***p_orig;

		long double ***n_orig_f;
		long double ***p_orig_f;

		long double ***n_orig_t;
		long double ***p_orig_t;

		long double ***t;			//in->Xi[z][x][y];
		long double ***tp;			//in->Xi[z][x][y]+in->Eg[z][x][y]
		long double ***t_ion;

	//Fermi levels
		long double ***Fi;

	//Bands
		long double ***Eg;
		long double ***Xi;
		long double ***Ev;
		long double ***Ec;

	//Recombination
		long double ***Rfree;
		long double ***Rauger;

		long double ***Rn;
		long double ***Rp;
		long double ***Rnet;

		long double ***Rn_srh;
		long double ***Rp_srh;

		long double ***Rbi_k;

		long double ***B;



	//Interfaces
		int interfaces_n;
		int interfaces_n_srh;
		int ***interface_type;
		long double ***interface_B;
		long double ***interface_Bt;
		long double ***interface_R;

		//Tunneling
		int interfaces_tunnels_e;
		int interfaces_tunnels_h;
		long double ***interface_Ge;
		long double ***interface_Gh;

	//Rates
		long double ***nrelax;
		long double ***ntrap_to_p;
		long double ***prelax;
		long double ***ptrap_to_n;

	//Mobility
		int mun_symmetric;
		long double ***mun_z;
		long double ***mun_x;
		long double ***mun_y;

		int mup_symmetric;
		long double ***mup_z;
		long double ***mup_x;
		long double ***mup_y;

		long double ***muion;

	//Auger
		long double ***Cn;
		long double ***Cp;
		int auger_enabled;

	//SS SRH
		int ss_srh_enabled;
		long double ***n1;
		long double ***p1;
		long double ***tau_n;
		long double ***tau_p;

	//Electrostatics
		long double ***epsilonr;
		long double ***epsilonr_e0;
		long double ***phi_save;

	//Temperature
		long double ***Tl;
		long double ***Te;
		long double ***Th;

	//Heating
		long double ***Hl;
		long double ***H_recombination;
		long double ***H_joule;

		long double ***He;
		long double ***Hh;

		long double ***ke;		//Thernal conductivities
		long double ***kh;

	//Current
		long double ***Jn;
		long double ***Jp;
		long double ***Jn_x;
		long double ***Jp_x;

		long double ***Jn_diffusion;
		long double ***Jn_drift;

		long double ***Jn_x_diffusion;
		long double ***Jn_x_drift;

		long double ***Jp_diffusion;
		long double ***Jp_drift;

		long double ***Jp_x_diffusion;
		long double ***Jp_x_drift;

		long double ***Jion;

	//Applied voltages
		long double **Vapplied_y0;
		long double **Vapplied_y1;
		long double **Vapplied_x0;
		long double **Vapplied_x1;

	//Passivation
		int **passivate_y0;
		int **passivate_y1;
		int **passivate_x0;
		int **passivate_x1;

	//Emission
		long double ***Photon_gen;

	//Device layout
		int ***imat_epitaxy;		//These should no longer be used
		int ***mask;				//These should no longer be used

		struct dim_zx_epitaxy dim_epitaxy;				//This is a dim object with zx and the length of the epitaxy
		int ***mask_epitaxy;							//?????????????????? Come back to


	//Trap control
		int srh_sim;
		int ntrapnewton;
		int ptrapnewton;

	//Traps 3d n
		long double ***nt_all;
		long double ***tt;			//in->Xi[z][x][y];

		long double ***nt_save;
		long double ***ntequlib;

	//Traps 3d p
		long double ***pt_all;
		long double ***tpt;			//in->Xi[z][x][y]+in->Eg[z][x][y]

		long double ***pt_save;
		long double ***ptequlib;

	//Traps 4d n
		long double  ****nt;
		long double  ****ntlast;
		long double  ****dnt;
		long double  ****srh_n_r1;
		long double  ****srh_n_r2;
		long double  ****srh_n_r3;
		long double  ****srh_n_r4;
		long double  ****dsrh_n_r1;
		long double  ****dsrh_n_r2;
		long double  ****dsrh_n_r3;
		long double  ****dsrh_n_r4;
		long double  ****Fnt;

		long double  ****nt_r1;
		long double  ****nt_r2;
		long double  ****nt_r3;
		long double  ****nt_r4;

		long double ****ntb_save;

	//Traps 4d p
		long double  ****pt;
		long double  ****ptlast;
		long double  ****dpt;
		long double  ****srh_p_r1;
		long double  ****srh_p_r2;
		long double  ****srh_p_r3;
		long double  ****srh_p_r4;
		long double  ****dsrh_p_r1;
		long double  ****dsrh_p_r2;
		long double  ****dsrh_p_r3;
		long double  ****dsrh_p_r4;
		long double  ****Fpt;

		long double  ****pt_r1;
		long double  ****pt_r2;
		long double  ****pt_r3;
		long double  ****pt_r4;


		long double ****ptb_save;

	//Newton solver control
		int max_electrical_itt;
		long double electrical_clamp;
		long double min_cur_error;

		int max_electrical_itt0;
		long double electrical_clamp0;
		long double electrical_error0;

		int math_enable_pos_solver;

		char newton_name[20];

		int kl_in_newton;
		int config_kl_in_newton;
		void (*newton_aux)(struct simulation *sim, struct device* ,gdouble ,gdouble* ,gdouble* ,gdouble* ,gdouble* ,gdouble* ,gdouble* ,gdouble* ,gdouble*);

		int newton_clever_exit;

		int newton_only_fill_matrix;
		long double omega;

		int newton_min_itt;

		int newton_last_ittr;

	//Arrays used by newton solver
		long double *newton_dntrap;
		long double *newton_dntrapdntrap;
		long double *newton_dntrapdn;
		long double *newton_dntrapdp;
		long double *newton_dJdtrapn;
		long double *newton_dJpdtrapn;

		long double *newton_dptrapdp;
		long double *newton_dptrapdptrap;
		long double *newton_dptrap;
		long double *newton_dptrapdn;
		long double *newton_dJpdtrapp;
		long double *newton_dJpdtrapp_interface_right;
		long double *newton_dJdtrapp;
		long double *newton_dphidntrap;
		long double *newton_dphidptrap;
		long double *newton_ntlast;
		long double *newton_ptlast;

	//Electrical components
		long double Rshunt;
		long double Rcontact;
		long double Rload;
		long double L;
		long double C;
		long double Rshort;
		long double other_layers;
		long double contact_charge;

	//Dump contorl
		long double dump_dynamic_pl_energy;

		int snapshot_number;

		long double map_start;
		long double map_stop;

		int timedumpcount;

		char plot_file[100];

	//time
		int go_time;
		long double dt;
		long double time;
		long double Ilast;
		long double start_stop_time;

	//Run control
		int stop;
		int onlypos;
		int odes;
		int stop_start;
		int dd_conv;
		int high_voltage_limit;
		int stoppoint;
		int drift_diffision_simulations_enabled;
		int electrical_simulation_enabled;
	//Matrix
		struct matrix mx;

	//Newton solver internal memory
		//none

	//meshing
		struct mesh_obj mesh_data;
		struct mesh_obj mesh_data_save;
		long double layer_start[100];
		long double layer_stop[100];

	//epitaxy
		struct epitaxy my_epitaxy;

	//plugins
		char simmode[200];

	//Newton states
		struct newton_state ns;
		struct newton_state ns_save;

	//Light
		struct light mylight;
		struct light probe_modes;

		struct math_xy steady_stark;
		struct light_sources lights;

	//Emission
		int emission_enabled;
		long double pl_intensity;
		long double pl_intensity_tot;
		int pl_use_experimental_emission_spectra;

	//thermal
		struct heat thermal;

	//exciton
		struct exciton ex;

	//singlet
		struct singlet sing;

	//Preovskite
		struct perovskite mobileion;

	//Circuit simulation
		int circuit_simulation;
		struct circuit cir;

	//Ray tracing
		struct image my_image;
		struct shape big_box;


	//Contacts
		struct contact *contacts;
		int ncontacts;
		int active_contact;
		int boundry_y0;

		long double flip_current;

	//objects
		struct object ****obj_zxy;
		struct object ****obj_zx_layer;

	//time mesh
		struct time_mesh tm;

	//solver cache
		char solver_type[200];
		struct solver_cache cache;

	//fxdomain config
		struct fxdomain fxdomain_config;
	
	//matrix solver memory
		struct matrix_solver_memory msm;

	//configure
		struct json config;

	//temp vars
		long double glob_wanted;

	//paths
	//paths
		char output_path[PATH_MAX];
		char input_path[PATH_MAX];

	//Newton solver dll
		int (*dll_solve_cur)();
		int (*dll_solver_realloc)();
		int (*dll_solver_free_memory)();
		void *dll_solver_handle;
		int solver_verbosity;

	//the world
	struct world w;
};


#endif
