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

#include <stdio.h>
#include <ray.h>
#include <ray_fun.h>
#include <gpvdm_const.h>
#include <math.h>
#include <stdlib.h>
#include <cal_path.h>
#include <log.h>
#include <device.h>
#include <inp.h>
#include <util.h>
#include <triangles.h>

/** @file build.c
	@brief Set up the simulation window for the ray tracer
*/

void light_update_ray_mat(struct simulation *sim,struct device *dev,struct image *my_image)
{
	int i;
	//struct epitaxy *my_epitaxy=&(dev->my_epitaxy);
	double n=1.0;
	double alpha=0.0;
	struct shape *s;
	//int epi_layer=-1;
	double lambda=0.0;
	int l;

		for (i=0;i<dev->objects;i++)
		{
			s=dev->obj[i].s;

			if (s!=NULL)		//It's a shape
			{
				//printf("%s %p\n",dev->obj[i].name,my_image->lam);
				for (l=0;l<my_image->ray_wavelength_points;l++)
				{
					lambda=my_image->lam[l];
					if (strcmp(s->optical_material,"none")!=0)
					{
						n=inter_get_noend(&(s->n),lambda);
						alpha=inter_get_noend(&(s->alpha),lambda);
					}else
					{
						n=1.0;
						alpha=1e-3;
					}

					dev->obj[i].n[l]=n;
					dev->obj[i].alpha[l]=alpha;
				}
				//printf("object %s\n",my_image->obj[i].name);

			}/*else
			//if (dev->obj[i].epi_layer!=-1)		//it's part of the epitaxy
			{
				epi_layer=dev->obj[i].epi_layer;

				for (l=0;l<my_image->ray_wavelength_points;l++)
				{
					lambda=my_image->lam[l];
					n=inter_get_noend(&(my_epitaxy->layer[epi_layer].s.n),lambda);
					alpha=inter_get_noend(&(my_epitaxy->layer[epi_layer].s.alpha),lambda);

					dev->obj[i].n[l]=n;
					dev->obj[i].alpha[l]=alpha;
					//printf("Epi> %d %s %lf %le\n",l,dev->obj[i].name,n,alpha);
				}
				//
				//getchar();


			}*///else
			//{
			//	printf("I'm not changing it %s\n",dev->obj[i].name);
			//}
		}

	//fgetchar();


}

