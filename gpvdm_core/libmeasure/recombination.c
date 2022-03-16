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

/** @file recombination.c
	@brief Extracts recombination parameters
*/

#include <exp.h>
#include "sim.h"
#include "i.h"
#include "dat_file.h"
#include "contacts.h"
#include <memory.h>

/**
* @brief Free electron recombination rate
* Note this calculates the real free electron recombination rate
* not the removal of electrons from the band
*/
long double get_avg_recom_n(struct device *in)
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
long double get_avg_recom_p(struct device *in)
{
	long double ret=0.0;

	ret=three_d_avg(in, in->ntrap_to_p);

	return ret;
}

/**
* @brief Removal of electrons from the band
*/
long double get_avg_Rn(struct device *in)
{
	long double ret=0.0;

	ret=three_d_avg(in, in->Rn);

	return ret;
}

/**
* @brief Removal of holes from the band
*/
long double get_avg_Rp(struct device *in)
{
	long double ret=0.0;

	ret=three_d_avg(in, in->Rp);

	return ret;
}

/**
* @brief Calculation of k the recombination prefactor
*/
long double get_avg_k(struct device *in)
{
	long double n=(get_extracted_n(in)*get_extracted_p(in));
	long double R=(get_avg_recom_n(in)+get_avg_recom_p(in))/2.0;
	long double k=R/n;
	return k;
}

/**
* @brief Get the average recombination density
*/
long double get_avg_recom(struct device *in)
{
	long double ret=0.0;

	ret=three_d_avg(in, in->ptrap_to_n);
	ret+=three_d_avg(in, in->ntrap_to_p);
	ret+=three_d_avg(in, in->Rfree);

	return ret;
}

void get_tau(struct device *dev, long double *ret_tau, long double *ret_tau_all)
{
	int x=0;
	int y=0;
	int z=0;

	long double R=0.0;
	long double tau=0.0;
	long double tau_all=0.0;
	long double n0=0.0;
	long double n_all=0.0;
	long double dy=0.0;
	long double y_tot=0.0;

	struct newton_state *ns=&dev->ns;
	struct dimensions *dim=&dev->ns.dim;

	if (dev->n==NULL)
	{
		return;
	}

	dy=dim->y[1]-dim->y[0];

	for (z=0;z<dim->zlen;z++)
	{
		for (x=0;x<dim->xlen;x++)
		{
			for (y=0;y<dim->ylen;y++)
			{

				R=dev->ptrap_to_n[z][x][y]+dev->ntrap_to_p[z][x][y]+dev->Rfree[z][x][y];

				n0=dev->nfequlib[z][x][y]+dev->ntequlib[z][x][y];		//n
				n0+=dev->pfequlib[z][x][y]+dev->ptequlib[z][x][y];		//p
				n0/=2.0;

				n_all=dev->n[z][x][y]+dev->nt_all[z][x][y];				//n
				n_all+=dev->p[z][x][y]+dev->pt_all[z][x][y];			//p
				n_all/=2.0;

				tau+=((n_all-n0)/R)*dy;
				tau_all+=(n_all/R)*dy;

				y_tot+=dy;
			}
		}
	}

	tau/=y_tot;
	tau_all/=y_tot;


	if (ret_tau!=NULL)
	{
		*ret_tau=tau;
	}

	if (ret_tau_all!=NULL)
	{ 
		*ret_tau_all=tau_all;
	}
}
