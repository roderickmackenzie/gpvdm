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

/** @file dos.c
	@brief Reads in the DoS files but does not generate them, also deals with interpolation.
*/

#include <enabled_libs.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>

	#include <zlib.h>
#include "code_ctrl.h"
#include "server.h"
#include "sim.h"
#include "dump.h"
#include "lang.h"
#include "log.h"
#include "cal_path.h"
#include "util.h"
#include "inp.h"
#include <memory.h>

long double get_dos_filled_n(struct device *in)
{
int x=0;
int y=0;
int z=0;

int band;
long double n_tot=0.0;
long double n_tot_den=0.0;

struct dimensions *dim=&(in->ns.dim);
struct shape *s;
for (z=0;z<dim->zlen;z++)
{
	for (x=0;x<dim->xlen;x++)
	{
		for (y=0;y<dim->ylen;y++)
		{
			for (band=0;band<dim->srh_bands;band++)
			{
				s=in->obj_zxy[z][x][y]->s;
				n_tot+=in->nt[z][x][y][band];
				n_tot_den+=s->dosn.srh_den[band];
			}
		}
	}
}

n_tot=(n_tot)/(n_tot_den);
return n_tot;
}

long double get_dos_filled_p(struct device *in)
{
int x=0;
int y=0;
int z=0;

int band;
long double p_tot=0.0;
long double p_tot_den=0.0;
struct dimensions *dim=&(in->ns.dim);
struct shape *s;

for (z=0;z<dim->zlen;z++)
{
	for (x=0;x<dim->xlen;x++)
	{
		for (y=0;y<dim->ylen;y++)
		{
			for (band=0;band<dim->srh_bands;band++)
			{
				s=in->obj_zxy[z][x][y]->s;
				p_tot+=in->pt[z][x][y][band];
				p_tot_den+=s->dosp.srh_den[band];
			}
		}
	}
}
p_tot=(p_tot)/(p_tot_den);
return p_tot;
}

long double dos_srh_get_fermi_n(struct shape *s,long double n, long double p, int band,long double T)
{
	long double srh_sigman=s->dosn.config.srh_sigman;
	long double srh_sigmap=s->dosn.config.srh_sigmap;
	long double Nc=s->dosn.config.Nc;
	long double Nv=s->dosn.config.Nv;
	long double srh_vth=s->dosn.config.srh_vth;
	//long double srh_Nt=s->dosn.srh_den[band];
	long double srh_en=srh_vth*srh_sigman*Nc*gexp((Qe*s->dosn.srh_E[band])/(T*kb));
	long double srh_ep=srh_vth*srh_sigmap*Nv*gexp((Qe*(-1.0-s->dosn.srh_E[band]))/(T*kb));

	long double f=0.0;
	f=(n*srh_vth*srh_sigman+srh_ep)/(n*srh_vth*srh_sigman+p*srh_vth*srh_sigmap+srh_en+srh_ep);
	long double level=0.0;
	level=s->dosn.srh_E[band]-T*kb*logl((1.0/f)-1.0)/Qe;
	//printf("rod=%Le %Le %Le\n",logl((1.0/f)-1.0),(1.0/f)-1.0,f);
return level;
}

long double dos_srh_get_fermi_p(struct shape *s,long double n, long double p,int band, long double T)
{
	long double srh_sigmap=s->dosp.config.srh_sigmap;
	long double srh_sigman=s->dosp.config.srh_sigman;
	long double Nc=s->dosp.config.Nc;
	long double Nv=s->dosp.config.Nv;
	long double srh_vth=s->dosp.config.srh_vth;
	//long double srh_Nt=s->dosn.srh_den[band];
	long double srh_ep=srh_vth*srh_sigmap*Nv*gexp((Qe*s->dosp.srh_E[band])/(T*kb));
	long double srh_en=srh_vth*srh_sigman*Nc*gexp((Qe*(-1.0-s->dosp.srh_E[band]))/(T*kb));
	long double f=0.0;
	f=(p*srh_vth*srh_sigmap+srh_en)/(p*srh_vth*srh_sigmap+n*srh_vth*srh_sigman+srh_ep+srh_en);
	long double level=0.0;
	level=s->dosp.srh_E[band]-T*kb*logl((1.0/f)-1.0)/Qe;
return level;
}




long double get_Nc_free(struct shape *s)
{
return s->dosn.config.Nc;

}

long double get_Nv_free(struct shape *s)
{
return s->dosp.config.Nv;
}

void load_txt_dos_from_json(struct simulation *sim,struct dos *dos_n,struct dos *dos_p, struct json_obj *json_dos)
{
	char temp[200];

	json_get_long_double(sim,json_dos, &(dos_n->muz),"mue_z");
	json_get_long_double(sim,json_dos, &(dos_n->mux),"mue_x");
	json_get_long_double(sim,json_dos, &(dos_n->muy),"mue_y");

	json_get_string(sim, json_dos, temp,"symmetric_mobility_e");

	if (strcmp(temp,"symmetric")==0)
	{
		dos_n->muy=fabsl(dos_n->muy);
		dos_n->mux=dos_n->muy;
		dos_n->muz=dos_n->muy;

		dos_n->mobility_symmetric=TRUE;
	}else
	{
		dos_n->muy=fabsl(dos_n->muy);
		dos_n->mux=fabsl(dos_n->mux);
		dos_n->muz=fabsl(dos_n->muz);

		dos_n->mobility_symmetric=FALSE;
	}



	json_get_long_double(sim,json_dos, &(dos_p->muz),"muh_z");
	json_get_long_double(sim,json_dos, &(dos_p->mux),"muh_x");
	json_get_long_double(sim,json_dos, &(dos_p->muy),"muh_y");

	if (strcmp(temp,"symmetric")==0)
	{
		dos_p->muy=fabsl(dos_p->muy);
		dos_p->mux=dos_p->muy;
		dos_p->muz=dos_p->muy;

		dos_p->mobility_symmetric=TRUE;
	}else
	{
		dos_p->muy=fabsl(dos_p->muy);
		dos_p->mux=fabsl(dos_p->mux);
		dos_p->muz=fabsl(dos_p->muz);

		dos_p->mobility_symmetric=FALSE;
	}


	json_get_english(sim,json_dos, &(dos_n->auger_enabled),"dos_enable_auger");
	json_get_long_double(sim,json_dos, &(dos_n->Cn),"dos_auger_Cn");
	json_get_long_double(sim,json_dos, &(dos_n->Cp),"dos_auger_Cp");

	dos_p->auger_enabled=dos_n->auger_enabled;
	if (dos_n->auger_enabled==TRUE)
	{
		dos_n->Cn=fabsl(dos_n->Cn);
		dos_n->Cp=fabsl(dos_n->Cp);

		dos_p->Cn=dos_n->Cn;
		dos_p->Cp=dos_n->Cp;
	}else
	{
		dos_n->Cn=0.0;
		dos_n->Cp=0.0;

		dos_p->Cn=0.0;
		dos_p->Cp=0.0;
	}


	json_get_english(sim,json_dos, &(dos_n->ss_srh_enabled),"ss_srh_enabled");
	json_get_long_double(sim,json_dos, &(dos_n->n1),"srh_n1");
	json_get_long_double(sim,json_dos, &(dos_n->p1),"srh_p1");
	json_get_long_double(sim,json_dos, &(dos_n->tau_n),"srh_tau_n");
	json_get_long_double(sim,json_dos, &(dos_n->tau_p),"srh_tau_p");

	if (dos_n->ss_srh_enabled==TRUE)
	{
		dos_n->n1=fabsl(dos_n->n1);
		dos_n->p1=fabsl(dos_n->p1);
		dos_n->tau_n=fabsl(dos_n->tau_n);
		dos_n->tau_p=fabsl(dos_n->tau_p);

		dos_p->n1=dos_n->n1;
		dos_p->p1=dos_n->p1;
		dos_p->tau_n=dos_n->tau_n;
		dos_p->tau_p=dos_n->tau_p;
	}else
	{
		dos_n->n1=0.0;
		dos_n->p1=0.0;
		dos_n->tau_n=0.0;
		dos_n->tau_p=0.0;

		dos_p->n1=0.0;
		dos_p->p1=0.0;
		dos_p->tau_n=0.0;
		dos_p->tau_p=0.0;
	}


	json_get_long_double(sim,json_dos, &(dos_n->B),"free_to_free_recombination");
	dos_n->B=fabsl(dos_n->B);
	dos_p->B=dos_n->B;

	json_get_string(sim, json_dos, temp,"symmetric_mobility_h");


}

void load_binary_dos_file(struct simulation *sim,struct dos *mydos,char *file)
{
#ifndef dos_bin
long double srh_r1=0.0;
long double srh_r2=0.0;
long double srh_r3=0.0;
long double srh_r4=0.0;
long double srh_c=0.0;
long double w0;
long double n;
#endif
mydos->enabled=TRUE;
mydos->srh_E=NULL;
mydos->srh_den=NULL;



//if (get_dump_status(sim,dump_print_text)==TRUE)
printf_log(sim,"%s %s\n",_("Loading file"),file);


	long double *buf;
	int buf_pos=0;
	read_zip_buffer(sim,file,&buf);


	int t=0;
	int x=0;
	int srh_band=0;
	mydos->xlen=(int)buf[buf_pos++];
	mydos->tlen=(int)buf[buf_pos++];
	mydos->srh_bands=(int)buf[buf_pos++];
	mydos->config.epsilonr=buf[buf_pos++];
	mydos->config.Na0=buf[buf_pos++];
	mydos->config.Na1=buf[buf_pos++];
	mydos->config.Nd0=buf[buf_pos++];
	mydos->config.Nd1=buf[buf_pos++];
	mydos->config.ion_density=buf[buf_pos++];
	mydos->config.ion_mobility=buf[buf_pos++];
	mydos->config.srh_vth=buf[buf_pos++];
	mydos->config.srh_sigman=buf[buf_pos++];
	mydos->config.srh_sigmap=buf[buf_pos++];
	mydos->config.Nc=buf[buf_pos++];
	mydos->config.Nv=buf[buf_pos++];
	mydos->config.Nt=buf[buf_pos++];
	//printf("wow::%Le\n",mydos->config.Nt);
	//getchar();
	mydos->config.Eg=buf[buf_pos++];
	mydos->config.Xi=buf[buf_pos++];
	mydos->config.dos_free_carrier_stats=(int)buf[buf_pos++];

	long double xsteps=mydos->xlen;
	long double tsteps=mydos->tlen;
	malloc_1d((void **)&(mydos->x), xsteps, sizeof(long double));
	malloc_1d((void **)&(mydos->t), tsteps, sizeof(long double));

	malloc_1d( (void **)&(mydos->srh_E), mydos->srh_bands, sizeof(long double));
	malloc_1d( (void **)&(mydos->srh_den), mydos->srh_bands, sizeof(long double));

	malloc_3d((void ****)&(mydos->srh_r1), mydos->tlen, mydos->xlen, mydos->srh_bands, sizeof(long double));
	malloc_3d((void ****)&(mydos->srh_r2), mydos->tlen, mydos->xlen, mydos->srh_bands, sizeof(long double));
	malloc_3d((void ****)&(mydos->srh_r3), mydos->tlen, mydos->xlen, mydos->srh_bands, sizeof(long double));
	malloc_3d((void ****)&(mydos->srh_r4), mydos->tlen, mydos->xlen, mydos->srh_bands, sizeof(long double));
	malloc_3d((void ****)&(mydos->srh_c), mydos->tlen, mydos->xlen, mydos->srh_bands, sizeof(long double));

	malloc_2d_long_double(mydos->tlen, mydos->xlen, &(mydos->c));
	malloc_2d_long_double(mydos->tlen, mydos->xlen, &(mydos->w));


	for (x=0;x<xsteps;x++)
	{
		mydos->x[x]=buf[buf_pos++];
		//fprintf(rt,"%le\n",(mydos->x[x]));
	}

	for (t=0;t<tsteps;t++)
	{
		mydos->t[t]=buf[buf_pos++];
		//fprintf(rt,"%le\n",(mydos->t[t]));
	}


	for (srh_band=0;srh_band<(mydos->srh_bands);srh_band++)
	{
		mydos->srh_E[srh_band]=buf[buf_pos++];
		//fprintf(rt,"%le\n",(mydos->srh_E[srh_band]));
	}

	for (srh_band=0;srh_band<(mydos->srh_bands);srh_band++)
	{
		mydos->srh_den[srh_band]=buf[buf_pos++];
		//fprintf(rt,"%le\n",(mydos->srh_den[srh_band]));
	}

	for (t=0;t<tsteps;t++)
	{
		for (x=0;x<xsteps;x++)
		{

			mydos->c[t][x]=buf[buf_pos++];
			mydos->w[t][x]=buf[buf_pos++];
			//fprintf(rt,"%.20le %.20le ",mydos->c[t][x],mydos->w[t][x]);

			for (srh_band=0;srh_band<mydos->srh_bands;srh_band++)
			{
				mydos->srh_r1[t][x][srh_band]=buf[buf_pos++];
				mydos->srh_r2[t][x][srh_band]=buf[buf_pos++];
				mydos->srh_r3[t][x][srh_band]=buf[buf_pos++];
				mydos->srh_r4[t][x][srh_band]=buf[buf_pos++];
				mydos->srh_c[t][x][srh_band]=buf[buf_pos++];

				//fprintf(rt,"%.20le %.20le %.20le %.20le %.20le",mydos->srh_r1[t][x][srh_band],mydos->srh_r2[t][x][srh_band],mydos->srh_r3[t][x][srh_band],mydos->srh_r4[t][x][srh_band],mydos->srh_c[t][x][srh_band]);

			}
		//fprintf(rt,"\n");
		}

	}
	free(buf);

//fclose(rt);

}

long double get_dos_ion_density(struct shape *s)
{
return s->dosn.config.ion_density;
}

long double get_dos_ion_mobility(struct shape *s)
{
return s->dosn.config.ion_mobility;
}


long double get_dos_epsilonr(struct shape *s)
{
	return s->dosn.config.epsilonr;
}

long double dos_get_band_energy_n(struct shape *s,int band)
{
return s->dosn.srh_E[band];
}

long double dos_get_band_energy_p(struct shape *s,int band)
{
return s->dosp.srh_E[band];
}

long double get_dos_Eg(struct shape *s)
{
return s->dosn.config.Eg;
}

long double get_dos_Xi(struct shape *s)
{
return s->dosn.config.Xi;
}

void load_dos(struct simulation *sim,struct device *in,struct shape *s,struct json_obj *json_dos)
{
	char binary_path[PATH_MAX];
	//char txt_path[PATH_MAX];
	char file_name[200];
	//electrons
	sprintf(file_name,"%s_dosn.dat",s->dos_file);
	join_path(2, binary_path,get_cache_path(sim),file_name);

	load_binary_dos_file(sim,&(s->dosn),binary_path);

	//holes
	sprintf(file_name,"%s_dosp.dat",s->dos_file);
	join_path(2, binary_path,get_cache_path(sim),file_name);

	load_binary_dos_file(sim,&(s->dosp),binary_path);

	load_txt_dos_from_json(sim,&(s->dosn),&(s->dosp),json_dos);


	in->ns.dim.srh_bands=s->dosn.srh_bands;


	if (s->dosn.mobility_symmetric==FALSE)
	{
		in->mun_symmetric=FALSE;
	}
	if (s->dosp.mobility_symmetric==FALSE)
	{
		in->mup_symmetric=FALSE;
	}
}

long double get_dos_E_n(struct shape *s,int band)
{
return s->dosn.srh_E[band];
}

long double get_dos_E_p(struct shape *s,int band)
{
return s->dosp.srh_E[band];
}



