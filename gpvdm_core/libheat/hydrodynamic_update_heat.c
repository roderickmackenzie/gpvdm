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
