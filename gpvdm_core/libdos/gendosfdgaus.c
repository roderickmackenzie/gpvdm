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

/** @file gendosfdgause.c
	@brief Generates the DoS files.
*/

#include <stdio.h>
#include <stdlib.h>

	#include <zlib.h>

#include <code_ctrl.h>
#include <sim.h>
#include <inp.h>
#include <util.h>
#include <dat_file.h>
#include <hard_limit.h>
#include <epitaxy.h>
#include <lang.h>
#include <dos.h>

static int unused __attribute__((unused));

#include "dos_an.h"

#include "server.h"
#include "dump.h"
#include "log.h"
#include "checksum.h"
#include "cal_path.h"


void dos_double_res(struct simulation *sim)
{
/*gdouble number;
FILE *in=fopen("srhbandp.inp","r");
FILE *out=fopen("srhbandp.new","w");
do
{
unused=fscanf(in,"%Le",&number);
fprintf(out,"%Le\n",number);
fprintf(out,"%Le\n",number);
}while(!feof(in));
fclose(in);
fclose(out);
*/
int i;
for (i=1;i<=80;i++)
{
printf_log(sim,"p srhbandp.inp %d 1\n",i);
}
}


void gen_do(struct simulation *sim,struct dosconfig *in,struct dosconfig *in2,char * outfile,int electrons)
{
char name[200];
char temp[1000];
gdouble tstart=0.0;
gdouble tstop=0.0;
gdouble tsteps=0.0;
gdouble dt=0.0;
gdouble xpos=0.0;
gdouble tpos=0.0;
int t=0;
int x=0;
int band;
gdouble *srh_r1=NULL;
gdouble *srh_r2=NULL;
gdouble *srh_r3=NULL;
gdouble *srh_r4=NULL;
gdouble *srh_n=NULL;
gdouble *srh_den=NULL;
gdouble *srh_dE_sum=NULL;
gdouble *srh_read=NULL;

gdouble *srh_x=NULL;
gdouble *srh_mid=NULL;
gdouble *band_E_mesh=NULL;
int *band_i=NULL;

int e=0;
gdouble E=0.0;
gdouble dE=0.0;
gdouble rho=0.0;
gdouble sum=0.0;
gdouble f=0.0;
gdouble last_n0=0;
gdouble *xmesh=NULL;

long double srh_pos=0.0;
long double srh_delta=0.0;

int i;
int band_pos=0;
int cur_band=0;
int points_per_band=0;

long double E_free_start=0.0;
long double E_free_stop=2.0;
int E_free_points=1000.0;
long double dE_free=(E_free_stop-E_free_start)/(long double)E_free_points;
long double sum_l=0.0;
long double sum_r=0.0;
long double pos=0.0;
struct dos_an_data my_dos_an;

//#define dos_test_stats
#ifdef dos_test_stats
FILE *freetest;
if (electrons==TRUE)
{
	sprintf(name,"freetestn.dat");
	remove_file(sim,name);
}else
{
	sprintf(name,"freetestp.dat");
	remove_file(sim,name);
}
#endif

if (in->srh_bands!=0)
{
	srh_r1=(gdouble *)malloc(sizeof(gdouble)*in->srh_bands);
	srh_r2=(gdouble *)malloc(sizeof(gdouble)*in->srh_bands);
	srh_r3=(gdouble *)malloc(sizeof(gdouble)*in->srh_bands);
	srh_r4=(gdouble *)malloc(sizeof(gdouble)*in->srh_bands);
	srh_n=(gdouble *)malloc(sizeof(gdouble)*in->srh_bands);
	srh_den=(gdouble *)malloc(sizeof(gdouble)*in->srh_bands);
	srh_dE_sum=(gdouble *)malloc(sizeof(gdouble)*in->srh_bands);
	srh_read=(gdouble *)malloc(sizeof(gdouble)*in->srh_bands);
	srh_x=(gdouble *)malloc(sizeof(gdouble)*in->srh_bands);
	srh_mid=(gdouble *)malloc(sizeof(gdouble)*in->srh_bands);
}

if (in->Esteps!=0)
{
	band_E_mesh=(gdouble *)malloc(sizeof(gdouble)*in->Esteps);
	band_i=(int *)malloc(sizeof(int)*in->Esteps);
}


tstart=in->Tstart;
tstop=in->Tstop;
tsteps=in->Tsteps;
dt=(tstop-tstart)/tsteps;

gdouble dxr=(in->nstop-in->nstart)/((gdouble)in->npoints);

xpos=in->nstart;
tpos=tstart;
t=0;
x=0;

int buf_len=0;
buf_len+=17;
buf_len+=in->npoints;		//mesh
buf_len+=tsteps;		//mesh
buf_len+=in->srh_bands;	//mesh
buf_len+=in->srh_bands;	//holds the density
buf_len+=tsteps*in->npoints*2; //data
buf_len+=tsteps*in->npoints*5*in->srh_bands; //data

int buf_pos=0;
gdouble *buf=(gdouble*)malloc(buf_len*sizeof(gdouble));
buf[buf_pos++]=(gdouble)in->npoints;
buf[buf_pos++]=(gdouble)tsteps;
buf[buf_pos++]=(gdouble)in->srh_bands;
buf[buf_pos++]=(gdouble)in->epsilonr;
buf[buf_pos++]=(gdouble)in->doping_start;
buf[buf_pos++]=(gdouble)in->doping_stop;
buf[buf_pos++]=(gdouble)in->ion_density;
buf[buf_pos++]=(gdouble)in->ion_mobility;
buf[buf_pos++]=(gdouble)in->srh_vth;
buf[buf_pos++]=(gdouble)in->srh_sigman;
buf[buf_pos++]=(gdouble)in->srh_sigmap;
buf[buf_pos++]=(gdouble)in->Nc;
buf[buf_pos++]=(gdouble)in->Nv;
buf[buf_pos++]=(gdouble)in->Nt;
buf[buf_pos++]=(gdouble)in->Eg;
buf[buf_pos++]=(gdouble)in->Xi;
buf[buf_pos++]=(gdouble)in->dos_free_carrier_stats;

xmesh=(gdouble *)malloc(sizeof(gdouble)*in->npoints);

for (x=0;x<in->npoints;x++)
{
	buf[buf_pos++]=xpos;
	xmesh[x]=xpos;
	xpos+=dxr;
}

for (t=0;t<tsteps;t++)
{
	buf[buf_pos++]=tpos;
	printf("t=%Le\n",buf[buf_pos-1]);
	tpos+=dt;
}

//Allocate where the traps are going to be in energy space
srh_delta=fabs(in->srh_stop-in->srh_start)/(long double)(in->srh_bands);
srh_pos=in->srh_start;

for (band=0;band<in->srh_bands;band++)
{
	srh_pos+=srh_delta/2.0;
	srh_mid[band]=srh_pos;
	buf[buf_pos++]=srh_pos;

	srh_pos+=srh_delta/2.0;
	srh_x[band]=srh_pos;

}


//Build the energy mesh for integration
if (in->srh_bands>0)
{
	points_per_band=in->Esteps/in->srh_bands;
}

pos=in->srh_start;
dE=fabs(in->srh_stop-in->srh_start)/((gdouble)in->Esteps);

//FILE *test4=fopen("test4.dat","w");
for (i=0;i<in->Esteps;i++)
{
	band_E_mesh[i]=pos;
	band_i[i]=cur_band;
	//fprintf(test4,"%Le %d\n",band_E_mesh[i],band_i[i]);
	//getchar();
	pos+=dE;
	band_pos++;
	if (band_pos>=points_per_band)
	{
		band_pos=0;
		cur_band++;
	}



}
//fclose(test4);
//getchar();

tpos=tstart;

if (in->dostype==dos_read)
{
FILE *dosread;
	if (electrons==TRUE)
	{
		sprintf(name,"%s_srhbandn.dat",in->dos_name);
		dosread=fopen(name,"r");
	}else
	{
		sprintf(name,"%s_srhbandp.dat",in2->dos_name);
		dosread=fopen(name,"r");
	}

	if (dosread==NULL)
	{
		ewe(sim,"can not open srhband file\n");
	}

	for (band=0;band<in->srh_bands;band++)
	{
		unused=fscanf(dosread,"%Le",&(srh_read[band]));
		printf_log(sim,"%Le\n",srh_read[band]);
		srh_read[band]=fabs(srh_read[band]);
	}


	fclose(dosread);
}else
if (in->dostype==dos_an)
{
printf_log(sim,"---------------------Generating---------------------\n");
	if (electrons==TRUE)
	{
		sprintf(name,"%s.inp",in->analytical_dos_file_name);
		dos_an_load(sim,&my_dos_an,name);
	}else
	{
		sprintf(name,"%s.inp",in2->analytical_dos_file_name);
		dos_an_load(sim,&my_dos_an,name);
	}

	for (band=0;band<in->srh_bands;band++)
	{
		srh_read[band]=fabs(dos_an_get_value(sim,&my_dos_an,srh_mid[band]));
		printf_log(sim,"%d %Le\n",band,srh_read[band]);
	}

}

#ifdef test_dist
FILE *munfile=fopen("munfile.dat","w");
if (munfile==NULL)
{
	ewe("problem\n");
}


FILE *plotbands;
plotbands=fopen("plotbandsn.dat","w");

FILE *plotbandsfree;
plotbandsfree=fopen("plotbandsfreen.dat","w");




FILE *rod=fopen("gau_test_n.dat","w");

#endif


#ifdef test_dist
int first=FALSE;
#endif

struct dat_file dos_out;
buffer_init(&dos_out);

if (get_dump_status(sim,dump_write_out_band_structure)==TRUE)
{
	if (electrons==TRUE)
	{
		buffer_malloc(&dos_out);
		dos_out.y_mul=1.0;
		dos_out.x_mul=1.0;
		strcpy(dos_out.title,"Discretized density of states");
		strcpy(dos_out.type,"xy");
		strcpy(dos_out.x_label,"Energy");
		strcpy(dos_out.y_label,"Density of states");
		strcpy(dos_out.x_units,"eV");
		strcpy(dos_out.y_units,"m^{-3}eV^{-1}");
		dos_out.logscale_x=0;
		dos_out.logscale_y=0;
		buffer_add_info(sim,&dos_out);
	}else
	{
		buffer_malloc(&dos_out);
		dos_out.y_mul=1.0;
		dos_out.x_mul=1.0;
		strcpy(dos_out.title,"Discretized density of states");
		strcpy(dos_out.type,"xy");
		strcpy(dos_out.x_label,"Energy");
		strcpy(dos_out.y_label,"Density of states");
		strcpy(dos_out.x_units,"eV");
		strcpy(dos_out.y_units,"m^{-3}eV^{-1}");
		dos_out.logscale_x=0;
		dos_out.logscale_y=0;
		buffer_add_info(sim,&dos_out);
	}
}

int srh_band=0;
gdouble srh_E=0.0;
gdouble srh_f=0.0;
for (t=0;t<tsteps;t++)
{

printf_log(sim,"%d/%d\n",t,(int)tsteps);

	for (x=0;x<in->npoints;x++)
	{
		xpos=xmesh[x];
		E=0.0;

		for (srh_band=0;srh_band<(in->srh_bands);srh_band++)
		{
			srh_r1[srh_band]=0.0;
			srh_r2[srh_band]=0.0;
			srh_r3[srh_band]=0.0;
			srh_r4[srh_band]=0.0;
			srh_n[srh_band]=0.0;
			srh_den[srh_band]=0.0;
			srh_dE_sum[srh_band]=0.0;
		}

		srh_band=0;

		for (e=0;e<in->Esteps;e++)
		{
			E=band_E_mesh[e];
			srh_band=band_i[e];
			srh_E=srh_mid[srh_band];

			f=1.0/(1.0+exp((E-xpos)*Q/(kb*tpos)));

			srh_f=1.0/(1.0+exp((srh_E-xpos)*Q/(kb*tpos)));

			if (in->dostype==dos_exp)
			{
				rho=in->Nt*exp((E)/(in->Et));
			}else
			if ((in->dostype==dos_read)||(in->dostype==dos_an))
			{
				rho=srh_read[srh_band];
			}else
			{
				ewe(sim,"I don't understand this DoS type.\n");
			}


			if (x==0)
			{
				if (get_dump_status(sim,dump_write_out_band_structure)==TRUE)
				{
					if (E>in->srh_start)
					{
						if (electrons==TRUE)
						{
							sprintf(temp,"%Le %Le\n",E-in->Xi,rho);
							buffer_add_string(&dos_out,temp);
						}else
						{
							sprintf(temp,"%Le %Le\n",-E-in->Xi-in->Eg,rho);
							buffer_add_string(&dos_out,temp);
						}

					}
				}
			}


			if (electrons==TRUE)
			{
				srh_r1[srh_band]+=in->srh_vth*in->srh_sigman*rho*(1.0-srh_f)*dE;
				srh_r2[srh_band]+=in->srh_vth*in->srh_sigman*in->Nc*exp((srh_E*Q)/(tpos*kb))*rho*srh_f*dE;//in->srh_vth*srh_sigman*rho*(1.0-srh_f)*dE;//
				srh_r3[srh_band]+=in->srh_vth*in->srh_sigmap*rho*srh_f*dE;
				srh_r4[srh_band]+=in->srh_vth*in->srh_sigmap*in->Nv*exp((-in->Eg*Q-srh_E*Q)/(tpos*kb))*rho*(1.0-srh_f)*dE;//in->srh_vth*srh_sigmap*rho*srh_f*dE;//
				//if (srh_r1[srh_band]<1e-3)
				//{
				//	printf("%Lf %Le %Le %Le %Le\n",srh_mid[srh_band],srh_r1[srh_band],srh_r2[srh_band],srh_r3[srh_band],srh_r4[srh_band]);
				//}
				srh_n[srh_band]+=rho*srh_f*dE;
				srh_den[srh_band]+=rho*dE;
				srh_dE_sum[srh_band]+=dE;
			}else
			{
				srh_r1[srh_band]+=in->srh_vth*in->srh_sigmap*rho*(1.0-srh_f)*dE;
				srh_r2[srh_band]+=in->srh_vth*in->srh_sigmap*in->Nv*exp((srh_E*Q)/(tpos*kb))*rho*srh_f*dE;//in->srh_vth*srh_sigman*rho*(1.0-srh_f)*dE;//
				srh_r3[srh_band]+=in->srh_vth*in->srh_sigman*rho*srh_f*dE;
				srh_r4[srh_band]+=in->srh_vth*in->srh_sigman*in->Nc*exp((-in->Eg*Q-(srh_E*Q))/(tpos*kb))*rho*(1.0-srh_f)*dE;//in->srh_vth*srh_sigmap*rho*srh_f*dE;//
				srh_n[srh_band]+=rho*srh_f*dE;
				srh_den[srh_band]+=rho*dE;
				srh_dE_sum[srh_band]+=dE;
			}

		}



		if (x==0)
		{
			for (band=0;band<in->srh_bands;band++)
			{
				buf[buf_pos++]=srh_den[band];
			}

			if (get_dump_status(sim,dump_band_structure)==TRUE)
			{
				FILE *bandsdump;
				if (electrons==TRUE)
				{
					bandsdump=fopen("lumo_out.dat","w");
					for (band=0;band<in->srh_bands;band++)
					{
						fprintf(bandsdump,"%Le\n",srh_den[band]/srh_dE_sum[band]);
					}
					fclose(bandsdump);
				}else
				{
					bandsdump=fopen("homo_out.dat","w");
					for (band=0;band<in->srh_bands;band++)
					{
						fprintf(bandsdump,"%Le\n",srh_den[band]/srh_dE_sum[band]);
					}
					fclose(bandsdump);
				}
			}
		}

		///////////Free stuff
		sum_l=0.0;
		sum_r=0.0;
		long double Nc=2.0*powl((in->me*m0*kb*tpos)/(2.0*PI*hbar*hbar),3.0/2.0);
		long double Nv=2.0*powl((in->mh*m0*kb*tpos)/(2.0*PI*hbar*hbar),3.0/2.0);

		if (in->dos_free_carrier_stats==fd_look_up_table)
		{
			E=E_free_start;
			sum=0.0;

			gdouble w0=0.0;
			for (e=0;e<E_free_points;e++)
			{

				if (electrons==TRUE)
				{
					rho=(sqrtl(E*Q)/(2.0*PI*PI))*pow((2.0*in->me*m0)/(hbar*hbar),3.0/2.0);
				}else
				{
					rho=(sqrtl(E*Q)/(2.0*PI*PI))*pow((2.0*in->mh*m0)/(hbar*hbar),3.0/2.0);
				}

				f=1.0/(1.0+expl((E-xpos)*Q/(kb*tpos)));
				sum+=rho*Q*f*dE_free;

				f=1.0/(1.0+expl((E-xpos-dxr/2.0)*Q/(kb*tpos)));
				sum_l+=rho*Q*f*dE_free;

				f=1.0/(1.0+expl((E-xpos+dxr/2.0)*Q/(kb*tpos)));
				sum_r+=rho*Q*f*dE_free;
				E+=dE_free;
				//if (electrons==FALSE)
				//{
				//	printf("%Le %Le\n",rho,in->mh);
				//	getchar();
				//}

			}

			//printf("here %Le %Le %Le \n",sum,in->Nc*exp((xpos*Q)/(kb*tpos)),xpos);//
			//getchar();


		}else
		if (in->dos_free_carrier_stats==mb_look_up_table)
		{
			E=E_free_start;
			sum=0.0;

			gdouble w0=0.0;

			for (e=0;e<E_free_points;e++)
			{

				if (electrons==TRUE)
				{
					rho=(sqrtl(E*Q)/(2.0*PI*PI))*pow((2.0*in->me*m0)/(hbar*hbar),3.0/2.0);
				}else
				{
					rho=(sqrtl(E*Q)/(2.0*PI*PI))*pow((2.0*in->mh*m0)/(hbar*hbar),3.0/2.0);
				}

				f=expl((xpos-E)*Q/(kb*tpos));
				sum+=rho*Q*f*dE_free;

				f=expl((xpos-E-dxr/2.0)*Q/(kb*tpos));
				sum_l+=rho*Q*f*dE_free;

				f=expl((xpos-E+dxr/2.0)*Q/(kb*tpos));
				sum_r+=rho*Q*f*dE_free;
				E+=dE_free;
				//if (electrons==FALSE)
				//{
				//	printf("%Le %Le\n",rho,in->mh);
				//	getchar();
				//}

			}

		}else
		if (in->dos_free_carrier_stats==mb_look_up_table_analytic)
		{
			if (electrons==TRUE)
			{
				sum_l=Nc*exp(((xpos-dxr/2.0)*Q)/(kb*tpos));
				sum=Nc*exp((xpos*Q)/(kb*tpos));
				sum_r=Nc*exp(((xpos+dxr/2.0)*Q)/(kb*tpos));

			}else
			{
				sum_l=Nv*exp(((xpos-dxr/2.0)*Q)/(kb*tpos));
				sum=Nv*exp((xpos*Q)/(kb*tpos));
				sum_r=Nv*exp(((xpos+dxr/2.0)*Q)/(kb*tpos));
			}
		}else
		{
			sum=-1.0;
		}

		//printf("%d %Le\n",in->dos_free_carrier_stats,sum);
		//if (electrons==TRUE)
		//{
		//	printf("%Le %Le %Lf\n",in->Nc*exp((xpos*Q)/(kb*tpos)),sum,tpos);
		//}
		//getchar();
		//printf("%Le %Le\n",xpos-dxr/2.0,xpos+dxr/2.0);
		gdouble w0=(3.0/2.0)*sum/((fabs(sum_l-sum_r))/(dxr*Q));

		//printf("%Le %Le\n",w0,(3.0/2.0)*kb*tpos);
		//getchar();
		//if (x==0) w0=(3.0/2.0)*kb*tpos;

		buf[buf_pos++]=sum;
		buf[buf_pos++]=w0;


		for (srh_band=0;srh_band<in->srh_bands;srh_band++)
		{
				buf[buf_pos++]=srh_r1[srh_band];
				buf[buf_pos++]=srh_r2[srh_band];
				buf[buf_pos++]=srh_r3[srh_band];
				buf[buf_pos++]=srh_r4[srh_band];

				buf[buf_pos++]=srh_n[srh_band];
		}

		#ifdef dos_test_stats
			if ((x%100)==0)
			{
				//printf("%d\n",x);
				if (electrons==TRUE)
				{
					freetest=fopen("freetestn.dat","a");
					//fprintf(freetest,"%.20le %.20le %.20le %.20le\n",xpos,sum,in->Nc*exp((xpos*Q)/(kb*tpos)),in->Nv*exp((-(in->Eg+xpos)*Q)/(kb*tpos)));
					//fprintf(freetest,"%Le %Le %Le\n",xpos,in->Nc*exp((xpos*Q)/(kb*tpos)),sum);

					for (srh_band=0;srh_band<in->srh_bands;srh_band++)
					{
						fprintf(freetest,"%Le %Le\n",srh_mid[srh_band],srh_r2[0]);
					}
					fprintf(freetest,"\n");

					fclose(freetest);
				}else
				{
					freetest=fopen("freetestp.dat","a");
					//fprintf(freetest,"%Le %Le %Le\n",xpos,in->Nv*exp((xpos*Q)/(kb*tpos)),sum);

					for (srh_band=0;srh_band<in->srh_bands;srh_band++)
					{
						fprintf(freetest,"%Le %Le\n",srh_mid[srh_band],srh_n[srh_band]);
					}
					fprintf(freetest,"\n");

					fclose(freetest);
				}
			}
		#endif


		#ifdef test_dist
		fprintf(rod,"\n");
		//if (t==0) fprintf(munfile,"%Le %Le\n",n_tot,mu_tot/n_tot);
		#endif


	}
	//getchar();
//getchar();

tpos+=dt;
}

#ifdef test_dist
	fclose(rod);
	fclose(plotbands);
	fclose(plotbandsfree);
	fclose(munfile);
#endif

if (get_dump_status(sim,dump_write_out_band_structure)==TRUE)
{
	if (electrons==TRUE)
	{
		sprintf(name,"%s_dosoutn.dat",in->dos_name);
	}else
	{
		sprintf(name,"%s_dosoutp.dat",in2->dos_name);
	}

	buffer_dump(sim,name,&dos_out);
	buffer_free(&dos_out);

}

	if (buf_len!=buf_pos)
	{
	ewe(sim,_("Expected dos size is different from generated\n"));
	}


	/*FILE* file;
	file = fopen ("one.dat", "w");
	for (i=0;i<buf_len;i++)
	{
		fprintf ( file,"%d %Le\n",i, buf[i]);
	}
	fclose (file);*/
	
	write_zip_buffer(sim,outfile,buf,buf_len);
	free(buf);

free(xmesh);
free(srh_r1);
free(srh_r2);
free(srh_r3);
free(srh_r4);
free(srh_n);
free(srh_den);
free(srh_dE_sum);
free(srh_read);

free(band_E_mesh);
free(band_i);

free(srh_x);
free(srh_mid);


//if (electrons==TRUE)
//{
//		FILE *test3=fopen("test3.dat","w");
//		for (srh_band=0;srh_band<in->srh_bands;srh_band++)
//		{
//			fprintf(test3,"%d %Le\n",srh_band,srh_den[srh_band]);
//		}
//		fclose(test3);
//		getchar();
//}

return;
}


void gen_dos_fd_gaus_n(struct simulation *sim,char * dos_file, char *lumo_file, char *homo_file)
{

	char temp[100];
	char path[PATH_MAX];
	struct dosconfig confige;
	struct dosconfig configh;

	dos_config_load(sim,&confige,&configh,dos_file, lumo_file, homo_file);
	//printf(">>%Le\n",confige.srh_stop);
	//getchar();

	printf_log(sim,"Electrons.... %s\n",confige.dos_name);

	join_path(2, path,get_input_path(sim),"cache");

	gpvdm_mkdir(path);

	sprintf(temp,"%s_dosn.dat",confige.dos_name);
	join_path(3, path,get_input_path(sim),"cache",temp);

	gen_do(sim,&confige,&configh,path,TRUE);

}

void gen_dos_fd_gaus_p(struct simulation *sim,char * dos_file, char *lumo_file, char *homo_file)
{
	char temp[100];
	char path[PATH_MAX];
	FILE *out;
	struct dosconfig confige;
	struct dosconfig configh;

	dos_config_load(sim,&confige,&configh,dos_file, lumo_file, homo_file);

	printf_log(sim,"Holes.... %s\n",configh.dos_name);

	join_path(2, path,get_input_path(sim),"cache");
	gpvdm_mkdir(path);

	sprintf(temp,"%s_dosp.dat",configh.dos_name);
	join_path(3, path,get_input_path(sim),"cache",temp);
	gen_do(sim,&configh,&confige,path,FALSE);

	join_path(3, path,get_input_path(sim),"cache","mat.inp");

	out=fopen(path,"w");
	fprintf(out,"#gpvdm_file_type\n");
	fprintf(out,"cache\n");
	fprintf(out,"#end\n");
	fclose(out);
}








