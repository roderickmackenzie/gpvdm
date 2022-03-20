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

/** @file error.c
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

gdouble get_cur_error(struct simulation *sim,struct device *in)
{
int i;
gdouble phi=0.0;
gdouble n=0.0;
gdouble p=0.0;
gdouble x=0.0;
gdouble te=0.0;
gdouble th=0.0;
gdouble tl=0.0;
gdouble ttn=0.0;
gdouble ttp=0.0;
gdouble i0=0.0;
gdouble nion=0.0;
long double x_y0=0.0;
long double Ns=0.0;
long double Nt=0.0;
long double Nsd=0.0;
long double Ntd=0.0;
long double Nho=0.0;

int band=0;
struct dimensions *dim=&in->ns.dim;
struct matrix *mx = &(in->mx);
struct newton_state *ns=&(in->ns);
struct perovskite *ion=&(in->mobileion);
struct singlet *sing=&(in->sing);

int Nion_offset=get_offset_nion(in);
int offset_Je=get_offset_Je(in);
int offset_Jh=get_offset_Jh(in);
int offset_srh_e=get_offset_srh_e(in);
int offset_srh_h=get_offset_srh_h(in);
int offset_kcl=get_offset_kcl(in);
int offset_Ns=get_offset_singlet_Ns(in);
int offset_Nt=get_offset_singlet_Nt(in);
int offset_Nsd=get_offset_singlet_Nsd(in);
int offset_Ntd=get_offset_singlet_Ntd(in);
int offset_Nho=get_offset_singlet_Nho(in);

int contact_left=in->contacts[in->n_contact_y0[0][0]].type;

	for (i=0;i<dim->ylen;i++)
	{
			//if ((in->interfaceleft==TRUE)&&(i==0))
			//{
			//}else
			//if ((in->interfaceright==TRUE)&&(i==dim->ylen-1))
			//{
			//}else
			//{
				phi+=gfabs(mx->b[i]);
			//}

			n+=gfabs(mx->b[offset_Je+i]);
			p+=+gfabs(mx->b[offset_Jh+i]);

			if (in->ntrapnewton==TRUE)
			{
				for (band=0;band<dim->srh_bands;band++)
				{
					ttn+=gfabs(mx->b[offset_srh_e+dim->ylen*band+i]);
				}
			}

			if (in->ptrapnewton==TRUE)
			{
				for (band=0;band<dim->srh_bands;band++)
				{
					ttp+=gfabs(mx->b[offset_srh_h+dim->ylen*band+i]);
				}
			}


	}

	if (contact_left==contact_schottky)
	{
		x_y0=gfabs(mx->b[offset_Je-1]);
	}


	gdouble tot=phi+n+p+x+te+th+tl+ttn+ttp+i0+nion+x_y0+Ns+Nt+Nsd+Ntd+Nho;
	if (isnan( tot))
	{
		printf_log(sim,"%Le %Le %Le %Le %Le %Le %Le %Le %Le %Le %Le\n",phi,n,p,x,te,th,tl,ttn,ttp,nion,x_y0);
		//dump_matrix(in);
		ewe(sim,"nan detected in newton solver\n");
	}

return tot;
}
