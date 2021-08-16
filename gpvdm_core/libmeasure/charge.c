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
* @brief Calculate extracted np
*/
gdouble get_extracted_np(struct device *in)
{

	if (in->n==NULL)
	{
		return 0.0;
	}

	return (get_extracted_n(in)+get_extracted_p(in))/2.0;
}

/**
* @brief Calculate extracted n from a save point
*/
gdouble get_extracted_n(struct device *in)
{
	long double ret=0.0;

	ret=three_d_avg(in, in->n);
	ret-=three_d_avg(in, in->nf_save);

	ret+=three_d_avg(in, in->nt_all);
	ret-=three_d_avg(in, in->nt_save);

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
* @brief This implements equation 12 from
* J. Phys. Chem. C 2019, 123, 14261-14271, 10.1021/acs.jpcc.9b03133
*/
long double cal_contact_charge(struct device *in)
{
	struct dimensions *dim=&in->ns.dim;
	int z=0;
	int x=0;
	int y=0;
	long double charge=0.0;
	long double sum=0.0;

	if (in->drift_diffision_simulations_enabled==TRUE)
	{
		for (y=0;y<dim->ylen;y++)
		{
			charge=in->p[z][x][y]+in->pt_all[z][x][y];
			charge-=in->n[z][x][y]+in->nt_all[z][x][y];
			sum+=(dim->ymesh[y]-(in->ylen/2.0))*charge*dim->dy[y];
		}
	}
	return sum/in->ylen/in->ylen;
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
