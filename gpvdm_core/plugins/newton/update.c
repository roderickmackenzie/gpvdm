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
int y;
int band=0;
long double Vapplied=0.0;
long double clamp_temp=300.0;
struct newton_state *ns=&(in->ns);
struct dimensions *dim=&in->ns.dim;
struct matrix *mx=&in->mx;
struct perovskite *ion=&(in->mobileion);
struct singlet *sing=&(in->sing);

int Nion_offset=get_offset_nion(in);
int Ns_offset=get_offset_singlet_Ns(in);
int Nt_offset=get_offset_singlet_Nt(in);
int Nsd_offset=get_offset_singlet_Nsd(in);
int Ntd_offset=get_offset_singlet_Ntd(in);
int Nho_offset=get_offset_singlet_Nho(in);


int offset_Je=get_offset_Je(in);
int offset_Jh=get_offset_Jh(in);
int offset_srh_e=get_offset_srh_e(in);
int offset_srh_h=get_offset_srh_h(in);
int offset_kcl=get_offset_kcl(in);

int contact_left=in->contacts[in->n_contact_y0[0][0]].type;

gdouble update=0.0;

	for (y=0;y<dim->ylen;y++)
	{

		update=(gdouble)mx->b[y];
		if (clamp==TRUE)
		{
			ns->phi[z][x][y]+=update/(1.0+gfabs(update/in->electrical_clamp/(clamp_temp*kb/Qe)));
		}else
		{
			ns->phi[z][x][y]+=update;
		}


		update=(gdouble)(mx->b[offset_Je+y]);
		if (clamp==TRUE)
		{
			ns->x[z][x][y]+=update/(1.0+gfabs(update/in->electrical_clamp/(clamp_temp*kb/Qe)));
		}else
		{
			ns->x[z][x][y]+=update;
		}


		update=(gdouble)(mx->b[offset_Jh+y]);
		if (clamp==TRUE)
		{
			ns->xp[z][x][y]+=update/(1.0+gfabs(update/in->electrical_clamp/(clamp_temp*kb/Qe)));
		}else
		{
			ns->xp[z][x][y]+=update;

		}


		if (in->ntrapnewton==TRUE)
		{
			for (band=0;band<dim->srh_bands;band++)
			{
				update=(gdouble)(mx->b[offset_srh_e+dim->ylen*band+y]);
				if (clamp==TRUE)
				{
					ns->xt[z][x][y][band]+=update/(1.0+gfabs(update/in->electrical_clamp/(clamp_temp*kb/Qe)));

				}else
				{
					ns->xt[z][x][y][band]+=update;
				}
			}
		}


		if (in->ptrapnewton==TRUE)
		{
			for (band=0;band<dim->srh_bands;band++)
			{
				update=(gdouble)(mx->b[offset_srh_h+dim->ylen*band+y]);
				if (clamp==TRUE)
				{
					ns->xpt[z][x][y][band]+=update/(1.0+gfabs(update/in->electrical_clamp/(clamp_temp*kb/Qe)));
				}else
				{
					ns->xpt[z][x][y][band]+=update;

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

