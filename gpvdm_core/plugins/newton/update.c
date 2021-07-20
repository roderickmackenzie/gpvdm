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

void update_solver_vars(struct simulation *sim,struct device *in,int z,int x,int clamp)
{
int i;
int band=0;
long double Vapplied=0.0;
long double clamp_temp=300.0;
struct newton_state *ns=&(in->ns);
struct dimensions *dim=&in->ns.dim;
struct matrix *mx=&in->mx;
struct perovskite *ion=&(in->mobileion);

int Nion_offset=get_offset_nion(in);
int offset_Je=get_offset_Je(in);
int offset_Jh=get_offset_Jh(in);
int offset_srh_e=get_offset_srh_e(in);
int offset_srh_h=get_offset_srh_h(in);
int offset_kcl=get_offset_kcl(in);

int contact_left=in->contacts[in->n_contact_y0[0][0]].type;

gdouble update=0.0;

	for (i=0;i<dim->ylen;i++)
	{

		update=(gdouble)mx->b[i];
		if (clamp==TRUE)
		{
			ns->phi[z][x][i]+=update/(1.0+gfabs(update/in->electrical_clamp/(clamp_temp*kb/Qe)));
		}else
		{
			ns->phi[z][x][i]+=update;
		}


		update=(gdouble)(mx->b[offset_Je+i]);
		if (clamp==TRUE)
		{
			ns->x[z][x][i]+=update/(1.0+gfabs(update/in->electrical_clamp/(clamp_temp*kb/Qe)));
		}else
		{
			ns->x[z][x][i]+=update;
		}


		update=(gdouble)(mx->b[offset_Jh+i]);
		if (clamp==TRUE)
		{
			ns->xp[z][x][i]+=update/(1.0+gfabs(update/in->electrical_clamp/(clamp_temp*kb/Qe)));
		}else
		{
			ns->xp[z][x][i]+=update;

		}


		if (in->ntrapnewton==TRUE)
		{
			for (band=0;band<dim->srh_bands;band++)
			{
				update=(gdouble)(mx->b[offset_srh_e+dim->ylen*band+i]);
				if (clamp==TRUE)
				{
					ns->xt[z][x][i][band]+=update/(1.0+gfabs(update/in->electrical_clamp/(clamp_temp*kb/Qe)));

				}else
				{
					ns->xt[z][x][i][band]+=update;
				}
			}
		}


		if (in->ptrapnewton==TRUE)
		{
			for (band=0;band<dim->srh_bands;band++)
			{
				update=(gdouble)(mx->b[offset_srh_h+dim->ylen*band+i]);
				if (clamp==TRUE)
				{
					ns->xpt[z][x][i][band]+=update/(1.0+gfabs(update/in->electrical_clamp/(clamp_temp*kb/Qe)));
				}else
				{
					ns->xpt[z][x][i][band]+=update;

				}
			}
		}


	}

	if (contact_left==contact_schottky)
	{
		update=(gdouble)(mx->b[offset_Je-1]);
		//printf(">>update %Le\n",update);
		if (clamp==TRUE)
		{
			ns->x_y0[0][0]+=update/(1.0+gfabs(update/in->electrical_clamp/(clamp_temp*kb/Qe)));
		}else
		{
			ns->x_y0[0][0]+=update;
		}
	}


//update_y_array(sim,in,z,x);

}

