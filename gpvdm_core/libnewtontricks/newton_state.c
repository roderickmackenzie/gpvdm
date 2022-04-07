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

/** @file ntricks.c
	@brief A collection of helper functions for the newton solver.
*/


#include <stdio.h>
#include <exp.h>
#include "sim.h"
#include "dump.h"
#include "newton_tricks.h"
#include "gui_hooks.h"
#include <plot.h>
#include <cal_path.h>
#include <contacts.h>
#include <dump.h>
#include <log.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <inp.h>
#include <memory.h>
#include <device.h>
#include <device_fun.h>

static int unused __attribute__((unused));

void newton_state_init(struct newton_state *ns)
{

	ns->phi=NULL;
	ns->x_Nion=NULL;

	//Singlet
	ns->x_Ns=NULL;
	ns->x_Nt=NULL;
	ns->x_Nsd=NULL;
	ns->x_Ntd=NULL;
	ns->x_Nho=-1;

	ns->x=NULL;
	ns->xp=NULL;

	ns->xt=NULL;
	ns->xpt=NULL;

	//Fermi levels for schottky contact
	ns->x_y0 = NULL;
	ns->xp_y0 = NULL;
	ns->x_y1 = NULL;
	ns->xp_y1 = NULL;

	ns->last_ittr= -1;
	ns->last_error= -1.0;
	ns->last_time=0;
	ns->last_error_pos=0;

	dim_init(&(ns->dim));

	ns->By_xi_plus=NULL;
	ns->By_xi_neg=NULL;
	ns->By_xip_plus=NULL;
	ns->By_xip_neg=NULL;

	ns->Bx_xi_plus=NULL;
	ns->Bx_xi_neg=NULL;
	ns->Bx_xip_plus=NULL;
	ns->Bx_xip_neg=NULL;

	ns->dBy_xi_plus=NULL;
	ns->dBy_xi_neg=NULL;
	ns->dBy_xip_plus=NULL;
	ns->dBy_xip_neg=NULL;

	ns->dBx_xi_plus=NULL;
	ns->dBx_xi_neg=NULL;
	ns->dBx_xip_plus=NULL;
	ns->dBx_xip_neg=NULL;

	ns->Nion_enabled=FALSE;
	ns->singlet_enabled=FALSE;
}

void newton_state_alloc_mesh(struct newton_state *ns,struct dimensions *dim, int alloc_dim)
{

	if (alloc_dim==TRUE)
	{
		ns->dim.xlen=dim->xlen;
		ns->dim.ylen=dim->ylen;
		ns->dim.zlen=dim->zlen;
		dim_malloc(&(ns->dim));
	}

	malloc_zxy_long_double(dim,&(ns->phi));
	if (ns->Nion_enabled==TRUE)
	{
		malloc_zxy_long_double(dim,&(ns->x_Nion));

	}

	if (ns->singlet_enabled==TRUE)
	{
		malloc_zxy_long_double(dim,&(ns->x_Ns));
		malloc_zxy_long_double(dim,&(ns->x_Nt));
		malloc_zxy_long_double(dim,&(ns->x_Nsd));
		malloc_zxy_long_double(dim,&(ns->x_Ntd));
		ns->x_Nho=-1.0;
	}

	malloc_zxy_long_double(dim,&(ns->x));
	malloc_zxy_long_double(dim,&(ns->xp));

	//Fermi levels for schottky contact
	malloc_zx_gdouble(dim,&(ns->x_y0));
	malloc_zx_gdouble(dim,&(ns->xp_y0));
	malloc_zx_gdouble(dim,&(ns->x_y1));
	malloc_zx_gdouble(dim,&(ns->xp_y1));

	malloc_zxy_long_double(dim,&(ns->By_xi_plus));
	malloc_zxy_long_double(dim,&(ns->By_xi_neg));
	malloc_zxy_long_double(dim,&(ns->By_xip_plus));
	malloc_zxy_long_double(dim,&(ns->By_xip_neg));

	malloc_zxy_long_double(dim,&(ns->Bx_xi_plus));
	malloc_zxy_long_double(dim,&(ns->Bx_xi_neg));
	malloc_zxy_long_double(dim,&(ns->Bx_xip_plus));
	malloc_zxy_long_double(dim,&(ns->Bx_xip_neg));


	malloc_zxy_long_double(dim,&(ns->dBy_xi_plus));
	malloc_zxy_long_double(dim,&(ns->dBy_xi_neg));
	malloc_zxy_long_double(dim,&(ns->dBy_xip_plus));
	malloc_zxy_long_double(dim,&(ns->dBy_xip_neg));

	malloc_zxy_long_double(dim,&(ns->dBx_xi_plus));
	malloc_zxy_long_double(dim,&(ns->dBx_xi_neg));
	malloc_zxy_long_double(dim,&(ns->dBx_xip_plus));
	malloc_zxy_long_double(dim,&(ns->dBx_xip_neg));

}

void newton_state_alloc_traps(struct newton_state *ns,struct dimensions *dim)
{
	ns->dim.srh_bands=dim->srh_bands;

	malloc_srh_bands(dim, &(ns->xt));
	malloc_srh_bands(dim, &(ns->xpt));

}

void newton_state_free(struct newton_state *ns)
{

	free_zxy_long_double(&(ns->dim),&ns->phi);
	free_zxy_long_double(&(ns->dim), &ns->x_Nion);


	free_zxy_long_double(&(ns->dim),&ns->x_Ns);
	free_zxy_long_double(&(ns->dim),&ns->x_Nt);
	free_zxy_long_double(&(ns->dim),&ns->x_Nsd);
	free_zxy_long_double(&(ns->dim),&ns->x_Ntd);
	ns->x_Nho=-1.0;

	free_zxy_long_double(&(ns->dim),&ns->x);
	free_zxy_long_double(&(ns->dim),&ns->xp);

	free_srh_bands(&(ns->dim), &ns->xt);
	free_srh_bands(&(ns->dim), &ns->xpt);

	//Fermi levels for schottky contact
	free_zx_gdouble(&(ns->dim),&ns->x_y0);
	free_zx_gdouble(&(ns->dim),&ns->xp_y0);
	free_zx_gdouble(&(ns->dim),&ns->x_y1);
	free_zx_gdouble(&(ns->dim),&ns->xp_y1);

	free_zxy_long_double(&(ns->dim),&(ns->By_xi_plus));
	free_zxy_long_double(&(ns->dim),&(ns->By_xi_neg));
	free_zxy_long_double(&(ns->dim),&(ns->By_xip_plus));
	free_zxy_long_double(&(ns->dim),&(ns->By_xip_neg));

	free_zxy_long_double(&(ns->dim),&(ns->Bx_xi_plus));
	free_zxy_long_double(&(ns->dim),&(ns->Bx_xi_neg));
	free_zxy_long_double(&(ns->dim),&(ns->Bx_xip_plus));
	free_zxy_long_double(&(ns->dim),&(ns->Bx_xip_neg));

	free_zxy_long_double(&(ns->dim),&(ns->dBy_xi_plus));
	free_zxy_long_double(&(ns->dim),&(ns->dBy_xi_neg));
	free_zxy_long_double(&(ns->dim),&(ns->dBy_xip_plus));
	free_zxy_long_double(&(ns->dim),&(ns->dBy_xip_neg));

	free_zxy_long_double(&(ns->dim),&(ns->dBx_xi_plus));
	free_zxy_long_double(&(ns->dim),&(ns->dBx_xi_neg));
	free_zxy_long_double(&(ns->dim),&(ns->dBx_xip_plus));
	free_zxy_long_double(&(ns->dim),&(ns->dBx_xip_neg));

	dim_free(&(ns->dim));

}

void newton_state_cpy(struct newton_state *out,struct newton_state *in)
{

	newton_state_free(out);
	out->Nion_enabled=in->Nion_enabled;

	dim_cpy(&(out->dim),&(in->dim));

	cpy_zxy_long_double(&in->dim,&(out->phi),&(in->phi));
	cpy_zxy_long_double(&in->dim,&(out->x_Nion),&(in->x_Nion));

	cpy_zxy_long_double(&in->dim,&(out->x_Ns),&(in->x_Ns));
	cpy_zxy_long_double(&in->dim,&(out->x_Nt),&(in->x_Nt));
	cpy_zxy_long_double(&in->dim,&(out->x_Nsd),&(in->x_Nsd));
	cpy_zxy_long_double(&in->dim,&(out->x_Ntd),&(in->x_Ntd));
	out->x_Nho=in->x_Nho;

	cpy_zxy_long_double(&in->dim,&(out->x),&(in->x));
	cpy_zxy_long_double(&in->dim,&(out->xp),&(in->xp));

	//Fermi levels for schottky contact
	cpy_zx_long_double(&in->dim, &(out->x_y0),&(in->x_y0));
	cpy_zx_long_double(&in->dim, &(out->xp_y0),&(in->xp_y0));
	cpy_zx_long_double(&in->dim, &(out->x_y1),&(in->x_y1));
	cpy_zx_long_double(&in->dim, &(out->xp_y1),&(in->xp_y1));

	cpy_zxy_long_double(&in->dim,&(out->By_xi_plus),&(in->By_xi_plus));
	cpy_zxy_long_double(&in->dim,&(out->By_xi_neg),&(in->By_xi_neg));
	cpy_zxy_long_double(&in->dim,&(out->By_xip_plus),&(in->By_xip_plus));
	cpy_zxy_long_double(&in->dim,&(out->By_xip_neg),&(in->By_xip_neg));

	cpy_zxy_long_double(&in->dim,&(out->Bx_xi_plus),&(in->Bx_xi_plus));
	cpy_zxy_long_double(&in->dim,&(out->Bx_xi_neg),&(in->Bx_xi_neg));
	cpy_zxy_long_double(&in->dim,&(out->Bx_xip_plus),&(in->Bx_xip_plus));
	cpy_zxy_long_double(&in->dim,&(out->Bx_xip_neg),&(in->Bx_xip_neg));


	cpy_zxy_long_double(&in->dim,&(out->dBy_xi_plus),&(in->dBy_xi_plus));
	cpy_zxy_long_double(&in->dim,&(out->dBy_xi_neg),&(in->dBy_xi_neg));
	cpy_zxy_long_double(&in->dim,&(out->dBy_xip_plus),&(in->dBy_xip_plus));
	cpy_zxy_long_double(&in->dim,&(out->dBy_xip_neg),&(in->dBy_xip_neg));

	cpy_zxy_long_double(&in->dim,&(out->dBx_xi_plus),&(in->dBx_xi_plus));
	cpy_zxy_long_double(&in->dim,&(out->dBx_xi_neg),&(in->dBx_xi_neg));
	cpy_zxy_long_double(&in->dim,&(out->dBx_xip_plus),&(in->dBx_xip_plus));
	cpy_zxy_long_double(&in->dim,&(out->dBx_xip_neg),&(in->dBx_xip_neg));

	cpy_srh_long_double(&in->dim, &(out->xt), &(in->xt));
	cpy_srh_long_double(&in->dim, &(out->xpt), &(in->xpt));

}




void newton_state_update_device(struct simulation *sim,struct device *in, struct newton_state *ns)
{
	int x=0;
	int z=0;
	long double *buf;
	struct dimensions *dim=&ns->dim;

	for (z=0;z<dim->zlen;z++)
	{
		for (x=0;x<dim->xlen;x++)
		{
			update_y_array(sim,in,z,x);
		}
	}


}

void newton_state_set_last_error(struct simulation *sim, struct newton_state *ns,long double error)
{
	ns->last_error=error;
	ns->last_errors[ns->last_error_pos]=error;
	ns->last_error_pos++;
	if (ns->last_error_pos>=10)
	{
		ns->last_error_pos=0;
	}
}

void newton_state_reset_error(struct simulation *sim, struct newton_state *ns)
{
	int i;
	ns->last_ittr=0;
	ns->last_error=0.0;
	ns->last_error_pos=0;
	for (i=0;i<10;i++)
	{
		ns->last_errors[i]=0.0;
	}
}

int newton_state_clever_exit(struct simulation *sim, struct newton_state *ns)
{
	//Wait for threshold to be reached
	//then wait for the graident to go up
	//then wait for error to drop below threshold and exit
	int i;
	long double avg=0.0;
	long double min=0.0;
	int pos=0;
	long double e0=0.0;
	long double e1=0.0;

	int steps=10;
	int looking_to_exit=FALSE;
	int last_error_pos=ns->last_error_pos;
	int bumping=FALSE;
	long double clever_exit_error=1e-5;
	long double e2=-1.0;
	long double ordered[steps];
	if (last_error_pos<0)
	{
		last_error_pos+=steps;
	}

	if (ns->last_ittr>=steps)
	{
		//avg=0.0;
		//min=ns->last_errors[0];
		//for (i=0;i<steps;i++)
		//{
		//	printf("%d %Le\n",i,ns->last_errors[i]);
		//}

		//printf("----\n");
		looking_to_exit=FALSE;
		bumping=FALSE;
		for (i=0;i<steps;i++)
		{
			pos=last_error_pos+i;
			if (pos>=steps)
			{
				pos-=steps;
			}
			//printf("%d\n",last,);
			e0=ns->last_errors[pos];
			ordered[i]=e0;


			if (e0<clever_exit_error)
			{	
				looking_to_exit=TRUE;
			}

			//printf("%d %Le %d %d\n",i,e0,last_error_pos,ns->last_ittr);

			if (looking_to_exit==TRUE)
			{
				if (e1<e0)
				{
					ns->last_error=e1;
					return 0;
					if (bumping==FALSE)
					{
						min=e1;
						bumping=TRUE;
					}
				}
			}


			if (bumping==TRUE)
			{
				if (e0<min*5.0)
				{
					return 0;//printf("exit!!!!\n");
				}
			}

			e1=e0;
		}

		/*e1=ordered[0];
		e0=ordered[steps-1];
		if (e1<1e-5)
		{
			if (e0<1e-5)
			{
				printf("ratio=%Le  %Le %Le\n",e0/e1,e0,e1);
			}
		}

		getchar();*/
		//getchar();
	}

return 1;
}

