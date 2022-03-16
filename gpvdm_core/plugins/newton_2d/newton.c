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
	@brief Full 2D newton solver.
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
#include <sys/time.h>
#include <timer.h>
#include <lock.h>



void update_solver_vars_2d(struct simulation *sim,struct device *in,int z,int x_in, long double electrical_clamp)
{
struct newton_state *ns=&(in->ns);
struct dimensions *dim=&in->ns.dim;
struct matrix_solver_memory *msm=&(in->msm);
struct matrix *mx=&in->mx;
int y=0;
int x=0;
int x_max=0;
int band=0;
long double Vapplied=0.0;
long double clamp_temp=300.0;
long double update=0.0;
int clamp=TRUE;

	x=0;

	if (x_in==-1)
	{
		x=0;
		x_max=dim->xlen;
	}else
	{
		x=x_in;
		x_max=x_in;
	}


	do
	{
		int shift=msm->x_matrix_offset*x;

		for (y=0;y<dim->ylen;y++)
		{

			update=(gdouble)mx->b[shift+y];

			if (clamp==TRUE)
			{
				ns->phi[z][x][y]+=update/(1.0+gfabs(update/electrical_clamp/(clamp_temp*kb/Qe)));
			}else
			{
				ns->phi[z][x][y]+=update;

			}



			update=(gdouble)(mx->b[shift+dim->ylen*(1)+y]);
			if (clamp==TRUE)
			{
				ns->x[z][x][y]+=update/(1.0+gfabs(update/electrical_clamp/(clamp_temp*kb/Qe)));
			}else
			{
				ns->x[z][x][y]+=update;
			}


			update=(gdouble)(mx->b[shift+dim->ylen*(1+1)+y]);
			if (clamp==TRUE)
			{
				ns->xp[z][x][y]+=update/(1.0+gfabs(update/electrical_clamp/(clamp_temp*kb/Qe)));
			}else
			{
				ns->xp[z][x][y]+=update;

			}



		}

		//update_y_array(sim,in,z,x);

		x++;

	}while(x<x_max);


}

void fill_matrix(struct simulation *sim,struct device *in,int z,int x_in)
{
//gdouble offset= -0.5;
int band=0;
int x=0;
int dim_=0;
int x_max=0;
struct newton_state *ns=&(in->ns);
struct dimensions *dim=&in->ns.dim;
struct matrix *mx=&in->mx;
struct heat *thermal=&(in->thermal);
struct epitaxy *epi=&(in->my_epitaxy);
struct matrix_solver_memory *msm=&(in->msm);

if (x_in==-1)
{
	for (x=0;x<dim->xlen;x++)
	{
		update_y_array(sim,in,z,x);
	}
	x=0;
	x_max=dim->xlen;
	dim_=2;
}else
{
	update_y_array(sim,in,z,x);
	x=x_in;
	x_max=x_in;
	dim_=1;
}

//FILE *file_j =fopen("myj.dat","w");
//getchar();
gdouble Jnl=0.0;
gdouble Jnr=0.0;
gdouble Jpl=0.0;
gdouble Jpr=0.0;

gdouble Dnl=0.0;
gdouble Dnc=0.0;
gdouble Dnr=0.0;
gdouble Dpl=0.0;
gdouble Dpc=0.0;
gdouble Dpr=0.0;

gdouble nl=0.0;
gdouble nc=0.0;
gdouble nr=0.0;

gdouble pl=0.0;
gdouble pc=0.0;
gdouble pr=0.0;

gdouble xil=0.0;
gdouble xir=0.0;
gdouble xipl=0.0;
gdouble xipr=0.0;

gdouble dJpdxipl=0.0;
gdouble dJpdxipc=0.0;
gdouble dJpdxipr=0.0;

gdouble dnl=0.0;
gdouble dnc=0.0;
gdouble dnr=0.0;

gdouble dpl=0.0;
gdouble dpc=0.0;
gdouble dpr=0.0;

gdouble munl=0.0;
gdouble munc=0.0;
gdouble munr=0.0;

gdouble mupl=0.0;
gdouble mupc=0.0;
gdouble mupr=0.0;


gdouble wnl=0.0;
gdouble wnc=0.0;
gdouble wnr=0.0;

gdouble wpl=0.0;
gdouble wpc=0.0;
gdouble wpr=0.0;

gdouble dJdxil=0.0;
gdouble dJdxic=0.0;
gdouble dJdxir=0.0;

gdouble dJdphil=0.0;
gdouble dJdphic=0.0;
gdouble dJdphir=0.0;

gdouble dJpdphil=0.0;
gdouble dJpdphic=0.0;
gdouble dJpdphir=0.0;


gdouble dphidxic=0.0;
gdouble dphidxipc=0.0;

gdouble phil;
gdouble phic;
gdouble phir;
gdouble yl;
gdouble yc;
gdouble yr;
gdouble dyl;
gdouble dyr;
gdouble ddh=0.0;
//gdouble dh;
int pos=0;

gdouble Ecl=0.0;
gdouble Ecr=0.0;
gdouble Ecc=0.0;
gdouble Evl=0.0;
gdouble Evr=0.0;
gdouble Evc=0.0;

gdouble Tel=0.0;
//gdouble Tec=0.0;
gdouble Ter=0.0;

gdouble Thl=0.0;
//gdouble Thc=0.0;
gdouble Thr=0.0;

gdouble xnr;
gdouble tnr;
gdouble xnl;
gdouble tnl;

gdouble xpr;
gdouble tpr;
gdouble xpl;
gdouble tpl;


//gdouble exl;
//gdouble exr;
//gdouble exc;
//gdouble Dexl;
//gdouble Dexc;
//gdouble Dexr;
//gdouble R;

gdouble epr;
gdouble epc;
gdouble epl;

//gdouble G;
gdouble Gn;
gdouble Gp;
int y;
gdouble dJdxipc=0.0;
gdouble dJpdxic=0.0;

gdouble e0=0.0;
gdouble e1=0.0;

gdouble dphil=0.0;
gdouble dphic=0.0;
gdouble dphir=0.0;
gdouble deriv=0.0;

gdouble nlast=0.0;
gdouble plast=0.0;
gdouble dt=0.0;

//gdouble kll=0.0;
//gdouble klc=0.0;
//gdouble klr=0.0;

//gdouble kl0=0.0;
//gdouble kl1=0.0;

gdouble one=0.0;
gdouble one0_l=0.0;
gdouble one0_r=0.0;



gdouble Rtrapn=0.0;
gdouble Rtrapp=0.0;



gdouble Bfree=0.0;
gdouble nceq=0.0;
gdouble pceq=0.0;
gdouble Rfree=0.0;

gdouble nc0_l=0.0;
//gdouble dnc0_l=0.0;
//gdouble pc0_l=0.0;
//gdouble dpc0_l=0.0;

gdouble nc0_r=0.0;
//gdouble dnc0_r=0.0;
//gdouble pc0_r=0.0;
//gdouble dpc0_r=0.0;

gdouble dJnldxil_l=0.0;
gdouble dJnldxil_c=0.0;
gdouble dJnrdxir_c=0.0;
gdouble dJnrdxir_r=0.0;
gdouble dJpldxipl_l=0.0;
gdouble dJpldxipl_c=0.0;
gdouble dJprdxipr_c=0.0;
gdouble dJprdxipr_r=0.0;

gdouble i0=0.0;
gdouble didv=0.0;  //not a function
gdouble diphic=0.0; //could be a function
gdouble didxic=0.0;
gdouble didxipc=0.0;
gdouble didphir=0.0;
gdouble didxir=0.0;
gdouble didxipr=0.0;
gdouble Nad=0.0;

gdouble phi_x_l=0.0;
gdouble phi_x_r=0.0;

long double xc;
long double dxl;
long double dxr;
long double ddhx;
long double dxlh;
long double dxrh;

long double Ter_x=0.0;
long double Thr_x=0.0;

long double Ecr_x=0.0;
long double Evr_x=0.0;

long double nr_x=0.0;
long double dnr_x=0.0;

long double wnr_x=0.0;
long double wpr_x=0.0;

long double pr_x=0.0;
long double dpr_x=0.0;


long double epr_x=0.0;

long double	Tel_x=0.0;
long double	Thl_x=0.0;
long double	Ecl_x=0.0;
long double	Evl_x=0.0;


long double	nl_x=0.0;
long double	dnl_x=0.0;


long double	wnl_x=0.0;
long double	wpl_x=0.0;

long double	pl_x=0.0;
long double	dpl_x=0.0;

long double	munl_x=0.0;
long double	mupl_x=0.0;

long double	munc_x=0.0;
long double	mupc_x=0.0;

long double munr_x=0.0;
long double mupr_x=0.0;

long double	epl_x=0.0;

long double dphil_x=0.0;
long double dphic_x=0.0;
long double dphir_x=0.0;

long double e0_x=0.0;
long double e1_x=0.0;

long double phil_x=0.0;
long double phir_x=0.0;

long double xl=0.0;
long double xr=0.0;

long double Dnl_x=0.0;
long double Dpl_x=0.0;

long double Dnc_x=0.0;
long double Dpc_x=0.0;

long double Dnr_x=0.0;
long double Dpr_x=0.0;

long double xil_x=0.0;
long double xir_x=0.0;

long double xpil_x=0.0;
long double xpir_x=0.0;

long double xipl_x=0.0;
long double xipr_x=0.0;

long double Jnl_x=0.0;
long double Jnr_x=0.0;

long double Jpl_x=0.0;
long double Jpr_x=0.0;

long double dJnldxil_l_x=0.0;
long double dJnldxil_c_x=0.0;
long double dJnrdxir_c_x=0.0;
long double dJnrdxir_r_x=0.0;

long double dJdxil_x=0.0;
long double dJdxic_x=0.0;
long double dJdxir_x=0.0;

long double dJpldxipl_l_x=0.0;
long double dJpldxipl_c_x=0.0;

long double dJprdxipr_c_x=0.0;
long double dJprdxipr_r_x=0.0;

long double dJpdxipl_x=0.0;
long double dJpdxipc_x=0.0;
long double dJpdxipr_x=0.0;

long double dJdphil_x=0.0;
long double dJdphic_x=0.0;
long double dJdphir_x=0.0;

long double dJpdphil_x=0.0;
long double dJpdphic_x=0.0;
long double dJpdphir_x=0.0;

long double dphil_d=0.0;
long double dphic_d=0.0;
long double dphir_d=0.0;

long double dphil_d_x=0.0;
long double dphir_d_x=0.0;
//gdouble dylh_left=0.0;
//gdouble dyrh_left=0.0;
//gdouble dncdphic=0.0;
//gdouble dpcdphic=0.0;

long double dBn=0.0;
long double dBp=0.0;
long double Bn=0.0;
long double Bp=0.0;

int contact_left=0;//=in->contacts[in->n_contact_y0[z][x]].type;

long double vl_e=-1.0;
long double vl_h=-1.0;
struct shape *s;

//printf("%Le\n",in->Gn[0][0][0]);

pos=0;
do	//x
{

	if (dim_==2)
	{
		if (x==0)
		{
			xl=dim->x[0]-(dim->x[1]-dim->x[0]);
		}else
		{
			xl=dim->x[x-1];
		}

		if (x==(dim->xlen-1))
		{
			xr=dim->x[x]+(dim->x[x]-dim->x[x-1]);
		}else
		{
			xr=dim->x[x+1];

		}

		xc=dim->x[x];
		dxl=xc-xl;
		dxr=xr-xc;
		ddhx=(dxl+dxr)/2.0;
		dxlh=dxl/2.0;
		dxrh=dxr/2.0;
	}

	contact_left=in->contacts[in->n_contact_y0[z][x]].type;
	for (y=0;y<dim->ylen;y++)
	{
		s=in->obj_zxy[z][x][y]->s;
		if (y==0)
		{
			//exl=0.0;
			//Dexl=in->Dex[0];
			//in->Vl[z][x]=0.0;
			phil=in->V_y0[z][x]+in->Vapplied_y0[z][x];

			//if ((dim_==2)&&(in->passivate_y0[z][x]==INTERPOLATE))
			//{
			//	phil=in->Vapplied_y0[z][x]*0.1;
			//}

			//printf("%d %Le %Le %d %d\n",x,in->Vapplied_y0[z][x],in->V_y0[z][x],in->passivate_y0[z][x],in->n_contact_y0[z][x]);
			//getchar();
			yl=dim->y[0]-(dim->y[1]-dim->y[0]);
			//Tll=thermal->Ty0;
			Tel=thermal->Ty0;
			Thl=thermal->Ty0;

			Ecl= -in->Xi[z][x][0]-phil;
			Evl= -in->Xi[z][x][0]-phil-in->Eg[z][x][0];
			epl=in->epsilonr_e0[z][x][0];//*epsilon0;

			xnl=in->V_y0[z][x]+in->Fi[z][x][0];
			tnl=in->Xi[z][x][0];
			one=xnl+tnl;

			vl_e=in->contacts[in->n_contact_y0[z][x]].ve0;
			get_n_den(s,one,Tel,&nl,&dnl,&wnl);
			//dnl=get_dn_den(in,one,Tel,in->imat[z][x][y]);
			//wnl=get_n_w(in,one,Tel,in->imat[z][x][y]);

			munl=in->mun_y[z][x][0];


			xpl= -(in->V_y0[z][x]+in->Fi[z][x][0]);
			tpl=(in->Xi[z][x][0]+in->Eg[z][x][0]);
			one=xpl-tpl;

			vl_h=in->contacts[in->n_contact_y0[z][x]].vh0;
			get_p_den(s,one,Thl,&pl,&dpl,&wpl);
			//dpl=get_dp_den(in,one,Thl,in->imat[z][x][y]);
			//wpl=get_p_w(in,one,Thl,in->imat[z][x][y]);


			mupl=in->mup_y[z][x][0];

			if ((dim_==2)&&(in->passivate_y0[z][x]==INTERPOLATE))
			{
				mupl=0.0;
				munl=0.0;
				//yl=-500e-9;
				//epl=1e5*epsilon0;
				//in->epsilonr_e0[z][x][0]=epl;
			}


			//printf("%d %d\n",x, in->passivate_y0[z][x]);
			//getchar();

			if ((dim_==2)&&((in->passivate_y0[z][x]==NEUMANN)||(in->passivate_y0[z][x]==INTERPOLATE2)))
			{
				phil=ns->phi[z][x][y];
				Tel=in->Te[z][x][y];
				Thl=in->Th[z][x][y];
				Ecl=in->Ec[z][x][y];
				Evl=in->Ev[z][x][y];

				nl=in->n[z][x][y];
				dnl=in->dn[z][x][y];

				wnl=in->wn[z][x][y];
				wpl=in->wp[z][x][y];

				pl=in->p[z][x][y];
				dpl=in->dp[z][x][y];
				munl=in->mun_y[z][x][y];
				mupl=in->mup_y[z][x][y];


				epl=in->epsilonr_e0[z][x][y];//*epsilon0;

				in->Vapplied_y0[z][x]=0.0;

				if (in->passivate_y0[z][x]==INTERPOLATE2)
				{
					phil=0.0;
				}

				in->V_y0[z][x]=phil;
				mupl=0.0;
				munl=0.0;
			}
			//kll=in->kl[y];
			//printf("%d %d %Le\n",x,in->passivate_y0[z][x],in->V_y0[z][x]);
			//getchar();

		}else
		{
			//Dexl=in->Dex[y-1];
			//exl=in->ex[z][x][y-1];
			phil=ns->phi[z][x][y-1];
			yl=dim->y[y-1];
			//Tll=in->Tl[z][x][y-1];
			Tel=in->Te[z][x][y-1];
			Thl=in->Th[z][x][y-1];
			Ecl=in->Ec[z][x][y-1];
			Evl=in->Ev[z][x][y-1];


			nl=in->n[z][x][y-1];
			dnl=in->dn[z][x][y-1];


			wnl=in->wn[z][x][y-1];
			wpl=in->wp[z][x][y-1];

			pl=in->p[z][x][y-1];
			dpl=in->dp[z][x][y-1];
			munl=in->mun_y[z][x][y-1];
			mupl=in->mup_y[z][x][y-1];


			epl=in->epsilonr_e0[z][x][y-1];//*epsilon0;


			//kll=in->kl[y-1];
		}

		Ecc=(-in->Xi[z][x][y]-ns->phi[z][x][y]);
		Evc=(-in->Xi[z][x][y]-ns->phi[z][x][y]-in->Eg[z][x][y]);

		if (y==(dim->ylen-1))
		{

			//Dexr=in->Dex[y];
			//exr=0.0;
			//phir=in->V_y1[z][x];


			phir=(in->V_y1[z][x]+in->Vapplied_y1[z][x]);

			yr=dim->y[y]+(dim->y[y]-dim->y[y-1]);
			//Tlr=thermal->Ty1;
			Ter=thermal->Ty1;
			Thr=thermal->Ty1;


			Ecr= -in->Xi[z][x][y]-phir;
			Evr= -in->Xi[z][x][y]-phir-in->Eg[z][x][y];


			xnr=(in->V_y1[z][x]+in->Fi[z][x][y]);
			tnr=(in->Xi[z][x][y]);

			one=xnr+tnr;

			get_n_den(s,one,Ter,&nr,&dnr,&wnr);
			//get_dn_den(in,one,Ter,in->imat[z][x][y]);
			//wnr=get_n_w(in,one,Ter,in->imat[z][x][y]);

			xpr= -(in->V_y1[z][x]+in->Fi[z][x][y]);
			tpr=(in->Xi[z][x][y]+in->Eg[z][x][y]);

			one=xpr-tpr;

			get_p_den(s,one,Thr,&pr,&dpr,&wpr);
			//dpr=get_dp_den(in,one,Thr,in->imat[z][x][y]);
			//wpr=get_p_w(in,one,Thr,in->imat[z][x][y]);

			//munr=1e-30;//in->mun_y[z][x][y];
			//mupr=1e-30;//in->mup_y[z][x][y];

			epr=in->epsilonr_e0[z][x][y];//*epsilon0;
			//klr=in->kl[y];

			//printf("%d %d %Le\n",x,in->passivate_y1[z][x],in->Vapplied_y1[z][x]);
			//getchar();

			//New***
			if ((dim_==2)&&(in->passivate_y1[z][x]==TRUE))
			{
				phir=ns->phi[z][x][y];
				Ter=in->Te[z][x][y];
				Thr=in->Th[z][x][y];
				Ecr=in->Ec[z][x][y];
				Evr=in->Ev[z][x][y];

				nr=in->n[z][x][y];
				dnr=in->dn[z][x][y];

				wnr=in->wn[z][x][y];
				wpr=in->wp[z][x][y];

				pr=in->p[z][x][y];
				dpr=in->dp[z][x][y];
				munr=in->mun_y[z][x][y];
				mupr=in->mup_y[z][x][y];


				epr=in->epsilonr_e0[z][x][y];//*epsilon0;

				in->Vapplied_y1[z][x]=phir;
			}

			//mupr=0.0;
			//munr=0.0;

		}else
		{

			//Dexr=in->Dex[z][x][y+1];
			//exr=in->ex[z][x][y+1];
			phir=ns->phi[z][x][y+1];
			yr=dim->y[y+1];
			//Tlr=in->Tl[z][x][y+1];
			Ter=in->Te[z][x][y+1];
			Thr=in->Th[z][x][y+1];

			Ecr=in->Ec[z][x][y+1];
			Evr=in->Ev[z][x][y+1];


			nr=in->n[z][x][y+1];
			dnr=in->dn[z][x][y+1];

			wnr=in->wn[z][x][y+1];
			wpr=in->wp[z][x][y+1];

			pr=in->p[z][x][y+1];
			dpr=in->dp[z][x][y+1];
			munr=in->mun_y[z][x][y+1];
			mupr=in->mup_y[z][x][y+1];

			epr=in->epsilonr_e0[z][x][y+1];//*epsilon0;
			//klr=in->kl[y+1];

		}

		if (is_errors(sim)==0)
		{

		}

		if (dim_==2)
		{
			if (x==0)
			{
				//xl=dim->x[0]-(dim->x[1]-dim->x[0]);
				Tel_x=in->Te[z][0][y];
				Thl_x=in->Th[z][0][y];

				munl_x=in->mun_x[z][0][y];
				mupl_x=in->mup_x[z][0][y];

				epl_x=in->epsilonr_e0[z][0][y];//*epsilon0;

				//if (in->passivate_x0[z][y]==FALSE)
				//{
					phil_x=in->V_x0[z][y]+in->Vapplied_x0[z][y];

					Ecl_x= -in->Xi[z][0][y]-phil_x;
					Evl_x= -in->Xi[z][0][y]-phil_x-in->Eg[z][0][y];

					xnl=in->V_x0[z][y]+in->Fi[z][0][y];
					tnl=in->Xi[z][0][y];
					one=xnl+tnl;

					get_n_den(s,one,Tel_x,&nl_x,&dnl_x,&wnl_x);
					//=get_dn_den(in,one,Tel_x,in->imat[z][x][y]);
					//wnl_x=get_n_w(in,one,Tel_x,in->imat[z][x][y]);

					xpl= -(in->V_x0[z][y]+in->Fi[z][0][y]);
					tpl=(in->Xi[z][x][0]+in->Eg[z][0][y]);
					one=xpl-tpl;

					get_p_den(s,one,Thl_x,&pl_x,&dpl_x,&wpl_x);
					//dpl_x=get_dp_den(in,one,Thl_x,in->imat[z][x][y]);
					//wpl_x=get_p_w(in,one,Thl_x,in->imat[z][x][y]);

					mupl_x=0.0;
					munl_x=0.0;
				/*}else
				{
					phil_x=ns->phi[z][x][y];

					Ecl_x=in->Ec[z][x][y];
					Evl_x=in->Ev[z][x][y];

					nl_x=in->n[z][x][y];
					dnl_x=in->dn[z][x][y];
					wnl_x=in->wn[z][x][y];

					pl_x=in->p[z][x][y];
					dpl_x=in->dp[z][x][y];
					wpl_x=in->wp[z][x][y];

				}*/

				//printf("%d %d %Le\n",y,in->passivate_x0[z][y],in->V_x0[z][y]);
				//getchar();
				/////////////////////////


			}else
			{
				//xl=dim->x[x-1];
				phil_x=ns->phi[z][x-1][y];

				Tel_x=in->Te[z][x-1][y];
				Thl_x=in->Th[z][x-1][y];
				Ecl_x=in->Ec[z][x-1][y];
				Evl_x=in->Ev[z][x-1][y];


				nl_x=in->n[z][x-1][y];
				dnl_x=in->dn[z][x-1][y];


				wnl_x=in->wn[z][x-1][y];
				wpl_x=in->wp[z][x-1][y];

				pl_x=in->p[z][x-1][y];
				dpl_x=in->dp[z][x-1][y];
				munl_x=in->mun_x[z][x-1][y];
				mupl_x=in->mup_x[z][x-1][y];


				epl_x=in->epsilonr_e0[z][x-1][y];//*epsilon0;

			}

			if (x==(dim->xlen-1))
			{

				/////////////////////////////
				//xr=dim->x[x]+(dim->x[x]-dim->x[x-1]);
				epr_x=in->epsilonr_e0[z][x][y];//*epsilon0;

				Ter_x=in->Te[z][x][y];
				Thr_x=in->Th[z][x][y];

				munr_x=in->mun_x[z][x][y];
				mupr_x=in->mup_x[z][x][y];

				//if (in->passivate_x1[z][y]==FALSE)
				//{
					phir_x=(in->V_x1[z][y]+in->Vapplied_x1[z][y]);

					Ecr_x= -in->Xi[z][x][y]-phir_x;
					Evr_x= -in->Xi[z][x][y]-phir_x-in->Eg[z][x][y];

					xnr=(in->V_x1[z][y]+in->Fi[z][x][y]);
					tnr=(in->Xi[z][x][y]);

					one=xnr+tnr;

					get_n_den(s,one,Ter_x,&nr_x,&dnr_x,&wnr_x);
					//dnr_x=get_dn_den(in,one,Ter_x,in->imat[z][x][y]);
					//wnr_x=get_n_w(in,one,Ter_x,in->imat[z][x][y]);

					xpr= -(in->V_x1[z][y]+in->Fi[z][x][y]);
					tpr=(in->Xi[z][x][y]+in->Eg[z][x][y]);

					one=xpr-tpr;

					get_p_den(s,one,Thr_x,&pr_x,&dpr_x,&wpr_x);
					//dpr_x=get_dp_den(in,one,Thr_x,in->imat[z][x][y]);
					//wpr_x=get_p_w(in,one,Thr_x,in->imat[z][x][y]);

					mupr_x=0.0;
					munr_x=0.0;
				/*}else
				{
					phir_x=ns->phi[z][x][y];

					Ecr_x=in->Ec[z][x][y];
					Evr_x=in->Ev[z][x][y];


					nr_x=in->n[z][x][y];
					dnr_x=in->dn[z][x][y];
					wnr_x=in->wn[z][x][y];

					pr_x=in->p[z][x][y];
					dpr_x=in->dp[z][x][y];
					wpr_x=in->wp[z][x][y];

				}*/



			}else
			{
				//xr=dim->x[x+1];
				phir_x=ns->phi[z][x+1][y];

				Ter_x=in->Te[z][x+1][y];
				Thr_x=in->Th[z][x+1][y];

				Ecr_x=in->Ec[z][x+1][y];
				Evr_x=in->Ev[z][x+1][y];


				nr_x=in->n[z][x+1][y];
				dnr_x=in->dn[z][x+1][y];

				wnr_x=in->wn[z][x+1][y];
				wpr_x=in->wp[z][x+1][y];

				pr_x=in->p[z][x+1][y];
				dpr_x=in->dp[z][x+1][y];
				munr_x=in->mun_x[z][x+1][y];
				mupr_x=in->mup_x[z][x+1][y];

				epr_x=in->epsilonr_e0[z][x+1][y];//*epsilon0;
			}
		}

		dJdxipc=0.0;
		dJpdxic=0.0;

		epc=in->epsilonr_e0[z][x][y];//*epsilon0;


		//exc=in->ex[z][x][y];
		//Dexc=in->Dex[z][x][y];
		yc=dim->y[y];
		dyl=yc-yl;
		dyr=yr-yc;

		ddh=(dyl+dyr)/2.0;
		gdouble dylh=dyl/2.0;
		gdouble dyrh=dyr/2.0;


		//dh=(dyl+dyr);
		phic=ns->phi[z][x][y];
		//Tlc=in->Tl[z][x][y];
		//Tec=in->Te[z][x][y];
		//Thc=in->Th[z][x][y];

		munc=in->mun_y[z][x][y];
		mupc=in->mup_y[z][x][y];


		wnc=in->wn[z][x][y];
		wpc=in->wp[z][x][y];

		//y
		Dnl=munl*(2.0/3.0)*wnl/Qe;
		Dpl=mupl*(2.0/3.0)*wpl/Qe;

		Dnc=munc*(2.0/3.0)*wnc/Qe;
		Dpc=mupc*(2.0/3.0)*wpc/Qe;
		in->Dn[z][x][y]=Dnc;
		in->Dp[z][x][y]=Dnc;

		Dnr=munr*(2.0/3.0)*wnr/Qe;
		Dpr=mupr*(2.0/3.0)*wpr/Qe;

		if (Dnl!=0.0)
		{
			Dnl=(Dnl+Dnc)/2.0;
		}

		if (Dnr!=0.0)
		{
			Dnr=(Dnr+Dnc)/2.0;
		}

		if (Dpl!=0.0)
		{
			Dpl=(Dpl+Dpc)/2.0;
		}

		if (Dpr!=0.0)
		{
			Dpr=(Dpr+Dpc)/2.0;
		}

		if (munl!=0.0)
		{
			munl=(munl+munc)/2.0;
		}

		if (munr!=0.0)
		{
			munr=(munr+munc)/2.0;
		}

		if (mupl!=0.0)
		{
			mupl=(mupl+mupc)/2.0;
		}

		if (mupr!=0.0)
		{
			mupr=(mupr+mupc)/2.0;
		}

		e0=(epl+epc)/2.0;
		e1=(epc+epr)/2.0;

		//x
		if (dim_==2)
		{
			munc_x=in->mun_x[z][x][y];
			mupc_x=in->mup_x[z][x][y];

			Dnl_x=munl_x*(2.0/3.0)*wnl_x/Qe;
			Dpl_x=mupl_x*(2.0/3.0)*wpl_x/Qe;

			Dnc_x=munc_x*(2.0/3.0)*wnc/Qe;
			Dpc_x=mupc_x*(2.0/3.0)*wpc/Qe;

			Dnr_x=munr_x*(2.0/3.0)*wnr_x/Qe;
			Dpr_x=mupr_x*(2.0/3.0)*wpr_x/Qe;
			//printf("a %d %d %Le %Le %Le %Le\n",x,y,mupr_x,wpr_x,in->mup_x[z][x+1][y],in->mup_y[z][x+1][y]);
			/*Dnl_x=(Dnl_x+Dnc_x)/2.0;
			Dnr_x=(Dnr_x+Dnc_x)/2.0;

			Dpl_x=(Dpl_x+Dpc_x)/2.0;
			Dpr_x=(Dpr_x+Dpc_x)/2.0;

			munl_x=(munl_x+munc_x)/2.0;
			munr_x=(munr_x+munc_x)/2.0;

			mupl_x=(mupl_x+mupc_x)/2.0;
			mupr_x=(mupr_x+mupc_x)/2.0;*/

			if (Dnl_x!=0.0)
			{
				Dnl_x=(Dnl_x+Dnc_x)/2.0;
			}

			if (Dnr_x!=0.0)
			{
				Dnr_x=(Dnr_x+Dnc_x)/2.0;
			}

			if (Dpl_x!=0.0)
			{
				Dpl_x=(Dpl_x+Dpc_x)/2.0;
			}

			if (Dpr_x!=0.0)
			{
				Dpr_x=(Dpr_x+Dpc_x)/2.0;
			}

			if (munl_x!=0.0)
			{
				munl_x=(munl_x+munc_x)/2.0;
			}

			if (munr_x!=0.0)
			{
				munr_x=(munr_x+munc_x)/2.0;
			}

			if (mupl_x!=0.0)
			{
				mupl_x=(mupl_x+mupc_x)/2.0;
			}

			if (mupr_x!=0.0)
			{
				mupr_x=(mupr_x+mupc_x)/2.0;
			}

			e0_x=(epl_x+epc)/2.0;
			e1_x=(epc+epr_x)/2.0;
		}

		//printf("%Le %Le\n",munc_x,munc);
		//getchar();
		nc=in->n[z][x][y];
		pc=in->p[z][x][y];

		dnc=in->dn[z][x][y];
		dpc=in->dp[z][x][y];
		//dncdphic=in->dndphi[z][x][y];
		//dpcdphic=in->dpdphi[z][x][y];

		Bfree=in->B[z][x][y];
		Nad=in->Nad[z][x][y];
		nceq=in->nfequlib[z][x][y];
		pceq=in->pfequlib[z][x][y];
		Rfree=Bfree*(nc*pc-nceq*pceq);
		in->Rfree[z][x][y]=Rfree;

		//klc=in->kl[y];
		nlast=in->nlast[z][x][y];
		plast=in->plast[z][x][y];

		for (band=0;band<dim->srh_bands;band++)
		{
			in->newton_ntlast[band]=in->ntlast[z][x][y][band];
			in->newton_ptlast[band]=in->ptlast[z][x][y][band];
		}

		dt=in->dt;

		Gn=in->Gn[z][x][y];
		Gp=in->Gp[z][x][y];


		//kl0=(klc+kll)/2.0;
		//kl1=(klr+klc)/2.0;

		dphil= -e0/dyl/ddh;
		dphic= e0/dyl/ddh+e1/dyr/ddh;
		dphir= -e1/dyr/ddh;

		dphil_d=dphil;
		dphic_d=dphic;
		dphir_d=dphir;

		deriv=phil*dphil+phic*dphic+phir*dphir;		//I think there is an error here by double counting the deriv for dphic come back and look on non work day. (7/2/17)

		if (dim_==2)
		{
			dphil_x= -e0_x/dxl/ddhx;
			dphic_x= e0_x/dxl/ddhx+e1_x/dxr/ddhx;
			dphir_x= -e1_x/dxr/ddhx;

			dphil_d_x= dphil_x;
			dphic_d += dphic_x;
			dphir_d_x= dphir_x;

			if (x==0)
			{
				//printf("%d\n",in->passivate_x0[z][y]);
				/*if (in->passivate_x0[z][y]==TRUE)
				{
					dphic_d+=dphil_d_x;
				}*/
			}

			if (y==0)
			{
				//printf("%d\n",in->passivate_y0[z][x]);
				if (in->passivate_y0[z][x]==TRUE)
				{
					dphic_d+=dphil;
				}
			}

			//new***
			if (y==(dim->ylen-1))
			{
				if (in->passivate_y1[z][x]==TRUE)
				{
					dphic_d+=dphir;
				}
			}

			if (x==(dim->xlen-1))
			{
				/*if (in->passivate_x1[z][y]==TRUE)
				{
					dphic_d+=dphir_d_x;
				}*/
			}

		}



		if (dim_==2)
		{
			deriv+=phil_x*dphil_x+phic*dphic_x+phir_x*dphir_x;
		}

		dphidxic=Qe*(dnc);
		dphidxipc= -Qe*(dpc);

		if (in->ntrapnewton==TRUE)
		{
			for (band=0;band<dim->srh_bands;band++)
			{
				in->newton_dphidntrap[band]=Qe*(in->dnt[z][x][y][band]);
			}
		}

		if (in->ptrapnewton==TRUE)
		{
			for (band=0;band<dim->srh_bands;band++)
			{
				in->newton_dphidptrap[band]= -Qe*(in->dpt[z][x][y][band]);

				//dphidxipc+= -Qe*(in->dpt[y]);
			}
		}




	//	G=in->G[y];


				//y
				xil=Qe*2.0*(3.0/2.0)*(Ecc-Ecl)/((wnc+wnl));
				xir=Qe*2.0*(3.0/2.0)*(Ecr-Ecc)/((wnr+wnc));

				//gdouble dxil= -Qe*2.0*(3.0/2.0)*(Ecc-Ecl)/pow((wnc+wnl),2.0);
				//gdouble dxir= -Qe*2.0*(3.0/2.0)*(Ecr-Ecc)/pow((wnr+wnc),2.0);

				xipl=Qe*2.0*(3.0/2.0)*(Evc-Evl)/(wpc+wpl);
				xipr=Qe*2.0*(3.0/2.0)*(Evr-Evc)/(wpr+wpc);

				//x
				xil_x=Qe*2.0*(3.0/2.0)*(Ecc-Ecl_x)/((wnc+wnl_x));
				xir_x=Qe*2.0*(3.0/2.0)*(Ecr_x-Ecc)/((wnr_x+wnc));

				xipl_x=Qe*2.0*(3.0/2.0)*(Evc-Evl_x)/(wpc+wpl_x);
				xipr_x=Qe*2.0*(3.0/2.0)*(Evr_x-Evc)/(wpr_x+wpc);

				dJdxil=0.0;
				dJdxic=0.0;
				dJdxir=0.0;

				dJpdxipl=0.0;
				dJpdxipc=0.0;
				dJpdxipr=0.0;


				dJdphil=0.0;
				dJdphic=0.0;
				dJdphir=0.0;


				dJpdphil=0.0;
				dJpdphic=0.0;
				dJpdphir=0.0;

				//y

				if (y==0)
				{
					BdB(&Bn,&dBn,-xil);
					BdB(&Bp,&dBp,xil);
				}else
				{
					Bn=ns->By_xi_neg[z][x][y-1];
					dBn=ns->dBy_xi_neg[z][x][y-1];

					Bp=ns->By_xi_plus[z][x][y-1];
					dBp=ns->dBy_xi_plus[z][x][y-1];

				}

				//Bp=B(xil);
				//Bn=B(-xil);

				Jnl=(Dnl/dyl)*(Bn*nc-Bp*nl);
				dJnldxil_l= -(Dnl/dyl)*(Bp*dnl);
				dJnldxil_c=(Dnl/dyl)*Bn*dnc;

				
				//dBp=dB(xil);
				//dBn=dB(-xil);

				gdouble dJnldphi_l= -(munl/dyl)*(dBn*nc+dBp*nl);
				gdouble dJnldphi_c=(munl/dyl)*(dBn*nc+dBp*nl);

				BdB(&Bn,&dBn,-xir);
				BdB(&Bp,&dBp,xir);

				//printf("%d %d %d\n",z,x,y);
				ns->By_xi_plus[z][x][y]=Bp;
				ns->dBy_xi_plus[z][x][y]=dBp;
				ns->By_xi_neg[z][x][y]=Bn;
				ns->dBy_xi_neg[z][x][y]=dBn;

				//Bp=B(xir);
				//Bn=B(-xir);

				Jnr=(Dnr/dyr)*(Bn*nr-Bp*nc);
				dJnrdxir_c= -(Dnr/dyr)*(Bp*dnc);
				dJnrdxir_r=(Dnr/dyr)*(Bn*dnr);

				//dBp=dB(xir);
				//dBn=dB(-xir);

				gdouble dJnrdphi_c=(munr/dyr)*(-dBn*nr-dBp*nc);
				gdouble dJnrdphi_r=(munr/dyr)*(dBn*nr+dBp*nc);

				//BdB(&Bp,&dBp,xipl);
				//BdB(&Bn,&dBn,-xipl);

				if (y==0)
				{
					BdB(&Bn,&dBn,-xipl);
					BdB(&Bp,&dBp,xipl);
				}else
				{
					Bn=ns->By_xip_neg[z][x][y-1];
					dBn=ns->dBy_xip_neg[z][x][y-1];

					Bp=ns->By_xip_plus[z][x][y-1];
					dBp=ns->dBy_xip_plus[z][x][y-1];
				}

				//Bp=B(xipl);
				//Bn=B(-xipl);

				Jpl=(Dpl/dyl)*(Bn*pl-Bp*pc);
				//if (y==0)
				//{
				//	printf("%d %Le %Le %Le\n",x,Jpl,Jnl,Dpl);
				//}
				dJpldxipl_l=(Dpl/dyl)*(Bn*dpl);
				dJpldxipl_c= -(Dpl/dyl)*Bp*dpc;

				//dBp=dB(xipl);
				//dBn=dB(-xipl);

				gdouble dJpldphi_l= -((mupl)/dyl)*(dBn*pl+dBp*pc);
				gdouble dJpldphi_c=((mupl)/dyl)*(dBn*pl+dBp*pc);

				BdB(&Bp,&dBp,xipr);
				BdB(&Bn,&dBn,-xipr);

				ns->By_xip_plus[z][x][y]=Bp;
				ns->dBy_xip_plus[z][x][y]=dBp;
				ns->By_xip_neg[z][x][y]=Bn;
				ns->dBy_xip_neg[z][x][y]=dBn;

				//Bp=B(xipr);
				//Bn=B(-xipr);

				Jpr=(Dpr/dyr)*(Bn*pc-Bp*pr);
				dJprdxipr_c=(Dpr/dyr)*(Bn*dpc);
				dJprdxipr_r= -(Dpr/dyr)*(Bp*dpr);

				//dBp=dB(xipr);
				//dBn=dB(-xipr);

				gdouble dJprdphi_c= -(mupr/dyr)*(dBn*pc+dBp*pr);
				gdouble dJprdphi_r=(mupr/dyr)*(dBn*pc+dBp*pr);


				if (y==0)
				{
					if (contact_left==contact_schottky)
					{
						Jnl=vl_e*(nc-nl);
						//printf("%Le\n",Jnl);
						dJnldxil_l= vl_e*(-dnl);
						dJnldxil_c= vl_e*(dnc);
						//printf("%Le %Le\n",Jnl,vl_e*(nl0-nl));

						dJnldphi_l= 0.0;//vl_e*(-dnl);
						dJnldphi_c= 0.0;//vl_e*(dnc);

						Jpl=-vl_h*(pc-pl);
						//printf("%Le\n",Jpl);
						dJpldxipl_l= -vl_h*(-dpl);
						dJpldxipl_c= -vl_h*(dpc);
						//printf("%Le %Le\n",Jnl,vl_e*(nl0-nl));

						dJpldphi_l= 0.0;//-vl_h*(-dpl);//vl_e*(-dnl);
						dJpldphi_c= 0.0;//-vl_h*(dpc);

						dylh=0.0;

					}

					in->Jn_y0[z][x]=Qe*Jnl;
					in->Jp_y0[z][x]=Qe*Jpl;
				}

				if (y==dim->ylen-1)
				{
					in->Jn_y1[z][x]=Qe*Jnr;
					in->Jp_y1[z][x]=Qe*Jpr;
				}

				in->Jn[z][x][y]=Qe*(Jnl+Jnr)/2.0;
				in->Jp[z][x][y]=Qe*(Jpl+Jpr)/2.0;

				in->Jn_diffusion[z][x][y]=Qe*(Dnl*(nc-nl)/dyl+Dnr*(nr-nc)/dyr)/2.0;
				in->Jn_drift[z][x][y]=in->Jn[z][x][y]-in->Jn_diffusion[z][x][y];

				in->Jp_diffusion[z][x][y]= Qe*(-Dpl*(pc-pl)/dyl-Qe*Dpr*(pr-pc)/dyr)/2.0;
				in->Jp_drift[z][x][y]=in->Jp[z][x][y]-in->Jp_diffusion[z][x][y];

				dJdxil+= -dJnldxil_l/(dylh+dyrh);
				dJdxic+=(-dJnldxil_c+dJnrdxir_c)/(dylh+dyrh);
				dJdxir+=dJnrdxir_r/(dylh+dyrh);

				dJpdxipl+= -dJpldxipl_l/(dylh+dyrh);
				dJpdxipc+=(-dJpldxipl_c+dJprdxipr_c)/(dylh+dyrh);
				dJpdxipr+=dJprdxipr_r/(dylh+dyrh);


				dJdphil+= -dJnldphi_l/(dylh+dyrh);
				dJdphic+=(-dJnldphi_c+dJnrdphi_c)/(dylh+dyrh);
				dJdphir+=dJnrdphi_r/(dylh+dyrh);


				dJpdphil+= -dJpldphi_l/(dylh+dyrh);
				dJpdphic+=(-dJpldphi_c+dJprdphi_c)/(dylh+dyrh);
				dJpdphir+=dJprdphi_r/(dylh+dyrh);

				//x

				if (dim_==2)
				{
					dJdxil_x=0.0;
					dJdxic_x=0.0;
					dJdxir_x=0.0;

					dJpdxipl_x=0.0;
					dJpdxipc_x=0.0;
					dJpdxipr_x=0.0;

					dJdphil_x=0.0;
					dJdphic_x=0.0;
					dJdphir_x=0.0;

					dJpdphil_x=0.0;
					dJpdphic_x=0.0;
					dJpdphir_x=0.0;


					if (x==0)
					{
						BdB(&Bp,&dBp,xil_x);
						BdB(&Bn,&dBn,-xil_x);
					}else
					{
						Bn=ns->Bx_xi_neg[z][x-1][y];
						dBn=ns->dBx_xi_neg[z][x-1][y];

						Bp=ns->Bx_xi_plus[z][x-1][y];
						dBp=ns->dBx_xi_plus[z][x-1][y];

					}

					//Bp=B(xil_x);
					//Bn=B(-xil_x);

					Jnl_x=(Dnl_x/dxl)*(Bn*nc-Bp*nl_x);
					/*if (x==0)
					{
						printf("%Le\n",Jnl_x);
					}*/
					dJnldxil_l_x= -(Dnl_x/dxl)*(Bp*dnl_x);
					dJnldxil_c_x=(Dnl_x/dxl)*Bn*dnc;

					//dBp=dB(xil_x);
					//dBn=dB(-xil_x);

					gdouble dJnldphi_l_x= -(munl_x/dxl)*(dBn*nc+dBp*nl_x);
					gdouble dJnldphi_c_x=(munl_x/dxl)*(dBn*nc+dBp*nl_x);


					//Bp=B(xir_x);
					//Bn=B(-xir_x);

					Jnr_x=(Dnr_x/dxr)*(Bn*nr_x-Bp*nc);
					/*if (x==dim->xlen-1)
					{
						printf("%Le\n",Jnr_x);
					}*/
					dJnrdxir_c_x= -(Dnr_x/dxr)*(Bp*dnc);
					dJnrdxir_r_x=(Dnr_x/dxr)*(Bn*dnr_x);

					//dBp=dB(xir_x);
					//dBn=dB(-xir_x);

					gdouble dJnrdphi_c_x=(munr_x/dxr)*(-dBn*nr_x-dBp*nc);
					gdouble dJnrdphi_r_x=(munr_x/dxr)*(dBn*nr_x+dBp*nc);

					if (x==0)
					{
						BdB(&Bp,&dBp,xipl_x);
						BdB(&Bn,&dBn,-xipl_x);
					}else
					{
						Bn=ns->Bx_xip_neg[z][x-1][y];
						dBn=ns->dBx_xip_neg[z][x-1][y];

						Bp=ns->Bx_xip_plus[z][x-1][y];
						dBp=ns->dBx_xip_plus[z][x-1][y];

					}


					//Bp=B(xipl_x);
					//Bn=B(-xipl_x);

					Jpl_x=(Dpl_x/dxl)*(Bn*pl_x-Bp*pc);
					dJpldxipl_l_x=(Dpl_x/dxl)*(Bn*dpl_x);
					dJpldxipl_c_x= -(Dpl_x/dxl)*Bp*dpc;

					//dBp=dB(xipl_x);
					//dBn=dB(-xipl_x);

					gdouble dJpldphi_l_x= -((mupl_x)/dxl)*(dBn*pl_x+dBp*pc);
					gdouble dJpldphi_c_x=((mupl_x)/dxl)*(dBn*pl_x+dBp*pc);

					BdB(&Bp,&dBp,xipr_x);
					BdB(&Bn,&dBn,-xipr_x);

					ns->Bx_xip_plus[z][x][y]=Bp;
					ns->dBx_xip_plus[z][x][y]=dBp;
					ns->Bx_xip_neg[z][x][y]=Bn;
					ns->dBx_xip_neg[z][x][y]=dBn;

					//Bp=B(xipr_x);
					//Bn=B(-xipr_x);

					Jpr_x=(Dpr_x/dxr)*(Bn*pc-Bp*pr_x);
					dJprdxipr_c_x=(Dpr_x/dxr)*(Bn*dpc);
					dJprdxipr_r_x= -(Dpr_x/dxr)*(Bp*dpr_x);


					//dBp=dB(xipr_x);
					//dBn=dB(-xipr_x);

					gdouble dJprdphi_c_x= -(mupr_x/dxr)*(dBn*pc+dBp*pr_x);
					gdouble dJprdphi_r_x=(mupr_x/dxr)*(dBn*pc+dBp*pr_x);

					in->Jn_x[z][x][y]=Qe*(Jnl_x+Jnr_x)/2.0;
					in->Jp_x[z][x][y]=Qe*(Jpl_x+Jpr_x)/2.0;

					in->Jn_x_diffusion[z][x][y]=Qe*(Dnl_x*(nc-nl_x)/dxl+Dnr_x*(nr_x-nc)/dxr)/2.0;
					in->Jn_x_drift[z][x][y]=in->Jn_x[z][x][y]-in->Jn_x_diffusion[z][x][y];

					in->Jp_x_diffusion[z][x][y]= Qe*(-Dpl_x*(pc-pl_x)/dxl-Qe*Dpr_x*(pr_x-pc)/dxr)/2.0;
					in->Jp_x_drift[z][x][y]=in->Jp_x[z][x][y]-in->Jp_x_diffusion[z][x][y];


					/*if (x==0)
					{
						if (in->passivate_x0[z][y]==TRUE)
						{
							//n
							dJdxic_x+=dJdxil_x;
							dJdphic_x+=dJdphil_x;

							//p
							dJpdxipc_x+=dJpdxipl_x;
							dJpdphic_x+=dJpdphil_x;
						}
					}*/

					/*if (x==dim->xlen-1)
					{
						if (in->passivate_x1[z][y]==TRUE)
						{
							//n
							dJdxic_x+=dJdxir_x;
							dJdphic_x+=dJdphir_x;

							//p
							dJpdxipc_x+=dJpdxipr_x;
							dJpdphic_x+=dJpdphir_x;
						}
					}*/


					dJdxic+=dJdxic_x;
					dJdphic+=dJdphic_x;

					dJpdxipc+=dJpdxipc_x;
					dJpdphic+=dJpdphic_x;

				}

				if (Bfree!=0.0)
				{
					dJdxic+= -Bfree*(dnc*pc);
					dJdxipc+= -Bfree*(nc*dpc);

					dJpdxipc+=Bfree*(nc*dpc);
					dJpdxic+=Bfree*(dnc*pc);

					dJdphic+= -Bfree*(dnc*pc);
					dJpdphic+=Bfree*(nc*dpc);

				}

				if (y==0)
				{

					if (in->passivate_y0[z][x]==TRUE)
					{
						//n
						dJdxic+=dJdxil;
						dJdphic+=dJdphil;

						//p
						dJpdxipc+=dJpdxipl;
						dJpdphic+=dJpdphil;
					}

				}
				//printf("%d %Le\n",y,dJpdxipc);
				//new****
				if (y==dim->ylen-1)
				{

					if (in->passivate_y1[z][x]==TRUE)
					{
						//n
						dJdxic+=dJdxir;
						dJdphic+=dJdphir;

						//p
						dJpdxipc+=dJpdxipr;
						dJpdphic+=dJpdphir;
					}

				}


				if (in->go_time==TRUE)
				{
					dJdxic+= -dnc/dt;
				}

				if (in->go_time==TRUE)
				{
					dJpdxipc+=dpc/dt;
				}

				/*dJdxic+=-Bfree*(dnc*pc);
				dJpdxipc+=Bfree*(nc*dpc);*/

				Rtrapn=0.0;
				Rtrapp=0.0;


				in->nrelax[z][x][y]=0.0;
				in->ntrap_to_p[z][x][y]=0.0;
				in->prelax[z][x][y]=0.0;
				in->ptrap_to_n[z][x][y]=0.0;


				if (in->ntrapnewton==TRUE)
				{
					for (band=0;band<dim->srh_bands;band++)
					{
						in->newton_dJdtrapn[band]=0.0;
						in->newton_dJpdtrapn[band]=0.0;
						in->newton_dntrap[band]=nc*in->srh_n_r1[z][x][y][band]-in->srh_n_r2[z][x][y][band]-pc*in->srh_n_r3[z][x][y][band]+in->srh_n_r4[z][x][y][band];
						in->newton_dntrapdntrap[band]=nc*in->dsrh_n_r1[z][x][y][band]-in->dsrh_n_r2[z][x][y][band]-pc*in->dsrh_n_r3[z][x][y][band]+in->dsrh_n_r4[z][x][y][band];
						in->newton_dntrapdn[band]=dnc*in->srh_n_r1[z][x][y][band];
						in->newton_dntrapdp[band]= -dpc*in->srh_n_r3[z][x][y][band];
						Rtrapn+=nc*in->srh_n_r1[z][x][y][band]-in->srh_n_r2[z][x][y][band];
						dJdxic-=dnc*in->srh_n_r1[z][x][y][band];
						in->newton_dJdtrapn[band]-=nc*in->dsrh_n_r1[z][x][y][band]-in->dsrh_n_r2[z][x][y][band];
						Rtrapp+= -(-pc*in->srh_n_r3[z][x][y][band]+in->srh_n_r4[z][x][y][band]);
						dJpdxipc+= -(-dpc*in->srh_n_r3[z][x][y][band]);
						in->newton_dJpdtrapn[band]= -(-pc*in->dsrh_n_r3[z][x][y][band]+in->dsrh_n_r4[z][x][y][band]);


						in->nrelax[z][x][y]+=nc*in->srh_n_r1[z][x][y][band]-in->srh_n_r2[z][x][y][band];
						in->ntrap_to_p[z][x][y]+= -(-pc*in->srh_n_r3[z][x][y][band]+in->srh_n_r4[z][x][y][band]);

						in->nt_r1[z][x][y][band]=nc*in->srh_n_r1[z][x][y][band];
						in->nt_r2[z][x][y][band]=in->srh_n_r2[z][x][y][band];
						in->nt_r3[z][x][y][band]=pc*in->srh_n_r3[z][x][y][band];
						in->nt_r4[z][x][y][band]=in->srh_n_r4[z][x][y][band];

						//printf("%Le %Le %Le %Le\n",in->nt_r1[z][x][y][band],in->nt_r2[z][x][y][band],in->nt_r3[z][x][y][band],in->nt_r4[z][x][y][band]);

					}
				}

				//band=0;

				if (in->ptrapnewton==TRUE)
				{

					for (band=0;band<dim->srh_bands;band++)
					{
						//dJdtrapn[band]=0.0;
						in->newton_dJpdtrapp[band]=0.0;
						in->newton_dJdtrapp[band]=0.0;
						in->newton_dptrap[band]=pc*in->srh_p_r1[z][x][y][band]-in->srh_p_r2[z][x][y][band]-nc*in->srh_p_r3[z][x][y][band]+in->srh_p_r4[z][x][y][band];
						in->newton_dptrapdptrap[band]=pc*in->dsrh_p_r1[z][x][y][band]-in->dsrh_p_r2[z][x][y][band]-nc*in->dsrh_p_r3[z][x][y][band]+in->dsrh_p_r4[z][x][y][band];
						in->newton_dptrapdp[band]=dpc*in->srh_p_r1[z][x][y][band];
						in->newton_dptrapdn[band]= -dnc*in->srh_p_r3[z][x][y][band];

						Rtrapp+=pc*in->srh_p_r1[z][x][y][band]-in->srh_p_r2[z][x][y][band];
						dJpdxipc+=dpc*in->srh_p_r1[z][x][y][band];
						in->newton_dJpdtrapp[band]+=pc*in->dsrh_p_r1[z][x][y][band]-in->dsrh_p_r2[z][x][y][band];

						Rtrapn+= -(-nc*in->srh_p_r3[z][x][y][band]+in->srh_p_r4[z][x][y][band]);
						dJdxic-= -(-dnc*in->srh_p_r3[z][x][y][band]);
						in->newton_dJdtrapp[band]-= -(-nc*in->dsrh_p_r3[z][x][y][band]+in->dsrh_p_r4[z][x][y][band]);

						in->prelax[z][x][y]+=pc*in->srh_p_r1[z][x][y][band]-in->srh_p_r2[z][x][y][band];
						in->ptrap_to_n[z][x][y]+= -(-nc*in->srh_p_r3[z][x][y][band]+in->srh_p_r4[z][x][y][band]);

						in->pt_r1[z][x][y][band]=pc*in->srh_p_r1[z][x][y][band];
						in->pt_r2[z][x][y][band]=in->srh_p_r2[z][x][y][band];
						in->pt_r3[z][x][y][band]=nc*in->srh_p_r3[z][x][y][band];
						in->pt_r4[z][x][y][band]=in->srh_p_r4[z][x][y][band];

					}

				}

				//band=0;


				in->Rn[z][x][y]=Rtrapn+Rfree;
				in->Rp[z][x][y]=Rtrapp+Rfree;

				in->Rnet[z][x][y]=Rtrapp+Rtrapn+Rfree;

				in->Rn_srh[z][x][y]=Rtrapn;
				in->Rp_srh[z][x][y]=Rtrapp;
				//Rtrapp=1e24;
				//Rtrapn=1e24;



				int shift=msm->x_matrix_offset*x;
				int shift_l=msm->x_matrix_offset*(x-1);
				int shift_r=msm->x_matrix_offset*(x+1);

				if (y!=0)
				{
					mx->Ti[pos]=shift+y;
					mx->Tj[pos]=shift+y-1;
					mx->Tx[pos]=dphil_d;
					pos++;
					//electron
					mx->Ti[pos]=shift+dim->ylen*(1)+y;
					mx->Tj[pos]=shift+dim->ylen*(1)+y-1;
					mx->Tx[pos]=dJdxil;
					pos++;

					mx->Ti[pos]=shift+dim->ylen*(1)+y;
					mx->Tj[pos]=shift+y-1;
					mx->Tx[pos]=dJdphil;
					pos++;

					//hole
					mx->Ti[pos]=shift+dim->ylen*(1+1)+y;
					mx->Tj[pos]=shift+dim->ylen*(1+1)+y-1;
					mx->Tx[pos]=dJpdxipl;
					pos++;

					mx->Ti[pos]=shift+y+dim->ylen*(1+1);
					mx->Tj[pos]=shift+y-1;
					mx->Tx[pos]=dJpdphil;
					pos++;

				}


				mx->Ti[pos]=shift+y;
				mx->Tj[pos]=shift+y;
				mx->Tx[pos]=dphic_d;
				pos++;


				mx->Ti[pos]=shift+y;
				mx->Tj[pos]=shift+y+dim->ylen*(1);
				mx->Tx[pos]=dphidxic;
				//strcpy(in->Tdebug[pos],"dphidxic");
				pos++;

				mx->Ti[pos]=shift+y;
				mx->Tj[pos]=shift+y+dim->ylen*(1+1);
				mx->Tx[pos]=dphidxipc;
				//strcpy(in->Tdebug[pos],"dphidxipc");
				pos++;


				//electron

				mx->Ti[pos]=shift+dim->ylen*(1)+y;
				mx->Tj[pos]=shift+dim->ylen*(1)+y;
				mx->Tx[pos]=dJdxic;
				//strcpy(in->Tdebug[pos],"dJdxic");
				pos++;

				mx->Ti[pos]=shift+dim->ylen*(1)+y;
				mx->Tj[pos]=shift+dim->ylen*(1+1)+y;
				mx->Tx[pos]=dJdxipc;
				pos++;

				mx->Ti[pos]=shift+dim->ylen*(1)+y;
				mx->Tj[pos]=shift+y;
				mx->Tx[pos]=dJdphic;
				pos++;



				//hole
				mx->Ti[pos]=shift+dim->ylen*(1+1)+y;
				mx->Tj[pos]=shift+dim->ylen*(1+1)+y;
				//printf("%d %d %d %Le\n",y,dim->ylen*(1+1)+y,dim->ylen*(1+1)+y,dJpdxipc);
				mx->Tx[pos]=dJpdxipc;
				pos++;

				mx->Ti[pos]=shift+dim->ylen*(1+1)+y;
				mx->Tj[pos]=shift+dim->ylen*(1)+y;
				mx->Tx[pos]=dJpdxic;
				pos++;

				mx->Ti[pos]=shift+dim->ylen*(1+1)+y;
				mx->Tj[pos]=shift+y;
				mx->Tx[pos]=dJpdphic;
				pos++;



				if (in->ntrapnewton==TRUE)
				{
					for (band=0;band<dim->srh_bands;band++)
					{
						mx->Ti[pos]=shift+dim->ylen*(1+1+1+band)+y;
						mx->Tj[pos]=shift+dim->ylen*(1+1+1+band)+y;
						mx->Tx[pos]=in->newton_dntrapdntrap[band];
						pos++;

						mx->Ti[pos]=shift+dim->ylen*(1+1+1+band)+y;
						mx->Tj[pos]=shift+dim->ylen*1+y;
						mx->Tx[pos]=in->newton_dntrapdn[band];
						pos++;

						mx->Ti[pos]=shift+dim->ylen*(1+1+1+band)+y;
						mx->Tj[pos]=shift+dim->ylen*(1+1)+y;
						mx->Tx[pos]=in->newton_dntrapdp[band];
						pos++;

						mx->Ti[pos]=shift+dim->ylen*(1)+y;
						mx->Tj[pos]=shift+dim->ylen*(1+1+1+band)+y;
						mx->Tx[pos]=in->newton_dJdtrapn[band];
						pos++;

						mx->Ti[pos]=shift+dim->ylen*(1+1)+y;
						mx->Tj[pos]=shift+dim->ylen*(1+1+1+band)+y;
						mx->Tx[pos]=in->newton_dJpdtrapn[band];
						pos++;

						mx->Ti[pos]=shift+y;
						mx->Tj[pos]=shift+dim->ylen*(1+1+1+band)+y;
						mx->Tx[pos]=in->newton_dphidntrap[band];
						pos++;


					}


				}

				if (in->ptrapnewton==TRUE)
				{
					for (band=0;band<dim->srh_bands;band++)
					{
						mx->Ti[pos]=shift+dim->ylen*(1+1+1+dim->srh_bands+band)+y;
						mx->Tj[pos]=shift+dim->ylen*(1+1+1+dim->srh_bands+band)+y;
						mx->Tx[pos]=in->newton_dptrapdptrap[band];
						pos++;

						mx->Ti[pos]=shift+dim->ylen*(1+1+1+dim->srh_bands+band)+y;
						mx->Tj[pos]=shift+dim->ylen*(1+1)+y;
						mx->Tx[pos]=in->newton_dptrapdp[band];
						pos++;

						mx->Ti[pos]=shift+dim->ylen*(1+1+1+dim->srh_bands+band)+y;
						mx->Tj[pos]=shift+dim->ylen*(1)+y;
						mx->Tx[pos]=in->newton_dptrapdn[band];
						pos++;

						mx->Ti[pos]=shift+dim->ylen*(1+1)+y;
						mx->Tj[pos]=shift+dim->ylen*(1+1+1+dim->srh_bands+band)+y;
						mx->Tx[pos]=in->newton_dJpdtrapp[band];
						pos++;

						mx->Ti[pos]=shift+dim->ylen*(1)+y;
						mx->Tj[pos]=shift+dim->ylen*(1+1+1+dim->srh_bands+band)+y;
						mx->Tx[pos]=in->newton_dJdtrapp[band];
						pos++;

						mx->Ti[pos]=shift+y;
						mx->Tj[pos]=shift+dim->ylen*(1+1+1+dim->srh_bands+band)+y;
						mx->Tx[pos]=in->newton_dphidptrap[band];
						pos++;
					}

				}

				if (y!=(dim->ylen-1))
				{


					mx->Ti[pos]=shift+y;
					mx->Tj[pos]=shift+y+1;
					mx->Tx[pos]=dphir_d;
					pos++;



					//electron
					mx->Ti[pos]=shift+dim->ylen*(1)+y;
					mx->Tj[pos]=shift+dim->ylen*(1)+y+1;
					mx->Tx[pos]=dJdxir;
					pos++;

					mx->Ti[pos]=shift+y+dim->ylen*(1);
					mx->Tj[pos]=shift+y+1;
					mx->Tx[pos]=dJdphir;
					pos++;

					//hole
					mx->Ti[pos]=shift+dim->ylen*(1+1)+y;
					mx->Tj[pos]=shift+dim->ylen*(1+1)+y+1;
					mx->Tx[pos]=dJpdxipr;
					pos++;

					mx->Ti[pos]=shift+y+dim->ylen*(1+1);
					mx->Tj[pos]=shift+y+1;
					mx->Tx[pos]=dJpdphir;
					pos++;




				}

				if (dim_==2)
				{
					if (x!=0)
					{
						mx->Ti[pos]=shift+y;
						mx->Tj[pos]=shift_l+y;
						mx->Tx[pos]=dphil_d_x;
						pos++;

						mx->Ti[pos]=shift+dim->ylen*(1)+y;
						mx->Tj[pos]=shift_l+dim->ylen*(1)+y;
						mx->Tx[pos]=dJdxil_x;
						pos++;

						mx->Ti[pos]=shift+dim->ylen*(1+1)+y;
						mx->Tj[pos]=shift_l+dim->ylen*(1+1)+y;
						mx->Tx[pos]=dJpdxipl_x;
						pos++;

						mx->Ti[pos]=shift+dim->ylen*(1)+y;
						mx->Tj[pos]=shift_l+y;
						mx->Tx[pos]=dJdphil_x;
						pos++;

						mx->Ti[pos]=shift+y+dim->ylen*(1+1);
						mx->Tj[pos]=shift_l+y;
						mx->Tx[pos]=dJpdphil_x;
						pos++;
					}

					if (x!=(dim->xlen-1))
					{
						mx->Ti[pos]=shift+y;
						mx->Tj[pos]=shift_r+y;
						mx->Tx[pos]=dphir_d_x;
						pos++;

						mx->Ti[pos]=shift+dim->ylen*(1)+y;
						mx->Tj[pos]=shift_r+dim->ylen*(1)+y;
						mx->Tx[pos]=dJdxir_x;
						pos++;

						mx->Ti[pos]=shift+dim->ylen*(1+1)+y;
						mx->Tj[pos]=shift_r+dim->ylen*(1+1)+y;
						mx->Tx[pos]=dJpdxipr_x;
						pos++;

						mx->Ti[pos]=shift+dim->ylen*(1)+y;
						mx->Tj[pos]=shift_r+y;
						mx->Tx[pos]=dJdphir_x;
						pos++;

						mx->Ti[pos]=shift+y+dim->ylen*(1+1);
						mx->Tj[pos]=shift_r+y;
						mx->Tx[pos]=dJpdphir_x;
						pos++;

					}
				}

				//Possion
				gdouble build=0.0;

				build= -(deriv);

				build+= -(-(pc-nc+Nad)*Qe);

				for (band=0;band<dim->srh_bands;band++)
				{
					build+= -(-Qe*(in->pt[z][x][y][band]-in->nt[z][x][y][band]));
				}

				//build+= -(-Qe*in->Nad[y]);

				mx->b[shift+y]=build;
				//getchar();
				build=0.0;
				build= -(((Jnr-Jnl)/(dylh+dyrh))-Rtrapn-Rfree);
				if (dim_==2)
				{
					build+= -((Jnr_x-Jnl_x)/(dxlh+dxrh));
				}


				//getchar();
				build-=Gn;
				mx->b[shift+dim->ylen*(1)+y]=build;

				//hole
				build=0.0;
				build= -((Jpr-Jpl)/(dylh+dyrh)+Rtrapp+Rfree);
				if (dim_==2)
				{
					build+=-(Jpr_x-Jpl_x)/(dxlh+dxrh);
				}

				build-= -Gp;


				mx->b[shift+dim->ylen*(1+1)+y]=build;

				if (in->ntrapnewton==TRUE)
				{
					for (band=0;band<dim->srh_bands;band++)
					{
						mx->b[shift+dim->ylen*(1+1+1+band)+y]= -(in->newton_dntrap[band]);
					}
				}

				if (in->ptrapnewton==TRUE)
				{
					for (band=0;band<dim->srh_bands;band++)
					{
						mx->b[shift+dim->ylen*(1+1+1+dim->srh_bands+band)+y]= -(in->newton_dptrap[band]);
					}

				}

			}

	x++;
//getchar();
}while(x<x_max);

	x=0.0;
	for (x=0;x<dim->xlen;x++)
	{
		for (y=0;y<dim->ylen;y++)
		{
			if ((dim->x[x]<2.5e-05)||(dim->x[x]>0.00012))
			{
				in->ntrap_to_p[z][x][y]=0.0;
				in->ptrap_to_n[z][x][y]=0.0;
				in->Rnet[z][x][y]=0.0;
			}
		}
	}

if (pos>mx->nz)
{
	ewe(sim,"Error %d %d %d\n",pos,mx->nz,in->kl_in_newton);
}

//fclose(file_j);
//getchar();

}

gdouble get_cur_error(struct simulation *sim,struct device *in,int x_in)
{
int y;
gdouble phi=0.0;
gdouble n=0.0;
gdouble p=0.0;
gdouble te=0.0;
gdouble th=0.0;
gdouble tl=0.0;
gdouble ttn=0.0;
gdouble ttp=0.0;
gdouble i0=0.0;
int band=0;
int x=0;
int x_max=0;
struct dimensions *dim=&in->ns.dim;
struct matrix *mx=&in->mx;
struct matrix_solver_memory *msm=&(in->msm);

if (x_in==-1)
{
	x=0;
	x_max=dim->xlen;
}else
{
	x=x_in;
	x_max=x_in;
}

do
{
	for (y=0;y<dim->ylen;y++)
	{
			int shift=msm->x_matrix_offset*x;
			phi+=gfabs(mx->b[shift+y]);

			n+=gfabs(mx->b[shift+dim->ylen*(1)+y]);
			p+=+gfabs(mx->b[shift+dim->ylen*(1+1)+y]);

			if (in->ntrapnewton==TRUE)
			{
				for (band=0;band<dim->srh_bands;band++)
				{
					ttn+=gfabs(mx->b[shift+dim->ylen*(1+1+1+band)+y]);
				}
			}

			if (in->ptrapnewton==TRUE)
			{
				for (band=0;band<dim->srh_bands;band++)
				{
					ttp+=gfabs(mx->b[shift+dim->ylen*(1+1+1+dim->srh_bands+band)+y]);
				}
			}


	}

	x++;
}while(x<x_max);


gdouble tot=phi+n+p+te+th+tl+ttn+ttp+i0;
if (isnan( tot))
{
	printf_log(sim,"%Le %Le %Le %Le %Le %Le %Le %Le\n",phi,n,p,te,th,tl,ttn,ttp);
	//dump_matrix(in);
	errors_add(sim,"nan detected in newton solver\n");
	return -1.0;
}

return tot;
}

void solver_cal_memory(struct simulation *sim,struct device *in,int *ret_N,int *ret_M, int dim_)
{
int b=0;
int N=0;
int M=0;
int x_max=0;
struct dimensions *dim=&in->ns.dim;
struct matrix_solver_memory *msm=&(in->msm);

N=dim->ylen*3-2;	//Possion main

N+=dim->ylen*3-2;	//Je main
N+=dim->ylen*3-2;	//Jh main

N+=dim->ylen*3-2;	//dJe/phi
N+=dim->ylen*3-2;	//dJh/phi

N+=dim->ylen;		//dphi/dn
N+=dim->ylen;		//dphi/dh

N+=dim->ylen;		//dJndp
N+=dim->ylen;		//dJpdn

M=dim->ylen;	//Pos

M+=dim->ylen;		//Je
M+=dim->ylen;		//Jh

if (in->ntrapnewton==TRUE)
{
	for (b=0;b<dim->srh_bands;b++)
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
	for (b=0;b<dim->srh_bands;b++)
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

if (dim_==2)
{
	if (dim->xlen>1)
	{
		msm->x_matrix_offset=M;
		N*=dim->xlen;		//multiply diagonals
		M*=dim->xlen;		//multiply diagonals

		N+=dim->ylen*(dim->xlen*2-2);		//dphix
		N+=dim->ylen*(dim->xlen*2-2);		//dJndxi
		N+=dim->ylen*(dim->xlen*2-2);		//dJpdxi
		N+=dim->ylen*(dim->xlen*2-2);		//dJndphi
		N+=dim->ylen*(dim->xlen*2-2);		//dJpdphi

	}
}


*ret_N=N;
*ret_M=M;
}

void dllinternal_solver_realloc(struct simulation *sim,struct device *in,int dim_)
{
int N=0;
int M=0;
gdouble *dtemp=NULL;
int *itemp=NULL;
struct dimensions *dim=&in->ns.dim;
struct matrix *mx=&in->mx;

solver_cal_memory(sim,in,&N,&M,dim_);


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
struct matrix *mx=&in->mx;


if (dim->srh_bands>0)
{
	free(in->newton_dntrap);
	free(in->newton_dntrapdntrap);
	free(in->newton_dntrapdn);
	free(in->newton_dntrapdp);
	free(in->newton_dJdtrapn);
	free(in->newton_dJpdtrapn);
	free(in->newton_dphidntrap);
	free(in->newton_dptrapdp);
	free(in->newton_dptrapdptrap);
	free(in->newton_dptrap);
	free(in->newton_dptrapdn);
	free(in->newton_dJpdtrapp);
	free(in->newton_dJdtrapp);
	free(in->newton_dphidptrap);

	free(in->newton_ntlast);
	free(in->newton_ptlast);

}

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
in->newton_dphidptrap=NULL;


in->newton_ntlast=NULL;
in->newton_ptlast=NULL;


}

int dllinternal_solve_cur(struct simulation *sim,struct device *in, int z, int x)
{
	FILE *file;
	int i;

		for (i=0;i<in->ncontacts;i++)
		{
			if (fabs(in->contacts[i].voltage_want)>10.0)
			{
				ewe(sim,"If you want to simulate above 10V in 2D please get in contact wiht me, roderick.mackenzie@durham.ac.uk\n");
			}

		}

double stop_time;
double start_time= timer_get_time_in_ms();

double update_start;
double update_tot=0;

double fill_matrix_start;
double fill_matrix_tot=0;

double time_matrix_start=0;
double time_matrix_solve_tot=0;

gdouble error=0.0;
int ittr=0;
char temp[PATH_MAX];

if (in->solver_verbosity>SOLVER_VERBOSITY_AT_END)
{
	printf_log(sim,"Solve cur a\n");
}




#ifdef only
only_update_thermal=FALSE;
#endif
//in->enable_back=FALSE;
int stop=FALSE;
int thermalrun=0;
struct newton_state *ns=&(in->ns);
struct matrix *mx=&(in->mx);
mx->use_cache=in->cache.enabled;
matrix_cache_reset(sim,mx);
long double clamp=in->electrical_clamp;
newton_state_reset_error(sim, ns);

	do
	{
		fill_matrix_start=timer_get_time_in_ms();
		fill_matrix(sim,in,z,x);
		fill_matrix_tot+=(timer_get_time_in_ms()-fill_matrix_start);

		if (is_errors(sim)==0)
		{
			return -1;
		}

		if (in->newton_only_fill_matrix==TRUE)
		{
			return 0;
		}

		//dump_for_plot(in);
		//plot_now(in,"plot");

		//matrix_dump_to_file(sim,mx,"matrix.dat");
		//exit(0);

		if (in->stop==TRUE)
		{
			break;
		}

		time_matrix_start=timer_get_time_in_ms();
		if (matrix_solve(sim,&(in->msm),mx)==0)
		{
			printf("*\n");
			//getchar();
			newton_state_load(sim,ns,mx->cache_file_path);
			newton_state_update_device(sim,in, ns);
			//getchar();
			return 0;
		}
		time_matrix_solve_tot+=timer_get_time_in_ms()-time_matrix_start;
		//printf("%s\n",mx->cache_file_path);
		//
		int start_time;

		error=get_cur_error(sim,in,x);
		//printf("laste error >> %Le\n",error);
		//getchar();
		if (error<1e-5)
		{
			clamp=5.0;//clamp*2;
		}

		newton_state_set_last_error(sim, ns, error);

		//printf("%d\n",in->newton_clever_exit);
		if (in->newton_clever_exit==TRUE)
		{
			if (newton_state_clever_exit(sim, ns)==0)
			{
				//printf("exit!\n");
				break;
			}
		}

		if (error==-1)
		{
			return -1;
		}

		update_start= timer_get_time_in_ms();
		update_solver_vars_2d(sim,in,z,x,clamp);
		update_tot+=timer_get_time_in_ms()-update_start;

		//solver_dump_matrix(in->M,in->N,in->Ti,in->Tj, in->Tx,in->b);
		//getchar();

		//thermalrun++;
		if (thermalrun==40) thermalrun=0;
		//update(in);
//getchar();

		if (in->solver_verbosity>SOLVER_VERBOSITY_AT_END)
		{
			printf_log(sim,"%d Cur error = %Le %Le I=%Le\n",ittr,error,contact_get_voltage(sim,in,0),get_I(in));

		}




		ns->last_ittr=ittr;
		ittr++;

		if (get_dump_status(sim,dump_write_converge)==TRUE)
		{
			file=fopena(in->output_path,"converge.dat","a");
			if (file!=NULL)
			{
				fprintf(file,"%Le\n",error);
				fclose(file);
			}
		}

		stop=TRUE;

		if (ittr<in->max_electrical_itt)
		{
			if (error>in->min_cur_error)
			{
				stop=FALSE;
			}
		}

		if (ittr<in->newton_min_itt)
		{
			stop=FALSE;
		}


		if ((ittr<2)&&(error<in->min_cur_error))
		{
			in->dd_conv=TRUE;
		}else
		{
			in->dd_conv=FALSE;
		}


	}while(stop==FALSE);

in->newton_last_ittr=ittr;

if (error>1e-3)
{
	printf_log(sim,"warning: The solver has not converged very well.\n");
}

//getchar();
/*if (get_dump_status(sim,dump_newton)==TRUE)
{
	join_path(2,temp,in->output_path,"solver");
	dump_1d_slice(sim,in,temp);
}*/
//plot_now(sim,in,"plot");
//getchar();
in->odes+=mx->M;
//getchar();
//matrix_cache_save(sim,mx);
if (mx->use_cache==TRUE)
{
	newton_state_save(sim,mx->cache_file_path,ns);
}
//printf("%s\n",mx->cache_file_path);
//getchar();

stop_time= timer_get_time_in_ms();

ns->last_time=(double)(stop_time-start_time);
//printf("update time=%lf ms\n",update_tot);
//printf("fill matrix =%lf ms\n",fill_matrix_tot);
//printf("solve matrix =%lf ms\n",time_matrix_solve_tot);

FILE *out;
out=fopen("matrix_times.dat","a");
if (out!=NULL)
{
	fprintf(out,"%d %d %lf %lf\n",(int)ns->last_time,mx->tot_time,update_tot,fill_matrix_tot);
	fclose(out);
}

//matrix_stats(sim,mx);

//ns->last_time=0.0;

return 0;
}


