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
* @brief Calculate the average mobility
*/
void get_avg_mu(struct device *in,long double *ret_mue,long double *ret_muh)
{
int x=0;
int y=0;
int z=0;
long double sum_mue=0.0;
long double sum_muh=0.0;

long double ret=0.0;
struct dimensions *dim=&in->ns.dim;

if (in->mun_y==NULL)
{
	return;
}

if (in->mup_y==NULL)
{
	return;
}

	for (z = 0; z < dim->zlen; z++)
	{
		for (x = 0; x < dim->xlen; x++)
		{
			for (y = 0; y < dim->ylen; y++)
			{
				sum_mue+=in->mun_y[z][x][y]*(in->n[z][x][y]*dim->dX[x]*dim->dY[y]*dim->dZ[z])/(in->nt_all[z][x][y]+in->n[z][x][y]);
				sum_muh+=in->mup_y[z][x][y]*(in->p[z][x][y]*dim->dX[x]*dim->dY[y]*dim->dZ[z])/(in->pt_all[z][x][y]+in->p[z][x][y]);
			}

		}
	}

*ret_mue=sum_mue/(in->zlen*in->xlen*in->ylen);
*ret_muh=sum_muh/(in->zlen*in->xlen*in->ylen);

}

void get_avg_geom_micro_mu(struct device *in,long double *ret_mu)
{
int x=0;
int y=0;
int z=0;
long double mue=0.0;
long double muh=0.0;

long double mu_tot=0.0;
struct dimensions *dim=&in->ns.dim;

if (in->mun_y==NULL)
{
	return;
}

if (in->mup_y==NULL)
{
	return;
}

	for (z = 0; z < dim->zlen; z++)
	{
		for (x = 0; x < dim->xlen; x++)
		{
			for (y = 0; y < dim->ylen; y++)
			{
				mue=in->mun_y[z][x][y]*in->n[z][x][y]/(in->nt_all[z][x][y]+in->n[z][x][y]);
				muh=in->mup_y[z][x][y]*in->p[z][x][y]/(in->pt_all[z][x][y]+in->p[z][x][y]);
				mu_tot+=sqrt(mue*muh)*dim->dX[x]*dim->dY[y]*dim->dZ[z];
			}

		}
	}

*ret_mu=mu_tot/(in->zlen*in->xlen*in->ylen);

}

