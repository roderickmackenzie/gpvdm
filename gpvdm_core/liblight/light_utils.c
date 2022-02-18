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
#include "exciton.h"
#include "exciton_fun.h"

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

void light_cal_photon_density_y(struct simulation *sim,struct light *li,struct device *dev, int z, int x, int l)
{
	struct dimensions *dim=&li->dim;

	struct shape* s;
	struct object* obj;
	long double Eg=0.0;
	int y;

	long double E=0.0;
	long double pointing_vector=0.0;
	long double E_tot_r=0.0;
	long double E_tot_i=0.0;
	double photon_density;

	for (y=0;y<dim->ylen;y++)
	{
		obj=li->obj[z][x][y];
		s=obj->s;

		//li->E_tot_r[z][x][y][l]
		E_tot_r=li->Ep[z][x][y][l]+li->En[z][x][y][l];
		//li->E_tot_i[z][x][y][l]
		E_tot_i=li->Enz[z][x][y][l]+li->Epz[z][x][y][l];
		//li->pointing_vector[z][x][y][l]=
		pointing_vector=0.5*epsilon0*cl*li->n[z][x][y][l]*(gpow(E_tot_r,2.0)+gpow(E_tot_i,2.0));

		photon_density=pointing_vector*(dim->l[l]/(hp*cl));
		//if (l==2)
		//{
		//	printf("b %e\n",photon_density);
		//}

		li->photons[z][x][y][l]+=photon_density;
		li->photons_asb[z][x][y][l]+=photon_density*li->alpha[z][x][y][l];

		Eg=0.0;
		if (obj->epi_layer!=-1)
		{
			Eg=s->dosn.config.Eg;
		}else
		{

			if (s->dosn.enabled==TRUE)
			{
				Eg=s->dosn.config.Eg;
			}

		}

		E=((hp*cl)/dim->l[l])/Qe-Eg;

		if (E>0.0)
		{
			li->H[z][x][y][l]+=E*Qe*li->photons_asb[z][x][y][l];
		}

	}

}

long double light_cal_photon_density(struct simulation *sim,struct light *li,struct device *dev)
{
struct dimensions *dim=&li->dim;

//struct shape* s;
//struct object* obj;
int x;
int y;
int z;
int l;

long double tot=0.0;
long double H_tot=0.0;
long double photons_tot=0.0;
//long double E=0.0;


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
					//obj=li->obj[z][x][y];
					//s=obj->s;
					for (l=0;l<dim->llen;l++)
					{
						photons_tot+=li->photons[z][x][y][l]*dim->dl;
						tot+=li->photons_asb[z][x][y][l]*dim->dl;
						H_tot+=li->H[z][x][y][l]*dim->dl;
					}
					//printf("%Le\n",photons_tot);
					li->Gn[z][x][y]=tot;
					li->Gp[z][x][y]=tot;

					li->Htot[z][x][y]=H_tot;
					li->photons_tot[z][x][y]=photons_tot;


				}
			}
		}

		for (l=0;l<dim->llen;l++)
		{
			li->reflect[l]=(gpow(li->En[0][0][0][l],2.0)+gpow(li->Enz[0][0][0][l],2.0))/(gpow(li->Ep[0][0][0][l],2.0)+gpow(li->Epz[0][0][0][l],2.0));
			li->transmit[l]=(gpow(li->Ep[0][0][dim->ylen-1][l],2.0)+gpow(li->Epz[0][0][dim->ylen-1][l],2.0))/(gpow(li->Ep[0][0][0][l],2.0)+gpow(li->Epz[0][0][0][l],2.0));

		}

		//getchar();

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


long double light_cal_photons_abs_l(struct simulation *sim,struct light *li,int l)
{
struct dimensions *dim=&li->dim;

int x;
int y;
int z;
long double tot=0.0;

	for (z=0;z<dim->zlen;z++)
	{
		for (x=0;x<dim->xlen;x++)
		{
			for (y=0;y<dim->ylen;y++)
			{
				tot+=li->photons_asb[z][x][y][l]*dim->dl*dim->dx*dim->dy*dim->dz;
				//printf("%le %Le %Le %Le %Le\n",li->photons_asb[z][x][y][l],dim->dl,dim->dx,dim->dy,dim->dz);
			}
		}
	}
//getchar();
return tot;
}

void light_norm_photon_density(struct simulation *sim, struct light *li)
{

int x=0;
int y=0;
int z=0;
int l=0;
struct epitaxy *epi=li->epi;
struct dimensions *dim=&li->dim;

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
int l=0;
int x=0;
int z=0;
struct dimensions *dim=&li->dim;
	for (z=0;z<dim->zlen;z++)
	{
		for (x=0;x<dim->xlen;x++)
		{

			for (l=0;l<dim->llen;l++)
			{
				light_calculate_complex_n_zxl(li,z,x,l);
			}
		}
	}

}


void light_calculate_complex_n_zxl(struct light *li, int z, int x,  int l)
{
int y=0;

struct dimensions *dim=&li->dim;

long double nc=0.0;
long double kc=0.0;

long double nr=0.0;
long double kr=0.0;
long double complex n0=0.0+0.0*I;
long double complex n1=0.0+0.0*I;

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

void light_set_sun_power(struct light *li,long double power, long double laser_eff)
{
int l;

struct dimensions *dim=&li->dim;
long double E=0.0;
long double laser_photons=0.0;
long double nl=0.0;
long double nr=0.0;
long double complex n0=0.0+0.0*I;
long double complex n1=0.0+0.0*I;
long double complex t0_complex=0.0+0.0*I;
long double complex t1_complex=0.0+0.0*I;
long double t0=0.0;
long double t1=0.0;
long double lam=0.0;

for (l=0;l<dim->llen;l++)
{
	lam=dim->l[l];
	//With filter
	li->sun_y0[l]=li->light_src_y0.spectra_tot.data[l]*power;
	li->sun_y1[l]=li->light_src_y1.spectra_tot.data[l]*power;

	//With no filter
	li->sun_y0_no_filter[l]=li->light_src_y0.spectra_tot_no_filter.data[l]*power;
	li->sun_y1_no_filter[l]=li->light_src_y1.spectra_tot_no_filter.data[l]*power;

	E=hp*cl/dim->l[l];

	//With filter
	li->sun_photons_y0[l]=li->sun_y0[l]/E;
	li->sun_photons_y1[l]=li->sun_y1[l]/E;

	//With no filter
	li->sun_photons_y0_no_filter[l]=li->sun_y0_no_filter[l]/E;
	li->sun_photons_y1_no_filter[l]=li->sun_y1_no_filter[l]/E;

	if (l==li->laser_pos)
	{

		if (li->pulse_width!=0.0)
		{
			laser_photons=laser_eff*((li->pulseJ/li->pulse_width/E)/(li->spotx*li->spoty))/dim->dl;
			printf("%Le\n",laser_photons);
			
			if (li->light_src_y0.nspectra>0)
			{
				if (li->light_src_y0.filter_enabled==TRUE)
				{
					if (li->light_src_y0.filter_read.len>0)
					{
						laser_photons*=inter_get_hard(&(li->light_src_y0.filter_read),lam);
					}
				}

				li->sun_photons_y0[l]+=laser_photons;
			}

			if (li->light_src_y1.nspectra>0)
			{
				if (li->light_src_y1.filter_enabled==TRUE)
				{
					if (li->light_src_y1.filter_read.len>0)
					{
						laser_photons*=inter_get_hard(&(li->light_src_y1.filter_read),lam);
					}
				}

				li->sun_photons_y1[l]+=laser_photons;
			}

		}

	}

	nl=li->n[0][0][0][l];
	nr=li->n[0][0][dim->ylen-1][l];
	t0=1.0;
	t1=1.0;

	if (li->light_src_y0.external_interface_enabled==TRUE)
	{
		nl=li->light_src_y0.n;
		n0=nl-0.0*I;
		n1=li->nbar[0][0][0][l];

		t0_complex=(2.0*n0)/(n0+n1);
		t0=cabs(t0_complex);
		//printf("%e + i%e %e\n", creal(t0_complex), cimag(t0_complex),cabs(t0_complex));
	}

	if (li->light_src_y1.external_interface_enabled==TRUE)
	{
		nr=li->light_src_y1.n;
		n0=nr-0.0*I;
		n1=li->nbar[0][0][dim->ylen-1][l];

		t1_complex=(2.0*n0)/(n0+n1);
		t1=cabs(t1_complex);
		//printf("%e + i%e %e\n", creal(t1_complex), cimag(t1_complex),cabs(t1_complex));
	}

	li->sun_E_y0[l]=gpow(2.0*(li->sun_photons_y0[l]*E)/(epsilon0*cl*nl),0.5)*t0;
	li->sun_E_y1[l]=gpow(2.0*(li->sun_photons_y1[l]*E)/(epsilon0*cl*nr),0.5)*t1;

}

}



void light_set_unity_power(struct light *li)
{
	int l=0;
	struct dimensions *dim=&li->dim;

	for  (l=0;l<dim->llen;l++)
	{
		li->sun_photons_y0[l]=0.0;
		li->sun_photons_y1[l]=0.0;
		li->sun_E_y0[l]=1.0;//pow(2.0*(1e20*E)/(epsilon0*cl*in->n[i][0]),0.5);
		li->sun_E_y1[l]=0.0;//pow(2.0*(1e20*E)/(epsilon0*cl*in->n[i][0]),0.5);
	}


}

void light_flip_y_float(struct dimensions *dim,float ****in,int z,int x,int l)
{
	int i;
	int j;
	float temp;
	i = dim->ylen - 1;
	j = 0;
	while(i > j)
	{
		temp = in[z][x][i][l];
		in[z][x][i][l] = in[z][x][j][l];
		in[z][x][j][l] = temp;
		i--;
		j++;
	}

}

void light_flip_y_float_complex(struct dimensions *dim,float complex ****in,int z,int x,int l)
{
	int i;
	int j;
	float complex temp;
	i = dim->ylen - 1;
	j = 0;
	while(i > j)
	{
		temp = in[z][x][i][l];
		in[z][x][i][l] = in[z][x][j][l];
		in[z][x][j][l] = temp;
		i--;
		j++;
	}

}

int light_get_pos_from_wavelength(struct simulation *sim,struct light *li,double lam)
{
	int l=0;
	struct dimensions *dim=&li->dim;

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



void light_get_mode(struct math_xy *mode,int lam,struct light *li)
{
	int y;
	long double device_start=0.0;
	struct epitaxy *epi=li->epi;
	struct dimensions *dim=&li->dim;
	device_start=epi->device_start;

	for (y=0;y<mode->len;y++)
	{
		mode->data[y]=math_interpolate_raw_double(dim->y,li->photons[0][0][lam],dim->ylen,device_start+mode->x[y]);
	}

}

long double light_get_optical_power(struct simulation *sim,struct light *li)
{
	struct dimensions *dim=&li->dim;
	long double Power=0.0;
	Power=intergrate_light_l_long_double(dim, li->sun_y0);
	Power+=intergrate_light_l_long_double(dim, li->sun_y1);

	return Power;
}

long double light_get_optical_power_no_filter(struct simulation *sim,struct light *li)
{
	struct dimensions *dim=&li->dim;
	long double Power=0.0;
	Power=intergrate_light_l_long_double(dim, li->sun_y0_no_filter);
	Power+=intergrate_light_l_long_double(dim, li->sun_y1_no_filter);

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

void light_transfer_gen_rate_to_device(struct simulation *sim,struct device *dev,struct light *li)
{
	int z=0;
	int x=0;
	int y=0;

	long double Gn=0.0;
	long double Gp=0.0;
	long double pos=0;
	struct dimensions *dim=&(dev->ns.dim);
	struct dimensions *dim_l=&(li->dim);
	struct epitaxy *epi=&(dev->my_epitaxy);
	struct exciton *ex=&(dev->ex);
	struct dimensions *dim_ex=&(ex->dim);

	if (dev->ex.exciton_enabled==TRUE)
	{
		printf("Running exciton solver\n");
		exciton_solve(sim, ex,dev);
	}

	if (dev->electrical_simulation_enabled==FALSE)
	{
		return;
	}


	//struct newton_state *ns=&cell->ns;


	for (z=0;z<dim->zlen;z++)
	{
		for (x=0;x<dim->xlen;x++)
		{

			for (y=0;y<dim->ylen;y++)
			{

				if (dev->ex.exciton_enabled==TRUE)
				{
					pos=epi->device_start+dim->y[y];
					Gn=interpolate_light_zxy_long_double(dim_ex, ex->Gn, z, x, pos)*li->Dphotoneff;
					Gp=Gn;
					//printf("%Le %Le %Le %Le %Le\n",Gn,ex->Gn[0][0][0],pos, dim_ex->y[0],dim_ex->y[dim_ex->ylen-1]);
					//printf("%Le %Le %Le %Le %Le\n",Gn,Gp,li->Gn[0][0][0],pos,li->Dphotoneff);
					dev->Gn[z][x][y]=Gn*li->electron_eff;
					dev->Gp[z][x][y]=Gp*li->hole_eff;

				}else
				{
					pos=epi->device_start+dim->y[y];
					Gn=interpolate_light_zxy_long_double(dim_l, li->Gn, z, x, pos)*li->Dphotoneff;
					Gp=interpolate_light_zxy_long_double(dim_l, li->Gp, z, x, pos)*li->Dphotoneff;

					//printf("%Le %Le %Le %Le %Le\n",Gn,Gp,li->Gn[0][0][0],pos,li->Dphotoneff);
					dev->Gn[z][x][y]=Gn*li->electron_eff;
					dev->Gp[z][x][y]=Gp*li->hole_eff;

				}
			}
		}
	}
	//getchar();

}

long double light_J_photo(struct light *li)
{
	int x=0;
	int y=0;
	int z=0;
	struct dimensions *dim=&li->dim;
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
					in_active+=(li->Gn[z][x][y]+li->Gp[z][x][y])*dim->dy/2.0;
				}
			}
		}

	}


ret=in_active*Qe;

return ret;
}

long double light_i_photo(struct light *li,struct device *dev)
{
	long double ret=0.0;
	ret=light_J_photo(li);
	ret=ret*dev->area;
	return ret;
}
