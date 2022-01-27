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
#include <gpvdm_const.h>
#include <math.h>
#include <stdlib.h>
#include <cal_path.h>
#include <log.h>
#include <ray_fun.h>

/** @file ray_stats.c
	@brief Perfrom stats on the ray tracing image
*/

void photon_extract_eff_reset(struct simulation *sim,struct epitaxy *epi,struct image *in)
{
	//int i;
	int l;
	int layer=0;
	for (layer=0;layer<epi->layers;layer++)
	{
		if (epi->layer[layer].pl_enabled==TRUE)
		{
			for (l=0;l<in->ray_wavelength_points;l++)
			{
					epi->layer[layer].photon_extract_eff[l]=0.0;
					epi->layer[layer].photon_extract_eff_count[l]=0.0;
			}
		}
	}

}

void photon_extract_eff_norm(struct simulation *sim,struct epitaxy *epi,struct image *in)
{
	//int i;
	int l;
	int layer=0;
	for (layer=0;layer<epi->layers;layer++)
	{
		if (epi->layer[layer].pl_enabled==TRUE)
		{
			for (l=0;l<in->ray_wavelength_points;l++)
			{
					if (epi->layer[layer].photon_extract_eff_count[l]>0)
					{
						epi->layer[layer].photon_extract_eff[l]/=epi->layer[layer].photon_extract_eff_count[l];
					}else
					{
						epi->layer[layer].photon_extract_eff[l]=0.0;
					}
			}
		}
	}

}



void ray_escape_angle_reset(struct image *in,int l)
{
	int i;

	for (i=0;i<in->escape_bins;i++)
	{
		in->ang_escape[l][i]=0.0;
	}
}

void ray_escape_angle_norm(struct image *in)
{
	int i;
	int l;
	double max=0.0;
	for (l=0;l<in->ray_wavelength_points;l++)
	{
		for (i=0;i<in->escape_bins;i++)
		{
			if (in->ang_escape[l][i]>max)
			{
				max=in->ang_escape[l][i];
			}
		}
	}

	for (l=0;l<in->ray_wavelength_points;l++)
	{
		for (i=0;i<in->escape_bins;i++)
		{
			in->ang_escape[l][i]/=max;
		}
	}

}

void ray_calculate_avg_extract_eff(struct simulation *sim,struct epitaxy *epi,struct image *in)
{
	int i;
	int layer=0;
	double tot=0.0;
	double count=0.0;
	for (layer=0;layer<epi->layers;layer++)
	{

		if (epi->layer[layer].pl_enabled==TRUE)
		{
			tot=0.0;
			count=0.0;

			for (i=0;i<in->ray_wavelength_points;i++)
			{
				tot+=epi->layer[layer].photon_extract_eff[i];
				count+=epi->layer[layer].photon_extract_eff_count[i];
			}

			epi->layer[layer].avg_photon_extract_eff=tot/count;
			//printf("%d %Le\n",layer,epi->layer[layer].avg_photon_extract_eff);
		}else
		{
			epi->layer[layer].avg_photon_extract_eff=0.0;
		}
	}


}

double ray_cal_escape_angle(struct image *in, struct ray_worker *worker)
{
	int i;
	//double mag=0.0;
	double tot=0.0;
	int l=worker->l;
	for (i=0;i<worker->nrays;i++)
	{
		if (worker->rays[i].state==DONE)
		{
			if (worker->rays[i].xy_end.y<in->y_escape_level)
			{
				//mag=in->rays[i].mag;
				double raw_ang=(360.0/(2*3.14159))*atan(fabs(worker->rays[i].dir.y)/worker->rays[i].dir.x);
				if (raw_ang<0.0)
				{
					raw_ang=raw_ang+180.0;
				}
				double ang=raw_ang;
				int bin=(int)((ang/180.0)*(double)in->escape_bins);
				in->ang_escape[l][bin]+=worker->rays[i].mag;
				tot=tot+worker->rays[i].mag;
				//printf("%lf\n",worker->rays[i].mag);
				//getchar();
			}
		}

	}

return tot;
}

double get_eff(struct image *in)
{
	int i;
	int w;

	double tot=0.0;
	struct ray_worker *worker;

	for (w=0;w<in->worker_max;w++)
	{
		worker=&(in->worker[w]);

		for (i=0;i<worker->nrays;i++)
		{
			if (worker->rays[i].state==DONE)
			{
				if (worker->rays[i].xy_end.y<in->y_escape_level)
				{
					tot+=worker->rays[i].mag;
				}
			}

		}
	}

return tot;
}
