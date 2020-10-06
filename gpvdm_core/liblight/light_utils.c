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

/** @file light_utils.c
	@brief Miscellaneous functions for the light model.
*/

#include "util.h"
#include "gpvdm_const.h"
#include "light.h"
#include "device.h"
#include "gpvdm_const.h"
#include "dump.h"
#include "config.h"
#include "inp.h"
#include "util.h"
#include "hard_limit.h"
#include "lang.h"
#include "log.h"
#include "memory.h"
#include <light_fun.h>

static int unused __attribute__((unused));

long double light_get_sun(struct light *li)
{
return li->Psun;
}

void light_set_sun(struct light *li,long double Psun)
{
	li->Psun=Psun;
}

void light_set_model(struct light *li,char *model)
{
	strcpy(li->mode,model);
}


long double light_cal_photon_density(struct simulation *sim,struct light *li,struct device *dev)
{
struct dim_light *dim=&li->dim;
struct epitaxy *epi=&dev->my_epitaxy;

struct shape* s;
struct object* obj;
long double Eg=0.0;
int x;
int y;
int z;
int l;

long double tot=0.0;
long double H_tot=0.0;
long double photons_tot=0.0;
long double E=0.0;

if (li->disable_cal_photon_density==FALSE)
{
	if ((li->laser_eff!=0)||(li->Psun!=0))
	{


		for (z=0;z<dim->zlen;z++)
		{
			for (x=0;x<dim->xlen;x++)
			{
				for (y=0;y<dim->ylen;y++)
				{
					tot=0.0;
					H_tot=0.0;
					photons_tot=0.0;
					obj=li->obj[z][x][y];

					for (l=0;l<dim->llen;l++)
					{
						li->E_tot_r[z][x][y][l]=li->Ep[z][x][y][l]+li->En[z][x][y][l];
						li->E_tot_i[z][x][y][l]=li->Enz[z][x][y][l]+li->Epz[z][x][y][l];
						li->pointing_vector[z][x][y][l]=0.5*epsilon0*cl*li->n[z][x][y][l]*(gpow(li->E_tot_r[z][x][y][l],2.0)+gpow(li->E_tot_i[z][x][y][l],2.0));

						li->photons[z][x][y][l]=li->pointing_vector[z][x][y][l]*(dim->l[l]/(hp*cl));
						li->photons_asb[z][x][y][l]=li->photons[z][x][y][l]*li->alpha[z][x][y][l];

						Eg=0.0;
						if (obj->epi_layer!=-1)
						{
							Eg=epi->dosn[obj->epi_layer].config.Eg;
						}else
						{
							s=obj->s;
							if (s->dos_index!=-1)
							{
								Eg=epi->dosn[s->dos_index].config.Eg;
							}

						}

						E=((hp*cl)/dim->l[l])/Q-Eg;

						if (E>0.0)
						{
							li->H[z][x][y][l]=E*Q*li->photons_asb[z][x][y][l];
						}else
						{
							li->H[z][x][y][l]=0.0;
						}

						photons_tot+=li->photons[z][x][y][l]*dim->dl;
						tot+=li->photons_asb[z][x][y][l]*dim->dl;

						H_tot+=li->H[z][x][y][l]*dim->dl;

					}

					li->Gn[z][x][y]=tot;
					li->Gp[z][x][y]=tot;

					li->Htot[z][x][y]=H_tot;
					//printf("Htot=%Le\n",H_tot);
					li->photons_tot[z][x][y]=photons_tot;

					for (l=0;l<dim->llen;l++)
					{
						li->reflect[l]=(gpow(li->En[0][0][0][l],2.0)+gpow(li->Enz[0][0][0][l],2.0))/(gpow(li->Ep[0][0][0][l],2.0)+gpow(li->Epz[0][0][0][l],2.0));
						li->transmit[l]=(gpow(li->Ep[0][0][dim->ylen-1][l],2.0)+gpow(li->Epz[0][0][dim->ylen-1][l],2.0))/(gpow(li->Ep[0][0][0][l],2.0)+gpow(li->Epz[0][0][0][l],2.0));

					}
				}
			}
		}

		//getchar();

		if (li->flip_field==TRUE)
		{
			flip_light_zxy_long_double_y(sim,dim,li->Gn);
			flip_light_zxy_long_double_y(sim,dim,li->Gp);
			flip_light_zxy_long_double_y(sim,dim,li->Htot);
			flip_light_zxy_long_double_y(sim,dim,li->photons_tot);

			flip_light_zxyl_long_double_y(sim,dim,li->H);
			flip_light_zxyl_long_double_y(sim,dim,li->photons_asb);
			flip_light_zxyl_long_double_y(sim,dim,li->photons);

		}
	}else
	{
		memset_light_zxy_long_double(dim,li->Gn,0);
		memset_light_zxy_long_double(dim,li->Gp,0);
		memset_light_zxy_long_double(dim,li->Htot,0);
		memset_light_zxy_long_double(dim,li->photons_tot,0);

		memset_light_zxyl_long_double(dim,li->H,0);
		memset_light_zxyl_long_double(dim,li->photons_asb,0);
		memset_light_zxyl_long_double(dim,li->photons,0);
	}

}else
{
	if ((li->laser_eff==0)&&(li->Psun==0))
	{
		memset_light_zxy_long_double(dim, li->Gn,0);
		memset_light_zxy_long_double(dim, li->Gp,0);
	}
}

return 0.0;
}


void light_norm_photon_density(struct simulation *sim, struct light *li)
{

int x=0;
int y=0;
int z=0;
int l=0;
struct epitaxy *epi=li->epi;
struct dim_light *dim=&li->dim;

long double max=0.0;

for (l=0;l<dim->llen;l++)
{

	max=0.0;

	for (z=0;z<dim->zlen;z++)
	{
		for (x=0;x<dim->xlen;x++)
		{
			for (y=0;y<dim->ylen;y++)
			{
				if ((dim->y[y]>epi->device_start)&&(dim->y[y]<epi->device_stop))
				{
					if (li->photons[z][x][y][l]>max)
					{
						max=li->photons[z][x][y][l];
					}
				}
			}
		}
	}

	if (max>0.0)
	{
		for (z=0;z<dim->zlen;z++)
		{
			for (x=0;x<dim->xlen;x++)
			{
				for (y=0;y<dim->ylen;y++)
				{
					li->photons[z][x][y][l]/=max;
				}
			}
		}
	}


}


}


void light_calculate_complex_n(struct light *li)
{
int x=0;
int y=0;
int z=0;
int l=0;

struct dim_light *dim=&li->dim;

long double nc=0.0;
long double kc=0.0;

long double nr=0.0;
long double kr=0.0;
long double complex n0=0.0+0.0*I;
long double complex n1=0.0+0.0*I;

	for (l=0;l<dim->llen;l++)
	{
		for (z=0;z<dim->zlen;z++)
		{
			for (x=0;x<dim->xlen;x++)
			{
				for (y=0;y<dim->ylen;y++)
				{

					if (y==dim->ylen-1)
					{
						nr=li->n[z][x][y][l];
						kr=li->alpha[z][x][y][l]*(dim->l[l]/(PI*4.0));
					}else
					{
						nr=li->n[z][x][y+1][l];
						kr=li->alpha[z][x][y+1][l]*(dim->l[l]/(PI*4.0));
					}

					nc=li->n[z][x][y][l];
					kc=li->alpha[z][x][y][l]*(dim->l[l]/(PI*4.0));

					n0=nc-kc*I;
					n1=nr-kr*I;

					li->nbar[z][x][y][l]=n0;

					li->r[z][x][y][l]=(n0-n1)/(n0+n1);
					li->t[z][x][y][l]=(2.0*n0)/(n0+n1);
				}
			}
		}
	}


	memset_light_zxyl_long_double(dim, li->En,0);
	memset_light_zxyl_long_double(dim, li->Ep,0);
	memset_light_zxyl_long_double(dim, li->Enz,0);
	memset_light_zxyl_long_double(dim, li->Epz,0);

}



void light_set_sun_power(struct light *li,long double power, long double laser_eff)
{
int l;

struct dim_light *dim=&li->dim;
long double E=0.0;
//long double tot=0.0;

for (l=0;l<dim->llen;l++)
{
	li->sun[l]=li->sun_norm[l]*power;

	E=hp*cl/dim->l[l];
	li->sun_photons[l]=li->sun[l]/E;

	if (l==li->laser_pos)
	{
		if (li->pulse_width!=0.0)
		{
			li->sun_photons[l]+=laser_eff*((li->pulseJ/li->pulse_width/E)/(li->spotx*li->spoty))/dim->dl;
		}

	}
	li->sun_E[l]=gpow(2.0*(li->filter[l]*li->sun_photons[l]*E)/(epsilon0*cl*li->n[0][0][0][l]),0.5);
	/*if (dim->l[l]<500e-9)
	{
		li->sun_E[l]=0.0;
	}*/
	//printf("%d %Le %Le %Le %Le\n",l,li->sun_E[l],li->sun_photons[l],li->sun[l],li->sun_norm[l]);

}

//getchar();
/*for  (l=0;l<dim->llen;l++)
{
	tot=tot+li->sun_photons[l]*dim->dl;
}*/

}



void light_set_unity_power(struct light *li)
{
	int l=0;
	struct dim_light *dim=&li->dim;

	for  (l=0;l<dim->llen;l++)
	{
	//	E=hp*cl/li->l[i];
		li->sun[l]=0.0;
		li->sun_photons[l]=0.0;
		li->sun_E[l]=1.0;//pow(2.0*(1e20*E)/(epsilon0*cl*in->n[i][0]),0.5);
	}


}

int light_get_pos_from_wavelength(struct simulation *sim,struct light *li,double lam)
{
	int l=0;
	struct dim_light *dim=&li->dim;

	if (lam<li->lstart)
	{
		ewe(sim,"The desired wavelenght is smaller than the simulated range");
	}

	if (lam>li->lstop)
	{
		ewe(sim,"The desired wavelenght is bigger than the simulated range");
	}

	l=(int)((lam-li->lstart)/dim->dl);

	return l;
}

void light_set_sun_delta_at_wavelength(struct simulation *sim,struct light *li,long double lam)
{
	int l=0;
	struct dim_light *dim=&li->dim;
	memset(li->sun, 0.0, dim->llen*sizeof(long double));
	memset(li->sun_photons, 0.0, dim->llen*sizeof(long double));
	memset(li->sun_E, 0.0, dim->llen*sizeof(long double));

	l=light_get_pos_from_wavelength(sim,li,lam);
	li->sun_E[l]=1.0;
}

void light_set_unity_laser_power(struct light *li,int lam)
{
	struct dim_light *dim=&li->dim;
	memset(li->sun, 0.0, dim->llen*sizeof(long double));
	memset(li->sun_photons, 0.0, dim->llen*sizeof(long double));
	memset(li->sun_E, 0.0, dim->llen*sizeof(long double));
	li->sun_E[lam]=1.0;
}

void light_get_mode(struct math_xy *mode,int lam,struct light *li)
{
	int y;
	long double device_start=0.0;
	struct epitaxy *epi=li->epi;
	struct dim_light *dim=&li->dim;
	device_start=epi->device_start;

	for (y=0;y<mode->len;y++)
	{
		mode->data[y]=inter_get_raw(dim->y,li->photons[0][0][lam],dim->ylen,device_start+mode->x[y]);
	}

}

long double light_get_optical_power(struct simulation *sim,struct light *li)
{
	struct dim_light *dim=&li->dim;
	long double Power=0.0;
	Power=intergrate_light_l_long_double(dim, li->sun);
	return Power;
}

long double light_convert_density(struct device *in,long double start, long double width)
{
	long double ratio=0.0;
	long double stop=start+width;

	if ((start>=in->time)&&(start<=(in->time+in->dt)))
	{
		if (width>=in->dt)
		{
			ratio=((in->time+in->dt)-start)/in->dt;
		}

		if (width<=in->dt)
		{
			ratio=width/in->dt;
		}
	}


	if ((in->time>=start)&&(in->time<=stop))
	{
		ratio=1.0;
	}

return ratio;
}

void light_transfer_gen_rate_to_device(struct device *dev,struct light *li)
{
	int z=0;
	int x=0;
	int y=0;

	long double Gn=0.0;
	long double Gp=0.0;
	long double pos=0;

	struct dimensions *dim=&(dev->ns.dim);
	struct dim_light *dim_l=&(li->dim);
	struct epitaxy *epi=&(dev->my_epitaxy);

	//struct newton_state *ns=&cell->ns;

	for (y=0;y<dim->ylen;y++)
	{

		for (z=0;z<dim->zlen;z++)
		{
			for (x=0;x<dim->xlen;x++)
			{
				pos=epi->device_start+dim->ymesh[y];

				Gn=interpolate_light_zxy_long_double(dim_l, li->Gn, z, x, pos)*li->Dphotoneff;
				Gp=interpolate_light_zxy_long_double(dim_l, li->Gp, z, x, pos)*li->Dphotoneff;

				//printf("%Le %Le %Le %Le %Le\n",Gn,Gp,li->Gn[0][0][0],pos,li->Dphotoneff);
				dev->Gn[z][x][y]=Gn*li->electron_eff;
				dev->Gp[z][x][y]=Gp*li->hole_eff;
			}
		}
	}


}

long double light_calculate_photons_absorbed_in_active_layer(struct light *li)
{
	int x=0;
	int y=0;
	int z=0;
	struct dim_light *dim=&li->dim;
	struct epitaxy *epi=li->epi;

	long double in_active=0.0;
	long double ret=-1.0;


	for (z=0;z<dim->zlen;z++)
	{
		for (x=0;x<dim->xlen;x++)
		{
			for (y=0;y<dim->ylen;y++)
			{
				if ((dim->y[y]>epi->device_start)&&(dim->y[y]<epi->device_stop))
				{
					in_active+=(li->Gn[z][x][y]+li->Gp[z][x][y])/2.0;
				}
			}
		}
			//tot+=in->photons_tot[i];//(in->Gn[i]+in->Gp[i])/2.0;
	}


ret=in_active*(epi->device_stop-epi->device_start);

return ret;
}
