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

/** @file newton_update.c
	@brief Update the dependent variables after running the newton solver to calculate new solution parameters.
*/

#include "sim.h"
#include "dump.h"
#include <dos.h>
#include <contacts.h>

void update_y_array(struct simulation *sim,struct device *in,int z,int x)
{
int y=0;
int band=0;
struct newton_state *ns=&(in->ns);
struct dimensions *dim=&in->ns.dim;
struct epitaxy *epi=&(in->my_epitaxy);

long double n=0.0;
long double p=0.0;
long double nt=0.0;
long double pt=0.0;
long double dnt=0.0;
long double dpt=0.0;
long double wn=0.0;
long double wp=0.0;

long double dn=0.0;
long double dp=0.0;
long double srh1=0.0;
long double srh2=0.0;
long double srh3=0.0;
long double srh4=0.0;

long double dsrh1=0.0;
long double dsrh2=0.0;
long double dsrh3=0.0;
long double dsrh4=0.0;
long double phi=0.0;
long double Te=0.0;
long double Th=0.0;
long double ion=0.0;
long double dion=0.0;

struct shape* s;

	for (y=0;y<dim->ylen;y++)
	{
		s=in->obj_zxy[z][x][y]->s;
		phi=ns->phi[z][x][y];
		Te=in->Te[z][x][y];
		Th=in->Th[z][x][y];

		in->Fn[z][x][y]=ns->x[z][x][y]-phi;
		in->Fp[z][x][y]= -ns->xp[z][x][y]-phi;

		in->Ec[z][x][y]= -phi-in->Xi[z][x][y];
		in->Ev[z][x][y]= in->Ec[z][x][y]-in->Eg[z][x][y];
		get_n_den(s,ns->x[z][x][y]+in->t[z][x][y],Te,&n,&dn,&wn);
		get_p_den(s,ns->xp[z][x][y]-in->tp[z][x][y],Th,&p,&dp,&wp);

		in->dn[z][x][y]=dn;
		in->n[z][x][y]=n;
		in->dndphi[z][x][y]=dn;

		in->dp[z][x][y]=dp;
		in->p[z][x][y]=p;
		in->dpdphi[z][x][y]= -dp;

		in->wn[z][x][y]=wn;//get_n_w(in,ns->x[z][x][y]+in->t[z][x][y],in->Te[z][x][y],in->imat[z][x][y]);
		in->wp[z][x][y]=wp;//get_p_w(in,ns->xp[z][x][y]-in->tp[z][x][y],in->Th[z][x][y],in->imat[z][x][y]);

		if (ns->Nion_enabled==TRUE)
		{
			if (in->mobileion.full_newton==TRUE)
			{
				get_ion_den(ns->x_Nion[z][x][y]-in->t_ion[z][x][y],&ion,&dion);
				in->dNion[z][x][y]=dion;
				in->Nion[z][x][y]=ion;
				//printf("%Le %Le\n",ns->x_Nion[z][x][y],ns->xp[z][x][y]);
				//getchar();
				in->dNiondphi[z][x][y]= -dion;
			}
		}

		if (in->ntrapnewton)
		{
			in->nt_all[z][x][y]=0.0;
			for (band=0;band<dim->srh_bands;band++)
			{
				in->Fnt[z][x][y][band]=ns->xt[z][x][y][band]-phi;
				get_n_srh(sim,s,ns->xt[z][x][y][band]+in->tt[z][x][y],Te,band,&nt,&srh1,&srh2,&srh3,&srh4);
				get_dn_srh(sim,s,ns->xt[z][x][y][band]+in->tt[z][x][y],Te,band,&dnt,&dsrh1,&dsrh2,&dsrh3,&dsrh4);

				in->srh_n_r1[z][x][y][band]=srh1;
				in->srh_n_r2[z][x][y][band]=srh2;
				in->srh_n_r3[z][x][y][band]=srh3;
				in->srh_n_r4[z][x][y][band]=srh4;

				in->dsrh_n_r1[z][x][y][band]=dsrh1;
				in->dsrh_n_r2[z][x][y][band]=dsrh2;
				in->dsrh_n_r3[z][x][y][band]=dsrh3;
				in->dsrh_n_r4[z][x][y][band]=dsrh4;

				in->nt[z][x][y][band]=nt;
				in->dnt[z][x][y][band]=dnt;
				in->nt_all[z][x][y]+=nt;

			}
		}

		if (in->ptrapnewton)
		{
			in->pt_all[z][x][y]=0.0;
			for (band=0;band<dim->srh_bands;band++)
			{
				in->Fpt[z][x][y][band]= -ns->xpt[z][x][y][band]-phi;
				get_p_srh(sim,s,ns->xpt[z][x][y][band]-in->tpt[z][x][y],Th,band,&pt,&srh1,&srh2,&srh3,&srh4);
				get_dp_srh(sim,s,ns->xpt[z][x][y][band]-in->tpt[z][x][y],Th,band,&dpt,&dsrh1,&dsrh2,&dsrh3,&dsrh4);

				in->srh_p_r1[z][x][y][band]=srh1;
				in->srh_p_r2[z][x][y][band]=srh2;
				in->srh_p_r3[z][x][y][band]=srh3;
				in->srh_p_r4[z][x][y][band]=srh4;

				in->dsrh_p_r1[z][x][y][band]=dsrh1;
				in->dsrh_p_r2[z][x][y][band]=dsrh2;
				in->dsrh_p_r3[z][x][y][band]=dsrh3;
				in->dsrh_p_r4[z][x][y][band]=dsrh4;

				in->pt[z][x][y][band]=pt;
				in->dpt[z][x][y][band]=dpt;
				in->pt_all[z][x][y]+=pt;
			}
		}

		}

}


void update_material_arrays(struct simulation *sim,struct device *in)
{
int x=0;
int y=0;
int z=0;
long double dy=0.0;
long double pos=0.0;
long double value=0.0;
long double Nad0=0.0;
long double Nad1=0.0;

struct newton_state *ns=&(in->ns);
struct dimensions *dim=&in->ns.dim;
struct heat* thermal=&(in->thermal);
struct epitaxy *epi=&(in->my_epitaxy);
struct shape* s;
	for (z=0;z<dim->zlen;z++)
	{

		for (x=0;x<dim->xlen;x++)
		{

			for (y=0;y<dim->ylen;y++)
			{
				in->Tl[z][x][y]=thermal->Ty0+dim->ymesh[y]*(thermal->Ty1-thermal->Ty0)/in->ylen;
				in->Te[z][x][y]=thermal->Ty0+dim->ymesh[y]*(thermal->Ty1-thermal->Ty0)/in->ylen;
				in->Th[z][x][y]=thermal->Ty0+dim->ymesh[y]*(thermal->Ty1-thermal->Ty0)/in->ylen;
				in->ex[z][x][y]=0.0;
				in->Hex[z][x][y]=0.0;


				s=in->obj_zxy[z][x][y]->s;
				in->epsilonr[z][x][y]=get_dos_epsilonr(s);
				//printf("%d %d %Le %s\n",x,y,in->epsilonr[z][x][y],s->name);
				in->epsilonr_e0[z][x][y]=in->epsilonr[z][x][y]*epsilon0;

				if (in->drift_diffision_simulations_enabled==TRUE)
				{
					in->Eg[z][x][y]=get_dos_Eg(s);

					in->muion[z][x][y]=get_dos_ion_mobility(s);

					//printf("%d %d %d %Lf %d\n",z,x,y,in->Eg[z][x][y],in->imat[z][x][y]);
					//getchar();

					in->B[z][x][y]=get_dos_B(s);

					in->Dex[z][x][y]=0.0;//get_mat_param(&(in->mat.l[in->imat[z][x][y]]),mat_Dex);

					in->Xi[z][x][y]=get_dos_Xi(s);

					in->Ec[z][x][y]= -in->Xi[z][x][y];

					in->Ev[z][x][y]= -in->Xi[z][x][y]-in->Eg[z][x][y];


					in->Nc[z][x][y]=get_Nc_free(s);

					in->Nv[z][x][y]=get_Nv_free(s);

					in->mun_z[z][x][y]=get_n_muz(s);
					in->mun_x[z][x][y]=get_n_mux(s);
					in->mun_y[z][x][y]=get_n_muy(s);

					in->mup_z[z][x][y]=get_p_muz(s);
					in->mup_x[z][x][y]=get_p_mux(s);
					in->mup_y[z][x][y]=get_p_muy(s);

					//printf("%Le %Le\n",in->mun[z][x][y],in->B[z][x][y]);
					//getchar();

					in->kf[z][x][y]=0.0;//get_mat_param(&(in->mat.l[in->imat[z][x][y]]),mat_kf);

					in->t[z][x][y]=in->Xi[z][x][y];
					in->tp[z][x][y]=in->Xi[z][x][y]+in->Eg[z][x][y];
					in->t_ion[z][x][y]=in->Xi[z][x][y]+in->Eg[z][x][y];

					in->tt[z][x][y]=in->Xi[z][x][y];
					in->tpt[z][x][y]=in->Xi[z][x][y]+in->Eg[z][x][y];

				
					dy=epi->layer[in->imat_epitaxy[z][x][y]].width;
					pos=in->ns.dim.ymesh[y]-in->layer_start[in->imat_epitaxy[z][x][y]];

					Nad0=s->dosn.config.Nd0;
					Nad1=s->dosn.config.Nd1;
					in->Nad[z][x][y]=Nad0+(Nad1-Nad0)*(pos/dy);

					Nad0=s->dosn.config.Na0;
					Nad1=s->dosn.config.Na1;
					in->Nad[z][x][y]-=Nad0+(Nad1-Nad0)*(pos/dy);

					in->Nad[z][x][y]-=s->dosn.config.ion_density;
					 //printf("%Le\n",in->Nad[z][x][y]);
				}
			}
			//getchar();
		}
	}


	contacts_ingress(sim,in);
}

