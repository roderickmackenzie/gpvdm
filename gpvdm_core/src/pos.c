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

/** @file pos.c
@brief poisson solver - make a better guess at 0V in the dark.
*/

#include <enabled_libs.h>
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
#include <device_fun.h>
#include <json.h>

void pos_dump(struct simulation *sim,struct device *in,struct pos_config *config)
{
struct newton_state *ns=&(in->ns);
struct dimensions *dim=&(ns->dim);

if (config->pos_dump_verbosity>0)
{
	struct stat st = {0};

	char out_dir[PATH_MAX];
	join_path(2,out_dir,get_output_path(in),"equilibrium");

	if (stat(out_dir, &st) == -1)
	{
		mkdir(out_dir, 0700);
	}

	struct dat_file buf;
	dat_file_init(&buf);
	char name[200];
	int band=0;
	int i=0;

	if (buffer_set_file_name(sim,in,&buf,"Fi.csv")==0)
	{
		buffer_malloc(&buf);
		buf.y_mul=1.0;
		buf.x_mul=1e9;
		strcpy(buf.title,_("Intrinsic Fermi - position"));
		strcpy(buf.type,"xy");
		strcpy(buf.x_label,_("Position"));
		strcpy(buf.y_label,"Fi");
		strcpy(buf.x_units,"nm");
		strcpy(buf.y_units,"$E_{LUMO}$");
		dat_file_add_zxy_data(sim,&buf,dim, in->Fi);
		buffer_dump_path(sim,out_dir,NULL,&buf);
		buffer_free(&buf);
	}

	if (buffer_set_file_name(sim,in,&buf,"Ec.csv")==0)
	{
		buffer_malloc(&buf);
		buf.y_mul=1.0;
		buf.x_mul=1e9;
		strcpy(buf.title,_("LUMO energy - position"));
		strcpy(buf.type,"xy");
		strcpy(buf.x_label,_("Position"));
		strcpy(buf.y_label,"LUMO");
		strcpy(buf.x_units,"nm");
		strcpy(buf.y_units,"$E_{LUMO}$");
		dat_file_add_zxy_data(sim,&buf,dim, in->Ec);
		buffer_dump_path(sim,out_dir,NULL,&buf);
		buffer_free(&buf);
	}

	if (buffer_set_file_name(sim,in,&buf,"Ev.csv")==0)
	{
		buffer_malloc(&buf);
		buf.y_mul=1.0;
		buf.x_mul=1e9;
		strcpy(buf.title,_("HOMO energy - position"));
		strcpy(buf.type,"xy");
		strcpy(buf.x_label,"Position");
		strcpy(buf.y_label,"LUMO");
		strcpy(buf.x_units,"nm");
		strcpy(buf.y_units,"$E_{HOMO}$");
		dat_file_add_zxy_data(sim,&buf,dim, in->Ev);
		buffer_dump_path(sim,out_dir,NULL,&buf);
		buffer_free(&buf);
	}


	if (buffer_set_file_name(sim,in,&buf,"n.csv")==0)
	{
		buffer_malloc(&buf);
		buf.y_mul=1.0;
		buf.x_mul=1e9;
		strcpy(buf.title,_("Electron density - position"));
		strcpy(buf.type,"xy");
		strcpy(buf.x_label,_("Position"));
		strcpy(buf.y_label,_("Electron density"));
		strcpy(buf.x_units,"nm");
		strcpy(buf.y_units,"m^{-3}");
		dat_file_add_zxy_data(sim,&buf,dim, in->n);
		buffer_dump_path(sim,out_dir,NULL,&buf);
		buffer_free(&buf);
	}

	if (buffer_set_file_name(sim,in,&buf,"p.csv")==0)
	{
		buffer_malloc(&buf);
		buf.y_mul=1.0;
		buf.x_mul=1e9;
		strcpy(buf.title,_("Hole density - position"));
		strcpy(buf.type,"xy");
		strcpy(buf.x_label,_("Position"));
		strcpy(buf.y_label,_("Hole density"));
		strcpy(buf.x_units,"nm");
		strcpy(buf.y_units,"m^{-3}");
		dat_file_add_zxy_data(sim,&buf,dim, in->p);
		buffer_dump_path(sim,out_dir,NULL,&buf);
		buffer_free(&buf);
	}

	if (buffer_set_file_name(sim,in,&buf,"phi.csv")==0)
	{
		buffer_malloc(&buf);
		buf.y_mul=1.0;
		buf.x_mul=1e9;
		strcpy(buf.title,_("Potential - position"));
		strcpy(buf.type,"xy");
		strcpy(buf.x_label,_("Position"));
		strcpy(buf.y_label,_("Potential"));
		strcpy(buf.x_units,"nm");
		strcpy(buf.y_units,"V");
		dat_file_add_zxy_data(sim,&buf,dim, ns->phi);
		buffer_dump_path(sim,out_dir,NULL,&buf);
		buffer_free(&buf);
	}


	/*out=fopena(get_output_path(sim),"first_guess_np_trap.dat","w");
	for (i=0;i<dim->ylen;i++)
	{
		fprintf(out,"%Le ",dim->y[i]);
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

int solve_pos(struct simulation *sim,struct device *dev)
{
	int x;
	int y;
	int z;
	char solver_type[100];
	struct json_obj *json_solver_type;
	struct json_obj *json_poisson;
	struct dimensions *dim=&(dev->ns.dim);
	struct pos_config config;

	if (dev->cir.enabled==TRUE)
	{
		return 0;
	}

	json_solver_type=json_obj_find_by_path(sim,&(dev->config.obj), "electrical_solver");
	json_get_string(sim,json_solver_type, solver_type,"solver_type");

	json_poisson=json_obj_find_by_path(sim,&(dev->config.obj), "electrical_solver.poisson");
	json_get_int(sim,json_poisson, &(config.max_ittr),"pos_max_ittr");
	json_get_long_double(sim,json_poisson, &(config.posclamp),"posclamp");
	json_get_long_double(sim,json_poisson, &(config.pos_min_error),"pos_min_error");
	json_get_int(sim,json_poisson, &(config.pos_dump_verbosity),"pos_dump_verbosity");


	if (strcmp(solver_type,"poisson")==0)
	{
		dev->math_enable_pos_solver=TRUE;
	}

	if (dev->math_enable_pos_solver==TRUE)
	{
		for (z=0;z<dim->zlen;z++)
		{
			for (x=0;x<dim->xlen;x++)
			{
				solve_pos_y(sim,dev,z,x,&config);
			}
		}
	}

	if (strcmp(solver_type,"poisson")==0)
	{
		return -1;
	}

	return 0;
}
int solve_pos_y(struct simulation *sim,struct device *in, int z, int x,struct pos_config *config)
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

int quit=FALSE;
int adv_step=0;
int adv=FALSE;
int band;
long double not_used;
long double deriv;
struct shape *s;
struct newton_state *ns=&(in->ns);

gdouble kTq=(in->Te[z][x][0]*kb/Qe);

	matrix_cache_reset(sim,&mx);
	strcpy(mx.dump_name,"pos");
	do
	{

		pos=0;

		for (i=0;i<dim->ylen;i++)
		{

			s=in->obj_zxy[z][x][i]->s;
			if (i==0)
			{
				phil=in->V_y0[z][x];
				el=in->epsilonr_e0[z][x][0];
				yl=dim->y[0]-(dim->y[1]-dim->y[0]);


			}else
			{
				el=in->epsilonr_e0[z][x][i-1];
				phil=ns->phi[z][x][i-1];
				yl=dim->y[i-1];
			}

			if (i==(dim->ylen-1))
			{
				phir=in->V_y1[z][x];
				er=in->epsilonr_e0[z][x][i];
				yr=dim->y[i]+(dim->y[i]-dim->y[i-1]);
			}else
			{
				er=in->epsilonr_e0[z][x][i+1];
				phir=ns->phi[z][x][i+1];
				yr=dim->y[i+1];

			}


			yc=dim->y[i];
			dyl=yc-yl;
			dyr=yr-yc;
			dyc=(dyl+dyr)/2.0;

			ec=in->epsilonr_e0[z][x][i];
			e0=(el+ec)/2.0;
			e1=(ec+er)/2.0;
			phic=ns->phi[z][x][i];



			gdouble dphidn=0.0;
			if (adv==FALSE)
			{
				dphidn=(Qe/(kb*in->Tl[z][x][i]))*in->Nc[z][x][i]*exp(((in->Fi[z][x][i]-(-ns->phi[z][x][i]-in->Xi[z][x][i])))/(kTq));

			}else
			{
				get_n_den(s,in->Fi[z][x][i]-in->Ec[z][x][i],in->Tl[z][x][i],&not_used,&dphidn,NULL);

			}


			gdouble dphidp=0.0;
			if (adv==FALSE)
			{
				dphidp= -(Qe/(kb*in->Tl[z][x][i]))*in->Nv[z][x][i]*exp((((-ns->phi[z][x][i]-in->Xi[z][x][i]-in->Eg[z][x][i])-in->Fi[z][x][i]))/(kTq));
			}else
			{
				get_p_den(s,ns->xp[z][x][i]-in->tp[z][x][i],in->Tl[z][x][i],&not_used,&(dphidp),NULL);
				dphidp= -dphidp;
			}

			gdouble dphil=-e0/dyl/dyc;
			gdouble dphic= (e0/dyl/dyc+e1/dyr/dyc);
			gdouble dphir=-e1/dyr/dyc;

			gdouble dphil_d=dphil;
			gdouble dphic_d=dphic;
			gdouble dphir_d=dphir;

			deriv=dphil*phil+dphic*phic+dphir*phir;
			//printf("%Le %Le %Le %Le %Le %Le\n",dphil,phil,dphic,phic,dphir,phir);

			dphic_d+= -Qe*(dphidp-dphidn); // just put in the _d to get it working again.

			//if (adv==TRUE)
			//{
			//	for (band=0;band<dim->srh_bands;band++)
			//	{
			//		dphic_d+=(-q*(in->dpt[i][band]-in->dnt[i][band]));
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


			//printf("%Le %Le %Le %Le\n",dphi,in->n[z][x][i],in->p[z][x][i],in->Nad[z][x][i]);
			mx.b[i]= -(deriv);
			mx.b[i]-= -(in->p[z][x][i]-in->n[z][x][i]+in->Nad[z][x][i]+in->Nion[z][x][i])*Qe;//
			//printf("%Le\n",in->Nad[z][x][i]);
			//getchar();
			if (adv==TRUE)
			{
				for (band=0;band<dim->srh_bands;band++)
				{
					mx.b[i]-= -(Qe*(in->pt[z][x][i][band]-in->nt[z][x][i][band]));
				}
			}


		}


		//printf("error%Le\n",error);
		matrix_solve(sim,&(in->msm),&mx);
		error=get_p_error(in,mx.b);
		//getchar();
		/*if (mx.ans_loaded_from_cache==TRUE)
		{
			error=matrix_cal_error(sim,&mx);
			adv=TRUE;
			quit=TRUE;
		}*/

		for (i=0;i<dim->ylen;i++)
		{
			gdouble update;

			gdouble clamp_temp=300.0;
			update=mx.b[i]/(1.0+fabs(mx.b[i]/config->posclamp/(clamp_temp*kb/Qe)));
			ns->phi[z][x][i]+=update;
		}

		//getchar();

		for (i=0;i<dim->ylen;i++)
		{
			s=in->obj_zxy[z][x][i]->s;
			in->Ec[z][x][i]= -ns->phi[z][x][i]-in->Xi[z][x][i];
			in->Ev[z][x][i]= -ns->phi[z][x][i]-in->Xi[z][x][i]-in->Eg[z][x][i];


				if (adv==FALSE)
				{
					in->n[z][x][i]=in->Nc[z][x][i]*exp(((in->Fi[z][x][i]-in->Ec[z][x][i])*Qe)/(kb*in->Tl[z][x][i]));
					in->dn[z][x][i]=(Qe/(kb*in->Tl[z][x][i]))*in->Nc[z][x][i]*exp((Qe*(in->Fi[z][x][i]-in->Ec[z][x][i]))/(kb*in->Tl[z][x][i]));
				}else
				{
					get_n_den(s,in->Fi[z][x][i]-in->Ec[z][x][i],in->Tl[z][x][i],&(in->n[z][x][i]),&(in->dn[z][x][i]),NULL);
				}

				in->Fn[z][x][i]=in->Fi[z][x][i];
				in->Fp[z][x][i]=in->Fi[z][x][i];

				ns->x[z][x][i]=ns->phi[z][x][i]+in->Fn[z][x][i];
				ns->xp[z][x][i]= -(ns->phi[z][x][i]+in->Fp[z][x][i]);

				//printf("a %d\n",adv);
				//getchar();

				if (adv==FALSE)
				{
					in->p[z][x][i]=in->Nv[z][x][i]*exp(((ns->xp[z][x][i]-in->tp[z][x][i])*Qe)/(kb*in->Tl[z][x][i]));
					in->dp[z][x][i]=(Qe/(kb*in->Tl[z][x][i]))*in->Nv[z][x][i]*exp(((ns->xp[z][x][i]-in->tp[z][x][i])*Qe)/(kb*in->Tl[z][x][i]));
				}else
				{
					get_p_den(s,ns->xp[z][x][i]-in->tp[z][x][i],in->Tl[z][x][i],&(in->p[z][x][i]),&(in->dp[z][x][i]),NULL);
				}



				for (band=0;band<dim->srh_bands;band++)
				{

					in->Fnt[z][x][i][band]= -ns->phi[z][x][i]-in->Xi[z][x][i]+dos_srh_get_fermi_n(s,in->n[z][x][i], in->p[z][x][i],band,in->Te[z][x][i]);
					in->Fpt[z][x][i][band]= -ns->phi[z][x][i]-in->Xi[z][x][i]-in->Eg[z][x][i]-dos_srh_get_fermi_p(s,in->n[z][x][i], in->p[z][x][i],band,in->Th[z][x][i]);

					//printf("p:%Le\n",dos_srh_get_fermi_p(s,in->n[z][x][i], in->p[z][x][i],band,in->imat[z][x][i],in->Th[z][x][i]));
					//getchar();
					ns->xt[z][x][i][band]=ns->phi[z][x][i]+in->Fnt[z][x][i][band];
					in->nt[z][x][i][band]=get_n_pop_srh(sim,s,ns->xt[z][x][i][band]+in->tt[z][x][i],in->Te[z][x][i],band);
					in->dnt[z][x][i][band]=get_dn_pop_srh(sim,s,ns->xt[z][x][i][band]+in->tt[z][x][i],in->Te[z][x][i],band);


					ns->xpt[z][x][i][band]= -(ns->phi[z][x][i]+in->Fpt[z][x][i][band]);
					in->pt[z][x][i][band]=get_p_pop_srh(sim,s,ns->xpt[z][x][i][band]-in->tpt[z][x][i],in->Th[z][x][i],band);
					in->dpt[z][x][i][band]=get_dp_pop_srh(sim,s,ns->xpt[z][x][i][band]-in->tpt[z][x][i],in->Th[z][x][i],band);

				}

		}

		update_y_array(sim,in,z,x);


		if (error<0.1)
		{
			adv=TRUE;
		}
		//#ifdef print_newtonerror

		if (get_dump_status(sim,dump_print_pos_error)==TRUE)
		{
			if ((ittr%10)==0)
			{
				printf_log(sim,"%d %s = %Le %d\n",ittr,_("Electrostatic solver f()="),error,adv);
			}
		//#endif
		}
		#ifdef dump_converge



		/*in->converge=fopena(get_output_path(sim),"converge.dat","a");
		if (in->converge!=NULL)
		{
			fprintf(in->converge,"%e\n",error);
			fclose(in->converge);
		}*/
		#endif



	ittr++;

	if (adv==TRUE)
	{
		adv_step++;
	}

	if (ittr>config->max_ittr)
	{
		quit=TRUE;
	}

	if ((error<config->pos_min_error)&&(adv_step>3))
	{
		quit=TRUE;
	}

	}while(quit==FALSE);
		//getchar();

	printf_log(sim,"%d %s = %Le %d\n",ittr,_("Electrostatic solver f()="),error,adv);

	pos_dump(sim,in,config);

	update_y_array(sim,in,z,x);

	if (in->srh_sim==TRUE)
	{
		time_init(sim,in);

	}




in->odes+=dim->ylen;


/*for (y=0;y<dim->ylen;y++)
{
	printf("%d %.20Le new\n",y,in->n[0][0][y]);
}
getchar();*/


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

