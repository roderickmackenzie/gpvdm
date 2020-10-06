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

/** @file hydrodynamic.c
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

long double hydrodynamic_get_error(struct heat *thermal)
{
	int y=0;
	long double tot=0.0;
	struct dim_heat *dim=&(thermal->dim);
	long double *b=thermal->mx.b;

	for (y=0;y<dim->ylen;y++)
	{
		if (thermal->thermal_l==TRUE)
		{
			tot+=gfabs(b[y]);
		}

		if (thermal->thermal_e==TRUE)
		{
			tot+=gfabs(b[dim->ylen+y]);
		}

		if (thermal->thermal_h==TRUE)
		{
			tot+=gfabs(b[dim->ylen+dim->ylen+y]);
		}
	}

return tot;
}


int hydrodynamic_solve(struct simulation *sim, struct heat *thermal,struct device *dev, int z, int x)
{
return 0;
}

