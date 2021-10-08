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


/** @file device_malloc.c
	@brief Malloc for the device structure.
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
#include <device_fun.h>
#include <enabled_libs.h>

static int unused __attribute__((unused));
static char* unused_pchar __attribute__((unused));

void device_get_memory(struct simulation *sim,struct device *in)
{
	in->odes = 0;
	struct newton_state *ns=(&in->ns);
	struct dimensions *dim=&in->ns.dim;

	if (in->electrical_simulation_enabled==FALSE)
	{
		return;
	}

	if ((dim->ylen<1)||(dim->xlen<1)||(dim->zlen<1))
	{
		ewe(sim,"%s\n",_("I can't allocate a device with less than 1 mesh point."));
	}

	if ((dim->ylen>50000)||(dim->xlen>50000)||(dim->zlen>50000))
	{
		ewe(sim,"%s\n",_("You are asking me to simulate a device with more than 50000 mesh points, although I could do this I am not going to because it seems a bad idea to me."));
	}



	//Dimensions

	//Meshing

	//Current at contacts
	malloc_zx_gdouble(dim,&(in->Jn_y0));
	malloc_zx_gdouble(dim,&(in->Jn_y1));
	malloc_zx_gdouble(dim,&(in->Jp_y0));
	malloc_zx_gdouble(dim,&(in->Jp_y1));

	//Charge density at contacts
		malloc_zx_int(dim,&(in->n_contact_y0));
		malloc_zx_int(dim,&(in->n_contact_y1));
		malloc_zy_int(dim,&(in->n_contact_x0));
		malloc_zy_int(dim,&(in->n_contact_x1));

	//Contact fermi levels
	if (in->drift_diffision_simulations_enabled==TRUE)
	{
		malloc_zx_gdouble(dim,&(in->Fi0_y0));
		malloc_zx_gdouble(dim,&(in->Fi0_y1));
		malloc_zy_long_double(dim,&(in->Fi0_x0));
		malloc_zy_long_double(dim,&(in->Fi0_x1));
	}

	//Charge densities on surfaces even away from contacts
		malloc_zx_gdouble(dim,&(in->electrons_y0));
		malloc_zx_gdouble(dim,&(in->holes_y0));

		malloc_zx_gdouble(dim,&(in->electrons_y1));
		malloc_zx_gdouble(dim,&(in->holes_y1));

		malloc_zy_long_double(dim,&(in->electrons_x0));
		malloc_zy_long_double(dim,&(in->holes_x0));

		malloc_zy_long_double(dim,&(in->electrons_x1));
		malloc_zy_long_double(dim,&(in->holes_x1));

	//Built in potentials

	malloc_zx_gdouble(dim,&(in->V_y0));
	malloc_zx_gdouble(dim,&(in->V_y1));
	malloc_zy_long_double(dim,&(in->V_x0));
	malloc_zy_long_double(dim,&(in->V_x1));

	//Ions
	if (in->drift_diffision_simulations_enabled==TRUE)
	{
		malloc_zxy_gdouble(dim,&(in->Nad));
		malloc_zxy_gdouble(dim,&(in->Nion));
		malloc_zxy_gdouble(dim,&(in->dNion));
		malloc_zxy_gdouble(dim,&(in->dNiondphi));
		malloc_zxy_gdouble(dim,&(in->Nion_last));
	}

	//Generation
	malloc_zxy_gdouble(dim,&(in->G));
	malloc_zxy_gdouble(dim,&(in->Gn));
	malloc_zxy_gdouble(dim,&(in->Gp));

	//Free charges
	if (in->drift_diffision_simulations_enabled==TRUE)
	{
		malloc_zxy_gdouble(dim,&(in->n));
		malloc_zxy_gdouble(dim,&(in->p));
		malloc_zxy_gdouble(dim,&(in->dn));
		malloc_zxy_gdouble(dim,&(in->dp));
		malloc_zxy_gdouble(dim,&(in->dndphi));
		malloc_zxy_gdouble(dim,&(in->dpdphi));

		malloc_zxy_gdouble(dim,&(in->Dn));
		malloc_zxy_gdouble(dim,&(in->Dp));

		malloc_zxy_gdouble(dim,&(in->Fn));
		malloc_zxy_gdouble(dim,&(in->Fp));

		malloc_zxy_gdouble(dim,&(in->Nc));
		malloc_zxy_gdouble(dim,&(in->Nv));

		malloc_zxy_gdouble(dim,&(in->nf_save));
		malloc_zxy_gdouble(dim,&(in->pf_save));

		malloc_zxy_gdouble(dim,&(in->nfequlib));
		malloc_zxy_gdouble(dim,&(in->pfequlib));

		malloc_zxy_gdouble(dim,&(in->nlast));
		malloc_zxy_gdouble(dim,&(in->plast));

		malloc_zxy_gdouble(dim,&(in->wn));
		malloc_zxy_gdouble(dim,&(in->wp));

		malloc_zxy_gdouble(dim,&(in->n_orig));
		malloc_zxy_gdouble(dim,&(in->p_orig));

		malloc_zxy_gdouble(dim,&(in->n_orig_f));
		malloc_zxy_gdouble(dim,&(in->p_orig_f));

		malloc_zxy_gdouble(dim,&(in->n_orig_t));
		malloc_zxy_gdouble(dim,&(in->p_orig_t));

		malloc_zxy_gdouble(dim,&(in->t));
		malloc_zxy_gdouble(dim,&(in->tp));
		malloc_zxy_gdouble(dim,&(in->t_ion));

	}
	//Fermi levels
	if (in->drift_diffision_simulations_enabled==TRUE)
	{
		malloc_zxy_gdouble(dim,&(in->Fi));
	}
	//Bands
	if (in->drift_diffision_simulations_enabled==TRUE)
	{
		malloc_zxy_gdouble(dim,&(in->Eg));
		malloc_zxy_gdouble(dim,&(in->Xi));
		malloc_zxy_gdouble(dim,&(in->Ev));
		malloc_zxy_gdouble(dim,&(in->Ec));
	}
	//Recombination
	if (in->drift_diffision_simulations_enabled==TRUE)
	{
		malloc_zxy_gdouble(dim,&(in->Rfree));

		malloc_zxy_gdouble(dim,&(in->Rn));
		malloc_zxy_gdouble(dim,&(in->Rp));

		malloc_zxy_gdouble(dim,&(in->Rn_srh));
		malloc_zxy_gdouble(dim,&(in->Rp_srh));

		malloc_zxy_gdouble(dim,&(in->Rnet));

		malloc_zxy_gdouble(dim,&(in->B));
	}
	//Interfaces
	if (in->drift_diffision_simulations_enabled==TRUE)
	{
		malloc_zxy_int(dim,&(in->interface_type));
		malloc_zxy_gdouble(dim,&in->interface_B);
		malloc_zxy_gdouble(dim,&in->interface_Bt);
		malloc_zxy_gdouble(dim,&in->interface_R);
	}
	//Rates
	if (in->drift_diffision_simulations_enabled==TRUE)
	{
		malloc_zxy_gdouble(dim,&(in->nrelax));
		malloc_zxy_gdouble(dim,&(in->ntrap_to_p));
		malloc_zxy_gdouble(dim,&(in->prelax));
		malloc_zxy_gdouble(dim,&(in->ptrap_to_n));
	}
	//Mobility
	if (in->drift_diffision_simulations_enabled==TRUE)
	{
		malloc_zxy_gdouble(dim,&(in->mun_z));
		malloc_zxy_gdouble(dim,&(in->mun_x));
		malloc_zxy_gdouble(dim,&(in->mun_y));

		malloc_zxy_gdouble(dim,&(in->mup_z));
		malloc_zxy_gdouble(dim,&(in->mup_x));
		malloc_zxy_gdouble(dim,&(in->mup_y));

		malloc_zxy_gdouble(dim,&(in->muion));
	}
	//Electrostatics
		malloc_zxy_gdouble(dim,&(in->epsilonr));
		malloc_zxy_gdouble(dim,&(in->epsilonr_e0));

		malloc_zxy_gdouble(dim,&(in->phi_save));

	//Temperature
		malloc_zxy_gdouble(dim,&(in->Tl));
		malloc_zxy_gdouble(dim,&(in->Te));
		malloc_zxy_gdouble(dim,&(in->Th));

		malloc_zxy_gdouble(dim,&(in->ke));
		malloc_zxy_gdouble(dim,&(in->kh));

	//Heating
		malloc_zxy_gdouble(dim,&(in->Hl));
		malloc_zxy_gdouble(dim,&(in->H_recombination));
		malloc_zxy_gdouble(dim,&(in->H_joule));

		malloc_zxy_gdouble(dim,&(in->He));
		malloc_zxy_gdouble(dim,&(in->Hh));



	//Current
		malloc_zxy_gdouble(dim,&(in->Jn));
		malloc_zxy_gdouble(dim,&(in->Jp));
		malloc_zxy_gdouble(dim,&(in->Jn_x));
		malloc_zxy_gdouble(dim,&(in->Jp_x));

		if (in->drift_diffision_simulations_enabled==TRUE)
		{
			malloc_zxy_gdouble(dim,&(in->Jn_drift));
			malloc_zxy_gdouble(dim,&(in->Jn_diffusion));

			malloc_zxy_gdouble(dim,&(in->Jn_x_drift));
			malloc_zxy_gdouble(dim,&(in->Jn_x_diffusion));

			malloc_zxy_gdouble(dim,&(in->Jp_drift));
			malloc_zxy_gdouble(dim,&(in->Jp_diffusion));

			malloc_zxy_gdouble(dim,&(in->Jp_x_drift));
			malloc_zxy_gdouble(dim,&(in->Jp_x_diffusion));
		}
	//Applied voltages
		malloc_zx_gdouble(dim,&(in->Vapplied_y0));
		malloc_zx_gdouble(dim,&(in->Vapplied_y1));

		malloc_zy_long_double(dim,&(in->Vapplied_x0));
		malloc_zy_long_double(dim,&(in->Vapplied_x1));

	//Passivation
		malloc_zx_int(dim,&(in->passivate_y0));
		malloc_zx_int(dim,&(in->passivate_y1));

		malloc_zy_int(dim,&(in->passivate_x0));
		malloc_zy_int(dim,&(in->passivate_x1));

	//Emission
		malloc_zxy_gdouble(dim,&(in->Photon_gen));

	//Device layout
		malloc_zxy_int(dim,&(in->imat_epitaxy));
		malloc_zxy_int(dim,&(in->mask));

		dim_alloc_zx_epitaxy(&(in->dim_epitaxy),in);
		malloc_zx_epitaxy_int(&(in->dim_epitaxy),&(in->mask_epitaxy));

	//Exciton
		malloc_zxy_gdouble(dim,&(in->ex));
		malloc_zxy_gdouble(dim,&(in->Dex));
		malloc_zxy_gdouble(dim,&(in->Hex));

		malloc_zxy_gdouble(dim,&(in->kf));
		malloc_zxy_gdouble(dim,&(in->kd));
		malloc_zxy_gdouble(dim,&(in->kr));

	//Trap control
		//none

	//Traps 3d n
	if (in->drift_diffision_simulations_enabled==TRUE)
	{
		malloc_zxy_gdouble(dim,&(in->nt_all));
		malloc_zxy_gdouble(dim,&(in->tt));

		malloc_zxy_gdouble(dim,&(in->nt_save));
		malloc_zxy_gdouble(dim,&(in->pt_save));
	}
	//Traps 3d p
	if (in->drift_diffision_simulations_enabled==TRUE)
	{
		malloc_zxy_gdouble(dim,&(in->pt_all));
		malloc_zxy_gdouble(dim,&(in->tpt));

		malloc_zxy_gdouble(dim,&(in->ntequlib));
		malloc_zxy_gdouble(dim,&(in->ptequlib));
	}

	//Traps 4d n
		//in memory.c

	//Traps 4d p
		//in memory.c

	//Newton solver control
		//none

	//Arrays used by newton solver
		//none

	//Electrical components
		//none

	//Dump contorl
		//none

	//Time
		//none

	//Run control
		//none

	//Newton solver internal memory
		matrix_solver_memory_load_dll(sim,&(in->msm));

	//meshing
		//none

	//epitaxy
		//none

	//plugins
		//none

	//Newton states
		//none

	//Light
		//none

	//Emission
		//none

	//Test
		//none

	//thermal
		//none

	//Perovskite
		//none

	//Circuit simulation
		//none

	//Ray tracing
		//none

	//Contacts
		//none

	//Objects
		malloc_zxy_p_object(dim, &(in->obj_zxy));
		malloc_zx_layer_p_object(&(in->dim_epitaxy), &(in->obj_zx_layer));

	//Time mesh
		//none

	//solver cache
		//none

	//////////////////////////////////// not yet done


	malloc_zxy_gdouble(dim,&(in->Rbi_k));

	newton_state_alloc_mesh(ns,dim, FALSE);
	//newton_state_alloc(ns,dim);
	//newton_state_alloc_mesh(&(in->ns),dim);

	//malloc_zxy_gdouble(dim,&(ns->phi));
	//malloc_zxy_gdouble(dim,&(ns->x));
	//malloc_zxy_gdouble(dim,&(ns->xp));
	#ifdef libcircuit_enabled
		circuit_alloc_nodes_and_links(sim,&(in->cir));
	#endif
		//free_zxy_p_object(dim, &(in->obj_zxy));
		//free_zx_layer_p_object(&(in->dim_epitaxy), &(in->obj_zx_layer));

}

void device_to_dim(struct simulation *sim,struct dimensions *dim,struct device *dev)
{
	struct newton_state *ns=(&dev->ns);
	struct json_obj *json_perovskite;
	dim->zlen=dev->mesh_data.meshdata_z.tot_points;
	dim->xlen=dev->mesh_data.meshdata_x.tot_points;
	dim->ylen=dev->mesh_data.meshdata_y.tot_points;
	json_perovskite=json_obj_find(&(dev->config.obj), "perovskite");
	if (json_perovskite==NULL)
	{
		ewe(sim,"Perovskite object not found\n");
	}
	json_get_english(sim, json_perovskite, &(ns->Nion_enabled),"perovskite_enabled");
	printf("here %d\n",ns->Nion_enabled);
}
