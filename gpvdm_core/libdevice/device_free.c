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


/** @file device_free.c
	@brief Free memory for the device structure.
*/

#define _FILE_OFFSET_BITS 64
#define _LARGEFILE_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <device.h>
#include <string.h>
#include <dump.h>
#include <mesh.h>
#include <ray_fun.h>
#include <newton_tricks.h>
#include <memory.h>
#include <circuit.h>
#include <shape.h>
#include <lang.h>
#include <util.h>
#include <heat_fun.h>
#include <solver_interface.h>
#include <log.h>
#include <light_fun.h>
#include <sim.h>
#include <fxdomain_fun.h>
#include <device_fun.h>
#include <contacts.h>
#include <perovskite.h>
#include <epitaxy.h>
#include <enabled_libs.h>
#include <light_srcs.h>

static int unused __attribute__((unused));
static char* unused_pchar __attribute__((unused));


void device_free(struct simulation *sim,struct device *dev)
{
	int i;
	struct dimensions *dim=&dev->ns.dim;

	

//	if (dim->ylen==0)
//	{
//		return;
//	}

	//Now following device.h file structure
	//2d

	//Dimensions

	//Meshing

	//Current at contacts
		free_zx_gdouble(dim,&dev->Jn_y0);
		free_zx_gdouble(dim,&dev->Jn_y1);
		free_zx_gdouble(dim,&dev->Jp_y0);
		free_zx_gdouble(dim,&dev->Jp_y1);

	//Charge density at contacts
		free_zx_int(dim,&dev->n_contact_y0);
		free_zx_int(dim,&dev->n_contact_y1);
		free_zy_int(dim,&dev->n_contact_x0);
		free_zy_int(dim,&dev->n_contact_x1);

	//Contact fermi levels
		free_zx_gdouble(dim,&dev->Fi0_y0);
		free_zx_gdouble(dim,&dev->Fi0_y1);
		free_zy_long_double(dim,&dev->Fi0_x0);
		free_zy_long_double(dim,&dev->Fi0_x1);

	//Charge densities on surfaces even away from contacts
		free_zx_gdouble(dim,&dev->electrons_y0);
		free_zx_gdouble(dim,&dev->holes_y0);

		free_zx_gdouble(dim,&dev->electrons_y1);
		free_zx_gdouble(dim,&dev->holes_y1);

		free_zy_long_double(dim,&dev->electrons_x0);
		free_zy_long_double(dim,&dev->holes_x0);

		free_zy_long_double(dim,&dev->electrons_x1);
		free_zy_long_double(dim,&dev->holes_x1);

	//Built in potentials
		free_zx_gdouble(dim,&dev->V_y0);
		free_zx_gdouble(dim,&dev->V_y1);
		free_zy_long_double(dim,&dev->V_x0);
		free_zy_long_double(dim,&dev->V_x1);

	//Ions
		free_zxy_gdouble(dim,&dev->Nad);
		free_zxy_gdouble(dim,&dev->Nion);
		free_zxy_gdouble(dim,&dev->dNion);
		free_zxy_gdouble(dim,&dev->dNiondphi);
		free_zxy_gdouble(dim,&dev->Nion_last);

	//Generation
		free_zxy_gdouble(dim,&dev->G);
		free_zxy_gdouble(dim,&dev->Gn);
		free_zxy_gdouble(dim,&dev->Gp);

	//Free charges
		free_zxy_gdouble(dim,&dev->n);
		free_zxy_gdouble(dim,&dev->p);
		free_zxy_gdouble(dim,&dev->dn);
		free_zxy_gdouble(dim,&dev->dp);
		free_zxy_gdouble(dim,&dev->dndphi);
		free_zxy_gdouble(dim,&dev->dpdphi);
		free_zxy_gdouble(dim,&dev->Dn);
		free_zxy_gdouble(dim,&dev->Dp);

		free_zxy_gdouble(dim,&dev->Fn);
		free_zxy_gdouble(dim,&dev->Fp);
		free_zxy_gdouble(dim,&dev->Nc);
		free_zxy_gdouble(dim,&dev->Nv);

		free_zxy_gdouble(dim,&dev->nf_save);
		free_zxy_gdouble(dim,&dev->pf_save);

		free_zxy_gdouble(dim,&dev->nfequlib);
		free_zxy_gdouble(dim,&dev->pfequlib);

		free_zxy_gdouble(dim,&dev->nlast);
		free_zxy_gdouble(dim,&dev->plast);

		free_zxy_gdouble(dim,&dev->n_orig);
		free_zxy_gdouble(dim,&dev->p_orig);

		free_zxy_gdouble(dim,&dev->n_orig_f);
		free_zxy_gdouble(dim,&dev->p_orig_f);

		free_zxy_gdouble(dim,&dev->n_orig_t);
		free_zxy_gdouble(dim,&dev->p_orig_t);

		free_zxy_gdouble(dim,&dev->t);
		free_zxy_gdouble(dim,&dev->tp);
		free_zxy_gdouble(dim,&dev->t_ion);

		free_zxy_gdouble(dim,&dev->wn);
		free_zxy_gdouble(dim,&dev->wp);

	//Fermi levels
		free_zxy_gdouble(dim,&dev->Fi);

	//Bands
		free_zxy_gdouble(dim,&dev->Eg);
		free_zxy_gdouble(dim,&dev->Xi);
		free_zxy_gdouble(dim,&dev->Ev);
		free_zxy_gdouble(dim,&dev->Ec);

	//Recombination
		free_zxy_gdouble(dim,&dev->Rfree);

		free_zxy_gdouble(dim,&dev->Rn);
		free_zxy_gdouble(dim,&dev->Rp);
		free_zxy_gdouble(dim,&dev->Rnet);

		free_zxy_gdouble(dim,&dev->Rn_srh);
		free_zxy_gdouble(dim,&dev->Rp_srh);

		free_zxy_gdouble(dim,&dev->Rbi_k);

		free_zxy_gdouble(dim,&dev->B);

	//Interfaces
		free_zxy_int(dim,&(dev->interface_type));
		free_zxy_gdouble(dim,&dev->interface_B);
		free_zxy_gdouble(dim,&dev->interface_Bt);
		free_zxy_gdouble(dim,&dev->interface_R);

	//Rates
		free_zxy_gdouble(dim,&dev->nrelax);
		free_zxy_gdouble(dim,&dev->ntrap_to_p);
		free_zxy_gdouble(dim,&dev->prelax);
		free_zxy_gdouble(dim,&dev->ptrap_to_n);

	//Mobility
		free_zxy_gdouble(dim,&dev->mun_z);
		free_zxy_gdouble(dim,&dev->mun_x);
		free_zxy_gdouble(dim,&dev->mun_y);

		free_zxy_gdouble(dim,&dev->mup_z);
		free_zxy_gdouble(dim,&dev->mup_x);
		free_zxy_gdouble(dim,&dev->mup_y);



		free_zxy_gdouble(dim,&dev->muion);

	//Electrostatics
		free_zxy_gdouble(dim,&dev->epsilonr);
		free_zxy_gdouble(dim,&dev->epsilonr_e0);

		free_zxy_gdouble(dim,&dev->phi_save);

	//Temperature
		free_zxy_gdouble(dim,&dev->Tl);
		free_zxy_gdouble(dim,&dev->Te);
		free_zxy_gdouble(dim,&dev->Th);

		free_zxy_gdouble(dim,&dev->ke);
		free_zxy_gdouble(dim,&dev->kh);

	//Heating
		free_zxy_gdouble(dim,&dev->Hl);
		free_zxy_gdouble(dim,&dev->H_joule);
		free_zxy_gdouble(dim,&dev->H_recombination);

		free_zxy_gdouble(dim,&dev->He);
		free_zxy_gdouble(dim,&dev->Hh);

	//Current
		free_zxy_gdouble(dim,&dev->Jn);
		free_zxy_gdouble(dim,&dev->Jp);
		free_zxy_gdouble(dim,&dev->Jn_x);
		free_zxy_gdouble(dim,&dev->Jp_x);

		free_zxy_gdouble(dim,&dev->Jn_drift);
		free_zxy_gdouble(dim,&dev->Jn_diffusion);

		free_zxy_gdouble(dim,&dev->Jn_x_drift);
		free_zxy_gdouble(dim,&dev->Jn_x_diffusion);

		free_zxy_gdouble(dim,&dev->Jp_drift);
		free_zxy_gdouble(dim,&dev->Jp_diffusion);

		free_zxy_gdouble(dim,&dev->Jp_x_drift);
		free_zxy_gdouble(dim,&dev->Jp_x_diffusion);

	//Applied voltages
		free_zx_gdouble(dim,&dev->Vapplied_y0);
		free_zx_gdouble(dim,&dev->Vapplied_y1);
		free_zy_long_double(dim,&dev->Vapplied_x0);
		free_zy_long_double(dim,&dev->Vapplied_x1);


	//Passivation
		free_zx_int(dim,&dev->passivate_y0);
		free_zx_int(dim,&dev->passivate_y1);
		free_zy_int(dim,&dev->passivate_x0);
		free_zy_int(dim,&dev->passivate_x1);

	//Emission
		free_zxy_gdouble(dim,&dev->Photon_gen);

	//objects
		free_zxy_p_object(dim, &(dev->obj_zxy));
		free_zx_layer_p_object(&(dev->dim_epitaxy), &(dev->obj_zx_layer));

	//Device layout
		free_zxy_int(dim,&(dev->imat_epitaxy));

		free_zxy_int(dim,&(dev->mask));

		free_zx_epitaxy_int(&(dev->dim_epitaxy),&(dev->mask_epitaxy));
		dim_free_zx_epitaxy(&(dev->dim_epitaxy));

	//Exciton
		free_zxy_gdouble(dim,&dev->ex);
		free_zxy_gdouble(dim,&dev->Dex);
		free_zxy_gdouble(dim,&dev->Hex);

		free_zxy_gdouble(dim,&dev->kf);
		free_zxy_gdouble(dim,&dev->kd);
		free_zxy_gdouble(dim,&dev->kr);

	//Trap control
		//none 

	//Traps 3d n
		free_zxy_gdouble(dim,&dev->nt_all);
		free_zxy_gdouble(dim,&dev->tt);

		free_zxy_gdouble(dim,&dev->nt_save);
		free_zxy_gdouble(dim,&dev->pt_save);


	//Traps 3d p
		free_zxy_gdouble(dim,&dev->pt_all);
		free_zxy_gdouble(dim,&dev->tpt);

		free_zxy_gdouble(dim,&dev->ntequlib);
		free_zxy_gdouble(dim,&dev->ptequlib);

	//Traps 4d n
		free_srh_bands(dim, &dev->nt);
		free_srh_bands(dim, &dev->ntlast);
		free_srh_bands(dim, &dev->dnt);
		free_srh_bands(dim, &dev->srh_n_r1);
		free_srh_bands(dim, &dev->srh_n_r2);
		free_srh_bands(dim, &dev->srh_n_r3);
		free_srh_bands(dim, &dev->srh_n_r4);
		free_srh_bands(dim, &dev->dsrh_n_r1);
		free_srh_bands(dim, &dev->dsrh_n_r2);
		free_srh_bands(dim, &dev->dsrh_n_r3);
		free_srh_bands(dim, &dev->dsrh_n_r4);
		free_srh_bands(dim, &dev->Fnt);

		free_srh_bands(dim, &dev->nt_r1);
		free_srh_bands(dim, &dev->nt_r2);
		free_srh_bands(dim, &dev->nt_r3);
		free_srh_bands(dim, &dev->nt_r4);

		free_srh_bands(dim, &dev->ntb_save);

	//Traps 4d p
		free_srh_bands(dim, &dev->pt);
		free_srh_bands(dim, &dev->ptlast);
		free_srh_bands(dim, &dev->dpt);
		free_srh_bands(dim, &dev->srh_p_r1);
		free_srh_bands(dim, &dev->srh_p_r2);
		free_srh_bands(dim, &dev->srh_p_r3);
		free_srh_bands(dim, &dev->srh_p_r4);
		free_srh_bands(dim, &dev->dsrh_p_r1);
		free_srh_bands(dim, &dev->dsrh_p_r2);
		free_srh_bands(dim, &dev->dsrh_p_r3);
		free_srh_bands(dim, &dev->dsrh_p_r4);
		free_srh_bands(dim, &dev->Fpt);

		free_srh_bands(dim, &dev->pt_r1);
		free_srh_bands(dim, &dev->pt_r2);
		free_srh_bands(dim, &dev->pt_r3);
		free_srh_bands(dim, &dev->pt_r4);

		free_srh_bands(dim, &dev->ptb_save);

	//Newton solver control
		//none

	//Arrays used by newton solver
		free_1d((void **)&dev->newton_dntrap, sizeof(long double ));
		free_1d((void **)&dev->newton_dntrapdntrap, sizeof(long double ));
		free_1d((void **)&dev->newton_dntrapdn, sizeof(long double ));
		free_1d((void **)&dev->newton_dntrapdp, sizeof(long double ));
		free_1d((void **)&dev->newton_dJdtrapn, sizeof(long double ));
		free_1d((void **)&dev->newton_dJpdtrapn, sizeof(long double ));
		free_1d((void **)&dev->newton_dphidntrap, sizeof(long double ));
		free_1d((void **)&dev->newton_dptrapdp, sizeof(long double ));
		free_1d((void **)&dev->newton_dptrapdptrap, sizeof(long double ));
		free_1d((void **)&dev->newton_dptrap, sizeof(long double ));
		free_1d((void **)&dev->newton_dptrapdn, sizeof(long double ));
		free_1d((void **)&dev->newton_dJpdtrapp, sizeof(long double ));
		free_1d((void **)&dev->newton_dJdtrapp, sizeof(long double ));

		free_1d((void **)&dev->newton_dJpdtrapp_interface_right, sizeof(long double ));
		free_1d((void **)&dev->newton_dphidptrap, sizeof(long double ));
		free_1d((void **)&dev->newton_ntlast, sizeof(long double ));
		free_1d((void **)&dev->newton_ptlast, sizeof(long double ));


	//Electrical components
		//none

	//Dump control
		//none

	//Time
		//none

	//Matrix
		matrix_free(sim,&dev->mx);

	//Newton solver internal memory
		solver_free_memory(sim,dev);

	//meshing
		mesh_obj_free(sim,&(dev->mesh_data));
		mesh_obj_free(sim,&(dev->mesh_data_save));

	//Epitaxy
		epitaxy_free(sim,&(dev->my_epitaxy));

	//plugins
		//none

	//Newton states
		newton_state_free(&(dev->ns));
		newton_state_free(&(dev->ns_save));

	//Light
		light_free(sim,&(dev->mylight));
		light_free(sim,&(dev->probe_modes));
		inter_free(&(dev->steady_stark));

		light_srcs_free(sim,&(dev->lights));
	//Emission
		//none

	//Test
		//none

	//Thermal
		#ifdef libheat_enabled
			heat_free_memory(sim,&(dev->thermal));
		#endif

	//Perovskite
		#ifdef libperovskite_enabled
			perovskite_free(sim,dev);
		#endif

	//Circuit simulation
		#ifdef libcircuit_enabled
			circuit_free(sim,&(dev->cir));
		#endif

	//Ray tracing
		ray_free(sim,dev,&(dev->my_image));
		shape_free(sim,&(dev->big_box));

	//Contacts
		contacts_free(sim,dev);



		device_objects_free(sim,dev);
	//Time mesh
		time_memory_free(&(dev->tm));

	//solver cache
		//none

	//fxdomain free
		#ifdef libfxdomain_enabled
			fxdomain_free(sim,&(dev->fxdomain_config));
		#endif

	//matrix solver memory
		matrix_solver_memory_free(sim,&(dev->msm));
	///////////






	//dim_free(&(dev->dim_max));
	//Free epitaxy

	//json
		json_free(&(dev->config));

}


