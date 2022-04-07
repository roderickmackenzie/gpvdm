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

/** @file newton_update.c
	@brief Update the dependent variables after running the newton solver to calculate new solution parameters.
*/

#include "sim.h"
#include "dump.h"
#include <dos.h>
#include <contacts.h>
#include <optical_mode_fun.h>

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

long double Ns;
long double Nt;
long double Nsd;
long double Ntd;
long double Nho;

long double dNs;
long double dNt;
long double dNsd;
long double dNtd;
long double dNho;

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
				in->Tl[z][x][y]=thermal->Ty0+dim->y[y]*(thermal->Ty1-thermal->Ty0)/in->ylen;
				in->Te[z][x][y]=thermal->Ty0+dim->y[y]*(thermal->Ty1-thermal->Ty0)/in->ylen;
				in->Th[z][x][y]=thermal->Ty0+dim->y[y]*(thermal->Ty1-thermal->Ty0)/in->ylen;


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

					in->Xi[z][x][y]=get_dos_Xi(s);

					in->Ec[z][x][y]= -in->Xi[z][x][y];

					in->Ev[z][x][y]= -in->Xi[z][x][y]-in->Eg[z][x][y];


					in->Nc[z][x][y]=get_Nc_free(s);

					in->Nv[z][x][y]=get_Nv_free(s);

					//mobility
					in->mun_z[z][x][y]=get_n_muz(s);
					in->mun_x[z][x][y]=get_n_mux(s);
					in->mun_y[z][x][y]=get_n_muy(s);

					in->mup_z[z][x][y]=get_p_muz(s);
					in->mup_x[z][x][y]=get_p_mux(s);
					in->mup_y[z][x][y]=get_p_muy(s);

					//Auger
					in->Cn[z][x][y]=get_Cn(s);
					in->Cp[z][x][y]=get_Cp(s);

					//SS SRH
					in->n1[z][x][y]=get_ss_srh_n1(s);
					in->p1[z][x][y]=get_ss_srh_p1(s);
					in->tau_n[z][x][y]=get_ss_srh_tau_n(s);
					in->tau_p[z][x][y]=get_ss_srh_tau_p(s);

					//printf("%Le %Le\n",in->mun[z][x][y],in->B[z][x][y]);
					//getchar();

					in->t[z][x][y]=in->Xi[z][x][y];
					in->tp[z][x][y]=in->Xi[z][x][y]+in->Eg[z][x][y];
					in->t_ion[z][x][y]=in->Xi[z][x][y]+in->Eg[z][x][y];

					in->tt[z][x][y]=in->Xi[z][x][y];
					in->tpt[z][x][y]=in->Xi[z][x][y]+in->Eg[z][x][y];

				
					dy=epi->layer[in->imat_epitaxy[z][x][y]].width;
					pos=in->ns.dim.y[y]-in->layer_start[in->imat_epitaxy[z][x][y]];

					Nad0=s->dosn.config.Nd0;
					Nad1=s->dosn.config.Nd1;
					in->Nad[z][x][y]=Nad0+(Nad1-Nad0)*(pos/dy);
					//double k=0.1;
					//long double mul=1.0;
					//mul=1.0/(1.0+exp(-2.0*k*(in->Te[z][x][y]-265)));


					//mul=0.0;
					Nad0=s->dosn.config.Na0;//*mul;
					Nad1=s->dosn.config.Na1;//*mul;



					in->Nad[z][x][y]-=Nad0+(Nad1-Nad0)*(pos/dy);
					//printf("%Le %Le\n",in->Te[z][x][y],mul);
					if (ns->Nion_enabled==TRUE)
					{
						in->Nad[z][x][y]-=s->dosn.config.ion_density;
					}
					 //printf("%Le\n",in->Nad[z][x][y]);
				}
			}
			//getchar();
		}
	}


	contacts_ingress(sim,in);
}

