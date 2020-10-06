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

/** @file dynamic_append.c
@brief append for: setup the dynamic dump stuff, this enables things like average charge density to be stored for each simulation stip and then wrtten to disk
*/

#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <i.h>
#include <exp.h>
#include <dos.h>
#include "sim.h"
#include "dump.h"
#include "dat_file.h"
#include "dynamic_store.h"
#include "memory.h"
#include "contacts.h"
#include <lang.h>
#include <cal_path.h>


static int unused __attribute__((unused));

void dump_dynamic_add_data(struct simulation *sim,struct dynamic_store *store,struct device *dev, gdouble x_value)
{
int x=0;
int y=0;
int z=0;
long double J_tot=0.0;
long double J_out=0.0;
long double dJ=0.0;
long double s0=0.0;
long double s1=0.0;

gdouble Vapplied=0.0;
struct newton_state *ns=&(dev->ns);
struct dimensions *dim=&dev->ns.dim;

if (dev->circuit_simulation==TRUE)
{
	return;
}

if (get_dump_status(sim,dump_dynamic)==TRUE)
{
	//recombination
	inter_append(&(store->R_nfree_to_pfree),x_value,three_d_avg(dev, dev->Rfree));		 
	inter_append(&(store->R_srh_nfree),x_value,three_d_avg(dev, dev->Rn_srh));
	inter_append(&(store->R_srh_pfree),x_value,three_d_avg(dev, dev->Rp_srh));

	inter_append(&(store->G_n),x_value,three_d_avg(dev, dev->Gn));
	inter_append(&(store->G_p),x_value,three_d_avg(dev, dev->Gn));

	inter_append(&(store->R_srh_nfree_to_ptrap),x_value,three_d_avg(dev, dev->ptrap_to_n));
	inter_append(&(store->R_srh_pfree_to_ntrap),x_value,three_d_avg(dev, dev->ntrap_to_p));

	inter_append(&(store->T_srh_nfree_to_ntrap),x_value,three_d_avg(dev, dev->nrelax));
	inter_append(&(store->T_srh_pfree_to_ptrap),x_value,three_d_avg(dev, dev->prelax));

	J_tot=fabs(dev->Jn_y0[0][0])+fabs(dev->Jp_y0[0][0]);
	J_out=fabs(dev->Jn_y0[0][0]+dev->Jp_y0[0][0]);
	dJ=J_tot-J_out;
	s0=dJ/dim->dy[0]/Q;
	inter_append(&(store->R_surface_y0),x_value,s0);

	J_tot=fabs(dev->Jn_y1[0][0])+fabs(dev->Jp_y1[0][0]);
	J_out=fabs(dev->Jn_y1[0][0]+dev->Jp_y1[0][0]);
	dJ=J_tot-J_out;
	s1=dJ/dim->dy[dim->ylen-1]/Q;
	inter_append(&(store->R_surface_y1),x_value,s1);

	//charge
	inter_append(&(store->Q_ntrap),x_value,get_n_trapped_charge(dev));
	inter_append(&(store->Q_ptrap),x_value,get_p_trapped_charge(dev));
	inter_append(&(store->Q_nfree),x_value,get_free_n_charge(dev));
	inter_append(&(store->Q_pfree),x_value,get_free_p_charge(dev));
	inter_append(&(store->Q_nfree_and_ntrap),x_value,get_free_n_charge(dev)+get_n_trapped_charge(dev));
	inter_append(&(store->Q_pfree_and_ptrap),x_value,get_free_p_charge(dev)+get_p_trapped_charge(dev));

	//inter_append(&(store->charge_change),x_value,get_charge_change(dev));
	//inter_append(&(store->dynamic_charge_tot),x_value,get_charge_tot(dev));
	//inter_append(&(store->ntrap_delta_out),x_value,get_n_trapped_charge_delta(dev));
	//inter_append(&(store->ptrap_delta_out),x_value,get_p_trapped_charge_delta(dev));
	//inter_append(&(store->nfree_delta_out),x_value,get_free_n_charge_delta(dev));
	//inter_append(&(store->pfree_delta_out),x_value,get_free_p_charge_delta(dev));
	//inter_append(&(store->tpc_filledn),x_value,get_dos_filled_n(dev));
	//inter_append(&(store->tpc_filledp),x_value,get_dos_filled_p(dev));
	//inter_append(&(store->dynamic_np),x_value,get_extracted_np(dev));


	//mobility
	inter_append(&(store->mu_n),x_value,fabs(get_avg_mue(dev)));
	inter_append(&(store->mu_p),x_value,fabs(get_avg_muh(dev)));
	inter_append(&(store->mu_n_p_avg),x_value,(fabs(get_avg_mue(dev))+fabs(get_avg_muh(dev)))/2.0);

	//printf(">>%Le %Le\n",1.0000e-5*get_free_n_charge(dev)/(get_free_n_charge(dev)+get_n_trapped_charge(dev)),get_avg_mue(dev));
	//srh rates
	int i;
	int band;
	gdouble srh_n_r1=0.0;
	gdouble srh_n_r2=0.0;
	gdouble srh_n_r3=0.0;
	gdouble srh_n_r4=0.0;
	gdouble srh_p_r1=0.0;
	gdouble srh_p_r2=0.0;
	gdouble srh_p_r3=0.0;
	gdouble srh_p_r4=0.0;

	for (z=0;z<dim->zlen;z++)
	{
		for (x=0;x<dim->xlen;x++)
		{
			for (y=0;y<dim->ylen;y++)
			{
				for (band=0;band<dim->srh_bands;band++)
				{
					srh_n_r1+=dev->n[z][x][y]*dev->srh_n_r1[z][x][y][band];
					srh_n_r2+=dev->srh_n_r2[z][x][y][band];
					srh_n_r3+=dev->p[z][x][y]*dev->srh_n_r3[z][x][y][band];
					srh_n_r4+=dev->srh_n_r4[z][x][y][band];

					srh_p_r1+=dev->p[z][x][y]*dev->srh_p_r1[z][x][y][band];
					srh_p_r2+=dev->srh_p_r2[z][x][y][band];
					srh_p_r3+=dev->n[z][x][y]*dev->srh_p_r3[z][x][y][band];
					srh_p_r4+=dev->srh_p_r4[z][x][y][band];
				}
			}
		}
	}

	inter_append(&(store->srh_n_r1),x_value,srh_n_r1/((gdouble)(dim->ylen*dim->srh_bands)));
	inter_append(&(store->srh_n_r2),x_value,srh_n_r2/((gdouble)(dim->ylen*dim->srh_bands)));
	inter_append(&(store->srh_n_r3),x_value,srh_n_r3/((gdouble)(dim->ylen*dim->srh_bands)));
	inter_append(&(store->srh_n_r4),x_value,srh_n_r4/((gdouble)(dim->ylen*dim->srh_bands)));

	inter_append(&(store->srh_p_r1),x_value,srh_p_r1/((gdouble)(dim->ylen*dim->srh_bands)));
	inter_append(&(store->srh_p_r2),x_value,srh_p_r2/((gdouble)(dim->ylen*dim->srh_bands)));
	inter_append(&(store->srh_p_r3),x_value,srh_p_r3/((gdouble)(dim->ylen*dim->srh_bands)));
	inter_append(&(store->srh_p_r4),x_value,srh_p_r4/((gdouble)(dim->ylen*dim->srh_bands)));

	//J
	inter_append(&(store->J_y0_n),x_value,dev->Jn_y0[0][0]);
	inter_append(&(store->J_y0_p),x_value,dev->Jp_y0[0][0]);
	inter_append(&(store->J_y1_n),x_value,dev->Jn_y1[0][0]);
	inter_append(&(store->J_y1_p),x_value,dev->Jp_y1[0][0]);

	inter_append(&(store->jn_avg),x_value,get_jn_avg(dev));
	inter_append(&(store->jp_avg),x_value,get_jp_avg(dev));
	inter_append(&(store->dynamic_jn),x_value,(dev->Jn_y0[0][0]+dev->Jn_y1[0][0])/2.0);
	inter_append(&(store->dynamic_jp),x_value,(dev->Jp_y0[0][0]+dev->Jp_y1[0][0])/2.0);
	inter_append(&(store->jout),x_value,get_J(dev));
	inter_append(&(store->dynamic_jn_drift),x_value,get_Jn_drift(dev));
	inter_append(&(store->dynamic_jn_diffusion),x_value,get_Jn_diffusion(dev));

	inter_append(&(store->dynamic_jp_drift),x_value,get_Jp_drift(dev));
	inter_append(&(store->dynamic_jp_diffusion),x_value,get_Jp_diffusion(dev));
	inter_append(&(store->jnout_mid),x_value,dev->Jn[dim->zlen/2][dim->xlen/2][dim->ylen/2]);

	inter_append(&(store->jpout_mid),x_value,dev->Jp[dim->zlen/2][dim->xlen/2][dim->ylen/2]);

	inter_append(&(store->iout),x_value,get_equiv_I(sim,dev));

	//pl
	if (dev->emission_enabled==TRUE)
	{
		inter_append(&(store->dynamic_pl),x_value,dev->pl_intensity);
		inter_append(&(store->dynamic_pl_tot),x_value,dev->pl_intensity_tot);
	}

	//field
	long double val=(ns->phi[0][0][(dim->ylen/2)+1]-ns->phi[0][0][(dim->ylen/2)])/(dim->ymesh[(dim->ylen/2)+1]-dim->ymesh[(dim->ylen/2)]);
	inter_append(&(store->E_field),x_value,val);

	Vapplied=contact_get_voltage(sim,dev,0);
	inter_append(&(store->dynamic_Vapplied),x_value,Vapplied);

	//other
	inter_append(&(store->dynamic_qe),x_value,-100.0*(get_J(dev)/dev->ylen)/get_avg_gen(dev)/Q);

	gdouble tot=0.0;

	if (store->band_snapshot==NULL)
	{
		malloc_zxy_gdouble(dim,&(store->band_snapshot));

		for (z=0;z<dim->zlen;z++)
		{
			for (x=0;x<dim->xlen;x++)
			{
				for (y=0;y<dim->ylen;y++)
				{
					store->band_snapshot[z][x][y]=ns->phi[z][x][y];
				}
			}
		}
	}

	for (z=0;z<dim->zlen;z++)
	{
		for (x=0;x<dim->xlen;x++)
		{
			for (y=0;y<dim->ylen;y++)
			{
				tot+=fabs(store->band_snapshot[z][x][y]-ns->phi[z][x][y])/fabs(ns->phi[z][x][y]);
			}
		}
	}

	tot/=(gdouble)(dim->ylen);
	inter_append(&(store->band_bend),x_value,tot);

}

}


