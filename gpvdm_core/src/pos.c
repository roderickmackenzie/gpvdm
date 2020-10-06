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

/** @file pos.c
@brief poisson solver - make a better guess at 0V in the dark.
*/

#include <string.h>
#include <stdlib.h>
#include <dump.h>
#include <dos.h>
#include "sim.h"
#include "solver_interface.h"
#include "dat_file.h"
#include "log.h"
#include <cal_path.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <lang.h>
#include <memory.h>

gdouble min_pos_error=1e-4;

void pos_dump(struct simulation *sim,struct device *in)
{
struct newton_state *ns=&(in->ns);
struct dimensions *dim=&(ns->dim);

if (get_dump_status(sim,dump_first_guess)==TRUE)
{
	struct stat st = {0};

	char out_dir[PATH_MAX];
	join_path(2,out_dir,get_output_path(sim),"equilibrium");

	if (stat(out_dir, &st) == -1)
	{
		mkdir(out_dir, 0700);
	}

	struct dat_file buf;
	buffer_init(&buf);
	char name[200];
	int band=0;
	int i=0;

	buffer_malloc(&buf);
	sprintf(name,"%s%s","first_guess_Fi",".dat");
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,_("Intrinsic Fermi - position"));
	strcpy(buf.type,"xy");
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.y_label,"Fi");
	strcpy(buf.x_units,"nm");
	strcpy(buf.y_units,"$E_{LUMO}$");
	buf.logscale_x=0;
	buf.logscale_y=0;
	buffer_add_info(sim,&buf);
	buffer_add_3d_data(sim,&buf,dim, in->Fi);
	buffer_dump_path(sim,out_dir,name,&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	sprintf(name,"%s%s","first_guess_Ec",".dat");
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,_("LUMO energy - position"));
	strcpy(buf.type,"xy");
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.y_label,"LUMO");
	strcpy(buf.x_units,"nm");
	strcpy(buf.y_units,"$E_{LUMO}$");
	buf.logscale_x=0;
	buf.logscale_y=0;
	buffer_add_info(sim,&buf);
	buffer_add_3d_data(sim,&buf,dim, in->Ec);
	buffer_dump_path(sim,out_dir,name,&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	sprintf(name,"%s%s","first_guess_Ev",".dat");
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,_("HOMO energy - position"));
	strcpy(buf.type,"xy");
	strcpy(buf.x_label,"Position");
	strcpy(buf.y_label,"LUMO");
	strcpy(buf.x_units,"nm");
	strcpy(buf.y_units,"$E_{HOMO}$");
	buf.logscale_x=0;
	buf.logscale_y=0;
	buffer_add_info(sim,&buf);
	buffer_add_3d_data(sim,&buf,dim, in->Ev);
	buffer_dump_path(sim,out_dir,name,&buf);
	buffer_free(&buf);


	buffer_malloc(&buf);
	sprintf(name,"%s%s","first_guess_n",".dat");
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,_("Electron density - position"));
	strcpy(buf.type,"xy");
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.y_label,_("Electron density"));
	strcpy(buf.x_units,"nm");
	strcpy(buf.y_units,"m^{-3}");
	buf.logscale_x=0;
	buf.logscale_y=0;
	buffer_add_info(sim,&buf);
	buffer_add_3d_data(sim,&buf,dim, in->n);
	buffer_dump_path(sim,out_dir,name,&buf);
	buffer_free(&buf);


	buffer_malloc(&buf);
	sprintf(name,"%s%s","first_guess_p",".dat");
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,_("Hole density - position"));
	strcpy(buf.type,"xy");
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.y_label,_("Hole density"));
	strcpy(buf.x_units,"nm");
	strcpy(buf.y_units,"m^{-3}");
	buf.logscale_x=0;
	buf.logscale_y=0;
	buffer_add_info(sim,&buf);
	buffer_add_3d_data(sim,&buf,dim, in->p);
	buffer_dump_path(sim,out_dir,name,&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	sprintf(name,"%s%s","first_guess_phi",".dat");
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,_("Potential - position"));
	strcpy(buf.type,"xy");
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.y_label,_("Potential"));
	strcpy(buf.x_units,"nm");
	strcpy(buf.y_units,"V");
	buf.logscale_x=0;
	buf.logscale_y=0;
	buffer_add_info(sim,&buf);
	buffer_add_3d_data(sim,&buf,dim, ns->phi);
	buffer_dump_path(sim,out_dir,name,&buf);
	buffer_free(&buf);



	/*out=fopena(get_output_path(sim),"first_guess_np_trap.dat","w");
	for (i=0;i<dim->ylen;i++)
	{
		fprintf(out,"%Le ",dim->ymesh[i]);
		for (band=0;band<dim->srh_bands;band++)
		{
			fprintf(out,"%Le %Le ",in->nt[i][band],in->pt[i][band]);
		}
		fprintf(out,"\n");
	}
	fclose(out);*/

}
}

long double get_p_error(struct device *in,long double *b)
{
gdouble tot=0.0;
int i;
struct dimensions *dim=&in->ns.dim;

for (i=0;i<dim->ylen;i++)
{
	//if ((in->interfaceleft==TRUE)&&(i==0))
	//{
	//}else
	//if ((in->interfaceright==TRUE)&&(i==dim->ylen-1))
	//{
	//}else
	//{
	tot+=gfabs(b[i]);
	//}
}
return tot;
}


int solve_pos(struct simulation *sim,struct device *in, int z, int x)
{

if (in->circuit_simulation==TRUE)
{
	return 0;
}
printf_log(sim,"%s\n",_("Solving the initial electrostatic field."));

int i;
int y;
struct dimensions *dim=&in->ns.dim;
struct epitaxy *epi=&(in->my_epitaxy);
struct matrix mx;
matrix_init(&mx);
mx.nz=dim->ylen*3-2;
mx.M=dim->ylen;

matrix_malloc(sim,&mx);

if (in->ncontacts<1)
{
	ewe(sim,"No contacts\n");
}

/*
Ti[0]=0;
Tj[0]=0;
Tx[0]=1.0;
Ti[1]=1;
Tj[1]=1;
Tx[1]=2.0;

b[0]=1;
b[1]=2;

solver(sim,M,N,Ti,Tj, Tx,b);
exit(0);
*/
gdouble phil;
gdouble phic;
gdouble phir;
gdouble yl;
gdouble yc;
gdouble yr;
gdouble dyl;
gdouble dyr;
gdouble dyc=0.0;
int ittr=0;
int pos=0;
gdouble error=1000;
gdouble el=0.0;
gdouble ec=0.0;
gdouble er=0.0;
gdouble e0=0.0;
gdouble e1=0.0;
int pos_max_ittr=250;

int quit=FALSE;
int adv_step=0;
int adv=FALSE;
int band;
long double not_used;

struct newton_state *ns=&(in->ns);

gdouble kTq=(in->Te[z][x][0]*kb/Q);

	matrix_cache_reset(sim,&mx);

	do
	{

		//if (in->interfaceleft==TRUE)
		//{
		//	ns->phi[z][x][0]=in->V_y0[z][x];
		//}

		//if (in->interfaceright==TRUE)
		//{
		//	ns->phi[z][x][dim->ylen-1]=in->V_y1[z][x];
		//}

		pos=0;

		for (i=0;i<dim->ylen;i++)
		{


			if (i==0)
			{
				phil=in->V_y0[z][x];
				el=in->epsilonr[z][x][0]*epsilon0;
				yl=dim->ymesh[0]-(dim->ymesh[1]-dim->ymesh[0]);


			}else
			{
				el=in->epsilonr[z][x][i-1]*epsilon0;
				phil=ns->phi[z][x][i-1];
				yl=dim->ymesh[i-1];
			}

			if (i==(dim->ylen-1))
			{
				phir=in->V_y1[z][x];
				er=in->epsilonr[z][x][i]*epsilon0;
				yr=dim->ymesh[i]+(dim->ymesh[i]-dim->ymesh[i-1]);
			}else
			{
				er=in->epsilonr[z][x][i+1]*epsilon0;
				phir=ns->phi[z][x][i+1];
				yr=dim->ymesh[i+1];

			}


			yc=dim->ymesh[i];
			dyl=yc-yl;
			dyr=yr-yc;
			dyc=(dyl+dyr)/2.0;

			ec=in->epsilonr[z][x][i]*epsilon0;
			e0=(el+ec)/2.0;
			e1=(ec+er)/2.0;
			phic=ns->phi[z][x][i];



			gdouble dphidn=0.0;
			if (adv==FALSE)
			{
				dphidn=(Q/(kb*in->Tl[z][x][i]))*in->Nc[z][x][i]*exp(((in->Fi[z][x][i]-(-ns->phi[z][x][i]-in->Xi[z][x][i])))/(kTq));

			}else
			{
				get_n_den(epi,in->Fi[z][x][i]-in->Ec[z][x][i],in->Tl[z][x][i],in->imat[z][x][i],&not_used,&dphidn,NULL);

			}


			gdouble dphidp=0.0;
			if (adv==FALSE)
			{
				dphidp= -(Q/(kb*in->Tl[z][x][i]))*in->Nv[z][x][i]*exp((((-ns->phi[z][x][i]-in->Xi[z][x][i]-in->Eg[z][x][i])-in->Fi[z][x][i]))/(kTq));
			}else
			{
				get_p_den(epi,ns->xp[z][x][i]-in->tp[z][x][i],in->Tl[z][x][i],in->imat[z][x][i],&not_used,&(dphidp),NULL);
				dphidp= -dphidp;
			}
			gdouble dphil=e0/dyl/dyc;
			gdouble dphic= -(e0/dyl/dyc+e1/dyr/dyc);
			gdouble dphir=e1/dyr/dyc;

			gdouble dphil_d=dphil;
			gdouble dphic_d=dphic;
			gdouble dphir_d=dphir;

			/*if (in->interfaceleft==TRUE)
			{

				if (i==1)
				{
					dphil_d=0.0;
					phil=in->Vl[z][x];

				}

				if (i==0)
				{
					dphil_d=0.0;
					dphic_d=1e-6;
					dphir_d=0.0;

				}
			}*/

			/*if (in->interfaceright==TRUE)
			{

				if (i==dim->ylen-2)
				{
					dphir_d=0.0;
					phir=in->V_y1[z][x];

				}

				if (i==dim->ylen-1)
				{
					dphil_d=0.0;
					dphic_d=1e-6;
					dphir_d=0.0;

				}
			}*/

			gdouble dphi=dphil*phil+dphic*phic+dphir*phir;


			dphic_d+= -Q*(dphidn-dphidp); // just put in the _d to get it working again.

			//if (adv==TRUE)
			//{
			//	for (band=0;band<dim->srh_bands;band++)
			//	{
			//		dphic_d+=(-q*(in->dnt[i][band]-in->dpt[i][band]));
			//	}
			//}

			if (i!=0)
			{
				mx.Ti[pos]=i;
				mx.Tj[pos]=i-1;
				mx.Tx[pos]=dphil_d;
				pos++;
			}

			mx.Ti[pos]=i;
			mx.Tj[pos]=i;
			mx.Tx[pos]=dphic_d;



			pos++;



			if (i!=(dim->ylen-1))
			{
				mx.Ti[pos]=i;
				mx.Tj[pos]=i+1;
				mx.Tx[pos]=dphir_d;
				pos++;

			}

			/*if ((in->interfaceleft==TRUE)&&(i==0))
			{
				mx.b[i]= -0.0;
			}else
			if ((in->interfaceright==TRUE)&&(i==dim->ylen-1))
			{
				mx.b[i]= -0.0;
			}else
			{*/
				mx.b[i]= -(dphi-Q*(in->n[z][x][i]-in->p[z][x][i]-in->Nad[z][x][i])); //
				if (adv==TRUE)
				{
					for (band=0;band<dim->srh_bands;band++)
					{
						mx.b[i]+= -(-Q*(in->nt[z][x][i][band]-in->pt[z][x][i][band]));
					}
				}
			//}
			//in->n[i]=in->Nc[z][x][i]*exp(((in->Fi[z][x][i]-in->Ec[z][x][i])*q)/(kb*in->Tl[z][x][i]));

		}

		error=get_p_error(in,mx.b);

		matrix_solve(sim,&(in->msm),&mx);

		/*if (mx.ans_loaded_from_cache==TRUE)
		{
			error=matrix_cal_error(sim,&mx);
			adv=TRUE;
			quit=TRUE;
		}*/

		for (i=0;i<dim->ylen;i++)
		{
			//if ((in->interfaceleft==TRUE)&&(i==0))
			//{
			//}else
			//if ((in->interfaceright==TRUE)&&(i==dim->ylen-1))
			//{
			//}else
			//{
			gdouble update;

			gdouble clamp_temp=300.0;
			update=mx.b[i]/(1.0+fabs(mx.b[i]/in->posclamp/(clamp_temp*kb/Q)));
			ns->phi[z][x][i]+=update;

			//}
		}

		//getchar();

		for (i=0;i<dim->ylen;i++)
		{
			in->Ec[z][x][i]= -ns->phi[z][x][i]-in->Xi[z][x][i];
			in->Ev[z][x][i]= -ns->phi[z][x][i]-in->Xi[z][x][i]-in->Eg[z][x][i];


				if (adv==FALSE)
				{
					in->n[z][x][i]=in->Nc[z][x][i]*exp(((in->Fi[z][x][i]-in->Ec[z][x][i])*Q)/(kb*in->Tl[z][x][i]));
					in->dn[z][x][i]=(Q/(kb*in->Tl[z][x][i]))*in->Nc[z][x][i]*exp((Q*(in->Fi[z][x][i]-in->Ec[z][x][i]))/(kb*in->Tl[z][x][i]));
				}else
				{
					get_n_den(epi,in->Fi[z][x][i]-in->Ec[z][x][i],in->Tl[z][x][i],in->imat[z][x][i],&(in->n[z][x][i]),&(in->dn[z][x][i]),NULL);
				}

				in->Fn[z][x][i]=in->Fi[z][x][i];
				in->Fp[z][x][i]=in->Fi[z][x][i];

				ns->x[z][x][i]=ns->phi[z][x][i]+in->Fn[z][x][i];
				ns->xp[z][x][i]= -(ns->phi[z][x][i]+in->Fp[z][x][i]);

				//printf("a %d\n",adv);
				//getchar();

				if (adv==FALSE)
				{
					in->p[z][x][i]=in->Nv[z][x][i]*exp(((ns->xp[z][x][i]-in->tp[z][x][i])*Q)/(kb*in->Tl[z][x][i]));
					in->dp[z][x][i]=(Q/(kb*in->Tl[z][x][i]))*in->Nv[z][x][i]*exp(((ns->xp[z][x][i]-in->tp[z][x][i])*Q)/(kb*in->Tl[z][x][i]));
				}else
				{
					get_p_den(epi,ns->xp[z][x][i]-in->tp[z][x][i],in->Tl[z][x][i],in->imat[z][x][i],&(in->p[z][x][i]),&(in->dp[z][x][i]),NULL);
					//in->dp[z][x][i]=get_dp_den(in,ns->xp[z][x][i]-in->tp[z][x][i],in->Tl[z][x][i],in->imat[z][x][i]);
				}


				for (band=0;band<dim->srh_bands;band++)
				{

					in->Fnt[z][x][i][band]= -ns->phi[z][x][i]-in->Xi[z][x][i]+dos_srh_get_fermi_n(epi,in->n[z][x][i], in->p[z][x][i],band,in->imat[z][x][i],in->Te[z][x][i]);
					in->Fpt[z][x][i][band]= -ns->phi[z][x][i]-in->Xi[z][x][i]-in->Eg[z][x][i]-dos_srh_get_fermi_p(epi,in->n[z][x][i], in->p[z][x][i],band,in->imat[z][x][i],in->Th[z][x][i]);

					//printf("p:%Le\n",dos_srh_get_fermi_p(epi,in->n[z][x][i], in->p[z][x][i],band,in->imat[z][x][i],in->Th[z][x][i]));
					//getchar();
					ns->xt[z][x][i][band]=ns->phi[z][x][i]+in->Fnt[z][x][i][band];
					in->nt[z][x][i][band]=get_n_pop_srh(sim,epi,ns->xt[z][x][i][band]+in->tt[z][x][i],in->Te[z][x][i],band,in->imat[z][x][i]);
					in->dnt[z][x][i][band]=get_dn_pop_srh(sim,epi,ns->xt[z][x][i][band]+in->tt[z][x][i],in->Te[z][x][i],band,in->imat[z][x][i]);


					ns->xpt[z][x][i][band]= -(ns->phi[z][x][i]+in->Fpt[z][x][i][band]);
					in->pt[z][x][i][band]=get_p_pop_srh(sim,epi,ns->xpt[z][x][i][band]-in->tpt[z][x][i],in->Th[z][x][i],band,in->imat[z][x][i]);
					in->dpt[z][x][i][band]=get_dp_pop_srh(sim,epi,ns->xpt[z][x][i][band]-in->tpt[z][x][i],in->Th[z][x][i],band,in->imat[z][x][i]);
				}

		}

		update_y_array(sim,in,z,x);


		if (error<1)
		{
			adv=TRUE;
		}
		//#ifdef print_newtonerror

		//if (get_dump_status(sim,dump_print_pos_error)==TRUE)
		if ((ittr%10)==0)
		{
			printf_log(sim,"%d %s = %Le %d\n",ittr,_("Electrostatic solver f()="),error,adv);
		}
		//#endif

		#ifdef dump_converge



		/*in->converge=fopena(get_output_path(sim),"converge.dat","a");
		fprintf(in->converge,"%e\n",error);
		fclose(in->converge);*/
		#endif



	ittr++;

	if (adv==TRUE)
	{
		adv_step++;
	}

	if (ittr>pos_max_ittr)
	{
		quit=TRUE;
	}

	if ((error<min_pos_error)&&(adv_step>3))
	{
		quit=TRUE;
	}

	}while(quit==FALSE);
		//getchar();

	printf_log(sim,"%d %s = %Le %d\n",ittr,_("Electrostatic solver f()="),error,adv);

	pos_dump(sim,in);

	update_y_array(sim,in,z,x);

	if (in->srh_sim==TRUE)
	{
		time_init(sim,in);

	}




in->odes+=dim->ylen;



for (y=0;y<dim->ylen;y++)
{

	in->nf_save[z][x][y]=in->n[z][x][y];
	in->pf_save[z][x][y]=in->p[z][x][y];
	in->nt_save[z][x][y]=0.0;
	in->pt_save[z][x][y]=0.0;
}

//matrix_cache_save(sim,&mx);
matrix_free(sim,&mx);


//printf_log(sim,"%s\n",_("Converged"));
printf_log(sim,"V_y0=%Le V_y1=%Le phi_mid=%Le\n",in->V_y0[0][0],in->V_y1[z][x], ns->phi[z][x][dim->ylen/2]);
//exit(0);
return 0;
}

