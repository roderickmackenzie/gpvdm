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

/** @file exp.c
	@brief Pefrorm experiments on the device, such as count charge density, calculate average position of electrons/holes etc...
*/

#include <exp.h>
#include "sim.h"
#include "i.h"
#include "dat_file.h"
#include "contacts.h"
#include <memory.h>

static gdouble n_count=0.0;
static gdouble p_count=0.0;
static gdouble rn_count=0.0;
static gdouble rp_count=0.0;

/**
* @brief Average position of electron and holes (not updated)
*/
void get_avg_np_pos(struct device *in,gdouble *nx,gdouble *px)
{
int x;
int y;
int z;

gdouble navg=0.0;
gdouble pavg=0.0;
gdouble nsum=0.0;
gdouble psum=0.0;
struct newton_state *ns=&in->ns;
struct dimensions *dim=&in->ns.dim;

for (z=0;z<dim->zlen;z++)
{
	for (x=0;x<dim->xlen;x++)
	{
		for (z=0;z<dim->zlen;z++)
		{
			navg+=(in->n[z][x][y]+in->nt_all[z][x][y]-in->n_orig[z][x][y])*dim->ymesh[y];
			pavg+=(in->p[z][x][y]+in->pt_all[z][x][y]-in->p_orig[z][x][y])*dim->ymesh[y];
			nsum+=(in->n[z][x][y]+in->nt_all[z][x][y]-in->n_orig[z][x][y]);
			psum+=in->p[z][x][y]+in->pt_all[z][x][y]-in->p_orig[z][x][y];
		}
	}
}

if (nsum!=0.0)
{
	*nx=navg/nsum;
}else
{
	*nx=0.0;
}

if (psum!=0.0)
{
*px=pavg/psum;
}else
{
*px=0.0;
}



}

/**
* @brief Get the change in charge density (not updated)
*/
gdouble get_charge_change(struct device *in)
{

gdouble diff=0.0;
gdouble n=0.0;
gdouble p=0.0;

int x=0;
int y=0;
int z=0;

int band=0;
struct dimensions *dim=&(in->ns.dim);

if (in->go_time==TRUE)
{

	for (z=0;z<dim->zlen;z++)
	{
		for (x=0;x<dim->xlen;x++)
		{
			for (y=0;y<dim->ylen;y++)
			{
				n=0.0;
				p=0.0;

				for (band=0;band<dim->srh_bands;band++)
				{
					n+=in->ntlast[z][x][y][band];
					p+=in->ptlast[z][x][y][band];
				}

				n+=in->nlast[z][x][y];
				p+=in->plast[z][x][y];

				diff+=fabs(in->p[z][x][y]+in->pt_all[z][x][y]-in->n[z][x][y]-in->nt_all[z][x][y]+n-p);
			}
		}
	}

	diff/=(n+p);
	diff*=100.0;
}

return diff;
}


/**
* @brief Get the average recombination density
*/
gdouble get_tot_photons_abs(struct device *in)
{
long double ret=0.0;

ret=three_d_integrate(&(in->ns.dim), in->Gn);

return ret;
}


/**
* @brief Get the average recombination density
*/
gdouble get_avg_recom(struct device *in)
{
long double ret=0.0;

ret=three_d_avg(in, in->ptrap_to_n);
ret+=three_d_avg(in, in->ntrap_to_p);
ret+=three_d_avg(in, in->Rfree);

return ret;
}

/**
* @brief Get the average electron relaxation rate
*/
gdouble get_avg_relax_n(struct device *in)
{
long double ret=0.0;

ret=three_d_avg(in, in->nrelax);

return ret;
}

/**
* @brief Get the average hole relaxation rate
*/
gdouble get_avg_relax_p(struct device *in)
{
long double ret=0.0;

ret=three_d_avg(in, in->prelax);

return ret;
}


/**
* @brief Free electron recombination rate
* Note this calculates the real free electron recombination rate
* not the removal of electrons from the band
*/
gdouble get_avg_recom_n(struct device *in)
{
long double ret=0.0;

ret=three_d_avg(in, in->ptrap_to_n);

return ret;
}

/**
* @brief Free hole recombination rate
* Note this calculates the real free hole recombination rate
* not the removal of holes from the band
*/
gdouble get_avg_recom_p(struct device *in)
{
long double ret=0.0;

ret=three_d_avg(in, in->ntrap_to_p);

return ret;
}

/**
* @brief Removal of electrons from the band
*/
gdouble get_avg_Rn(struct device *in)
{
long double ret=0.0;

ret=three_d_avg(in, in->Rn);

return ret;
}

/**
* @brief Removal of holes from the band
*/
gdouble get_avg_Rp(struct device *in)
{
long double ret=0.0;

ret=three_d_avg(in, in->Rp);

return ret;
}

/**
* @brief Calculation of k the recombination prefactor
*/
gdouble get_avg_k(struct device *in)
{
gdouble n=(get_extracted_n(in)*get_extracted_p(in));
gdouble R=(get_avg_recom_n(in)+get_avg_recom_p(in))/2.0;
gdouble k=R/n;
return k;
}

/**
* @brief Carrier count (depreshated)
*/
gdouble carrier_count_get_n(struct device *in)
{
return n_count;
}

/**
* @brief Carrier count (depreshated)
*/
gdouble carrier_count_get_p(struct device *in)
{
return p_count;
}

/**
* @brief Carrier count (depreshated)
*/
gdouble  carrier_count_get_rn(struct device *in)
{
return rn_count;
}

/**
* @brief Carrier count (depreshated)
*/
gdouble carrier_count_get_rp(struct device *in)
{
return rp_count;
}

/**
* @brief Carrier count (depreshated)
*/
void carrier_count_reset(struct device *in)
{
	n_count=0.0;
	p_count=0.0;
	rn_count=0.0;
	rp_count=0.0;
}


/**
* @brief Calculate J from the recombination rate
* Not updated for variable mesh
*/
gdouble get_J_recom(struct device *in)
{
int x=0;
int y=0;
int z=0;

gdouble Rtot=0.0;
gdouble add=0.0;
struct newton_state *ns=&in->ns;
struct dimensions *dim=&in->ns.dim;

gdouble dx=dim->ymesh[1]-dim->ymesh[0];



for (z=0;z<dim->zlen;z++)
{
	for (x=0;x<dim->xlen;x++)
	{
		for (y=0;y<dim->ylen;y++)
		{

			add=(in->Rn[z][x][y]-in->Gn[z][x][y]-in->Rp[z][x][y]-in->Gn[z][x][y])*dx/2.0;

			Rtot+=add;
		}
	}
}

return Rtot*Q;
}


/**
* @brief Calculate the average J but ignores and the std of J
* throw away values of J which are outside a given std range of J
* used when the current does not converge very well.
* Not updated for variable mesh
*/
gdouble get_avg_J_std(struct device *in)
{
int x=0;
int y=0;
int z=0;

gdouble J=0.0;
gdouble Javg=0.0;
gdouble Jstd_dev=0.0;
gdouble Jtot=0.0;

struct dimensions *dim=&in->ns.dim;

for (z=0;z<dim->zlen;z++)
{
	for (x=0;x<dim->xlen;x++)
	{
		for (y=0;y<dim->ylen;y++)
		{
			Javg+=in->Jn[z][x][y]+in->Jp[z][x][y];
		}
	}
}

Javg/=(gdouble)dim->ylen;

for (z=0;z<dim->zlen;z++)
{
	for (x=0;x<dim->xlen;x++)
	{
		for (y=0;y<dim->ylen;y++)
		{
			Jstd_dev+=pow(((in->Jn[z][x][y]+in->Jp[z][x][y])-Javg),2.0);
		}
	}
}

Jstd_dev/=(gdouble)dim->ylen;
Jstd_dev=sqrt(Jstd_dev);

for (z=0;z<dim->zlen;z++)
{
	for (x=0;x<dim->xlen;x++)
	{
		for (y=0;y<dim->ylen;y++)
		{
			J=in->Jn[z][x][y]+in->Jp[z][x][y];
			if (fabs(J-Javg)<Jstd_dev*0.05)
			{
				Jtot+=in->Jn[z][x][y]+in->Jp[z][x][y];


			}
		}
	}
}
Jtot/=(gdouble)dim->xlen;
Jtot/=(gdouble)dim->ylen;
Jtot/=(gdouble)dim->zlen;


return Javg;
}

/**
* @brief Calculate the average J but ignores and the std of J
* throw away values of J which are outside a given std range of J
* used when the current does not converge very well.
* Not updated for variable mesh
*/
gdouble get_avg_J(struct device *in)
{
gdouble Javg=0.0;
Javg=(three_d_avg(in, in->Jn)+three_d_avg(in, in->Jp))/2.0;

return Javg;
}

/**
* @brief Calculate the average Jn
*/
gdouble get_jn_avg(struct device *in)
{
long double ret=0.0;

ret=three_d_avg(in, in->Jn);

ret*=in->flip_current;

return ret;
}

/**
* @brief Calculate the average Jp
*/
gdouble get_jp_avg(struct device *in)
{
long double ret=0.0;

ret=three_d_avg(in, in->Jp);

ret*=in->flip_current;

return ret;
}

/**
* @brief Carrier count (depreshated)
*/
void carrier_count_add(struct device *in)
{
int x=0;
int y=0;
int z=0;
struct newton_state *ns=&in->ns;
struct dimensions *dim=&in->ns.dim;

gdouble locat_n_tot=0.0;
gdouble locat_p_tot=0.0;

for (z=0;z<dim->zlen;z++)
{
	for (x=0;x<dim->xlen;x++)
	{
		n_count+=(in->Jn[z][x][0]+in->Jn[z][x][dim->ylen-1])*in->dt/Q;
		p_count+=(in->Jp[z][x][0]+in->Jp[z][x][dim->ylen-1])*in->dt/Q;
	}
}

gdouble dx=dim->ymesh[1]-dim->ymesh[0];
for (z=0;z<dim->zlen;z++)
{
	for (x=0;x<dim->xlen;x++)
	{
		for (y=0;y<dim->ylen;y++)
		{
			locat_n_tot+=in->Rfree[z][x][y]*dx;
			locat_p_tot+=in->Rfree[z][x][y]*dx;
		}
	}
}
rn_count+=locat_n_tot*in->dt;
rp_count+=locat_p_tot*in->dt;

}

/**
* @brief Calculate extracted np
*/
gdouble get_extracted_np(struct device *in)
{
return (get_extracted_n(in)+get_extracted_p(in))/2.0;
}

/**
* @brief Calculate extracted n from a save point
*/
gdouble get_extracted_n(struct device *in)
{
long double ret=0.0;

long double n=0.0;
long double nf_save=0.0;

long double nt_all=0.0;
long double nt_save=0.0;

n=three_d_avg(in, in->n);
nf_save=three_d_avg(in, in->nf_save);

nt_all=three_d_avg(in, in->n);
nt_save=three_d_avg(in, in->nf_save);

ret=n-nf_save +nt_all-nt_save;

return ret;
}

/**
* @brief Get the total number of charge carriers in the device.
*/
gdouble get_total_np(struct device *in)
{
long double ret=0.0;
long double n=0.0;
long double p=0.0;
long double nt_all=0.0;
long double pt_all=0.0;

n=three_d_avg(in, in->n);
p=three_d_avg(in, in->p);

nt_all=three_d_avg(in, in->n);
pt_all=three_d_avg(in, in->n);

ret=n+p+nt_all+pt_all;

ret/=2.0;
return ret;
}

/**
* @brief Get the number of extracted holes
*/
gdouble get_extracted_p(struct device *in)
{
long double sum_p=0.0;

sum_p=three_d_avg(in, in->p);
sum_p-=three_d_avg(in, in->pf_save);

sum_p+=three_d_avg(in, in->pt_all);
sum_p-=three_d_avg(in, in->pt_save);

return sum_p;
}

/**
* @brief Calculate the maximum possible Jsc value from the generation rate.
*/
gdouble get_max_Jsc(struct device *in)
{
int x=0;
int y=0;
int z=0;

gdouble tot=0.0;
struct dimensions *dim=&in->ns.dim;

for (z=0;z<dim->zlen;z++)
{
	for (x=0;x<dim->xlen;x++)
	{
		for (y=0;y<dim->ylen;y++)
		{
			tot+=in->Gn[z][x][y]*dim->dy[y];
		}
	}
}

return -Q*tot;
}

gdouble get_background_charge(struct device *in)
{
long double sum=0.0;

sum=three_d_avg(in, in->nf_save);
sum+=three_d_avg(in, in->nt_save);

sum+=three_d_avg(in, in->pf_save);
sum+=three_d_avg(in, in->pt_save);

sum/=2.0;

return sum;
}

/**
* @brief Calculate the value of k
* Not updated for a variable mesh
*/
gdouble get_extracted_k(struct device *in)
{
int x=0;
int y=0;
int z=0;

gdouble tot=0.0;
gdouble n=0.0;
struct dimensions *dim=&in->ns.dim;


for (z=0;z<dim->zlen;z++)
{
	for (x=0;x<dim->xlen;x++)
	{
		for (y=0;y<dim->ylen;y++)
		{
			tot+=in->Rfree[z][x][y];
			n+=in->n[z][x][y]*in->p[z][x][y];
		}
	}
}

return tot/n;
}

/**
* @brief Calculate the average generation rate
*/
gdouble get_avg_gen(struct device *in)
{
long double ret_Gn=0.0;
long double ret_Gp=0.0;

ret_Gn=three_d_avg(in, in->Gn);
ret_Gp=three_d_avg(in, in->Gp);

return (ret_Gn+ret_Gp)/2.0;
}

/**
* @brief Calculate the average electron mobility
*/
gdouble get_avg_mue(struct device *in)
{
int x=0;
int y=0;
int z=0;
long double sum=0.0;
long double ret=0.0;
struct dimensions *dim=&in->ns.dim;
long double count=0.0;
	for (z = 0; z < dim->zlen; z++)
	{
		for (x = 0; x < dim->xlen; x++)
		{
			for (y = 0; y < dim->ylen; y++)
			{
				sum+=in->mun_y[z][x][y]*(in->n[z][x][y]*dim->dx[x]*dim->dy[y]*dim->dz[z])/(in->nt_all[z][x][y]+in->n[z][x][y]);
			}

		}
	}

ret=sum/(in->zlen*in->xlen*in->ylen);

return ret;
}

/**
* @brief Calculate the average hole mobility
*/
gdouble get_avg_muh(struct device *in)
{
int x=0;
int y=0;
int z=0;
long double sum=0.0;
long double ret=0.0;

struct dimensions *dim=&in->ns.dim;

	for (z = 0; z < dim->zlen; z++)
	{
		for (x = 0; x < dim->xlen; x++)
		{
			for (y = 0; y < dim->ylen; y++)
			{
				sum+=in->mup_y[z][x][y]*in->p[z][x][y]*dim->dx[x]*dim->dy[y]*dim->dz[z]/(in->pt_all[z][x][y]+in->p[z][x][y]);
			}

		}
	}

ret=sum/(in->zlen*in->xlen*in->ylen);

return ret;
}

/**
* @brief Calculate average charge density in the device.
*/
gdouble get_np_tot(struct device *in)
{
long double sum=0.0;

sum=three_d_avg(in, in->n);
sum+=three_d_avg(in, in->p);

sum+=three_d_avg(in, in->pt_all);
sum+=three_d_avg(in, in->nt_all);

sum/=2.0;

return sum;
}

/**
* @brief Calculate average change in free carrier density in the device
*/
gdouble get_free_np_avg(struct device *in)
{
long double sum=0.0;

sum=three_d_avg(in, in->n);
sum+=three_d_avg(in, in->p);

sum-=three_d_avg(in, in->nf_save);
sum-=three_d_avg(in, in->pf_save);

sum/=2.0;

return sum;
}

/**
* @brief Calculate free electron density
*/
gdouble get_free_n_charge(struct device *in)
{
long double ret=0.0;

ret=three_d_avg(in, in->n);

return ret;
}

/**
* @brief Calculate free hole density
*/
gdouble get_free_p_charge(struct device *in)
{
long double ret=0.0;

ret=three_d_avg(in, in->p);

return ret;
}

/**
* @brief Total charge in the device
*/
gdouble get_charge_tot(struct device *in)
{
long double ret=0.0;
long double n=0.0;
long double p=0.0;
long double pt_all=0.0;
long double nt_all=0.0;

n=three_d_avg(in, in->n);
p=three_d_avg(in, in->p);
nt_all=three_d_avg(in, in->nt_all);
pt_all=three_d_avg(in, in->pt_all);

ret=p-n+pt_all-nt_all;
return ret;
}

/**
* @brief Set the origonal charge density
*/
void set_orig_charge_den(struct device *in)
{
int x=0;
int y=0;
int z=0;
struct dimensions *dim=&in->ns.dim;

for (z=0;z<dim->zlen;z++)
{
	for (x=0;x<dim->xlen;x++)
	{
		for (y=0;y<dim->ylen;y++)
		{
			in->n_orig[z][x][y]=in->n[z][x][y]+in->nt_all[z][x][y];
			in->p_orig[z][x][y]=in->p[z][x][y]+in->pt_all[z][x][y];

			in->n_orig_f[z][x][y]=in->n[z][x][y];
			in->p_orig_f[z][x][y]=in->p[z][x][y];

			in->n_orig_t[z][x][y]=in->nt_all[z][x][y];
			in->p_orig_t[z][x][y]=in->pt_all[z][x][y];
		}
	}
}
}

/**
* @brief Calculate the change in free electron density
*/
gdouble get_free_n_charge_delta(struct device *in)
{
long double sum=0.0;

sum=three_d_avg(in, in->n);
sum-=three_d_avg(in, in->n_orig_f);

return sum;
}

/**
* @brief Calculate the change in free hole density
*/
gdouble get_free_p_charge_delta(struct device *in)
{
long double sum=0.0;

sum=three_d_avg(in, in->p);
sum-=three_d_avg(in, in->p_orig_f);

return sum;
}

/**
* @brief Save the device parameters
*/
void reset_np_save(struct device *in)
{
int x=0;
int y=0;
int z=0;

int band;

struct newton_state *ns=&(in->ns);
struct dimensions *dim=&in->ns.dim;

for (z=0;z<dim->zlen;z++)
{
	for (x=0;x<dim->xlen;x++)
	{
		for (y=0;y<dim->ylen;y++)
		{
			in->nf_save[z][x][y]=in->n[z][x][y];
			in->pf_save[z][x][y]=in->p[z][x][y];
			in->nt_save[z][x][y]=in->nt_all[z][x][y];
			in->pt_save[z][x][y]=in->pt_all[z][x][y];
			in->phi_save[z][x][y]=ns->phi[z][x][y];
			for (band=0;band<dim->srh_bands;band++)
			{
				in->ntb_save[z][x][y][band]=in->nt[z][x][y][band];
				in->ptb_save[z][x][y][band]=in->pt[z][x][y][band];
			}
		}
	}
}

}

/**
* @brief Reset np equlib
*/
void reset_npequlib(struct device *in)
{

	int x=0;
	int y=0;
	int z=0;

	struct dimensions *dim=&in->ns.dim;

	for (z=0;z<dim->zlen;z++)
	{
		for (x=0;x<dim->xlen;x++)
		{
			for (y=0;y<dim->ylen;y++)
			{
				in->nfequlib[z][x][y]=in->n[z][x][y];
				in->pfequlib[z][x][y]=in->p[z][x][y];
				in->ntequlib[z][x][y]=in->nt_all[z][x][y];
				in->ptequlib[z][x][y]=in->pt_all[z][x][y];
			}
		}
	}
}

/**
* @brief Get the trapped electron density
*/
gdouble get_n_trapped_charge(struct device *in)
{
long double ret=0.0;

ret=three_d_avg(in, in->nt_all);

return ret;
}

/**
* @brief Get the trapped hole density
*/
gdouble get_p_trapped_charge(struct device *in)
{
long double ret=0.0;

ret=three_d_avg(in, in->pt_all);

return ret;
}

/**
* @brief Get the change in charge using orig
*/
gdouble get_charge_delta(struct device *in)
{
long double tot=0.0;

tot=three_d_avg(in, in->n);
tot+=three_d_avg(in, in->nt_all);
tot-=three_d_avg(in, in->n_orig);

tot+=three_d_avg(in, in->p);
tot+=three_d_avg(in, in->pt_all);
tot-=three_d_avg(in, in->p_orig);

tot/=2.0;

return tot;
}

/**
* @brief Get the change in trapped electrons using orig
*/
gdouble get_n_trapped_charge_delta(struct device *in)
{
long double tot=0.0;

tot=three_d_avg(in, in->nt_all);
tot-=three_d_avg(in, in->n_orig_t);

return tot;
}

/**
* @brief Get the change in trapped holes using orig
*/
gdouble get_p_trapped_charge_delta(struct device *in)
{
long double tot=0.0;

tot=three_d_avg(in, in->pt_all);
tot-=three_d_avg(in, in->p_orig_t);

return tot;
}

/**
* @brief Calculate the external current using internal recombination
*/
gdouble get_I_recomb(struct device *in)
{
gdouble ret=0.0;

ret=(get_J_recom(in))*(in->xlen*in->zlen)/2.0;

return ret;
}

/**
* @brief Calculate the external current using the values on the contacts
*/
gdouble get_I(struct device *in)
{
gdouble ret=0.0;
ret+=(get_J_left(in)+get_J_right(in))*(in->xlen*in->zlen)/2.0;

return ret;
}

/**
* @brief Calculate i by intergrating across the device.
*/
gdouble get_i_intergration(struct device *in)
{
long double tot=0.0;
long double ret=0.0;

tot=three_d_avg(in, in->Jn);
tot+=three_d_avg(in, in->Jp);

ret=(in->xlen*in->zlen)*tot;
return ret;
}

/**
* @brief Calculate I including paracitic commponents
*/
gdouble get_equiv_I(struct simulation *sim,struct device *in)
{
gdouble Iout=0.0;
Iout=get_equiv_J(sim,in)*(in->xlen*in->zlen);
return Iout;
}

/**
* @brief Calculate J including paracitic commponents
*/
gdouble get_equiv_J(struct simulation *sim,struct device *in)
{
gdouble Vapplied=0.0;
Vapplied=contact_get_active_contact_voltage(sim,in);
gdouble J=0.0;
J=get_J(in);
//if (in->lr_pcontact==RIGHT) J*= -1.0;
J+=Vapplied/in->Rshunt/in->area;

return J;
}

/**
* @brief Get I for charge extraction
*/
gdouble get_I_ce(struct simulation *sim,struct device *in)
{
gdouble Vapplied=0.0;
Vapplied=contact_get_active_contact_voltage(sim,in);

gdouble ret=Vapplied/(in->Rcontact+in->Rshort);
if (in->time<0.0)
{
	ret=0.0;
}
return ret;
}

/**
* @brief Get the average field across the device
*/
gdouble get_avg_field(struct device *in)
{
	struct newton_state *ns=&(in->ns);
	struct dimensions *dim=&in->ns.dim;
	return (ns->phi[0][0][dim->ylen-1]-ns->phi[0][0][0]);
}

/**
* @brief Calculate the drift and diffusion currents
* (Not updated for varialbe mesh)
*/
void cal_J_drift_diffusion(struct device *in)
{
return;
gdouble Ecl=0.0;
gdouble Ecr=0.0;
gdouble dEc=0.0;

gdouble Evl=0.0;
gdouble Evr=0.0;
gdouble dEv=0.0;

gdouble nl=0.0;
gdouble nr=0.0;
gdouble dn=0.0;
gdouble pl=0.0;
gdouble pr=0.0;
gdouble dp=0.0;
gdouble xl=0.0;
gdouble xr=0.0;
gdouble dx=0.0;

int x=0;
int y=0;
int z=0;
struct newton_state *ns=&in->ns;

struct dimensions *dim=&in->ns.dim;

for (z=0;z<dim->zlen;z++)
{
	for (x=0;x<dim->xlen;x++)
	{
		for (y=0;y<dim->ylen;y++)
		{
			if (y==0)
			{
				nl=in->n[z][x][0];
				pl=in->p[z][x][0];
				Ecl=in->Ec[z][x][0];
				Evl=in->Ev[z][x][0];
				xl=dim->ymesh[0];
			}else
			{
				nl=in->n[z][x][y-1];
				pl=in->p[z][x][y-1];
				Ecl=in->Ec[z][x][y-1];
				Evl=in->Ev[z][x][y-1];
				xl=dim->ymesh[y-1];
			}

			if (y==dim->ylen-1)
			{
				nr=in->n[z][x][dim->ylen-1];
				pr=in->p[z][x][dim->ylen-1];
				Ecr=in->Ec[z][x][dim->ylen-1];
				Evr=in->Ev[z][x][dim->ylen-1];
				xr=dim->ymesh[dim->ylen-1];
			}else
			{
				nr=in->n[z][x][y+1];
				pr=in->p[z][x][y+1];
				Ecr=in->Ec[z][x][y+1];
				Evr=in->Ev[z][x][y+1];
				xr=dim->ymesh[y+1];
			}
			dn=(nr-nl);
			dp=(pr-pl);
			dEc=Ecr-Ecl;
			dEv=Evr-Evl;
			dx=(xr-xl);
			in->Jn_diffusion[z][x][y]=Q*in->Dn[z][x][y]*dn/dx;
			in->Jn_drift[z][x][y]=Q*in->mun_y[z][x][y]*in->n[z][x][y]*dEc/dx;

			in->Jp_diffusion[z][x][y]= -Q*in->Dp[z][x][y]*dp/dx;
			in->Jp_drift[z][x][y]=Q*in->mup_y[z][x][y]*in->p[z][x][y]*dEv/dx;
		}
	}
}

}

/**
* @brief Jn diffusion
*/
gdouble get_Jn_diffusion(struct device *in)
{
long double J=0.0;

J=three_d_avg(in, in->Jn_diffusion);

return J;
}

/**
* @brief Jn drift
*/
gdouble get_Jn_drift(struct device *in)
{
long double J=0.0;


J=three_d_avg(in, in->Jn_drift);

return J;
}

/**
* @brief Jp diffusion
*/
gdouble get_Jp_diffusion(struct device *in)
{
long double J=0.0;

J=three_d_avg(in, in->Jp_diffusion);

return J;
}

/**
* @brief Jp drift
*/
gdouble get_Jp_drift(struct device *in)
{
long double J=0.0;

J=three_d_avg(in, in->Jp_drift);

return J;
}

/**
* @brief Get equivlent V
*/
gdouble get_equiv_V(struct simulation *sim,struct device *in)
{
gdouble J=0.0;
gdouble Vapplied=0.0;
Vapplied=contact_get_active_contact_voltage(sim,in);
//if (in->adv_sim==FALSE)
//{
//J=get_J_recom(in);
//}else
//{
J=get_equiv_J(sim,in);
//}

gdouble V=J*in->Rcontact*in->area+Vapplied;
return V;
}

/**
* @brief Get J
*/
gdouble get_J(struct device *in)
{
//int i;
gdouble ret=0.0;

ret=(get_J_left(in)+get_J_right(in))/2.0;

return ret;
}

/**
* @brief Get J left
*/
gdouble get_J_left(struct device *in)
{
long double ret=0.0;
ret = contacts_get_Jleft(in);
return ret;
}

/**
* @brief Get J right
*/
gdouble get_J_right(struct device *in)
{
long double ret=0.0;
ret = contacts_get_Jright(in);
return ret;
}

