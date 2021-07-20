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

long double get_tau(struct device *dev)
{

	if (dev->n==NULL)
	{
		return 0.0;
	}

	int x=0;
	int y=0;
	int z=0;

	long double R=0.0;
	long double Rtot=0.0;
	long double n0=0.0;
	long double n_all=0.0;
	long double dy=0.0;
	long double y_tot=0.0;
	struct newton_state *ns=&dev->ns;
	struct dimensions *dim=&dev->ns.dim;

	dy=dim->ymesh[1]-dim->ymesh[0];

	for (z=0;z<dim->zlen;z++)
	{
		for (x=0;x<dim->xlen;x++)
		{
			for (y=0;y<dim->ylen;y++)
			{

				R=dev->ptrap_to_n[z][x][y]+dev->ntrap_to_p[z][x][y]+dev->Rfree[z][x][y];
				n0=dev->nfequlib[z][x][y]+dev->ntequlib[z][x][y]+dev->pfequlib[z][x][y]+dev->ptequlib[z][x][y];
				n_all=dev->n[z][x][y]+dev->p[z][x][y]+dev->pt_all[z][x][y]+dev->nt_all[z][x][y];
				//printf("%Le %Le %Le\n",n_all,n0,R);
				Rtot+=((n_all-n0)/R)*dy;
				y_tot+=dy;
			}
		}
	}

	Rtot/=y_tot;

	return Rtot;
}
