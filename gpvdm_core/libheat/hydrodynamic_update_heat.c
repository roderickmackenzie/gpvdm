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

/** @file hydrodynamic_update_heat.c
	@brief A hydrodynamic hermal model.
*/


#include <stdio.h>
#include <stdlib.h>
#include "sim.h"
#include "solver_interface.h"
#include <cal_path.h>
#include <log.h>
#include <memory.h>
#include <heat.h>
#include <heat_fun.h>

void hydrodynamic_update_heat(struct simulation *sim, struct heat *thermal,struct device *dev,int z, int x)
{
int y;
struct dimensions *dim=&(dev->ns.dim);

hydrodynamic_mesh_check(sim, thermal,dev);

	for (y=0;y<dim->ylen;y++)
	{
		thermal->ke[z][x][y]=(5/2+1.5)*kb*(kb/Qe)*thermal->Te[z][x][y]*dev->mun_y[z][x][y]*dev->n[z][x][y];
		thermal->kh[z][x][y]=(5/2+1.5)*kb*(kb/Qe)*thermal->Th[z][x][y]*dev->mup_y[z][x][y]*dev->p[z][x][y];
	}

}

void hydrodynamic_transfer_temperatures_to_device(struct simulation *sim, struct device *dev,struct heat *thermal,int z, int x)
{
	int y;
	struct dimensions *dim=&(dev->ns.dim);

	hydrodynamic_mesh_check(sim, thermal,dev);

	for (y=0;y<dim->ylen;y++)
	{
		dev->Tl[z][x][y]=thermal->Tl[z][x][y];
		dev->Te[z][x][y]=thermal->Te[z][x][y];
		dev->Th[z][x][y]=thermal->Th[z][x][y];
	}

}
