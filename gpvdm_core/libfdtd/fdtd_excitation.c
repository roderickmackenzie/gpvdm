//
// General-purpose Photovoltaic Device Model gpvdm.com - a drift diffusion
// base/Shockley-Read-Hall model for 1st, 2nd and 3rd generation solardevs.
// The model can simulate OLEDs, Perovskite devs, and OFETs.
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

/** @file fdtd_excitation.c
	@brief Setup the FDTD mesh.
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
#include <epitaxy_struct.h>
#include <epitaxy.h>
#include <world.h>
#include "vec.h"
#include <ray_fun.h>

void fdtd_excitation(struct simulation *sim,struct fdtd_data *data,struct device *dev)
{
	int i;
	//struct dimensions *dim=&(data->dim);
	//float start=dim->y[1];
	float x_mag=0.0;
	float y_mag=0.0;
	float z_mag=0.0;
	int x;
	int y;
	int z;
	//float phi=0.0;
	//float theta=0.0;
	//float dot=0.0;
	float mod=1.0;
	//float shift=-2.0;

	struct light_src *lightsrc;

	for (i=0;i<dev->lights.nlight_sources;i++)
	{
		lightsrc=&(dev->lights.light_sources[i]);
		if (strcmp(lightsrc->illuminate_from,"xyz")==0)
		{

			//phi=0.0;
			//theta=90.0;
			x=fdtd_world_x_to_mesh(data,lightsrc->x0);
			y=fdtd_world_y_to_mesh(data,lightsrc->y0);
			z=fdtd_world_z_to_mesh(data,lightsrc->z0);
			//printf("%Le %Le %Le\n",lightsrc->x0,lightsrc->y0,lightsrc->z0);

			//printf("%d %d %d\n",x,y,z);
			//a=sin(theta*(2.0*M_PI/360.0))*cos(phi*(2.0*M_PI/360.0));
			//b=sin(theta*(2.0*M_PI/360.0))*sin(phi*(2.0*M_PI/360.0));
			//c=cos(theta*(2.0*M_PI/360.0));

			//dot=tan(2.0*M_PI*(shift)/360.0)*(dim->y[y]-start)*2.0*M_PI/data->lambda;

			//dot=0.0;
			x_mag=0.0;
			y_mag=0.0;
			z_mag=0.0;

			if (data->excite_Ex==TRUE)
			{
				x_mag=1.0;
			}

			if (data->excite_Ey==TRUE)
			{
				y_mag=1.0;
			}

			if (data->excite_Ez==TRUE)
			{
				z_mag=1.0;
			}

			if (data->excitation_type==FDTD_SIN)
			{
				data->Ex[z][x][y]+=mod*x_mag*sin(data->time*data->omega);
				data->Ey[z][x][y]+=mod*y_mag*sin(data->time*data->omega);
				data->Ez[z][x][y]+=mod*z_mag*sin(data->time*data->omega);
			}else
			{
				if (data->step<data->pulse_length)
				{
				data->Ex[z][x][y]+=mod*x_mag;
				data->Ey[z][x][y]+=mod*y_mag;
				data->Ez[z][x][y]+=mod*z_mag;
				}
			}
			/*	raysrc->theta_steps=lightsrc->theta_steps;
				raysrc->theta_start=lightsrc->theta_start;
				raysrc->theta_stop=lightsrc->theta_stop;

				raysrc->phi_steps=lightsrc->phi_steps;
				raysrc->phi_start=lightsrc->phi_start;
				raysrc->phi_stop=lightsrc->phi_stop;
				raysrc->epi_layer=-1;
				raysrc->emission_source=-1;
				raysrc->light=i;
			}
			emitters++;*/
		}
	}

}
