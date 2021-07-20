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

/** @file newton.c
	@brief Standard newton solver.
*/


#include <string.h>
#include <log.h>
#include <gpvdm_const.h>
#include "newton.h"
#include <dll_export.h>
#include <util.h>
#include <exp.h>
#include <advmath.h>
#include <dump.h>
#include <cal_path.h>
#include <dos.h>
#include <sim.h>
#include <solver_interface.h>
#include <contacts.h>
#include <memory.h>
#include <newton_tricks.h>
#include <timer.h>
#include <perovskite.h>

void solver_cal_memory(struct device *in,int *ret_N,int *ret_M)
{
int i=0;
int N=0;
int M=0;
struct newton_state *ns=&(in->ns);
struct dimensions *dim=&in->ns.dim;
//struct matrix *mx=&(in->mx);
struct perovskite *ion=&(in->mobileion);
int contact_left=in->contacts[in->n_contact_y0[0][0]].type;
//in->contact_shift_l=0;


N=dim->ylen*3-2;	//Possion main

N+=dim->ylen*3-2;	//Je main
N+=dim->ylen*3-2;	//Jh main

N+=dim->ylen*3-2;	//dJe/phi
N+=dim->ylen*3-2;	//dJh/phi

N+=dim->ylen;		//dphi/dn
N+=dim->ylen;		//dphi/dh

N+=dim->ylen;		//dJndp
N+=dim->ylen;		//dJpdn

M=dim->ylen;			//Pos
M+=dim->ylen;		//Je
M+=dim->ylen;		//Jh

if (contact_left==contact_schottky)
{
	N+=1;
	M+=1;
}

if (ion->full_newton==TRUE)
{
	if (ns->Nion_enabled==TRUE)
	{
		int delta=ion->ystop-ion->ystart;

		N+=delta*3-2;		//Nion	main
		N+=delta*3-2;		//dNion/dphi
		N+=delta;			//dphi/dNion
		M+=delta;			//Nion
	}
}

if (in->interfaces_n>0)
{
	N+=in->interfaces_n*2;	//for the dp+1 term
}

if (in->ntrapnewton==TRUE)
{
	for (i=0;i<dim->srh_bands;i++)
	{
		N+=dim->ylen;		//dntrapdn
		N+=dim->ylen;		//dntrapdntrap
		N+=dim->ylen;		//dntrapdp
		N+=dim->ylen;		//dJndtrapn
		N+=dim->ylen;		//dJpdtrapn
		N+=dim->ylen;		//dphidntrap

		M+=dim->ylen;		//nt
	}

}

if (in->ptrapnewton==TRUE)
{
	for (i=0;i<dim->srh_bands;i++)
	{
		N+=dim->ylen;		//dptrapdp
		N+=dim->ylen;		//dptrapdptrap
		N+=dim->ylen;		//dptrapdn
		N+=dim->ylen;		//dJpdtrapp
		N+=dim->ylen;		//dJdtrapp
		N+=dim->ylen;		//dphidptrap

		M+=dim->ylen;		//pt
	}
}

//printf(">>>>>>>>>>>> %d %d %d\n",N,ns->Nion_enabled,ion->full_newton);
//getchar();
*ret_N=N;
*ret_M=M;
}

void dllinternal_solver_realloc(struct simulation *sim,struct device *in, int dim_)
{
int N=0;
int M=0;
struct dimensions *dim=&in->ns.dim;
struct matrix *mx=&(in->mx);
gdouble *dtemp=NULL;



solver_cal_memory(in,&N,&M);

//printf("realloc %d %d\n",mx->nz,N);
//getchar();

int alloc=FALSE;
if ((mx->nz==0)||(mx->M==0))
{
	mx->nz=N;
	mx->M=M;
	alloc=TRUE;
}else
if ((N!=mx->nz)||(M!=mx->M))
{
	mx->nz=N;
	mx->M=M;
	alloc=TRUE;
}


if (alloc==TRUE)
{

	matrix_realloc(sim,mx);

	if (dim->srh_bands>0)
	{
		dtemp=realloc(in->newton_dntrap,dim->srh_bands*sizeof(gdouble));
		if (dtemp==NULL)
		{
			ewe(sim,"memory error\n");
		}else
		{
			in->newton_dntrap=dtemp;
		}

		dtemp=realloc(in->newton_dntrapdntrap,dim->srh_bands*sizeof(gdouble));
		if (dtemp==NULL)
		{
			ewe(sim,"memory error\n");
		}else
		{
			in->newton_dntrapdntrap=dtemp;
		}

		dtemp=realloc(in->newton_dntrapdn,dim->srh_bands*sizeof(gdouble));
		if (dtemp==NULL)
		{
			ewe(sim,"memory error\n");
		}else
		{
			in->newton_dntrapdn=dtemp;
		}

		dtemp=realloc(in->newton_dntrapdp,dim->srh_bands*sizeof(gdouble));
		if (dtemp==NULL)
		{
			ewe(sim,"memory error\n");
		}else
		{
			in->newton_dntrapdp=dtemp;
		}

		dtemp=realloc(in->newton_dJdtrapn,dim->srh_bands*sizeof(gdouble));
		if (dtemp==NULL)
		{
			ewe(sim,"memory error\n");
		}else
		{
			in->newton_dJdtrapn=dtemp;
		}

		dtemp=realloc(in->newton_dJpdtrapn,dim->srh_bands*sizeof(gdouble));
		if (dtemp==NULL)
		{
			ewe(sim,"memory error\n");
		}else
		{
			in->newton_dJpdtrapn=dtemp;
		}

		dtemp=realloc(in->newton_dphidntrap,dim->srh_bands*sizeof(gdouble));
		if (dtemp==NULL)
		{
			ewe(sim,"memory error\n");
		}else
		{
			in->newton_dphidntrap=dtemp;
		}


		dtemp=realloc(in->newton_ntlast,dim->srh_bands*sizeof(gdouble));
		if (dtemp==NULL)
		{
			ewe(sim,"memory error\n");
		}else
		{
			in->newton_ntlast=dtemp;
		}



		dtemp=realloc(in->newton_dptrapdp,dim->srh_bands*sizeof(gdouble));
		if (dtemp==NULL)
		{
			ewe(sim,"memory error\n");
		}else
		{
			in->newton_dptrapdp=dtemp;
		}

		dtemp=realloc(in->newton_dptrapdptrap,dim->srh_bands*sizeof(gdouble));
		if (dtemp==NULL)
		{
			ewe(sim,"memory error\n");
		}else
		{
			in->newton_dptrapdptrap=dtemp;
		}

		dtemp=realloc(in->newton_dptrap,dim->srh_bands*sizeof(gdouble));
		if (dtemp==NULL)
		{
			ewe(sim,"memory error\n");
		}else
		{
			in->newton_dptrap=dtemp;
		}

		dtemp=realloc(in->newton_dptrapdn,dim->srh_bands*sizeof(gdouble));
		if (dtemp==NULL)
		{
			ewe(sim,"memory error\n");
		}else
		{
			in->newton_dptrapdn=dtemp;
		}

		dtemp=realloc(in->newton_dJpdtrapp,dim->srh_bands*sizeof(gdouble));
		if (dtemp==NULL)
		{
			ewe(sim,"memory error\n");
		}else
		{
			in->newton_dJpdtrapp=dtemp;
		}


		dtemp=realloc(in->newton_dJdtrapp,dim->srh_bands*sizeof(gdouble));
		if (dtemp==NULL)
		{
			ewe(sim,"memory error\n");
		}else
		{
			in->newton_dJdtrapp=dtemp;
		}

		dtemp=realloc(in->newton_dphidptrap,dim->srh_bands*sizeof(gdouble));
		if (dtemp==NULL)
		{
			ewe(sim,"memory error\n");
		}else
		{
			in->newton_dphidptrap=dtemp;
		}

		dtemp=realloc(in->newton_ptlast,dim->srh_bands*sizeof(gdouble));
		if (dtemp==NULL)
		{
			ewe(sim,"memory error\n");
		}else
		{
			in->newton_ptlast=dtemp;
		}

	}

}

}

void dllinternal_solver_free_memory(struct simulation *sim,struct device *in)
{
//int i=0;
struct dimensions *dim=&in->ns.dim;
if (dim->srh_bands>0)
{
	free_1d((void **)&in->newton_dntrap, sizeof(long double ));
	free_1d((void **)&in->newton_dntrapdntrap, sizeof(long double ));
	free_1d((void **)&in->newton_dntrapdn, sizeof(long double ));
	free_1d((void **)&in->newton_dntrapdp, sizeof(long double ));
	free_1d((void **)&in->newton_dJdtrapn, sizeof(long double ));
	free_1d((void **)&in->newton_dJpdtrapn, sizeof(long double ));
	free_1d((void **)&in->newton_dphidntrap, sizeof(long double ));
	free_1d((void **)&in->newton_dptrapdp, sizeof(long double ));
	free_1d((void **)&in->newton_dptrapdptrap, sizeof(long double ));
	free_1d((void **)&in->newton_dptrap, sizeof(long double ));
	free_1d((void **)&in->newton_dptrapdn, sizeof(long double ));
	free_1d((void **)&in->newton_dJpdtrapp, sizeof(long double ));
	free_1d((void **)&in->newton_dJdtrapp, sizeof(long double ));

	free_1d((void **)&in->newton_dJpdtrapp_interface_right, sizeof(long double ));
	free_1d((void **)&in->newton_dphidptrap, sizeof(long double ));
	free_1d((void **)&in->newton_ntlast, sizeof(long double ));
	free_1d((void **)&in->newton_ptlast, sizeof(long double ));


}


//for (i=0;i<mx->nz;i++)
//{
//	free(in->Tdebug[i]);
//}
//free(in->Tdebug);

in->newton_dntrapdntrap=NULL;
in->newton_dntrap=NULL;
in->newton_dntrapdn=NULL;
in->newton_dntrapdp=NULL;
in->newton_dJdtrapn=NULL;
in->newton_dJpdtrapn=NULL;
in->newton_dphidntrap=NULL;
in->newton_dptrapdp=NULL;
in->newton_dptrapdptrap=NULL;
in->newton_dptrap=NULL;
in->newton_dptrapdn=NULL;
in->newton_dJpdtrapp=NULL;
in->newton_dJdtrapp=NULL;
in->newton_dJpdtrapp_interface_right=NULL;
in->newton_dphidptrap=NULL;


in->newton_ntlast=NULL;
in->newton_ptlast=NULL;

//in->Tdebug=NULL;

}
