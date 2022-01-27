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

	struct dimensions *dim_t=&(thermal->dim);

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
				dx=dim_t->dX[x];
				dy=dim_t->dY[y];
				dz=dim_t->dZ[z];
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


