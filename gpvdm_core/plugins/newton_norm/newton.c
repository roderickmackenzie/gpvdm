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
	@brief Newton solver which uses slotboom variables.
*/


#include <stdlib.h>
#include <string.h>
#include <gpvdm_const.h>
#include "newton.h"
#include <dll_export.h>
#include <log.h>
#include <exp.h>
#include <util.h>
#include <dump.h>
#include <cal_path.h>
#include <dos.h>
#include <sim.h>
#include <solver_interface.h>
#include <contacts.h>
#include <plot.h>
#include <memory.h>



gdouble Vapplied=0.0;

static gdouble T0=1e20;
static gdouble D0=1.0;
static gdouble n0=1.0;
static gdouble phi0=0.0;
static gdouble l0=0.0;
static gdouble r0=0.0;
static gdouble r_bi0=0.0;


void update_solver_vars(struct simulation *sim,struct device *in, int z, int x,int clamp)
{
int i;
int band=0;
struct newton_state *ns=&(in->ns);
struct dimensions *dim=&in->ns.dim;
struct matrix *mx=&in->mx;

gdouble clamp_temp=300.0;

gdouble update=0.0;
	for (i=0;i<dim->ylen;i++)
	{

		update=(gdouble)mx->b[i]*phi0;
		//if ((in->interfaceleft==TRUE)&&(i==0))
		//{
		//}else
		//if ((in->interfaceright==TRUE)&&(i==dim->ylen-1))
		//{
		//}else
		//{
			if (clamp==TRUE)
			{
				ns->phi[z][x][i]+=update/(1.0+gfabs(update/in->electrical_clamp/(clamp_temp*kb/Qe)));
			}else
			{
				ns->phi[z][x][i]+=update;

			}
		//}


		update=(gdouble)(mx->b[dim->ylen*(1)+i])*phi0;
		if (clamp==TRUE)
		{
			ns->x[z][x][i]+=update/(1.0+gfabs(update/in->electrical_clamp/(clamp_temp*kb/Qe)));
		}else
		{
			ns->x[z][x][i]+=update;
		}


		update=(gdouble)(mx->b[dim->ylen*(1+1)+i])*phi0;
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
				update=(gdouble)(mx->b[dim->ylen*(1+1+1+band)+i]);
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
				update=(gdouble)(mx->b[dim->ylen*(1+1+1+dim->srh_bands+band)+i]);
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


update_y_array(sim,in,z,x);

}

void fill_matrix(struct simulation *sim,struct device *in, int z, int x)
{
//gdouble offset= -0.5;
int band=0;
struct matrix *mx=&in->mx;
struct heat *thermal=&(in->thermal);
struct epitaxy *epi=&(in->my_epitaxy);
update_y_array(sim,in,z,x);

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
int i;
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

gdouble dJdphil_leftl=0.0;
gdouble dJdphil_leftc=0.0;
gdouble dJpdphil_leftl=0.0;
gdouble dJpdphil_leftc=0.0;
gdouble dphil_left=0.0;
gdouble dJdxil_leftc=0.0;
gdouble dJpdxipl_leftc=0.0;

gdouble dJdxic_rightc=0.0;
gdouble dJpdxipc_rightc=0.0;
gdouble dJpdphi_rightc=0.0;
gdouble dJdphi_rightc=0.0;

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

//gdouble dylh_left=0.0;
//gdouble dyrh_left=0.0;
//gdouble dncdphic=0.0;
//gdouble dpcdphic=0.0;

struct newton_state *ns=&(in->ns);
struct dimensions *dim=&in->ns.dim;
struct shape *s;

	/*if (in->interfaceleft==TRUE)
	{
		ns->phi[z][x][0]=in->Vapplied_y0[z][x];
	}*/

/*if (in->interfaceright==TRUE)
{
		ns->phi[z][x][dim->ylen-1]=in->Vr[z][x]-in->Vapplied_y1[z][x];
}*/

		pos=0;
		for (i=0;i<dim->ylen;i++)
		{
			s=in->obj_zxy[z][x][i]->s;
			if (i==0)
			{
//				exl=0.0;
//				Dexl=in->Dex[0];

				phil=(in->Vapplied_y0[z][x]/phi0);


				yl=dim->y[0]/l0-(dim->y[1]-dim->y[0])/l0;
//				Tll=thermal->Ty0;
				Tel=thermal->Ty0;
				Thl=thermal->Ty0;




				Ecl= -in->Xi[z][x][0]/phi0-phil;
				Evl= -in->Xi[z][x][0]/phi0-phil-in->Eg[z][x][0]/phi0;
				epl=in->epsilonr[z][x][0];//*epsilon0;


				xnl=in->Fi[z][x][0]/phi0;
				tnl=in->Xi[z][x][0]/phi0;
				one=xnl+tnl;

				get_n_den(s,one*phi0,Tel,&nl,&dnl,&wnl);
				nl/=n0;
				dnl*=phi0/n0;

				//dnl=get_dn_den(in,one*phi0,Tel,in->imat[z][x][i])*phi0/n0;
				//wnl=get_n_w(in,one*phi0,Tel,in->imat[z][x][i]);

				munl=in->mun_y[z][x][0];


				xpl= -in->Fi[z][x][0]/phi0;
				tpl=(in->Xi[z][x][0]/phi0+in->Eg[z][x][0]/phi0);
				one=xpl-tpl;

				get_p_den(s,one*phi0,Thl,&pl,&dpl,&wpl);
				pl/=n0;
				dpl*=phi0/n0;
				//dpl=get_dp_den(in,one*phi0,Thl,in->imat[z][x][i])*phi0/n0;
				//wpl=get_p_w(in,one*phi0,Thl,in->imat[z][x][i]);


				mupl=in->mup_y[z][x][0];


//				kll=in->kl[i];

			}else
			{
//				Dexl=in->Dex[i-1];
//				exl=in->ex[i-1];
				phil=ns->phi[z][x][i-1]/phi0;
				yl=dim->y[i-1]/l0;
//				Tll=in->Tl[z][x][i-1];
				Tel=in->Te[z][x][i-1];
				Thl=in->Th[z][x][i-1];
				Ecl=in->Ec[z][x][i-1]/phi0;
				Evl=in->Ev[z][x][i-1]/phi0;


				nl=in->n[z][x][i-1]/n0;
				dnl=in->dn[z][x][i-1]*phi0/n0;


				wnl=in->wn[z][x][i-1];
				wpl=in->wp[z][x][i-1];

				pl=in->p[z][x][i-1]/n0;
				dpl=in->dp[z][x][i-1]*phi0/n0;
				munl=in->mun_y[z][x][i-1];
				mupl=in->mup_y[z][x][i-1];


				epl=in->epsilonr[z][x][i-1];//*epsilon0;


//				kll=in->kl[z][x][i-1];
			}

			Ecc=(-in->Xi[z][x][i]/phi0-ns->phi[z][x][i]/phi0);
			Evc=(-in->Xi[z][x][i]/phi0-ns->phi[z][x][i]/phi0-in->Eg[z][x][i]/phi0);

			if (i==(dim->ylen-1))
			{

//				Dexr=in->Dex[z][x][i];
//				exr=0.0;
				//phir=in->Vr[z][x];

				phir=(in->V_y1[z][x]/phi0+in->Vapplied_y1[z][x]/phi0);

				yr=dim->y[i]/l0+(dim->y[i]-dim->y[i-1])/l0;
//				Tlr=thermal->Ty1;
				Ter=thermal->Ty1;
				Thr=thermal->Ty1;


				Ecr= -in->Xi[z][x][i]/phi0-phir;
				Evr= -in->Xi[z][x][i]/phi0-phir-in->Eg[z][x][i]/phi0;


				xnr=(in->V_y1[z][x]/phi0+in->Fi[z][x][i]/phi0);
				tnr=(in->Xi[z][x][i]/phi0);

				one=xnr+tnr;

				get_n_den(s,one*phi0,Ter,&nr,&dnr,&wnr);
				nr/=n0;
				dnr*=phi0/n0;

				//dnr=get_dn_den(in,one*phi0,Ter,in->imat[z][x][i])*phi0/n0;
				//wnr=get_n_w(in,one*phi0,Ter,in->imat[z][x][i]);



				xpr= -(in->V_y1[z][x]/phi0+in->Fi[z][x][i]/phi0);
				tpr=(in->Xi[z][x][i]/phi0+in->Eg[z][x][i]/phi0);

				one=xpr-tpr;

				get_p_den(s,one*phi0,Thr,&pr,&dpr,&wpr);///n0;
				pr/=n0;
				dpr*=phi0/n0;
				//dpr=get_dp_den(in,one*phi0,Thr,in->imat[z][x][i])*phi0/n0;
				//wpr=get_p_w(in,one*phi0,Thr,in->imat[z][x][i]);

				munr=in->mun_y[z][x][i];
				mupr=in->mup_y[z][x][i];

				epr=in->epsilonr[z][x][i];//*epsilon0;
//				klr=in->kl[z][x][i];

			}else
			{

//				Dexr=in->Dex[z][x][i+1];
//				exr=in->ex[z][x][i+1];
				phir=ns->phi[z][x][i+1]/phi0;
				yr=dim->y[i+1]/l0;
//				Tlr=in->Tl[z][x][i+1];
				Ter=in->Te[z][x][i+1];
				Thr=in->Th[z][x][i+1];

				Ecr=in->Ec[z][x][i+1]/phi0;
				Evr=in->Ev[z][x][i+1]/phi0;


				nr=in->n[z][x][i+1]/n0;
				dnr=in->dn[z][x][i+1]*phi0/n0;

				wnr=in->wn[z][x][i+1];
				wpr=in->wp[z][x][i+1];

				pr=in->p[z][x][i+1]/n0;
				dpr=in->dp[z][x][i+1]*phi0/n0;
				munr=in->mun_y[z][x][i+1];
				mupr=in->mup_y[z][x][i+1];

				epr=in->epsilonr[z][x][i+1];//*epsilon0;
//				klr=in->kl[z][x][i+1];

			}

			dJdxipc=0.0;
			dJpdxic=0.0;

			epc=in->epsilonr[z][x][i];//*epsilon0;


//			exc=in->ex[z][x][i];
//			Dexc=in->Dex[z][x][i];
			yc=dim->y[i]/l0;
			dyl=yc-yl;
			dyr=yr-yc;
			ddh=(dyl+dyr)/2.0;
			gdouble dylh=dyl/2.0;
			gdouble dyrh=dyr/2.0;

//			dh=(dyl+dyr);
			phic=ns->phi[z][x][i]/phi0;
//			Tlc=in->Tl[z][x][i];
//			Tec=in->Te[z][x][i];
//			Thc=in->Th[z][x][i];

				munc=in->mun_y[z][x][i];
				mupc=in->mup_y[z][x][i];

				wnc=in->wn[z][x][i];
				wpc=in->wp[z][x][i];

				Dnl=munl*(2.0/3.0)*wnl/Qe;
				Dpl=mupl*(2.0/3.0)*wpl/Qe;

				Dnc=munc*(2.0/3.0)*wnc/Qe;
				Dpc=mupc*(2.0/3.0)*wpc/Qe;
				in->Dn[z][x][i]=Dnc;
				in->Dp[z][x][i]=Dnc;


				Dnr=munr*(2.0/3.0)*wnr/Qe;
				Dpr=mupr*(2.0/3.0)*wpr/Qe;


				Dnl=(Dnl+Dnc)/2.0;
				Dnr=(Dnr+Dnc)/2.0;

				Dpl=(Dpl+Dpc)/2.0;
				Dpr=(Dpr+Dpc)/2.0;

				munl=(munl+munc)/2.0;
				munr=(munr+munc)/2.0;

				mupl=(mupl+mupc)/2.0;
				mupr=(mupr+mupc)/2.0;



				nc=in->n[z][x][i]/n0;
				pc=in->p[z][x][i]/n0;

				dnc=in->dn[z][x][i]*phi0/n0;
				dpc=in->dp[z][x][i]*phi0/n0;
//				dncdphic=in->dndphi[i];
//				dpcdphic=in->dpdphi[i];

				Bfree=in->B[z][x][i]/r_bi0;
				Nad=in->Nad[z][x][i]/n0;
				nceq=in->nfequlib[z][x][i]/n0;
				pceq=in->pfequlib[z][x][i]/n0;
				Rfree=Bfree*(nc*pc-nceq*pceq);
				in->Rfree[z][x][i]=Rfree*r_bi0;

//			klc=in->kl[i];
			nlast=in->nlast[z][x][i]/n0;
			plast=in->plast[z][x][i]/n0;

			for (band=0;band<dim->srh_bands;band++)
			{
				in->newton_ntlast[band]=in->ntlast[z][x][i][band];
				in->newton_ptlast[band]=in->ptlast[z][x][i][band];
			}

			dt=in->dt;

//	R=in->R[z][x][i];
	Gn=in->Gn[z][x][i]/r0;
	Gp=in->Gp[z][x][i]/r0;

	e0=(epl+epc)/2.0;
	e1=(epc+epr)/2.0;

//	kl0=(klc+kll)/2.0;
//	kl1=(klr+klc)/2.0;

	dphil= -e0/dyl/ddh;
	dphic=e0/dyl/ddh+e1/dyr/ddh;
	dphir= -e1/dyr/ddh;

	gdouble dphil_d=dphil;
	gdouble dphic_d=dphic;
	gdouble dphir_d=dphir;


	deriv=phil*dphil+phic*dphic+phir*dphir;
	dphidxic=(dnc);	//Q*
	dphidxipc= -(dpc);//Q*

	/*if (((in->interfaceleft==TRUE)&&(i==0))||((in->interfaceright==TRUE)&&(i==dim->ylen-1)))
	{
		dphidxic=0.0;
		dphidxipc=0.0;
	}*/

	if (in->ntrapnewton==TRUE)
	{
		for (band=0;band<dim->srh_bands;band++)
		{
			in->newton_dphidntrap[band]=Qe*(in->dnt[z][x][i][band]);
			//if ((in->interfaceleft==TRUE)&&(i==0)) in->newton_dphidntrap[band]=0.0;
			//if ((in->interfaceright==TRUE)&&(i==dim->ylen-1)) in->newton_dphidntrap[band]=0.0;
		}
	}

	if (in->ptrapnewton==TRUE)
	{
		for (band=0;band<dim->srh_bands;band++)
		{
			in->newton_dphidptrap[band]= -Qe*(in->dpt[z][x][i][band]);
			//if ((in->interfaceleft==TRUE)&&(i==0)) in->newton_dphidptrap[band]=0.0;
			//if ((in->interfaceright==TRUE)&&(i==dim->ylen-1)) in->newton_dphidptrap[band]=0.0;

			//dphidxipc+= -Q*(in->dpt[i]);
		}
	}




//	G=in->G[i];



			xil=Qe*2.0*(3.0/2.0)*phi0*(Ecc-Ecl)/((wnc+wnl));
			xir=Qe*2.0*(3.0/2.0)*phi0*(Ecr-Ecc)/((wnr+wnc));

			//gdouble dxil= -Qe*2.0*(3.0/2.0)*(Ecc-Ecl)/pow((wnc+wnl),2.0);
			//gdouble dxir= -Qe*2.0*(3.0/2.0)*(Ecr-Ecc)/pow((wnr+wnc),2.0);

			xipl=Qe*2.0*(3.0/2.0)*phi0*(Evc-Evl)/(wpc+wpl);
			xipr=Qe*2.0*(3.0/2.0)*phi0*(Evr-Evc)/(wpr+wpc);

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


			Jnl=(Dnl/D0/dyl)*(B(-xil)*nc-B(xil)*nl);
			dJnldxil_l= -(Dnl/D0/dyl)*(B(xil)*dnl);
			dJnldxil_c=(Dnl/D0/dyl)*B(-xil)*dnc;

			gdouble dJnldphi_l= -phi0*(munl/D0/dyl)*(dB(-xil)*nc+dB(xil)*nl);
			gdouble dJnldphi_c=phi0*(munl/D0/dyl)*(dB(-xil)*nc+dB(xil)*nl);

			Jnr=(Dnr/D0/dyr)*(B(-xir)*nr-B(xir)*nc);
			dJnrdxir_c= -(Dnr/D0/dyr)*(B(xir)*dnc);
			dJnrdxir_r=(Dnr/D0/dyr)*(B(-xir)*dnr);

			gdouble dJnrdphi_c=phi0*(munr/D0/dyr)*(-dB(-xir)*nr-dB(xir)*nc);
			gdouble dJnrdphi_r=phi0*(munr/D0/dyr)*(dB(-xir)*nr+dB(xir)*nc);

			Jpl=(Dpl/D0/dyl)*(B(-xipl)*pl-B(xipl)*pc);
			dJpldxipl_l=(Dpl/D0/dyl)*(B(-xipl)*dpl);
			dJpldxipl_c= -(Dpl/D0/dyl)*B(xipl)*dpc;

			gdouble dJpldphi_l= -phi0*((mupl/D0)/dyl)*(dB(-xipl)*pl+dB(xipl)*pc);
			gdouble dJpldphi_c=phi0*((mupl/D0)/dyl)*(dB(-xipl)*pl+dB(xipl)*pc);

			Jpr=(Dpr/D0/dyr)*(B(-xipr)*pc-B(xipr)*pr);
			dJprdxipr_c=(Dpr/D0/dyr)*(B(-xipr)*dpc);
			dJprdxipr_r= -(Dpr/D0/dyr)*(B(xipr)*dpr);

			gdouble dJprdphi_c= -phi0*(mupr/D0/dyr)*(dB(-xipr)*pc+dB(xipr)*pr);
			gdouble dJprdphi_r=phi0*(mupr/D0/dyr)*(dB(-xipr)*pc+dB(xipr)*pr);


			if (i==0)
			{
				in->Jn_y0[z][x]=Qe*Jnl*D0*n0/l0;
				in->Jp_y0[z][x]=Qe*Jpl*D0*n0/l0;
			}

			if (i==dim->ylen-1)
			{
				in->Jn_y1[z][x]=Qe*Jnr*D0*n0/l0;
				in->Jp_y1[z][x]=Qe*Jpr*D0*n0/l0;
			}


			in->Jn[z][x][i]=Qe*(Jnl+Jnr)*D0*n0/l0/2.0;
			in->Jp[z][x][i]=Qe*(Jpl+Jpr)*D0*n0/l0/2.0;

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

			if (Bfree!=0.0)
			{
				dJdxic+= -Bfree*(dnc*pc);
				dJdxipc+= -Bfree*(nc*dpc);

				dJpdxipc+=Bfree*(nc*dpc);
				dJpdxic+=Bfree*(dnc*pc);

				//if ((in->interfaceleft==TRUE)&&(i==0))
				//{
				//}else
				//if ((in->interfaceright==TRUE)&&(i==dim->ylen-1))
				//{
				//}else
				//{
					dJdphic+= -Bfree*(dnc*pc);
					dJpdphic+=Bfree*(nc*dpc);
				//}

			}

			if (i==0)
			{

				dJdphil_leftl=dJnldphi_l;
				dJdphil_leftc=dJnldphi_c;
				dJpdphil_leftl=dJpldphi_l;
				dJpdphil_leftc=dJpldphi_c;
				dphil_left= -e0/dyl/ddh;
				dJdxil_leftc=dJnldxil_c;
				dJpdxipl_leftc=dJpldxipl_c;
				//dylh_left=dylh;
				//dyrh_left=dyrh;

			}

			if (i==dim->ylen-1)
			{
				dJdxic_rightc=dJnrdxir_c;
				dJpdxipc_rightc=dJprdxipr_c;
				dJdphi_rightc= -dJnrdphi_r;
				dJpdphi_rightc=dJprdphi_c;
			}





			Rtrapn=0.0;
			Rtrapp=0.0;



			in->nrelax[z][x][i]=0.0;
			in->ntrap_to_p[z][x][i]=0.0;
			in->prelax[z][x][i]=0.0;
			in->ptrap_to_n[z][x][i]=0.0;


			if (in->ntrapnewton==TRUE)
			{
				for (band=0;band<dim->srh_bands;band++)
				{
					in->newton_dJdtrapn[band]=0.0;
					in->newton_dJpdtrapn[band]=0.0;
					in->newton_dntrap[band]=nc*in->srh_n_r1[z][x][i][band]-in->srh_n_r2[z][x][i][band]-pc*in->srh_n_r3[z][x][i][band]+in->srh_n_r4[z][x][i][band];
					in->newton_dntrapdntrap[band]=nc*in->dsrh_n_r1[z][x][i][band]-in->dsrh_n_r2[z][x][i][band]-pc*in->dsrh_n_r3[z][x][i][band]+in->dsrh_n_r4[z][x][i][band];
					in->newton_dntrapdn[band]=dnc*in->srh_n_r1[z][x][i][band];
					in->newton_dntrapdp[band]= -dpc*in->srh_n_r3[z][x][i][band];
					Rtrapn+=nc*in->srh_n_r1[z][x][i][band]-in->srh_n_r2[z][x][i][band];
					dJdxic-=dnc*in->srh_n_r1[z][x][i][band];
					in->newton_dJdtrapn[band]-=nc*in->dsrh_n_r1[z][x][i][band]-in->dsrh_n_r2[z][x][i][band];
					Rtrapp+= -(-pc*in->srh_n_r3[z][x][i][band]+in->srh_n_r4[z][x][i][band]);
					dJpdxipc+= -(-dpc*in->srh_n_r3[z][x][i][band]);
					in->newton_dJpdtrapn[band]= -(-pc*in->dsrh_n_r3[z][x][i][band]+in->dsrh_n_r4[z][x][i][band]);


					in->nrelax[z][x][i]+=nc*in->srh_n_r1[z][x][i][band]-in->srh_n_r2[z][x][i][band];
					in->ntrap_to_p[z][x][i]+= -(-pc*in->srh_n_r3[z][x][i][band]+in->srh_n_r4[z][x][i][band]);

					in->nt_r1[z][x][i][band]=nc*in->srh_n_r1[z][x][i][band];
					in->nt_r2[z][x][i][band]=in->srh_n_r2[z][x][i][band];
					in->nt_r3[z][x][i][band]=pc*in->srh_n_r3[z][x][i][band];
					in->nt_r4[z][x][i][band]=in->srh_n_r4[z][x][i][band];

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
					in->newton_dptrap[band]=pc*in->srh_p_r1[z][x][i][band]-in->srh_p_r2[z][x][i][band]-nc*in->srh_p_r3[z][x][i][band]+in->srh_p_r4[z][x][i][band];
					in->newton_dptrapdptrap[band]=pc*in->dsrh_p_r1[z][x][i][band]-in->dsrh_p_r2[z][x][i][band]-nc*in->dsrh_p_r3[z][x][i][band]+in->dsrh_p_r4[z][x][i][band];
					in->newton_dptrapdp[band]=dpc*in->srh_p_r1[z][x][i][band];
					in->newton_dptrapdn[band]= -dnc*in->srh_p_r3[z][x][i][band];

					Rtrapp+=pc*in->srh_p_r1[z][x][i][band]-in->srh_p_r2[z][x][i][band];
					dJpdxipc+=dpc*in->srh_p_r1[z][x][i][band];
					in->newton_dJpdtrapp[band]+=pc*in->dsrh_p_r1[z][x][i][band]-in->dsrh_p_r2[z][x][i][band];

					Rtrapn+= -(-nc*in->srh_p_r3[z][x][i][band]+in->srh_p_r4[z][x][i][band]);
					dJdxic-= -(-dnc*in->srh_p_r3[z][x][i][band]);
					in->newton_dJdtrapp[band]-= -(-nc*in->dsrh_p_r3[z][x][i][band]+in->dsrh_p_r4[z][x][i][band]);


					in->prelax[z][x][i]+=pc*in->srh_p_r1[z][x][i][band]-in->srh_p_r2[z][x][i][band];
					in->ptrap_to_n[z][x][i]+= -(-nc*in->srh_p_r3[z][x][i][band]+in->srh_p_r4[z][x][i][band]);

					in->pt_r1[z][x][i][band]=pc*in->srh_p_r1[z][x][i][band];
					in->pt_r2[z][x][i][band]=in->srh_p_r2[z][x][i][band];
					in->pt_r3[z][x][i][band]=nc*in->srh_p_r3[z][x][i][band];
					in->pt_r4[z][x][i][band]=in->srh_p_r4[z][x][i][band];

				}

			}

			//band=0;



			in->Rn[z][x][i]=Rtrapn+Rfree;
			in->Rp[z][x][i]=Rtrapp+Rfree;

			in->Rn_srh[z][x][i]=Rtrapn;
			in->Rp_srh[z][x][i]=Rtrapp;

			//Rtrapp=1e24;
			//Rtrapn=1e24;




			if (i!=0)
			{
				mx->Ti[pos]=i;
				mx->Tj[pos]=i-1;
				mx->Tx[pos]=dphil_d;
				pos++;
				//electron
					mx->Ti[pos]=dim->ylen*(1)+i;
					mx->Tj[pos]=dim->ylen*(1)+i-1;
					mx->Tx[pos]=dJdxil;
					pos++;

					mx->Ti[pos]=dim->ylen*(1)+i;
					mx->Tj[pos]=i-1;
					mx->Tx[pos]=dJdphil;
					pos++;

					//hole
					mx->Ti[pos]=dim->ylen*(1+1)+i;
					mx->Tj[pos]=dim->ylen*(1+1)+i-1;
					mx->Tx[pos]=dJpdxipl;
					pos++;

					mx->Ti[pos]=i+dim->ylen*(1+1);
					mx->Tj[pos]=i-1;
					mx->Tx[pos]=dJpdphil;
					pos++;

			}


			//if ((in->kl_in_newton==TRUE)&&(in->interfaceleft==TRUE)&&(i==0))
			//{
			//	//getchar();
			//}else
			//{
				//phi
				mx->Ti[pos]=i;
				mx->Tj[pos]=i;
				mx->Tx[pos]=dphic_d;
				pos++;
			//}


			mx->Ti[pos]=i;
			mx->Tj[pos]=i+dim->ylen*(1);
			mx->Tx[pos]=dphidxic;
			//strcpy(in->Tdebug[pos],"dphidxic");
			pos++;

			mx->Ti[pos]=i;
			mx->Tj[pos]=i+dim->ylen*(1+1);
			mx->Tx[pos]=dphidxipc;
			//strcpy(in->Tdebug[pos],"dphidxipc");
			pos++;


			//electron

			mx->Ti[pos]=dim->ylen*(1)+i;
			mx->Tj[pos]=dim->ylen*(1)+i;
			mx->Tx[pos]=dJdxic;
			//strcpy(in->Tdebug[pos],"dJdxic");
			pos++;

			mx->Ti[pos]=dim->ylen*(1)+i;
			mx->Tj[pos]=dim->ylen*(1+1)+i;
			mx->Tx[pos]=dJdxipc;
			//strcpy(in->Tdebug[pos],"dJdxipc");
			pos++;

			mx->Ti[pos]=dim->ylen*(1)+i;
			mx->Tj[pos]=i;
			mx->Tx[pos]=dJdphic;
			//strcpy(in->Tdebug[pos],"dJdphic");
			pos++;





			//hole
			mx->Ti[pos]=dim->ylen*(1+1)+i;
			mx->Tj[pos]=dim->ylen*(1+1)+i;
			mx->Tx[pos]=dJpdxipc;
			pos++;

			mx->Ti[pos]=dim->ylen*(1+1)+i;
			mx->Tj[pos]=dim->ylen*(1)+i;
			mx->Tx[pos]=dJpdxic;
			pos++;

			mx->Ti[pos]=dim->ylen*(1+1)+i;
			mx->Tj[pos]=i;
			mx->Tx[pos]=dJpdphic;
			pos++;



			if (in->ntrapnewton==TRUE)
			{
				for (band=0;band<dim->srh_bands;band++)
				{
					mx->Ti[pos]=dim->ylen*(1+1+1+band)+i;
					mx->Tj[pos]=dim->ylen*(1+1+1+band)+i;
					mx->Tx[pos]=in->newton_dntrapdntrap[band];
					pos++;

					mx->Ti[pos]=dim->ylen*(1+1+1+band)+i;
					mx->Tj[pos]=dim->ylen*1+i;
					mx->Tx[pos]=in->newton_dntrapdn[band];
					pos++;

					mx->Ti[pos]=dim->ylen*(1+1+1+band)+i;
					mx->Tj[pos]=dim->ylen*(1+1)+i;
					mx->Tx[pos]=in->newton_dntrapdp[band];
					pos++;

					mx->Ti[pos]=dim->ylen*(1)+i;
					mx->Tj[pos]=dim->ylen*(1+1+1+band)+i;
					mx->Tx[pos]=in->newton_dJdtrapn[band];
					pos++;

					mx->Ti[pos]=dim->ylen*(1+1)+i;
					mx->Tj[pos]=dim->ylen*(1+1+1+band)+i;
					mx->Tx[pos]=in->newton_dJpdtrapn[band];
					pos++;

					mx->Ti[pos]=i;
					mx->Tj[pos]=dim->ylen*(1+1+1+band)+i;
					mx->Tx[pos]=in->newton_dphidntrap[band];
					pos++;


				}


			}

			if (in->ptrapnewton==TRUE)
			{
				for (band=0;band<dim->srh_bands;band++)
				{
					mx->Ti[pos]=dim->ylen*(1+1+1+dim->srh_bands+band)+i;
					mx->Tj[pos]=dim->ylen*(1+1+1+dim->srh_bands+band)+i;
					mx->Tx[pos]=in->newton_dptrapdptrap[band];
					pos++;

					mx->Ti[pos]=dim->ylen*(1+1+1+dim->srh_bands+band)+i;
					mx->Tj[pos]=dim->ylen*(1+1)+i;
					mx->Tx[pos]=in->newton_dptrapdp[band];
					pos++;

					mx->Ti[pos]=dim->ylen*(1+1+1+dim->srh_bands+band)+i;
					mx->Tj[pos]=dim->ylen*(1)+i;
					mx->Tx[pos]=in->newton_dptrapdn[band];
					pos++;

					mx->Ti[pos]=dim->ylen*(1+1)+i;
					mx->Tj[pos]=dim->ylen*(1+1+1+dim->srh_bands+band)+i;
					mx->Tx[pos]=in->newton_dJpdtrapp[band];
					pos++;

					mx->Ti[pos]=dim->ylen*(1)+i;
					mx->Tj[pos]=dim->ylen*(1+1+1+dim->srh_bands+band)+i;
					mx->Tx[pos]=in->newton_dJdtrapp[band];
					pos++;

					mx->Ti[pos]=i;
					mx->Tj[pos]=dim->ylen*(1+1+1+dim->srh_bands+band)+i;
					mx->Tx[pos]=in->newton_dphidptrap[band];
					pos++;
				}

			}

			if (i!=(dim->ylen-1))
			{

				//if ((in->kl_in_newton==TRUE)&&(in->interfaceleft==TRUE)&&(i==0))
				//{
					//getchar();
				//}else
				//{
					//phi
					mx->Ti[pos]=i;
					mx->Tj[pos]=i+1;
					mx->Tx[pos]=dphir_d;
					pos++;
				//}



				//electron
				mx->Ti[pos]=dim->ylen*(1)+i;
				mx->Tj[pos]=dim->ylen*(1)+i+1;
				mx->Tx[pos]=dJdxir;
				pos++;

				mx->Ti[pos]=i+dim->ylen*(1);
				mx->Tj[pos]=i+1;
				mx->Tx[pos]=dJdphir;
				pos++;

				//hole
				mx->Ti[pos]=dim->ylen*(1+1)+i;
				mx->Tj[pos]=dim->ylen*(1+1)+i+1;
				mx->Tx[pos]=dJpdxipr;
				pos++;

				mx->Ti[pos]=i+dim->ylen*(1+1);
				mx->Tj[pos]=i+1;
				mx->Tx[pos]=dJpdphir;
				pos++;




			}

			//Possion
			gdouble build=0.0;
			//if ((in->interfaceleft==TRUE)&&(i==0))
			//{
			//	build= -0.0;
			//}else
			//if ((in->interfaceright==TRUE)&&(i==dim->ylen-1))
			//{
			//	build= -0.0;
			//}else
			//{
				build= -(deriv);

				build+= -(-(pc-nc+Nad));

				for (band=0;band<dim->srh_bands;band++)
				{
					build+= -(-Qe*(in->pt[z][x][i][band]-in->nt[z][x][i][band]));
				}

				//build+= -(-Qe*in->Nad[i]);

			//}
			mx->b[i]=build;

			build=0.0;
			build= -((Jnr-Jnl)/(dylh+dyrh)-Rtrapn-Rfree);


			//getchar();
			build-=Gn;
			mx->b[dim->ylen*(1)+i]=build;

			//hole
			build=0.0;
			build= -((Jpr-Jpl)/(dylh+dyrh)+Rtrapp+Rfree);

			build-= -Gp;


			mx->b[dim->ylen*(1+1)+i]=build;

			if (in->ntrapnewton==TRUE)
			{
				for (band=0;band<dim->srh_bands;band++)
				{
					mx->b[dim->ylen*(1+1+1+band)+i]= -(in->newton_dntrap[band]);
				}
			}

			if (in->ptrapnewton==TRUE)
			{
				for (band=0;band<dim->srh_bands;band++)
				{
					mx->b[dim->ylen*(1+1+1+dim->srh_bands+band)+i]= -(in->newton_dptrap[band]);
				}

			}

		}


if (pos>mx->nz)
{
	ewe(sim,"Error %d %d %d\n",pos,mx->nz,in->kl_in_newton);
}

//solver_dump_matrix_comments(in->M,pos,in->Tdebug,mx->Ti[,in->Tj, in->Tx,mx->b,"");
//getchar();
//fclose(file_j);
//getchar();

}

gdouble get_cur_error(struct simulation *sim, struct device *in)
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
int band=0;
struct dimensions *dim=&in->ns.dim;
struct matrix *mx=&in->mx;

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

		n+=gfabs(mx->b[dim->ylen*(1)+i]);
		p+=+gfabs(mx->b[dim->ylen*(1+1)+i]);

		if (in->ntrapnewton==TRUE)
		{
			for (band=0;band<dim->srh_bands;band++)
			{
				ttn+=gfabs(mx->b[dim->ylen*(1+1+1+band)+i]);
			}
		}

		if (in->ptrapnewton==TRUE)
		{
			for (band=0;band<dim->srh_bands;band++)
			{
				ttp+=gfabs(mx->b[dim->ylen*(1+1+1+dim->srh_bands+band)+i]);
			}
		}


}


gdouble tot=phi+n+p+x+te+th+tl+ttn+ttp+i0;
if (isnan( tot))
{
	printf_log(sim,"%Le %Le %Le %Le %Le %Le %Le %Le %Le\n",phi,n,p,x,te,th,tl,ttn,ttp);
	//dump_matrix(sim,in->M,in->N,mx->Ti[,in->Tj, in->Tx,mx->b,"");

	ewe(sim,"nan detected in newton solver\n");
}

return tot;
}

gdouble get_abs_error(struct device *in)
{
int i;
gdouble tot=0.0;
struct matrix *mx=&in->mx;

for (i=0;i<mx->M;i++)
{
	tot+=gfabs(mx->b[i]);
}

return tot;
}
void solver_cal_memory(struct device *in,int *ret_N,int *ret_M,int dim_)
{
int i=0;
int N=0;
int M=0;
struct dimensions *dim=&in->ns.dim;
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


*ret_N=N;
*ret_M=M;
}


void dllinternal_solver_free_memory(struct simulation *sim,struct device *in)
{
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


//for (i=0;i<in->N;i++)
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
in->newton_dphidptrap=NULL;


in->newton_ntlast=NULL;
in->newton_ptlast=NULL;


}

int dllinternal_solve_cur(struct simulation *sim,struct device *in,int z, int x)
{
FILE *file;
T0=sim->T0;//300.0;
D0=sim->D0;//243.75;
n0=sim->n0;//1e20;
phi0=((kb/Qe)*T0);
l0=sqrtl(epsilon0*(kb/Qe)*T0/(Qe*n0));
r0=n0*D0/l0/l0;
r_bi0=D0/(n0*l0*l0);


gdouble last_J=get_J(in);
gdouble delta_J=0.0;

gdouble error=0.0;
int ittr=0;
if (in->solver_verbosity>SOLVER_VERBOSITY_AT_END)
{
	printf_log(sim,"Solve cur\n");
}





#ifdef only
only_update_thermal=FALSE;
#endif
//in->enable_back=FALSE;
int stop=FALSE;
int thermalrun=0;
gdouble check[10];
int cpos=0;
char temp[PATH_MAX];
struct newton_state *ns=&(in->ns);
struct matrix *mx=&in->mx;

gdouble abs_error=0.0;
	do
	{

		fill_matrix(sim,in,z,x);
		abs_error=get_abs_error(in);

		plot_now(sim,in,"plot");
	//solver_dump_matrix(in->M,in->N,mx->Ti[,in->Tj, in->Tx,mx->b);
//getchar();

			if (in->stop==TRUE)
			{
				break;
			}

			matrix_solve(sim,&(in->msm),mx);

			update_solver_vars(sim,in,z,x,TRUE);

			//solver_dump_matrix(in->M,in->N,mx->Ti[,in->Tj, in->Tx,mx->b);

			//getchar();
		delta_J=fabs(last_J-get_J(in));
		last_J=get_J(in);
		error=get_cur_error(sim,in);

		//thermalrun++;
		if (thermalrun==40) thermalrun=0;
		//update(in);
//getchar();

		if (in->solver_verbosity>SOLVER_VERBOSITY_AT_END)
		{
			printf_log(sim,"%d error = %Le  abs_error = %Le dJ=%Le %Le I=%Le",ittr,error,abs_error,delta_J,contact_get_active_contact_voltage(sim,in),get_J(in));

			printf_log(sim,"\n");
		}
		error=delta_J;
		ns->last_error=error;
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

		if (in->newton_clever_exit==TRUE)
		{
			check[cpos]=error;
			cpos++;

			if (cpos>10)
			{
				cpos=0;
			}

			if (ittr>=in->newton_min_itt)
			{
					if ((check[0]<error)||(check[1]<error))
					{
						stop=TRUE;
					}
			}
		}



	}while(stop==FALSE);

in->newton_last_ittr=ittr;

//if (error>1e-3)
//{
//	printf_log(sim,"warning: The solver has not converged very well.\n");
//}

//getchar();

if (get_dump_status(sim,dump_newton)==TRUE)
{
	join_path(2,temp,in->output_path,"solver");
	dump_1d_slice(sim,in,temp);
}

//plot_now(in,"plot");
//getchar();
in->odes+=mx->M;
//getchar();
return 0;
}

void dllinternal_solver_realloc(struct simulation *sim,struct device *in, int dim_)
{
int N=0;
int M=0;
long double* dtemp=NULL;
int *itemp=NULL;
struct dimensions *dim=&in->ns.dim;
struct matrix *mx=&in->mx;

solver_cal_memory(in,&N,&M,dim_);


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
