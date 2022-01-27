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
#include <enabled_libs.h>
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
#include <memory.h>
#include <device_fun.h>


/** @file build.c
	@brief Set up the simulation window for the ray tracer
*/

void ray_malloc(struct simulation *sim,struct device *dev,struct image *my_image)
{
	int d;
	int y=0;
	int i=0;
	int iw=0;
	struct detector *det;
	struct ray_worker *worker;
	struct world *w=&(dev->w);

	int o;
	for (o=0;o<w->objects;o++)
	{
		object_nalpha_malloc(&(w->obj[o]),my_image->ray_wavelength_points);
	}

	if (my_image->enabled==FALSE)
	{
		ewe(sim,"Ray not enabled");
	}

	my_image->worker=malloc(sizeof(struct ray_worker)*my_image->worker_max);

	for (iw=0;iw<my_image->worker_max;iw++)
	{
		worker=&(my_image->worker[iw]);
		worker->nray_max=1000;
		worker->top_of_done_rays=0;
		worker->nrays=0;
		//#ifndef windows
		worker->working=FALSE;
		worker->thread_han=THREAD_NULL;
		//#endif
		worker->rays=malloc(sizeof(struct ray)*worker->nray_max);
	}
	//my_image->tri=malloc(sizeof(struct triangle)*my_image->triangles_max);


	my_image->lam=malloc(sizeof(double)*my_image->ray_wavelength_points);


	my_image->ang_escape=(long double **)malloc(sizeof(long double*)*my_image->ray_wavelength_points);
	my_image->angle=(long double *)malloc(sizeof(long double)*my_image->escape_bins);

	long double da=180.0/(long double)my_image->escape_bins;
	long double apos=0.0;
	for (i=0;i<my_image->escape_bins;i++)
	{
		apos+=da;
		my_image->angle[i]=apos;
	}

	long double min=1e9;
	long double max=-1.0;
	int layer=0;

	for (layer=0;layer<dev->my_epitaxy.layers;layer++)
	{

		if (dev->my_epitaxy.layer[layer].pl_enabled==TRUE)
		{
			if (strcmp(dev->my_epitaxy.layer[layer].pl_spectrum_file,"none")!=0)
			{
				if (dev->my_epitaxy.layer[layer].pl_spectrum.x[0]<min)
				{
					min=dev->my_epitaxy.layer[layer].pl_spectrum.x[0];
				}

				if (dev->my_epitaxy.layer[layer].pl_spectrum.x[dev->my_epitaxy.layer[layer].pl_spectrum.len-1]>max)
				{
					max=dev->my_epitaxy.layer[layer].pl_spectrum.x[dev->my_epitaxy.layer[layer].pl_spectrum.len-1];
				}

			}

				dev->my_epitaxy.layer[layer].photon_extract_eff=malloc(sizeof(double)*my_image->ray_wavelength_points);
				dev->my_epitaxy.layer[layer].photon_extract_eff_count=malloc(sizeof(double)*my_image->ray_wavelength_points);

		}
	}

	//if (my_image->ray_auto_wavelength_range==FALSE)
	{
		min=my_image->ray_lambda_start;
		max=my_image->ray_lambda_stop;
	}

	double lam=min;
	double dl=(max-min)/((long double)my_image->ray_wavelength_points);

	for (i=0;i<my_image->ray_wavelength_points;i++)
	{
		my_image->lam[i]=lam;
		my_image->ang_escape[i]=(long double*)malloc(sizeof(long double)*my_image->escape_bins);
		//printf("%le\n",lam);
		lam+=dl;
	}

	//getchar();

	for (d=0;d<w->detectors;d++)
	{
		det=&(w->det[d]);
		if (det->viewpoint_enabled==TRUE)
		{
			dim_malloc_xyz(&(my_image->viewpoint_dim),'y');

			for (y=0;y<my_image->ray_wavelength_points;y++)
			{
				my_image->viewpoint_dim.y[y]=my_image->lam[y];
			}

			malloc_zxy_long_double(&(my_image->viewpoint_dim),&(my_image->viewpoint_image));
			break;
		}
	}

}

void ray_free(struct simulation *sim,struct device *dev,struct image *my_image)
{
	int i=0;
	int layer;
	int iw=0;
	struct ray_worker *worker;
	struct world *w=&(dev->w);

	if (my_image->worker==NULL)
	{
		return;
	}

	for (iw=0;iw<my_image->worker_max;iw++)
	{
		worker=&(my_image->worker[iw]);
		worker->nray_max=-1;
		worker->top_of_done_rays=-1;
		worker->nrays=-1;
		free(worker->rays);
		worker->rays=NULL;
		//#ifndef windows
		worker->thread_han=THREAD_NULL;
		//#endif
	}

	free(my_image->worker);

	my_image->worker=NULL;

	//free(my_image->tri);

	free(my_image->lam);

	for (layer=0;layer<dev->my_epitaxy.layers;layer++)
	{

		if (dev->my_epitaxy.layer[layer].pl_enabled==TRUE)
		{
			free(dev->my_epitaxy.layer[layer].photon_extract_eff);
			free(dev->my_epitaxy.layer[layer].photon_extract_eff_count);

		}
	}

	for (i=0;i<my_image->ray_wavelength_points;i++)
	{
		free(my_image->ang_escape[i]);
	}


	free(my_image->ang_escape);

	free(my_image->angle);

	free_zxy_long_double(&(my_image->viewpoint_dim),&(my_image->viewpoint_image));
	dim_free(&(my_image->viewpoint_dim));


	for (i=0;i<w->objects;i++)
	{
		object_nalpha_free(&(w->obj[i]));
	}

}

