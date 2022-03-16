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
#include <heat_fun.h>

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
struct heat *thermal=&(dev->thermal);
long double mue=0.0;
long double muh=0.0;
long double H_joule=0.0;
long double H_recombination=0.0;
long double H_parasitic=0.0;

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
	s0=dJ/dim->dY[0]/Qe;
	inter_append(&(store->R_surface_y0),x_value,s0);

	J_tot=fabs(dev->Jn_y1[0][0])+fabs(dev->Jp_y1[0][0]);
	J_out=fabs(dev->Jn_y1[0][0]+dev->Jp_y1[0][0]);
	dJ=J_tot-J_out;
	s1=dJ/dim->dY[dim->ylen-1]/Qe;
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
	get_avg_mu(dev,&mue,&muh);
	inter_append(&(store->mu_n),x_value,fabs(mue));
	inter_append(&(store->mu_p),x_value,fabs(muh));
	inter_append(&(store->mu_n_p_avg),x_value,(fabs(mue)+fabs(muh))/2.0);

	//Thermal
	if (thermal->newton_enable_external_thermal==TRUE)
	{
		heat_cal_get_heating_sources_percent(sim,dev,&(dev->thermal),&H_joule,&H_recombination,&H_parasitic);
		inter_append(&(store->H_joule),x_value,fabs(H_joule));
		inter_append(&(store->H_recombination),x_value,fabs(H_recombination));
		inter_append(&(store->H_parasitic),x_value,fabs(H_parasitic));
	}
	//getchar();

	//srh rates

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
	long double val=(ns->phi[0][0][(dim->ylen/2)+1]-ns->phi[0][0][(dim->ylen/2)])/(dim->y[(dim->ylen/2)+1]-dim->y[(dim->ylen/2)]);
	inter_append(&(store->E_field),x_value,val);

	Vapplied=contact_get_voltage(sim,dev,0);
	inter_append(&(store->dynamic_Vapplied),x_value,Vapplied);

	//other
	inter_append(&(store->dynamic_qe),x_value,-100.0*(get_J(dev)/dev->ylen)/get_avg_gen(dev)/Qe);

	gdouble tot=0.0;

	if (store->band_snapshot==NULL)
	{
		malloc_zxy_long_double(dim,&(store->band_snapshot));

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


