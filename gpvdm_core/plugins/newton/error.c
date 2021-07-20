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
int band=0;
struct dimensions *dim=&in->ns.dim;
struct matrix *mx = &(in->mx);
struct newton_state *ns=&(in->ns);
struct perovskite *ion=&(in->mobileion);

int Nion_offset=get_offset_nion(in);
int offset_Je=get_offset_Je(in);
int offset_Jh=get_offset_Jh(in);
int offset_srh_e=get_offset_srh_e(in);
int offset_srh_h=get_offset_srh_h(in);
int offset_kcl=get_offset_kcl(in);

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


	gdouble tot=phi+n+p+x+te+th+tl+ttn+ttp+i0+nion+x_y0;
	if (isnan( tot))
	{
		printf_log(sim,"%Le %Le %Le %Le %Le %Le %Le %Le %Le %Le %Le\n",phi,n,p,x,te,th,tl,ttn,ttp,nion,x_y0);
		//dump_matrix(in);
		ewe(sim,"nan detected in newton solver\n");
	}

return tot;
}
