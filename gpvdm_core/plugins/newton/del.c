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



void fill_matrix(struct simulation *sim,struct device *in,int z,int x,int debug)
{
long double Jnl=0.0;
long double Jnr=0.0;
long double Jpl=0.0;
long double Jpr=0.0;

long double Dnl=0.0;
long double Dnc=0.0;
long double Dnr=0.0;
long double Dpl=0.0;
long double Dpc=0.0;
long double Dpr=0.0;

long double nl=0.0;
long double nc=0.0;
long double nr=0.0;

long double pl=0.0;
long double pc=0.0;
long double pr=0.0;

long double xil=0.0;
long double xir=0.0;
long double xipl=0.0;
long double xipr=0.0;

long double dJpdxipl=0.0;
long double dJpdxipc=0.0;
long double dJpdxipr=0.0;

long double dnl=0.0;
long double dnc=0.0;
long double dnr=0.0;

long double dpl=0.0;
long double dpc=0.0;
long double dpr=0.0;

long double munl=0.0;
long double munc=0.0;
long double munr=0.0;

long double mupl=0.0;
long double mupc=0.0;
long double mupr=0.0;


long double wnl=0.0;
long double wnc=0.0;
long double wnr=0.0;

long double wpl=0.0;
long double wpc=0.0;
long double wpr=0.0;

long double dJdxil=0.0;
long double dJdxic=0.0;
long double dJdxir=0.0;

long double dJdphil=0.0;
long double dJdphic=0.0;
long double dJdphir=0.0;

long double dJpdphil=0.0;
long double dJpdphic=0.0;
long double dJpdphir=0.0;


long double dphidxic=0.0;
long double dphidxipc=0.0;

struct matrix *mx=&(in->mx);
struct heat *thermal=&(in->thermal);

int band=0;

update_y_array(sim,in,z,x);
if (debug==TRUE)
{
	return;
}
//FILE *file_j =fopen("myj.dat","w");
//getchar();
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
gdouble Tec=0.0;
gdouble Ter=0.0;

gdouble Thl=0.0;
gdouble Thc=0.0;
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
//gdouble one0_l=0.0;
//gdouble one0_r=0.0;



gdouble Rtrapn=0.0;
gdouble Rtrapp=0.0;

gdouble dJndphil_leftl=0.0;
gdouble dJndphil_leftc=0.0;
gdouble dJpdphil_leftl=0.0;
gdouble dJpdphil_leftc=0.0;
gdouble dphil_left=0.0;
gdouble dJndxil_leftc=0.0;
gdouble dJpdxipl_leftc=0.0;

gdouble dJdxic_rightc=0.0;
gdouble dJpdxipc_rightc=0.0;
gdouble dJpdphi_rightc=0.0;
gdouble dJdphi_rightc=0.0;

//Interface
gdouble Bfree=0.0;
gdouble Binterface_c=0.0;
gdouble Binterface_l=0.0;
//Tunnel
long double interface_Ge_l=0.0;
long double interface_Ge_c=0.0;
long double interface_Gh_l=0.0;
long double interface_Gh_c=0.0;


//Auger
gdouble Cn=0.0;
gdouble Cp=0.0;
long double Rauger=0.0;

//SS SRH
long double Rss_srh=0.0;
long double n1=0.0;
long double p1=0.0;
long double tau_n=0.0;
long double tau_p=0.0;

gdouble nleq=0.0;
gdouble nceq=0.0;
gdouble nreq=0.0;

gdouble pleq=0.0;
gdouble pceq=0.0;
gdouble preq=0.0;

gdouble Rfree=0.0;

gdouble Rinterface_pc=0.0;
gdouble Rinterface_nc=0.0;


//gdouble nc0_l=0.0;
//gdouble dnc0_l=0.0;
//gdouble pc0_l=0.0;
//gdouble dpc0_l=0.0;

//gdouble nc0_r=0.0;
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
long double dphir_right=0.0;
long double dJndphir_rightr=0.0;
long double dJpdphir_rightr=0.0;
long double dJpdphil_rightc=0.0;
long double dJndphil_rightc=0.0;
long double dJndxir_rightc=0.0;
long double dJpdxipr_rightc=0.0;
long double didphil=0.0;
long double dJdphi_leftc=0.0;
long double dJpdphi_leftc=0.0;
long double dJdxic_leftc=0.0;
long double dJpdxipc_leftc=0.0;
long double didxil=0.0;
long double didxipl=0.0;

long double vl_e=-1.0;
long double vl_h=-1.0;

long double vr_e=-1.0;
long double vr_h=-1.0;


//long double nl0=-1.0;
//long double pl0=-1.0;

//long double p_sh=-1.0;
//long double dp_sh=-1.0;

//long double Jnl_sh=-1.0;
//long double dJnlsh=-1.0;
//long double dJnlsh_c=-1.0;

//long double Jpl_sh=-1.0;
//long double dJplsh=-1.0;
//long double dJplsh_c=-1.0;

struct newton_state *ns=&(in->ns);
struct dimensions *dim=&in->ns.dim;

int contact_left=in->contacts[in->n_contact_y0[z][x]].type;
int contact_right=in->contacts[in->n_contact_y1[z][x]].type;

long double	dJdxic_imag=0.0;
long double dJpdxipc_imag=0.0;

long double Bn;
long double dBn;
long double Bp;
long double dBp;

long double pci=0.0;
long double dpci=0.0;

long double pli=-1.0;

long double pri=-1.0;

long double dpli=-1.0;
long double dpri=-1.0;

long double pci_last=-1.0;

long double muli=-1.0;
long double muci=-1.0;
long double muri=-1.0;

long double Dpli=-1.0;
long double Dpci=-1.0;
long double Dpri=-1.0;

long double Jpli=-1.0;
long double Jpri=-1.0;

long double xipli=-1.0;
long double xipri=-1.0;

long double dJpldpli=-1.0;
long double dJpldpci=-1.0;
long double dJpldpri=-1.0;

long double dJprdpli=-1.0;
long double dJprdpci=-1.0;
long double dJprdpri=-1.0;

long double dJpli=-1.0;
long double dJpri=-1.0;
long double dJpci=-1.0;

long double Nion_build=-1.0;
int Nion_offset=get_offset_nion(in);
int offset_Je=get_offset_Je(in);
int offset_Jh=get_offset_Jh(in);
int offset_srh_e=get_offset_srh_e(in);
int offset_srh_h=get_offset_srh_h(in);
int offset_kcl=get_offset_kcl(in);

long double Ns_build=-1.0;
long double dNs_build=-1.0;
long double dNs_build_dNt=-1.0;
long double dNs_build_dxic=-1.0;

int Ns_offset=get_offset_singlet_Ns(in);

long double Nt_build=-1.0;
long double dNt_build=-1.0;
long double dNt_build_dNs=-1.0;
int Nt_offset=get_offset_singlet_Nt(in);

long double Nsd_build=-1.0;
long double dNsd_build_dNs=-1.0;
long double dNsd_build=-1.0;
long double dNsd_build_dNtd=-1.0;

int Nsd_offset=get_offset_singlet_Nsd(in);

long double Ntd_build=-1.0;
long double dNtd_build=-1.0;
long double dNtd_build_dNsd=-1.0;
int Ntd_offset=get_offset_singlet_Ntd(in);

long double Nho_build=-1.0;
long double dNho_build=-1.0;
long double dNho_build_dNsd=-1.0;
int Nho_offset=get_offset_singlet_Nho(in);

long double dphi_dNion=0.0;

long double dJpdphil_i=0.0;
long double dJpdphic_i=0.0;
long double dJpdphir_i=0.0;

int ion_pos=0;

int *Tx;
int *Ty;
long double *Ta;
Tx=mx->Tj;
Ty=mx->Ti;
Ta=mx->Tx;

struct perovskite *ion=&(in->mobileion);
struct singlet *sing=&(in->sing);

struct shape *s;
	//if (in->interfaceleft==TRUE)
	//{
	//	ns->phi[z][x][0]=in->Vapplied_y0[z][x];
	//}


		pos=0;
		for (i=0;i<dim->ylen;i++)
		{

			s=in->obj_zxy[z][x][i]->s;
			//printf("density=%Le\n",get_dos_ion_density(in,in->imat[z][x][i]));
			//printf("mobility=%Le\n",get_dos_ion_mobility(in,in->imat[z][x][i]));

			if (i==0)
			{

				phil=in->Vapplied_y0[z][x];

				yl=dim->y[0]-(dim->y[1]-dim->y[0]);
				//printf("%Le %Le %Le\n",yl,dim->y[0],dim->y[1]);
				//getchar();
//				Tll=thermal->Ty0;
				Tel=in->Te[z][x][i];//thermal->Ty0;
				Thl=in->Th[z][x][i];//thermal->Ty0;

				Ecl= -in->Xi[z][x][0]-phil;
				Evl= Ecl-in->Eg[z][x][0];
				epl=in->epsilonr_e0[z][x][0];//*epsilon0;

				xnl=in->Fi[z][x][0];
				tnl=in->Xi[z][x][0];
				one=xnl+tnl;

				vl_e=in->contacts[in->n_contact_y0[z][x]].ve0;
				get_n_den(s,one,Tel,&nl,&dnl,&wnl);
				//printf("nl=%Le %Le %Le %Le\n",Tel,nl,dnl,wnl);
				//getchar();
				//dnl=get_dn_den(in,one,Tel,in->imat[z][x][i]);
				//wnl=get_n_w(in,one,Tel,in->imat[z][x][i]);



				munl=in->mun_y[z][x][0];


				xpl= -in->Fi[z][x][0];
				tpl=(in->Xi[z][x][0]+in->Eg[z][x][0]);
				one=xpl-tpl;

				vl_h=in->contacts[in->n_contact_y0[z][x]].vh0;
				get_p_den(s,one,Thl,&pl,&dpl,&wpl);
				//=get_dp_den(in,one,Thl,in->imat[z][x][i]);
				//wpl=get_p_w(in,one,Thl,in->imat[z][x][i]);


				mupl=in->mup_y[z][x][0];
				Binterface_l=in->interface_B[z][x][0];
				interface_Ge_l=in->interface_Ge[z][x][0];
				interface_Gh_l=in->interface_Gh[z][x][0];

//				kll=in->kl[i];
				pleq=pl;
				nleq=nl;

			}else
			{
//				Dexl=in->Dex[i-1];
//				exl=in->ex[z][x][i-1];
				phil=ns->phi[z][x][i-1];
				yl=dim->y[i-1];
//				Tll=in->Tl[z][x][i-1];
				Tel=in->Te[z][x][i-1];
				Thl=in->Th[z][x][i-1];
				Ecl=in->Ec[z][x][i-1];
				Evl=in->Ev[z][x][i-1];


				nl=in->n[z][x][i-1];
				dnl=in->dn[z][x][i-1];


				wnl=in->wn[z][x][i-1];
				wpl=in->wp[z][x][i-1];
				//printf("%Le\n",wnl/1.38064852e-23/300/(3/2));
				pl=in->p[z][x][i-1];
				dpl=in->dp[z][x][i-1];



				munl=in->mun_y[z][x][i-1];
				mupl=in->mup_y[z][x][i-1];


				epl=in->epsilonr_e0[z][x][i-1];//*epsilon0;
				Binterface_l=in->interface_B[z][x][i-1];
				interface_Ge_l=in->interface_Ge[z][x][i-1];
				interface_Gh_l=in->interface_Gh[z][x][i-1];

				pleq=in->pfequlib[z][x][i-1];
				nleq=in->nfequlib[z][x][i-1];
//				kll=in->kl[i-1];
			}

			Ecc=(-in->Xi[z][x][i]-ns->phi[z][x][i]);
			Evc=Ecc-in->Eg[z][x][i];
			//printf("%Le\n",ns->Nion[z][x][i]);
			//getchar();
			if (i==(dim->ylen-1))
			{

//				Dexr=in->Dex[i];
//				exr=0.0;
				//phir=in->V_y1;

				phir=(in->V_y1[z][x]+in->Vapplied_y1[z][x]);


				yr=dim->y[i]+(dim->y[i]-dim->y[i-1]);
//				Tlr=thermal->Ty1;
				//Ter=thermal->Ty1;
				//Thr=thermal->Ty1;
				Ter=in->Te[z][x][i];//thermal->Ty0;
				Thr=in->Th[z][x][i];//thermal->Ty0;

				Ecr= -in->Xi[z][x][i]-phir;
				Evr= Ecr-in->Eg[z][x][i];


				xnr=(in->V_y1[z][x]+in->Fi[z][x][i]);
				tnr=(in->Xi[z][x][i]);


				one=xnr+tnr;

				vr_e=in->contacts[in->n_contact_y1[z][x]].ve0;
				get_n_den(s,one,Ter,&nr,&dnr,&wnr);
				//dnr=get_dn_den(in,one,Ter,in->imat[z][x][i]);
				//wnr=get_n_w(in,one,Ter,in->imat[z][x][i]);



				xpr= -(in->V_y1[z][x]+in->Fi[z][x][i]);
				tpr=(in->Xi[z][x][i]+in->Eg[z][x][i]);

				one=xpr-tpr;

				vr_h=in->contacts[in->n_contact_y1[z][x]].vh0;
				get_p_den(s,one,Thr,&pr,&dpr,&wpr);
				//printf("%Le %Le %Le %Le %Le %Le",Thr,pr,in->Xi[z][x][i],in->Eg[z][x][i],in->V_y1[z][x],in->Fi[z][x][i]);
				//getchar();
				//dpr=get_dp_den(in,one,Thr,in->imat[z][x][i]);
				//wpr=get_p_w(in,one,Thr,in->imat[z][x][i]);

				munr=in->mun_y[z][x][i];
				mupr=in->mup_y[z][x][i];

				epr=in->epsilonr_e0[z][x][i];//*epsilon0;
//				klr=in->kl[i];
				nreq=nr;
				preq=pr;
				//printf("%Le %Le\n",pr,pri);
				//getchar();
			}else
			{

//				Dexr=in->Dex[z][x][i+1];
//				exr=in->ex[z][x][i+1];
				phir=ns->phi[z][x][i+1];
				yr=dim->y[i+1];
//				Tlr=in->Tl[z][x][i+1];
				Ter=in->Te[z][x][i+1];
				Thr=in->Th[z][x][i+1];

				Ecr=in->Ec[z][x][i+1];
				Evr=in->Ev[z][x][i+1];


				nr=in->n[z][x][i+1];
				dnr=in->dn[z][x][i+1];

				wnr=in->wn[z][x][i+1];
				wpr=in->wp[z][x][i+1];

				pr=in->p[z][x][i+1];
				dpr=in->dp[z][x][i+1];
				munr=in->mun_y[z][x][i+1];
				mupr=in->mup_y[z][x][i+1];

				epr=in->epsilonr_e0[z][x][i+1];//*epsilon0;

				nreq=in->nfequlib[z][x][i+1];
				preq=in->pfequlib[z][x][i+1];
//				klr=in->kl[i+1];
			}

			dJdxipc=0.0;
			dJpdxic=0.0;

			epc=in->epsilonr_e0[z][x][i];//*epsilon0;

//			exc=in->ex[z][x][i];
//			Dexc=in->Dex[z][x][i];
			yc=dim->y[i];
			dyl=yc-yl;
			dyr=yr-yc;
			ddh=(dyl+dyr)/2.0;
			gdouble dylh=dyl/2.0;
			gdouble dyrh=dyr/2.0;

//			dh=(dyl+dyr);
			phic=ns->phi[z][x][i];
//			Tlc=in->Tl[z][x][i];
			Tec=in->Te[z][x][i];
			Thc=in->Th[z][x][i];

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



				nc=in->n[z][x][i];
				//printf("%Le\n",in->Gn[z][x][i]);
				//getchar();
				pc=in->p[z][x][i];

				dnc=in->dn[z][x][i];
				dpc=in->dp[z][x][i];

//				dncdphic=in->dndphi[z][x][i];
//				dpcdphic=in->dpdphi[z][x][i];

				Bfree=in->B[z][x][i];

				//Auger
				Cn=in->Cn[z][x][i];
				Cp=in->Cp[z][x][i];

				//SS SRH
				n1=in->n1[z][x][i];
				p1=in->p1[z][x][i];
				tau_n=in->tau_n[z][x][i];
				tau_p=in->tau_p[z][x][i];

				Binterface_c=in->interface_B[z][x][i];

				interface_Ge_c=in->interface_Ge[z][x][i];
				interface_Gh_c=in->interface_Gh[z][x][i];

				Nad=in->Nad[z][x][i];


				nceq=in->nfequlib[z][x][i];
				pceq=in->pfequlib[z][x][i];

				Rfree=Bfree*(nc*pc-nceq*pceq);
				in->Rfree[z][x][i]=Rfree;

				//https://www.iue.tuwien.ac.at/phd/ayalew/node73.html
				if (in->auger_enabled==TRUE)
				{
					Rauger=(Cn*nc+Cp*pc)*(nc*pc-nceq*pceq);
					in->Rauger[z][x][i]=Rauger;
				}

				//https://www.iue.tuwien.ac.at/phd/ayalew/node72.html
				if (in->ss_srh_enabled==TRUE)
				{
					if ((tau_n>0.0)||(tau_p>0.0))
					{
						Rss_srh=(nc*pc-nceq*pceq)/(tau_p*(nc+n1)+tau_n*(pc+p1));
					}
				}

				Rinterface_pc=Binterface_c*(pc*nr-pceq*nreq);
				Rinterface_nc=Binterface_l*(nc*pl-nceq*pleq);

				in->interface_R[z][x][i]=Rinterface_pc+Rinterface_nc;

				//printf("%d %Le %Le %Le\n",i,Rinterface_pc,Rinterface_nc,Rfree);
//			klc=in->kl[i];
			nlast=in->nlast[z][x][i];
			plast=in->plast[z][x][i];

			for (band=0;band<dim->srh_bands;band++)
			{
				in->newton_ntlast[band]=in->ntlast[z][x][i][band];
				in->newton_ptlast[band]=in->ptlast[z][x][i][band];
			}

			dt=in->dt;

//	R=in->R[z][x][i];
	Gn=in->Gn[z][x][i];
	Gp=in->Gp[z][x][i];
	//printf("%d %Le %Le\n",i,in->Gn[z][x][i],in->Gp[z][x][i]);
	e0=(epl+epc)/2.0;
	e1=(epc+epr)/2.0;

//	kl0=(klc+kll)/2.0;
//	kl1=(klr+klc)/2.0;

	dphil= -e0/dyl/ddh;
	dphic= e0/dyl/ddh+e1/dyr/ddh;
	dphir= -e1/dyr/ddh;

	gdouble dphil_d=dphil;
	gdouble dphic_d=dphic;
	gdouble dphir_d=dphir;

	deriv=phil*dphil+phic*dphic+phir*dphir;
	//dphic_d+=-Qe*(-dnc);

	dphidxic=Qe*(dnc);
	dphidxipc= -Qe*(dpc);


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

			//dphidxipc+= -Qe*(in->dpt[i]);
		}
	}




//	G=in->G[i];



			xil=Qe*2.0*(3.0/2.0)*(Ecc-Ecl)/((wnc+wnl));
			xir=Qe*2.0*(3.0/2.0)*(Ecr-Ecc)/((wnr+wnc));

			//gdouble dxil= -Qe*2.0*(3.0/2.0)*(Ecc-Ecl)/pow((wnc+wnl),2.0);
			//gdouble dxir= -Qe*2.0*(3.0/2.0)*(Ecr-Ecc)/pow((wnr+wnc),2.0);

			xipl=Qe*2.0*(3.0/2.0)*(Evc-Evl)/(wpc+wpl);
			xipr=Qe*2.0*(3.0/2.0)*(Evr-Evc)/(wpr+wpc);

			if (thermal->newton_enable_external_thermal==TRUE)
			{
				xil+=2.0*((Tec-Tel)/(Tec+Tel));
				xir+=2.0*((Ter-Tec)/(Ter+Tec));

				xipl+=-2.0*((Thc-Thl)/(Thc+Thl));
				xipr+=-2.0*((Thr-Thc)/(Thr+Thc));
			}

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

			

			/*
			Jnl=(Dnl/dyl)*(B(-xil)*nc-B(xil)*nl);
			dJnldxil_l= -(Dnl/dyl)*(B(xil)*dnl);
			dJnldxil_c=(Dnl/dyl)*B(-xil)*dnc;

			gdouble dJnldphi_l= -(munl/dyl)*(dB(-xil)*nc+dB(xil)*nl);
			gdouble dJnldphi_c=(munl/dyl)*(dB(-xil)*nc+dB(xil)*nl);

			Jnr=(Dnr/dyr)*(B(-xir)*nr-B(xir)*nc);
			dJnrdxir_c= -(Dnr/dyr)*(B(xir)*dnc);
			dJnrdxir_r=(Dnr/dyr)*(B(-xir)*dnr);

			gdouble dJnrdphi_c=(munr/dyr)*(-dB(-xir)*nr-dB(xir)*nc);
			gdouble dJnrdphi_r=(munr/dyr)*(dB(-xir)*nr+dB(xir)*nc);

			Jpl=(Dpl/dyl)*(B(-xipl)*pl-B(xipl)*pc);
			dJpldxipl_l=(Dpl/dyl)*(B(-xipl)*dpl);
			dJpldxipl_c= -(Dpl/dyl)*B(xipl)*dpc;

			gdouble dJpldphi_l= -((mupl)/dyl)*(dB(-xipl)*pl+dB(xipl)*pc);
			gdouble dJpldphi_c=((mupl)/dyl)*(dB(-xipl)*pl+dB(xipl)*pc);

			Jpr=(Dpr/dyr)*(B(-xipr)*pc-B(xipr)*pr);
			dJprdxipr_c=(Dpr/dyr)*(B(-xipr)*dpc);
			dJprdxipr_r= -(Dpr/dyr)*(B(xipr)*dpr);

			gdouble dJprdphi_c= -(mupr/dyr)*(dB(-xipr)*pc+dB(xipr)*pr);
			gdouble dJprdphi_r=(mupr/dyr)*(dB(-xipr)*pc+dB(xipr)*pr);
			*/

			
			if (i==0)
			{
				BdB(&Bn,&dBn,-xil);
				BdB(&Bp,&dBp,xil);
			}else
			{
				Bn=ns->By_xi_neg[z][x][i-1];
				dBn=ns->dBy_xi_neg[z][x][i-1];

				Bp=ns->By_xi_plus[z][x][i-1];
				dBp=ns->dBy_xi_plus[z][x][i-1];
			}

			Jnl=(Dnl/dyl)*(Bn*nc-Bp*nl);
			dJnldxil_l= -(Dnl/dyl)*(Bp*dnl);
			dJnldxil_c=(Dnl/dyl)*Bn*dnc;

			gdouble dJnldphi_l= -(munl/dyl)*(dBn*nc+dBp*nl);
			gdouble dJnldphi_c=(munl/dyl)*(dBn*nc+dBp*nl);


			BdB(&Bn,&dBn,-xir);
			BdB(&Bp,&dBp,xir);

			ns->By_xi_plus[z][x][i]=Bp;
			ns->dBy_xi_plus[z][x][i]=dBp;
			ns->By_xi_neg[z][x][i]=Bn;
			ns->dBy_xi_neg[z][x][i]=dBn;

			Jnr=(Dnr/dyr)*(Bn*nr-Bp*nc);
			dJnrdxir_c= -(Dnr/dyr)*(Bp*dnc);
			dJnrdxir_r=(Dnr/dyr)*(Bn*dnr);

			gdouble dJnrdphi_c=(munr/dyr)*(-dBn*nr-dBp*nc);
			gdouble dJnrdphi_r=(munr/dyr)*(dBn*nr+dBp*nc);
			
			if (i==0)
			{
				BdB(&Bn,&dBn,-xipl);
				BdB(&Bp,&dBp,xipl);
			}else
			{
				Bn=ns->By_xip_neg[z][x][i-1];
				dBn=ns->dBy_xip_neg[z][x][i-1];

				Bp=ns->By_xip_plus[z][x][i-1];
				dBp=ns->dBy_xip_plus[z][x][i-1];

			}

			Jpl=(Dpl/dyl)*(Bn*pl-Bp*pc);
			dJpldxipl_l=(Dpl/dyl)*(Bn*dpl);
			dJpldxipl_c= -(Dpl/dyl)*Bp*dpc;

			gdouble dJpldphi_l= -((mupl)/dyl)*(dBn*pl+dBp*pc);
			gdouble dJpldphi_c=((mupl)/dyl)*(dBn*pl+dBp*pc);

			BdB(&Bn,&dBn,-xipr);
			BdB(&Bp,&dBp,xipr);

			ns->By_xip_plus[z][x][i]=Bp;
			ns->dBy_xip_plus[z][x][i]=dBp;
			ns->By_xip_neg[z][x][i]=Bn;
			ns->dBy_xip_neg[z][x][i]=dBn;

			Jpr=(Dpr/dyr)*(Bn*pc-Bp*pr);
			dJprdxipr_c=(Dpr/dyr)*(Bn*dpc);
			dJprdxipr_r= -(Dpr/dyr)*(Bp*dpr);

			gdouble dJprdphi_c= -(mupr/dyr)*(dBn*pc+dBp*pr);
			gdouble dJprdphi_r=(mupr/dyr)*(dBn*pc+dBp*pr);

			if (in->interfaces_tunnels_e>0)
			{
				//Jnr=(Dnr/dyr)*(Bn*nr-Bp*nc);
				if (interface_Ge_c>0.0)
				{
					Jnr+=-interface_Ge_c*((nc-nceq)-(nr-nreq));
					dJnrdxir_c+=-interface_Ge_c*(dnc);
					dJnrdxir_r+=-interface_Ge_c*(-dnr);

					dJnrdphi_c+=0.0;//interface_Ge_c*(-dnc);
					dJnrdphi_r+=0.0;//interface_Ge_c*(dnr);
				}
	
				//Jnl=(Dnl/dyl)*(Bn*nc-Bp*nl);
				if (interface_Ge_l>0.0)
				{
					Jnl+=-interface_Ge_l*((nl-nleq)-(nc-nceq));
					dJnldxil_l+=-interface_Ge_l*(dnl);
					dJnldxil_c+=-interface_Ge_l*(-dnc);

					dJnldphi_l+=0.0;//interface_Ge_l*(-dnl);
					dJnldphi_c+=0.0;//interface_Ge_l*(dnc);
				}
				//printf("%ld %Le %Le %Le\n",i,interface_Ge_l,Jnl,interface_Ge_l*((nl-nleq)-(nc-nceq)));
			}

			if (in->interfaces_tunnels_h>0)
			{
				if (interface_Gh_c>0.0)
				{
					Jpr+=interface_Gh_c*((pc-pceq)-(pr-preq));
					dJprdxipr_c+=interface_Gh_c*(dpc);
					dJprdxipr_r+=interface_Gh_c*(-dpr);

					dJprdphi_c+= 0.0;//interface_Gh_c*(dpc);
					dJprdphi_r+=0.0;//interface_Gh_c*(-dpr);
				}

				if (interface_Gh_l>0.0)
				{
					Jpl+=interface_Gh_c*((pl-pleq)-(pc-pceq));
					dJpldxipl_l+=interface_Gh_c*(dpl);
					dJpldxipl_c+=interface_Gh_c*(-dpc);

					dJpldphi_l+=0.0;//interface_Gh_c*(dpl);
					dJpldphi_c+=0.0;//interface_Gh_c*(-dpc);
				}

			}


			if (i==0)
			{
				//printf("%Le %Le %Le %Le %Le %Le\n",Dnl,dyl,Bn,nc,Bp,nl);
				/*if (contact_left==contact_schottky)
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

				}*/

				in->Jn_y0[z][x]=Qe*Jnl;
				in->Jp_y0[z][x]=Qe*Jpl;

				//getchar();
			}

			if (i==dim->ylen-1)
			{
				//printf("%d\n",contact_right);
				//getchar();
				/*if (contact_right==contact_schottky)
				{
					
					Jnr=-vr_e*(nc-nr);
					dJnrdxir_r= -vr_e*(-dnr);
					dJnrdxir_c= -vr_e*(dnc);
					//printf("%Le %Le\n",Jnl,vr_e*(nl0-nl));

					dJnrdphi_r= 0.0;//-vr_e*(-dnr);
					dJnrdphi_c= 0.0;//-vr_e*(dnc);

					Jpr=vr_h*(pc-pr);
					dJprdxipr_r= vr_h*(-dpr);
					dJprdxipr_c= vr_h*(dpc);
					//printf("%Le %Le\n",Jnl,vr_e*(nl0-nl));

					dJprdphi_r= 0.0;//vr_h*(-dpl);
					dJprdphi_c= 0.0;//vr_h*(dpc);

					//dyrh=0.0;
				}*/

				in->Jn_y1[z][x]=Qe*Jnr;
				in->Jp_y1[z][x]=Qe*Jpr;

			}
			/*if ((in->boundry_y0==contact_schottky)&&(i==0))
			{
				in->Jn[z][x][i]=in->Jn_y0[z][x];
				in->Jp[z][x][i]=in->Jp_y0[z][x];
			}else*/
			{
				//printf("%Le\n",Jnl);
				in->Jn[z][x][i]=Qe*(Jnl+Jnr)/2.0;
				in->Jp[z][x][i]=Qe*(Jpl+Jpr)/2.0;
			}


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

			if (in->auger_enabled==TRUE)
			{
				if ((Cn>0.0)||(Cp>0.0))
				{
				dJdxic+= -((Cn*dnc)*(nc*pc-nceq*pceq)+(Cn*nc+Cp*pc)*(dnc*pc));
				dJdxipc+= -((Cp*dpc)*(nc*pc-nceq*pceq)+(Cn*nc+Cp*pc)*(nc*dpc));

				dJpdxipc+=(Cp*dpc)*(nc*pc-nceq*pceq)+(Cn*nc+Cp*pc)*(nc*dpc);
				dJpdxic+=(Cn*dnc)*(nc*pc-nceq*pceq)+(Cn*nc+Cp*pc)*(dnc*pc);

				//dJdphic+= -((Cn*dnc)*(nc*pc-nceq*pceq)+(Cn*nc+Cp*pc)*(dnc*pc));
				//dJpdphic+=((Cp*dpc)*(nc*pc-nceq*pceq)+(Cn*nc+Cp*pc)*(nc*dpc));
				}
			}

			if (in->ss_srh_enabled==TRUE)
			{
				if ((tau_n>0.0)||(tau_p>0.0))
				{
					dJdxic+=  -((dnc*pc)/(tau_p*(nc+n1)+tau_n*(pc+p1))-(nc*pc-nceq*pceq)*(tau_p*dnc)/powl((tau_p*(nc+n1)+tau_n*(pc+p1)),2.0));
					dJdxipc+= -((nc*dpc)/(tau_p*(nc+n1)+tau_n*(pc+p1))-(nc*pc-nceq*pceq)*(tau_n*dpc)/powl((tau_p*(nc+n1)+tau_n*(pc+p1)),2.0));

					dJpdxic+=((dnc*pc)/(tau_p*(nc+n1)+tau_n*(pc+p1))-(nc*pc-nceq*pceq)*(tau_p*dnc)/powl((tau_p*(nc+n1)+tau_n*(pc+p1)),2.0));
					dJpdxipc+=((nc*dpc)/(tau_p*(nc+n1)+tau_n*(pc+p1))-(nc*pc-nceq*pceq)*(tau_n*dpc)/powl((tau_p*(nc+n1)+tau_n*(pc+p1)),2.0));
				}
			}
			//Binterface*(pc*nr-pceq*nreq);
			//
			long double dJpdxinr=0.0;
			//long double dJpdphir=0.0;

			int insert_couple_right=FALSE;
			//int insert_couple_SRH_right=FALSE;

			long double dJdxipl=0.0;
			int insert_couple_left=FALSE;
			//Rinterface_nc=Binterface_l*(nc*pl-nceq*pleq);


			if (i==0)
			{
				dJdxic_leftc= dJnldxil_c;
				dJpdxipc_leftc=dJpldxipl_c;
				dJdphi_leftc= dJnldphi_c;
				dJpdphi_leftc=dJpldphi_c;

				dJndphil_leftl=dJnldphi_l;
				dJndphil_leftc=dJnldphi_c;
				dJpdphil_leftl=dJpldphi_l;
				dJpdphil_leftc=dJpldphi_c;

				dphil_left= -e0/dyl/ddh;
				dJndxil_leftc=dJnldxil_c;
				dJpdxipl_leftc=dJpldxipl_c;
				//dylh_left=dylh;
				//dyrh_left=dyrh;

			}

			if (i==dim->ylen-1)
			{
				dJdxic_rightc=dJnrdxir_c;
				dJpdxipc_rightc=dJprdxipr_c;
				dJdphi_rightc= dJnrdphi_c;	//I don't understand the -
				dJpdphi_rightc=dJprdphi_c;

				//new

				dJndphir_rightr=dJnrdphi_r;
				dJndphil_rightc=dJnrdphi_c;
				dJpdphir_rightr=dJprdphi_r;
				dJpdphil_rightc=dJprdphi_c;

				dphir_right= -e1/dyr/ddh;
				dJndxir_rightc=dJnrdxir_c;
				dJpdxipr_rightc=dJprdxipr_c;
			}

			if (in->go_time==TRUE)
			{
				dJdxic+= -dnc/dt;
			}

			if (in->go_time==TRUE)
			{
				dJpdxipc+=dpc/dt;
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

					if (in->go_time==TRUE)
					{
						in->newton_dntrap[band]+= -(in->nt[z][x][i][band]-in->newton_ntlast[band])/dt;
						in->newton_dntrapdntrap[band]+= -(in->dnt[z][x][i][band])/dt;
					}

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


					if (in->go_time==TRUE)
					{
						in->newton_dptrap[band]+= -(in->pt[z][x][i][band]-in->newton_ptlast[band])/dt;
						in->newton_dptrapdptrap[band]+= -(in->dpt[z][x][i][band])/dt;
					}

					in->prelax[z][x][i]+=pc*in->srh_p_r1[z][x][i][band]-in->srh_p_r2[z][x][i][band];
					in->ptrap_to_n[z][x][i]+= -(-nc*in->srh_p_r3[z][x][i][band]+in->srh_p_r4[z][x][i][band]);

					in->pt_r1[z][x][i][band]=pc*in->srh_p_r1[z][x][i][band];
					in->pt_r2[z][x][i][band]=in->srh_p_r2[z][x][i][band];
					in->pt_r3[z][x][i][band]=nc*in->srh_p_r3[z][x][i][band];
					in->pt_r4[z][x][i][band]=in->srh_p_r4[z][x][i][band];

				}

			}

			//band=0;

			in->Rn[z][x][i]=Rtrapn+Rfree+Rauger+Rinterface_nc+Rss_srh;
			//printf("%Le %Le %Le\n",Rtrapn,Rfree,Rinterface_nc);
			//getchar();
			in->Rp[z][x][i]=Rtrapp+Rfree+Rauger+Rinterface_pc+Rss_srh;

			in->Rn_srh[z][x][i]=Rtrapn+Rss_srh;
			in->Rp_srh[z][x][i]=Rtrapp+Rss_srh;
			//Rtrapp=1e24;
			//Rtrapn=1e24;


			//Nion

			if (i!=0)
			{
				Ty[pos]=i;
				Tx[pos]=i-1;
				Ta[pos]=dphil_d;

				pos++;
				//electron
				Ty[pos]=offset_Je+i;
				Tx[pos]=offset_Je+i-1;
				Ta[pos]=dJdxil;
				pos++;

				Ty[pos]=offset_Je+i;
				Tx[pos]=i-1;
				Ta[pos]=dJdphil;
				pos++;

				//hole
				Ty[pos]=offset_Jh+i;
				Tx[pos]=offset_Jh+i-1;
				Ta[pos]=dJpdxipl;
				pos++;

				Ty[pos]=offset_Jh+i;
				Tx[pos]=i-1;
				Ta[pos]=dJpdphil;
				pos++;

			}


			//phi
			Ty[pos]=i;
			Tx[pos]=i;
			Ta[pos]=dphic_d;
			pos++;

			Ty[pos]=i;
			Tx[pos]=offset_Je+i;
			Ta[pos]=dphidxic;
			//strcpy(in->Tdebug[pos],"dphidxic");
			pos++;

			Ty[pos]=i;
			Tx[pos]=offset_Jh+i;
			Ta[pos]=dphidxipc;
			//strcpy(in->Tdebug[pos],"dphidxipc");
			pos++;


			//electron

			Ty[pos]=offset_Je+i;
			Tx[pos]=offset_Je+i;
			Ta[pos]=dJdxic;
			if (mx->complex_matrix==TRUE)
			{
				mx->Txz[pos]=dJdxic_imag;
			}
			//strcpy(in->Tdebug[pos],"dJdxic");
			pos++;


			Ty[pos]=offset_Je+i;
			Tx[pos]=offset_Jh+i;
			Ta[pos]=dJdxipc;
			pos++;

			Ty[pos]=offset_Je+i;
			Tx[pos]=i;
			Ta[pos]=dJdphic;
			pos++;



			if (insert_couple_left==TRUE)
			{
				Ty[pos]=offset_Je+i;
				Tx[pos]=offset_Jh+i-1;
				Ta[pos]=dJdxipl;
				pos++;
			}
	
			//hole
			Ty[pos]=offset_Jh+i;
			Tx[pos]=offset_Jh+i;
			Ta[pos]=dJpdxipc;
			if (mx->complex_matrix==TRUE)
			{
				mx->Txz[pos]=dJpdxipc_imag;
			}
			pos++;


			Ty[pos]=offset_Jh+i;
			Tx[pos]=offset_Je+i;
			Ta[pos]=dJpdxic;
			pos++;

			Ty[pos]=offset_Jh+i;
			Tx[pos]=i;
			Ta[pos]=dJpdphic;
			pos++;


			if (insert_couple_right==TRUE)
			{
				Ty[pos]=offset_Jh+i;
				Tx[pos]=offset_Je+i +1;
				Ta[pos]=dJpdxinr;
				pos++;

			}


			if (in->ntrapnewton==TRUE)
			{
				for (band=0;band<dim->srh_bands;band++)
				{
					Ty[pos]=offset_srh_e+dim->ylen*band+i;
					Tx[pos]=offset_srh_e+dim->ylen*band+i;
					Ta[pos]=in->newton_dntrapdntrap[band];
					pos++;

					Ty[pos]=offset_srh_e+dim->ylen*band+i;
					Tx[pos]=offset_Je+i;
					Ta[pos]=in->newton_dntrapdn[band];
					pos++;

					Ty[pos]=offset_srh_e+dim->ylen*band+i;
					Tx[pos]=offset_Jh+i;
					Ta[pos]=in->newton_dntrapdp[band];
					pos++;

					Ty[pos]=offset_Je+i;
					Tx[pos]=offset_srh_e+dim->ylen*band+i;
					Ta[pos]=in->newton_dJdtrapn[band];
					pos++;

					Ty[pos]=offset_Jh+i;
					Tx[pos]=offset_srh_e+dim->ylen*band+i;
					Ta[pos]=in->newton_dJpdtrapn[band];
					pos++;


					Ty[pos]=i;
					Tx[pos]=offset_srh_e+dim->ylen*band+i;
					Ta[pos]=in->newton_dphidntrap[band];
					pos++;

				}


			}

			if (in->ptrapnewton==TRUE)
			{
				for (band=0;band<dim->srh_bands;band++)
				{
					Ty[pos]=offset_srh_h+dim->ylen*band+i;
					Tx[pos]=offset_srh_h+dim->ylen*band+i;
					Ta[pos]=in->newton_dptrapdptrap[band];
					pos++;

					Ty[pos]=offset_srh_h+dim->ylen*band+i;
					Tx[pos]=offset_Jh+i;
					Ta[pos]=in->newton_dptrapdp[band];
					pos++;

					Ty[pos]=offset_srh_h+dim->ylen*band+i;
					Tx[pos]=offset_Je+i;
					Ta[pos]=in->newton_dptrapdn[band];
					pos++;

					Ty[pos]=offset_Jh+i;
					Tx[pos]=offset_srh_h+dim->ylen*band+i;
					Ta[pos]=in->newton_dJpdtrapp[band];
					pos++;

					Ty[pos]=offset_Je+i;
					Tx[pos]=offset_srh_h+dim->ylen*band+i;
					Ta[pos]=in->newton_dJdtrapp[band];
					pos++;

					Ty[pos]=i;
					Tx[pos]=offset_srh_h+dim->ylen*band+i;
					Ta[pos]=in->newton_dphidptrap[band];
					pos++;
				}

			}

			if (i!=(dim->ylen-1))
			{


				//phi
				Ty[pos]=i;
				Tx[pos]=i+1;
				Ta[pos]=dphir_d;
				pos++;


				//electron
				Ty[pos]=offset_Je+i;
				Tx[pos]=offset_Je+i+1;
				Ta[pos]=dJdxir;
				pos++;

				Ty[pos]=offset_Je+i;
				Tx[pos]=i+1;
				Ta[pos]=dJdphir;
				pos++;

				//hole
				Ty[pos]=offset_Jh+i;
				Tx[pos]=offset_Jh+i+1;
				Ta[pos]=dJpdxipr;
				pos++;

				Ty[pos]=offset_Jh+i;
				Tx[pos]=i+1;
				Ta[pos]=dJpdphir;
				pos++;


			}

			//Possion
			gdouble build=0.0;

			build= -(deriv);

			build+= -(-(pc-nc+Nad+pci)*Qe);	//+Nion 
			//printf("%Le\n",in->mobileion.Nion_mul);
			//getchar();
			for (band=0;band<dim->srh_bands;band++)
			{
				build+= -(-Qe*(in->pt[z][x][i][band]-in->nt[z][x][i][band]));
			}

			//build+= -(-Qe*in->Nad[i]);

			mx->b[i]=build;


			//Electron
			build=0.0;
			build= -((Jnr-Jnl)/(dylh+dyrh)-Rtrapn-Rfree-Rauger-Rss_srh-Rinterface_nc);

			if (in->go_time==TRUE)
			{
				build-= -(nc-nlast)/dt;
			}

			//getchar();
			build-=Gn;
			mx->b[offset_Je+i]=build;

			if (mx->complex_matrix==TRUE)
			{
				build=0.0;
				build-=in->omega*Gn*0.05;
				mx->bz[offset_Je+i] = build;
			}

			//hole
			build=0.0;
			build= -((Jpr-Jpl)/(dylh+dyrh)+Rtrapp+Rfree+Rauger+Rss_srh+Rinterface_pc);

			build-= -Gp;

			if (in->go_time==TRUE)
			{
				build-=(pc-plast)/dt;
			}

			mx->b[offset_Jh+i]=build;

			if (mx->complex_matrix==TRUE)
			{
				build=0.0;
				build-=-in->omega*Gp*0.05;
				mx->bz[offset_Jh+i] = build;
			}

			if (in->ntrapnewton==TRUE)
			{
				for (band=0;band<dim->srh_bands;band++)
				{
					mx->b[offset_srh_e+dim->ylen*band+i]= -(in->newton_dntrap[band]);
				}
			}

			if (in->ptrapnewton==TRUE)
			{
				for (band=0;band<dim->srh_bands;band++)
				{
					mx->b[offset_srh_h+dim->ylen*band+i]= -(in->newton_dptrap[band]);
				}

			}

		}

if (contact_left==contact_schottky)
{
	mx->b[offset_Je-1]= -(1-ns->x_y0[0][0]);
	Ty[pos]=offset_Je-1;
	Tx[pos]=offset_Je-1;
	Ta[pos]=-1.0;
	pos++;
}


if (pos>mx->nz)
{
	ewe(sim,"Error %d %d %d\n",pos,mx->nz,in->kl_in_newton);
}

}



int solve_electrical(struct simulation *sim,struct device *in, int z, int x)
{
struct matrix *mx=&(in->mx);

FILE *file;
double stop_time;
double start_time= timer_get_time_in_ms();

double update_start;
double update_tot=0;

double fill_matrix_start;
double fill_matrix_tot=0;

gdouble error=0.0;
int ittr=0;
char temp[PATH_MAX];
//struct dimensions *dim=&in->ns.dim;
struct newton_state *ns=&(in->ns);


newton_state_reset_error(sim, ns);


//if (get_dump_status(sim,dump_print_newtonerror)==TRUE)
//{
//	printf_log(sim,"Solve cur\n");
//}


#ifdef only
only_update_thermal=FALSE;
#endif
//in->enable_back=FALSE;
int stop=FALSE;
int thermalrun=0;
mx->use_cache=in->cache.enabled;
in->dd_conv=FALSE;

if (in->max_electrical_itt<5)
{
	ewe(sim,"max_electrical_itt too low\n");
}

matrix_cache_reset(sim,mx);
	do
	{

		fill_matrix_start=timer_get_time_in_ms();
		fill_matrix(sim,in,z,x,FALSE);
		fill_matrix_tot+=(timer_get_time_in_ms()-fill_matrix_start);

		if (in->newton_only_fill_matrix==TRUE)
		{
			return 0;
		}


		//dump_for_plot(in);
		//plot_now(in,"plot");
			//solver_dump_matrix(in->M,mx->nz,in->Ti,in->Tj, in->Tx,mx->b);
		//getchar();
		//matrix_dump(sim,mx);
		//getchar();
		if (in->stop==TRUE)
		{
			break;
		}
		//matrix_solve(sim,&(in->msm),mx);

		if (matrix_solve(sim,&(in->msm),mx)==0)
		{
			//printf("found in cache %s\n",mx->cache_file_path);
			//getchar();
			newton_state_load(sim,ns,mx->cache_file_path);
			newton_state_update_device(sim,in, ns);
			//getchar();
			return 0;
		}/*else
		{
			printf("state not foudn %s\n",mx->cache_file_path);
			getchar();
		}*/
		//solver_dump_matrix(in->M,mx->nz,in->Ti,in->Tj, in->Tx,mx->b);
		//getchar();


		


		//////////
		error=get_cur_error(sim,in);
		//printf("laste error >> %Le\n",error);
		//getchar();
		/*if (error<1e-5)
		{
			clamp=5.0;//clamp*2;
		}*/

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
		//////////

		update_start= timer_get_time_in_ms();
		update_solver_vars(sim,in,z,x,TRUE);
		update_tot+=timer_get_time_in_ms()-update_start;

		//thermalrun++;
		if (thermalrun==40) thermalrun=0;
		//update(in);
//getchar();

		if (in->solver_verbosity>SOLVER_VERBOSITY_AT_END)
		{
			printf_log(sim,"%d Cur error = %Le %Le I=%Le",ittr,error,contact_get_active_contact_voltage(sim,in),get_I(in));

			printf_log(sim,"\n");
			//printf_log(sim,"%Le\n",get_equiv_V(sim,in));
		}

		//ns->last_error=error;
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


		if ((ittr==2)&&(error<in->min_cur_error))
		{
			in->dd_conv=TRUE;
		}

	}while(stop==FALSE);

in->newton_last_ittr=ittr;

if (error>1e-3)
{
	printf_log(sim,"warning: The solver has not converged very well.\n");

	if (sim->math_stop_on_convergence_problem==TRUE)
	{
		ewe(sim,"%s\n","Not converged very well");
	}
}


if (get_dump_status(sim,dump_newton)==TRUE)
{
	join_path(2,temp,in->output_path,"solver");
	dump_1d_slice(sim,in,temp);
}
//plot_now(sim,in,"plot");
//getchar();
in->odes+=mx->M;

if (mx->use_cache==TRUE)
{
	newton_state_save(sim,mx->cache_file_path,ns);
}

stop_time= timer_get_time_in_ms();

ns->last_time=(double)(stop_time-start_time);

if (get_dump_status(sim,dump_write_converge)==TRUE)
{
	FILE *out;
	out=fopen("matrix_times.dat","a");
	if (out!=NULL)
	{
		fprintf(out,"%lf %d %lf %lf\n",ns->last_time,mx->tot_time,update_tot,fill_matrix_tot);
		fclose(out);
	}
}
//getchar();

return 0;
}

int dllinternal_solve_cur(struct simulation *sim,struct device *in, int z, int x)
{
	struct perovskite *ion=&(in->mobileion);

	if (ion->full_newton==FALSE)
	{
		
		int cont=TRUE;
		int ittr=0;
		struct newton_state *ns=&(in->ns);

		if (ns->Nion_enabled==FALSE)
		{
			solve_electrical(sim,in, 0, 0);
		}
	}else
	{
		solve_electrical(sim,in, 0, 0);
	}
	return 0;
}
