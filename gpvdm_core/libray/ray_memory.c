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
	int y=0;
	int i=0;
	int w=0;
	struct ray_worker *worker;

	int o;
	for (o=0;o<dev->objects;o++)
	{
		object_nalpha_malloc(&(dev->obj[o]),my_image->ray_wavelength_points);
	}

	if (dev->emission_enabled==FALSE)
	{
		ewe(sim,"don't run the ray tracer with no emission layers");
	}

	my_image->worker=malloc(sizeof(struct ray_worker)*my_image->worker_max);

	for (w=0;w<my_image->worker_max;w++)
	{
		worker=&(my_image->worker[w]);
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

	if (my_image->ray_auto_wavelength_range==FALSE)
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


	if (my_image->viewpoint_enabled==TRUE)
	{
		dim_alloc_xyz(&(my_image->viewpoint_dim),'x');
		dim_alloc_xyz(&(my_image->viewpoint_dim),'z');
		dim_alloc_xyz(&(my_image->viewpoint_dim),'y');

		//dim_alloc_gen_untiy_mesh_x(&(my_image->viewpoint_dim));	//This is now done when we add the object to the scene
		//dim_alloc_gen_untiy_mesh_z(&(my_image->viewpoint_dim));

		for (y=0;y<my_image->ray_wavelength_points;y++)
		{
			my_image->viewpoint_dim.ymesh[y]=my_image->lam[y];
		}

		malloc_zxy_gdouble(&(my_image->viewpoint_dim),&(my_image->viewpoint_image));
		//printf("one\n");
		//getchar();
	}

}

void ray_free(struct simulation *sim,struct device *dev,struct image *my_image)
{
	int i=0;
	int layer;
	int w=0;
	struct ray_worker *worker;

	if (my_image->worker==NULL)
	{
		return;
	}

	for (w=0;w<my_image->worker_max;w++)
	{
		worker=&(my_image->worker[w]);
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

	free_zxy_gdouble(&(my_image->viewpoint_dim),&(my_image->viewpoint_image));
	dim_free(&(my_image->viewpoint_dim));


	for (i=0;i<dev->objects;i++)
	{
		object_nalpha_free(&(dev->obj[i]));
	}

}

