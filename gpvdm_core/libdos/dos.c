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

/** @file dos.c
	@brief Reads in the DoS files but does not generate them, also deals with interpolation.
*/

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
struct epitaxy *epi=&(in->my_epitaxy);

for (z=0;z<dim->zlen;z++)
{
	for (x=0;x<dim->xlen;x++)
	{
		for (y=0;y<dim->ylen;y++)
		{
			for (band=0;band<dim->srh_bands;band++)
			{
				n_tot+=in->nt[z][x][y][band];
				n_tot_den+=epi->dosn[in->imat[z][x][y]].srh_den[band];
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
struct epitaxy *epi=&(in->my_epitaxy);

for (z=0;z<dim->zlen;z++)
{
	for (x=0;x<dim->xlen;x++)
	{
		for (y=0;y<dim->ylen;y++)
		{
			for (band=0;band<dim->srh_bands;band++)
			{
				p_tot+=in->pt[z][x][y][band];
				p_tot_den+=epi->dosp[in->imat[z][x][y]].srh_den[band];
			}
		}
	}
}
p_tot=(p_tot)/(p_tot_den);
return p_tot;
}

long double dos_srh_get_fermi_n(struct epitaxy *epi,long double n, long double p,int band,int mat,long double T)
{
	long double srh_sigman=epi->dosn[mat].config.srh_sigman;
	long double srh_sigmap=epi->dosn[mat].config.srh_sigmap;
	long double Nc=epi->dosn[mat].config.Nc;
	long double Nv=epi->dosn[mat].config.Nv;
	long double srh_vth=epi->dosn[mat].config.srh_vth;
	//long double srh_Nt=epi->dosn[mat].srh_den[band];
	long double srh_en=srh_vth*srh_sigman*Nc*gexp((Q*epi->dosn[mat].srh_E[band])/(T*kb));
	long double srh_ep=srh_vth*srh_sigmap*Nv*gexp((Q*(-1.0-epi->dosn[mat].srh_E[band]))/(T*kb));

	long double f=0.0;
	f=(n*srh_vth*srh_sigman+srh_ep)/(n*srh_vth*srh_sigman+p*srh_vth*srh_sigmap+srh_en+srh_ep);
	long double level=0.0;
	level=epi->dosn[mat].srh_E[band]-T*kb*logl((1.0/f)-1.0)/Q;
	//printf("rod=%Le %Le %Le\n",logl((1.0/f)-1.0),(1.0/f)-1.0,f);
return level;
}

long double dos_srh_get_fermi_p(struct epitaxy *epi,long double n, long double p,int band,int mat, long double T)
{
	long double srh_sigmap=epi->dosp[mat].config.srh_sigmap;
	long double srh_sigman=epi->dosp[mat].config.srh_sigman;
	long double Nc=epi->dosp[mat].config.Nc;
	long double Nv=epi->dosp[mat].config.Nv;
	long double srh_vth=epi->dosp[mat].config.srh_vth;
	//long double srh_Nt=epi->dosn[mat].srh_den[band];
	long double srh_ep=srh_vth*srh_sigmap*Nv*gexp((Q*epi->dosp[mat].srh_E[band])/(T*kb));
	long double srh_en=srh_vth*srh_sigman*Nc*gexp((Q*(-1.0-epi->dosp[mat].srh_E[band]))/(T*kb));
	long double f=0.0;
	f=(p*srh_vth*srh_sigmap+srh_en)/(p*srh_vth*srh_sigmap+n*srh_vth*srh_sigman+srh_ep+srh_en);
	long double level=0.0;
	level=epi->dosp[mat].srh_E[band]-T*kb*logl((1.0/f)-1.0)/Q;
return level;
}




long double get_Nc_free(struct epitaxy *epi,int mat)
{
return epi->dosn[mat].config.Nc;

}

long double get_Nv_free(struct epitaxy *epi,int mat)
{
return epi->dosp[mat].config.Nv;
}

void load_txt_dos_file(struct simulation *sim,struct dos *dos_n,struct dos *dos_p,char *file)
{
	struct inp_file inp;

	inp_init(sim,&inp);
	inp_load(sim,&inp,file);
	inp_search_gdouble(sim,&inp,&(dos_n->muz),"#mue_z");
	inp_search_gdouble(sim,&inp,&(dos_n->mux),"#mue_x");
	inp_search_gdouble(sim,&inp,&(dos_n->muy),"#mue_y");
	dos_n->muz=fabsl(dos_n->muz);
	dos_n->mux=fabsl(dos_n->mux);
	dos_n->muy=fabsl(dos_n->muy);

	inp_search_gdouble(sim,&inp,&(dos_p->muz),"#muh_z");
	inp_search_gdouble(sim,&inp,&(dos_p->mux),"#muh_x");
	inp_search_gdouble(sim,&inp,&(dos_p->muy),"#muh_y");
	dos_p->muz=fabsl(dos_p->muz);
	dos_p->mux=fabsl(dos_p->mux);
	dos_p->muy=fabsl(dos_p->muy);

	inp_search_gdouble(sim,&inp,&(dos_n->B),"#free_to_free_recombination");
	dos_n->B=fabsl(dos_n->B);
	dos_p->B=dos_n->B;

	char temp[200];
	inp_search_string(sim,&inp,temp,"#symmetric_mobility_e");
	//printf("%s\n",temp);
	//getchar();
	if (strcmp(temp,"symmetric")==0)
	{
		dos_n->mobility_symmetric=TRUE;
	}else
	{
		dos_n->mobility_symmetric=FALSE;
	}

	inp_search_string(sim,&inp,temp,"#symmetric_mobility_h");
	if (strcmp(temp,"symmetric")==0)
	{
		dos_p->mobility_symmetric=TRUE;
	}else
	{
		dos_p->mobility_symmetric=FALSE;
	}

	inp_free(sim,&inp);
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
mydos->used=TRUE;
mydos->used=TRUE;
mydos->srh_E=NULL;
mydos->srh_den=NULL;



if (get_dump_status(sim,dump_print_text)==TRUE) printf_log(sim,"%s %s\n",_("Loading file"),file);


	long double *buf;
	int buf_pos=0;
	int buf_len=read_zip_buffer(sim,file,&buf);


	int t=0;
	int x=0;
	int srh_band=0;
	mydos->xlen=(int)buf[buf_pos++];
	mydos->tlen=(int)buf[buf_pos++];
	mydos->srh_bands=(int)buf[buf_pos++];
	mydos->config.epsilonr=buf[buf_pos++];
	mydos->config.doping_start=buf[buf_pos++];
	mydos->config.doping_stop=buf[buf_pos++];
	mydos->config.ion_density=buf[buf_pos++];
	mydos->config.ion_mobility=buf[buf_pos++];
	mydos->config.srh_vth=buf[buf_pos++];
	mydos->config.srh_sigman=buf[buf_pos++];
	mydos->config.srh_sigmap=buf[buf_pos++];
	mydos->config.Nc=buf[buf_pos++];
	mydos->config.Nv=buf[buf_pos++];

	mydos->config.Nt=buf[buf_pos++];
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

long double get_dos_ion_density(struct epitaxy *epi,int mat)
{
return epi->dosn[mat].config.ion_density;
}

long double get_dos_ion_mobility(struct epitaxy *epi,int mat)
{
return epi->dosn[mat].config.ion_mobility;
}


long double get_dos_doping_start(struct epitaxy *epi,int mat)
{
return epi->dosn[mat].config.doping_start;
}

long double get_dos_doping_stop(struct epitaxy *epi,int mat)
{
return epi->dosn[mat].config.doping_stop;
}

long double get_dos_epsilonr(struct epitaxy *epi,int mat)
{
return epi->dosn[mat].config.epsilonr;
}

long double dos_get_band_energy_n(struct epitaxy *epi,int band,int mat)
{
return epi->dosn[mat].srh_E[band];
}

long double dos_get_band_energy_p(struct epitaxy *epi,int band,int mat)
{
return epi->dosp[mat].srh_E[band];
}

long double get_dos_Eg(struct epitaxy *epi,int mat)
{
return epi->dosn[mat].config.Eg;
}

long double get_dos_Xi(struct epitaxy *epi,int mat)
{
return epi->dosn[mat].config.Xi;
}

void load_dos(struct simulation *sim,struct device *in,char *dos_file,int mat)
{
	char binary_path[PATH_MAX];
	char txt_path[PATH_MAX];
	char file_name[200];
	struct epitaxy *epi=&(in->my_epitaxy);
	//electrons
	sprintf(file_name,"%s_dosn.dat",dos_file);
	join_path(3, binary_path,sim->root_simulation_path,"cache",file_name);

	load_binary_dos_file(sim,&(epi->dosn[mat]),binary_path);


	//holes
	sprintf(file_name,"%s_dosp.dat",dos_file);
	join_path(3, binary_path,sim->root_simulation_path,"cache",file_name);

	load_binary_dos_file(sim,&(epi->dosp[mat]),binary_path);


	//text
	sprintf(file_name,"%s.inp",dos_file);
	join_path(2, txt_path,sim->root_simulation_path,file_name);

	load_txt_dos_file(sim,&(epi->dosn[mat]),&(epi->dosp[mat]),txt_path);


	in->ns.dim.srh_bands=epi->dosn[mat].srh_bands;


	if (epi->dosn[mat].mobility_symmetric==FALSE)
	{
		in->mun_symmetric=FALSE;
	}
	if (epi->dosp[mat].mobility_symmetric==FALSE)
	{
		in->mup_symmetric=FALSE;
	}
}

long double get_dos_E_n(struct epitaxy *epi,int band,int mat)
{
return epi->dosn[mat].srh_E[band];
}

long double get_dos_E_p(struct epitaxy *epi,int band,int mat)
{
return epi->dosp[mat].srh_E[band];
}



