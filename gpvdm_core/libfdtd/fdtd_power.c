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
	struct dimensions *dim=&(data->dim);
	for (z=0;z<dim->zlen;z++)
	{
		for (x=0;x<dim->xlen;x++)
		{
			tot=tot+fdtd_power_zxy(sim,data,z, x,y);
		}
	}

	return tot;
}

float fdtd_test_conv(struct simulation *sim,struct fdtd_data *data)
{
struct dimensions *dim=&(data->dim);
float ret=0.0;
float src=fdtd_power_zxy(sim,data,0, data->excitation_mesh_point_x, data->excitation_mesh_point_y);
float almost_top=fdtd_power_y(sim,data,  dim->ylen-dim->ylen/4);
float top=fdtd_power_y(sim,data,  dim->ylen-2);

	if (top!=0.0)
	{
		ret=almost_top/src;
		data->escape=ret;
	}

return ret;
}
