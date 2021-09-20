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

/** @file heat_stats.c
	@brief Gets stats about the termal model
*/

#include "util.h"
#include "gpvdm_const.h"
#include "heat.h"
#include "device.h"
#include "gpvdm_const.h"
#include "dump.h"
#include "config.h"
#include "inp.h"
#include "util.h"
#include "hard_limit.h"
#include "lang.h"
#include "log.h"
#include "memory.h"
#include <heat_fun.h>
#include <contacts.h>
#include <exp.h>

static int unused __attribute__((unused));


void heat_cal_get_heating_sources_percent(struct simulation *sim,struct device *dev,struct heat *thermal,long double *H_joule,long double *H_recombination,long double *H_parasitic)
{
	int z=0;
	int x=0;
	int y=0;

	struct dim_heat *dim_t=&(thermal->dim);

	//long double H_tot=0.0;

	long double dy=0.0;
	long double dx=0.0;
	long double dz=0.0;

	long double Vol=0.0;
	long double dV=0.0;
	for (z=0;z<dim_t->zlen;z++)
	{
		for (x=0;x<dim_t->xlen;x++)
		{
			for (y=0;y<dim_t->ylen;y++)
			{
				dx=dim_t->dx[x];
				dy=dim_t->dy[y];
				dz=dim_t->dz[z];
				dV=dx*dy*dz;
				//if (thermal->H_joule[z][x][y]>0.0)
				//{
					*H_joule+=thermal->H_joule[z][x][y]*dV;
				//}
				*H_recombination+=thermal->H_recombination[z][x][y]*dV;
				*H_parasitic+=thermal->H_parasitic[z][x][y]*dV;
				Vol+=dV;
			}
		}
	}

//printf("sum: %Le %Le %Le\n",H_joule,H_recombination,H_parasitic);
//H_tot=*H_joule+*H_recombination+*H_parasitic;
*H_joule/=Vol;
*H_recombination/=Vol;
*H_parasitic/=Vol;

//printf("p: %Le %Le %Le\n",H_joule,H_recombination,H_parasitic);

}


