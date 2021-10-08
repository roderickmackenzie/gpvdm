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
	struct world *w=&(dev->w);

		for (i=0;i<w->objects;i++)
		{
			s=w->obj[i].s;

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

					w->obj[i].n[l]=n;
					w->obj[i].alpha[l]=alpha;
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

