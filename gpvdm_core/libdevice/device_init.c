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


/** @file device.c
	@brief Initialize the device structure.
*/

#define _FILE_OFFSET_BITS 64
#define _LARGEFILE_SOURCE
#include <stdio.h>
#include <device.h>
#include <string.h>
#include <dump.h>
#include <mesh.h>
#include <ray_fun.h>
#include <newton_tricks.h>
#include <memory.h>
#include <circuit.h>
#include <shape.h>
#include <heat.h>
#include <heat_fun.h>
#include <exciton.h>
#include <exciton_fun.h>
#include <gpvdm_const.h>
#include <lib_fxdomain.h>
#include <fxdomain_fun.h>
#include <perovskite.h>
#include <sim.h>
#include <device_fun.h>
#include <epitaxy.h>
#include <light_fun.h>
#include <enabled_libs.h>
#include <lock.h>
#include <light_srcs.h>

#include <world_struct.h>
#include <world.h>
#include <optical_mode_fun.h>

static int unused __attribute__((unused));
static char* unused_pchar __attribute__((unused));

void device_init(struct simulation *sim,struct device *dev)
{
	char cwd[PATH_MAX];

		dev->sim_number=-1;
	//Dimensions
		dev->xlen= -1.0;
		dev->ylen= -1.0;
		dev->zlen= -1.0;

		dev->A= -1.0;
		dev->Vol= -1.0;
		dev->area= -1.0;


	//Meshing
		dev->remesh= -1;
		dev->newmeshsize= -1;
		dev->dynamic_mesh=-1;

	//Current at contacts
		dev->Jn_y0=NULL;
		dev->Jn_y1=NULL;
		dev->Jp_y0=NULL;
		dev->Jp_y1=NULL;

	//Charge density at contacts
		dev->n_contact_y0=NULL;
		dev->n_contact_y1=NULL;
		dev->n_contact_x0=NULL;
		dev->n_contact_x1=NULL;

	//Contact fermi levels
		dev->Fi0_y0 = NULL;
		dev->Fi0_y1 = NULL;
		dev->Fi0_x0 = NULL;
		dev->Fi0_x1 = NULL;

	//Charge densities on surfaces even away from contacts
		dev->electrons_y0= NULL;
		dev->holes_y0= NULL;

		dev->electrons_y1= NULL;
		dev->holes_y1= NULL;

		dev->electrons_x0= NULL;
		dev->holes_x0= NULL;

		dev->electrons_x1= NULL;
		dev->holes_x1= NULL;

	//Built in potentials
		dev->V_y0= NULL;
		dev->V_y1= NULL;
		dev->V_x0= NULL;
		dev->V_x1= NULL;

	//Ions
		dev->Nad= NULL;
		dev->Nion= NULL;
		dev->dNion= NULL;
		dev->dNiondphi= NULL;
		dev->Nion_last= NULL;

	//Singlet
		dev->Ns=NULL;
		dev->Nt=NULL;
		dev->Nsd=NULL;
		dev->Ntd=NULL;
		dev->Nho=-1.0;

		dev->dNs=NULL;
		dev->dNt=NULL;
		dev->dNsd=NULL;
		dev->dNtd=NULL;
		dev->dNho=-1.0;

		dev->Ns_last=NULL;
		dev->Nt_last=NULL;
		dev->Nsd_last=NULL;
		dev->Ntd_last=NULL;
		dev->Nho_last=-1.0;

		mode_init(sim,&(dev->mode));

	//Generation
		dev->G= NULL;
		dev->Gn= NULL;
		dev->Gp= NULL;

	//Free charges
		dev->n= NULL;
		dev->p= NULL;
		dev->dn= NULL;
		dev->dndphi= NULL;
		dev->dp= NULL;
		dev->dpdphi= NULL;
		dev->Dn= NULL;
		dev->Dp= NULL;

		dev->Fn= NULL;
		dev->Fp= NULL;
		dev->Nc= NULL;
		dev->Nv= NULL;

		dev->nf_save= NULL;
		dev->pf_save= NULL;

		dev->nfequlib= NULL;
		dev->pfequlib= NULL;

		dev->nlast= NULL;
		dev->plast= NULL;

		dev->wn= NULL;
		dev->wp= NULL;

		dev->n_orig= NULL;
		dev->p_orig= NULL;

		dev->n_orig_f= NULL;
		dev->p_orig_f= NULL;

		dev->n_orig_t= NULL;
		dev->p_orig_t= NULL;

		dev->t= NULL;
		dev->tp= NULL;
		dev->t_ion= NULL;

	//Fermi levels
		dev->Fi= NULL;

	//Bands
		dev->Eg= NULL;
		dev->Xi= NULL;
		dev->Ev= NULL;
		dev->Ec= NULL;

	//Recombination
		dev->Rfree= NULL;
		dev->Rauger= NULL;

		dev->Rn= NULL;
		dev->Rp= NULL;
		dev->Rnet= NULL;

		dev->Rn_srh= NULL;
		dev->Rp_srh= NULL;

		dev->Rbi_k= NULL;

		dev->B= NULL;

	//Interfaces
		dev->interfaces_n=0;
		dev->interfaces_n_srh=0;
		dev->interface_type=NULL;
		dev->interface_B=NULL;
		dev->interface_Bt=NULL;
		dev->interface_R=NULL;

		//Tunnel
		dev->interface_Ge=NULL;
		dev->interface_Gh=NULL;
		dev->interfaces_tunnels_e=0;
		dev->interfaces_tunnels_h=0;

	//Rates
		dev->nrelax= NULL;
		dev->ntrap_to_p= NULL;
		dev->prelax= NULL;
		dev->ptrap_to_n= NULL;

	//Mobility
		dev->mun_symmetric=TRUE;
		dev->mun_z= NULL;
		dev->mun_x= NULL;
		dev->mun_y= NULL;

		dev->mup_symmetric=TRUE;
		dev->mup_z= NULL;
		dev->mup_x= NULL;
		dev->mup_y= NULL;

		dev->muion=NULL;

	//Auger
		dev->Cn=NULL;
		dev->Cp=NULL;
		dev->auger_enabled=FALSE;

	//SS SRH
		dev->ss_srh_enabled=FALSE;
		dev->n1=NULL;
		dev->p1=NULL;
		dev->tau_n=NULL;
		dev->tau_p=NULL;

	//Electrostatics
		dev->epsilonr= NULL;
		dev->epsilonr_e0= NULL;

		dev->phi_save= NULL;

	//Temperature
		dev->Tl= NULL;
		dev->Te= NULL;
		dev->Th= NULL;

	//Heating
		dev->Hl= NULL;
		dev->H_recombination= NULL;
		dev->H_joule= NULL;

		dev->He= NULL;
		dev->Hh= NULL;

		dev->ke= NULL;
		dev->kh= NULL;

	//Current
		dev->Jn= NULL;
		dev->Jp= NULL;
		dev->Jn_x= NULL;
		dev->Jp_x= NULL;

		dev->Jn_diffusion= NULL;
		dev->Jn_drift= NULL;

		dev->Jn_x_diffusion= NULL;
		dev->Jn_x_drift= NULL;

		dev->Jp_diffusion= NULL;
		dev->Jp_drift= NULL;

		dev->Jp_x_diffusion= NULL;
		dev->Jp_x_drift= NULL;

		dev->Jion= NULL;
	//Applied voltages
		dev->Vapplied_y0=NULL;
		dev->Vapplied_y1=NULL;
		dev->Vapplied_x0=NULL;
		dev->Vapplied_x1=NULL;

		dev->Vbi= -1.0;

	//Passivation
		dev->passivate_y0 = NULL;
		dev->passivate_y1 = NULL;
		dev->passivate_x0 = NULL;
		dev->passivate_x1 = NULL;

	//Emission
		dev->Photon_gen=NULL;

	//Device layout
		dev->imat_epitaxy= NULL;
		dev->mask= NULL;

		dim_init_zx_epitaxy(&(dev->dim_epitaxy));
		dev->mask_epitaxy= NULL;

	//Trap control
		dev->ntrapnewton= -1;
		dev->ptrapnewton= -1;
		dev->srh_sim= -1;

	//Traps 3d n
		dev->nt_all= NULL;
		dev->tt= NULL;

		dev->nt_save= NULL;
		dev->pt_save= NULL;

	//Traps 3d p
		dev->pt_all= NULL;
		dev->tpt= NULL;

		dev->ntequlib= NULL;
		dev->ptequlib= NULL;



	//Traps 4d n
		dev->nt= NULL;
		dev->ntlast= NULL;
		dev->dnt= NULL;
		dev->srh_n_r1= NULL;
		dev->srh_n_r2= NULL;
		dev->srh_n_r3= NULL;
		dev->srh_n_r4= NULL;
		dev->dsrh_n_r1= NULL;
		dev->dsrh_n_r2= NULL;
		dev->dsrh_n_r3= NULL;
		dev->dsrh_n_r4= NULL;
		dev->Fnt= NULL;


		dev->nt_r1= NULL;
		dev->nt_r2= NULL;
		dev->nt_r3= NULL;
		dev->nt_r4= NULL;

		dev->ntb_save= NULL;

	//Traps 4d p
		dev->pt= NULL;
		dev->ptlast= NULL;
		dev->dpt= NULL;
		dev->srh_p_r1= NULL;
		dev->srh_p_r2= NULL;
		dev->srh_p_r3= NULL;
		dev->srh_p_r4= NULL;
		dev->dsrh_p_r1= NULL;
		dev->dsrh_p_r2= NULL;
		dev->dsrh_p_r3= NULL;
		dev->dsrh_p_r4= NULL;
		dev->Fpt= NULL;


		dev->pt_r1= NULL;
		dev->pt_r2= NULL;
		dev->pt_r3= NULL;
		dev->pt_r4= NULL;

		dev->ptb_save= NULL;

	//Newton solver control
		dev->max_electrical_itt= -1;
		dev->electrical_clamp= -1.0;
		dev->min_cur_error= -1.0;

		dev->max_electrical_itt0= -1;
		dev->electrical_clamp0= -1.0;
		dev->electrical_error0= -1.0;

		dev->math_enable_pos_solver= -1.0;

		strcpy(dev->newton_name,"");

		dev->kl_in_newton=-1;
		dev->config_kl_in_newton=-1;
		dev->newton_aux=NULL;

		dev->newton_clever_exit= -1;

		dev->newton_only_fill_matrix=-1;
		dev->omega=-1.0;

		dev->newton_min_itt= -1;

		dev->newton_last_ittr=-1;

	//Arrays used by newton solver
		dev->newton_dntrap=NULL;
		dev->newton_dntrapdntrap=NULL;
		dev->newton_dntrapdn=NULL;
		dev->newton_dntrapdp=NULL;
		dev->newton_dJdtrapn=NULL;
		dev->newton_dJpdtrapn=NULL;

		dev->newton_dptrapdp=NULL;
		dev->newton_dptrapdptrap=NULL;
		dev->newton_dptrap=NULL;
		dev->newton_dptrapdn=NULL;
		dev->newton_dJpdtrapp=NULL;
		dev->newton_dJpdtrapp_interface_right=NULL;
		dev->newton_dJdtrapp=NULL;
		dev->newton_dphidntrap=NULL;
		dev->newton_dphidptrap=NULL;
		dev->newton_ntlast=NULL;
		dev->newton_ptlast=NULL;

	//Electrical components
		dev->Rshunt= -1.0;
		dev->Rcontact= -1.0;
		dev->Rload= -1.0;
		dev->L= -1.0;
		dev->C= -1.0;
		dev->Rshort= -1.0;
		dev->other_layers= -1.0;
		dev->contact_charge= -1.0;

	//Dump control
		dev->dump_dynamic_pl_energy= -1;

		dev->snapshot_number= -1;

		dev->map_start=-1;
		dev->map_stop=-1;

		dev->timedumpcount=-1;

		strcpy(dev->plot_file,"");
	//Time
		dev->go_time= -1;
		dev->dt= -1.0;
		dev->time= -1.0;
		dev->Ilast= -1.0;
		dev->start_stop_time=-1.0;

	//Run control
		dev->stop=-1;
		dev->onlypos=-1;
		dev->odes=-1;
		dev->stop_start=-1;
		dev->dd_conv=-1;
		dev->high_voltage_limit=-1;
		dev->stoppoint= -1;
		dev->drift_diffision_simulations_enabled=TRUE;
		dev->electrical_simulation_enabled=TRUE;
	//Matrix
		matrix_init(&(dev->mx));

	//Newton solver internal memory
		//none

	//meshing
		mesh_obj_init(&(dev->mesh_data));
		mesh_obj_init(&(dev->mesh_data_save));

	//epitaxy
		epitaxy_init(sim,&(dev->my_epitaxy));

	//plugins
		strcpy(dev->simmode,"");

	//Newton states
		newton_state_init(&(dev->ns));
		newton_state_init(&(dev->ns_save));

	//Light
		light_init(sim,&(dev->mylight));
		light_init(sim,&(dev->probe_modes));
		inter_init(sim,&(dev->steady_stark));

		light_srcs_init(sim,&(dev->lights));

	//Emission
		dev->emission_enabled=-1;
		dev->pl_intensity= -1.0;
		dev->pl_intensity_tot= -1.0;
		dev->pl_use_experimental_emission_spectra=-1;

	//thermal
		#ifdef libheat_enabled
			heat_init(&(dev->thermal));
		#endif

	//Exciton
		#ifdef libheat_enabled
			exciton_init(&(dev->ex));
		#endif


	//Perovskite
		#ifdef libperovskite_enabled
			perovskite_init(sim,dev);
		#endif

	//Circuit simulation
		dev->circuit_simulation=FALSE;
		#ifdef libcircuit_enabled
			circuit_init(&(dev->cir));
		#endif

	//Ray tracing

		ray_engine_init(&(dev->my_image));
		shape_init(sim,&(dev->big_box));

	//Contacts
		dev->ncontacts=-1;
		dev->active_contact=-1;
		dev->boundry_y0=contact_ohmic;

		dev->flip_current=1.0;

	//Objects
		dev->obj_zxy=NULL;
		dev->obj_zx_layer=NULL;

	//Time mesh
		time_mesh_init(&(dev->tm));

	//device cache
		strcpy(dev->solver_type,"none");
		device_cache_init(&(dev->cache));

	//fxdomain
		#ifdef libfxdomain_enabled
			fxdomain_init(sim,&(dev->fxdomain_config));
		#endif
	//matrix solver memory
		matrix_solver_memory_init(&(dev->msm));

	//json
		json_init(&(dev->config));

	//temp vars
		dev->glob_wanted=0.0;

	//temp vars
		if (getcwd(cwd,PATH_MAX)==NULL)
		{
			ewe(sim,"cwd returned NULL\n");
		}

		strcpy(dev->input_path,cwd);
		strcpy(dev->output_path,cwd);

	//Newton solver dll
		dev->dll_solve_cur=NULL;
		dev->dll_solver_realloc=NULL;
		dev->dll_solver_free_memory=NULL;
		dev->dll_solver_handle=NULL;
		dev->solver_verbosity=-1;

	//world
	world_init(sim,&(dev->w));
}

