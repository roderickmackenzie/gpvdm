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

/** @file utils.c
	@brief Miscellaneous functions for the heat model.
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



void heat_transfer_temperatures_to_device(struct device *dev,struct heat *thermal)
{
	int z=0;
	int x=0;
	int y=0;

	long double Tl=0.0;
	long double Te=0.0;
	long double Th=0.0;
	long double pos=0;

	struct dimensions *dim=&(dev->ns.dim);
	struct dimensions *dim_t=&(thermal->dim);
	struct epitaxy *epi=&(dev->my_epitaxy);

	//struct newton_state *ns=&cell->ns;


	for (z=0;z<dim->zlen;z++)
	{
		for (x=0;x<dim->xlen;x++)
		{
			for (y=0;y<dim->ylen;y++)
			{
				pos=epi->device_start+dim->y[y];

				Tl=interpolate_heat_zxy_long_double(dim_t, thermal->Tl, z, x, pos);
				Te=interpolate_heat_zxy_long_double(dim_t, thermal->Te, z, x, pos);
				Th=interpolate_heat_zxy_long_double(dim_t, thermal->Th, z, x, pos);

				dev->Tl[z][x][y]=Tl;
				dev->Te[z][x][y]=Te;
				dev->Te[z][x][y]=Th;

			}
		}
	}


}


void heat_transfer_device_heat_to_heat_mesh(struct simulation *sim,struct heat *thermal, struct device *dev)
{
	int z=0;
	int x=0;
	int y=0;

	long double H_joule=0.0;
	long double H_recombination=0.0;

	long double He=0.0;
	long double Hh=0.0;
	long double pos=0;

	long double Vapplied=0.0;
	long double J=0.0;
	long double P=0.0;
	long double H_parasitic=0.0;
	struct dimensions *dim=&(dev->ns.dim);
	struct dimensions *dim_t=&(thermal->dim);
	struct epitaxy *epi=&(dev->my_epitaxy);

	Vapplied=contact_get_active_contact_voltage(sim,dev);


	if (thermal->parasitic_heating==TRUE)
	{
		J=fabs(Vapplied)/dev->Rshunt/dev->area;
		P=J*fabs(Vapplied);
		H_parasitic=P/dev->ylen;
	}

	for (z=0;z<dim_t->zlen;z++)
	{
		for (x=0;x<dim_t->xlen;x++)
		{
			for (y=0;y<dim_t->ylen;y++)
			{
				if ((dim_t->y[y]>=epi->device_start)&&(dim_t->y[y]<epi->device_stop))
				{
					pos=dim_t->y[y]-epi->device_start;
					He=interpolate_zxy_long_double(dim, dev->He, z, x, pos);
					Hh=interpolate_zxy_long_double(dim, dev->Hh, z, x, pos);

					thermal->He[z][x][y]=He;
					thermal->Hh[z][x][y]=Hh;

					H_joule=interpolate_zxy_long_double(dim, dev->H_joule, z, x, pos);
					thermal->H_joule[z][x][y]=H_joule;
					//printf("%Le %Le\n",dim_t->y[y],H_joule);

					H_recombination=interpolate_zxy_long_double(dim, dev->H_recombination, z, x, pos);
					thermal->H_recombination[z][x][y]=H_recombination;
					//printf("%Le %Le\n",dim_t->y[y],H_recombination);

					thermal->H_parasitic[z][x][y]=H_parasitic;
				}
			}
		}
	}

//getchar();

}


void heat_transfer_optical_heat_to_heat_mesh(struct heat *thermal, struct light *li)
{
	int z=0;
	int x=0;
	int y=0;

	long double H=0.0;
	long double ypos=0.0;
	long double y_start=0.0;
	long double y_stop=0.0;

	struct dimensions *dim_li=&(li->dim);
	struct dimensions *dim_t=&(thermal->dim);

	for (z=0;z<dim_t->zlen;z++)
	{
		for (x=0;x<dim_t->xlen;x++)
		{
			ypos=0.0;
			for (y=0;y<dim_t->ylen;y++)
			{
				y_start=ypos;
				ypos+=dim_t->dY[y];
				y_stop=ypos;

				//H=interpolate_light_zxy_long_double(dim_li, li->Htot, z, x, dim_t->y[y]);
				H=interpolate_light_zxy_long_double_intergral(dim_li, li->Htot, z, x, y_start,y_stop);
				//printf("H=%Le\n",H);
				thermal->H_optical[z][x][y]=H;

			}
		}
	}

}
