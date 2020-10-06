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

/** @file fdtd_power.c
	@brief Calculate fdtd power.
*/

#include <math.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <inp.h>
#include <sim.h>
#include <log.h>
#include <fdtd.h>

#include "vec.h"


float fdtd_power_zxy(struct simulation *sim,struct fdtd_data *data,int z, int x, int y)
{
	float e_power=0.0;
	float h_power=0.0;

	e_power=0.5*epsilon0f*(data->Ex[z][x][y]*data->Ex[z][x][y]+data->Ey[z][x][y]*data->Ey[z][x][y]+data->Ez[z][x][y]*data->Ez[z][x][y]);
	h_power=0.5*mu0f*(data->Hx[z][x][y]*data->Hx[z][x][y]+data->Hy[z][x][y]*data->Hy[z][x][y]+data->Hz[z][x][y]*data->Hz[z][x][y]);

	return e_power+h_power;
}

float fdtd_power_y(struct simulation *sim,struct fdtd_data *data, int y)
{
	int z;
	int x;
	float tot=0.0;
	for (z=0;z<data->zlen;z++)
	{
		for (x=0;x<data->xlen;x++)
		{
			tot=tot+fdtd_power_zxy(sim,data,z, x,y);
		}
	}

	return tot;
}

float fdtd_test_conv(struct simulation *sim,struct fdtd_data *data)
{
float ret=0.0;
float src=fdtd_power_zxy(sim,data,0, data->excitation_mesh_point_x, data->excitation_mesh_point_y);
float almost_top=fdtd_power_y(sim,data,  data->ylen-data->ylen/4);
float top=fdtd_power_y(sim,data,  data->ylen-2);

	if (top!=0.0)
	{
		ret=almost_top/src;
		data->escape=ret;
	}

return ret;
}
