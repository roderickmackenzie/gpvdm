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

/** @file dos_config_load.c
	@brief Load a DoS configuration file
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


void dos_config_load(struct simulation *sim,struct dosconfig *confige,struct dosconfig *configh,char * dos_file, char *lumo_file, char *homo_file)
{

	char file_name[100];
	char temp[100];
	char full_name[100];
	strcpy(confige->dos_name,dos_file);

	strcpy(confige->analytical_dos_file_name,lumo_file);

	strcpy(configh->dos_name,dos_file);
	strcpy(configh->analytical_dos_file_name,homo_file);


	sprintf(file_name,"%s.inp",confige->dos_name);
	join_path(2, full_name,get_input_path(sim),file_name);

	struct inp_file inp;
	inp_init(sim,&inp);
	inp_load(sim,&inp,full_name);
	inp_check(sim,&inp,1.24);

	inp_search_string(sim,&inp,temp,"#dostype");
	confige->dostype=english_to_bin(sim,temp);
	configh->dostype=confige->dostype;

	inp_search_string(sim,&inp,temp,"#dos_free_carrier_stats");
	confige->dos_free_carrier_stats=english_to_bin(sim,temp);
	configh->dos_free_carrier_stats=confige->dos_free_carrier_stats;

	inp_search_gdouble(sim,&inp,&(confige->Nt),"#Ntrape");
	inp_search_gdouble(sim,&inp,&(configh->Nt),"#Ntraph");

	confige->Nt=fabs(confige->Nt);
	configh->Nt=fabs(configh->Nt);

	if (confige->Nt<1e7) confige->Nt=1e7;
	if (configh->Nt<1e7) configh->Nt=1e7;


	inp_search_gdouble(sim,&inp,&(confige->Et),"#Etrape");
	inp_search_gdouble(sim,&inp,&(configh->Et),"#Etraph");

	confige->Et=fabs(confige->Et);
	configh->Et=fabs(configh->Et);

	if (confige->Et<2e-3) confige->Et=2e-3;
	if (configh->Et<2e-3) configh->Et=2e-3;

	if (confige->Et>200e-3) confige->Et=200e-3;
	if (configh->Et>200e-3) configh->Et=200e-3;

	inp_search_gdouble(sim,&inp,&(confige->ion_density),"#ion_density");
	configh->ion_density=confige->ion_density;

	inp_search_gdouble(sim,&inp,&(confige->ion_mobility),"#ion_mobility");
	configh->ion_mobility=confige->ion_mobility;

	inp_search_gdouble(sim,&inp,&(confige->epsilonr),"#epsilonr");
	confige->epsilonr=fabs(confige->epsilonr);
	hard_limit(sim,"#epsilonr",&(confige->epsilonr));

	confige->epsilonr=fabs(confige->epsilonr);
	configh->epsilonr=fabs(confige->epsilonr);

	inp_search_gdouble(sim,&inp,&(confige->doping_start),"#doping_start");
	configh->doping_start=confige->doping_start;

	inp_search_gdouble(sim,&inp,&(confige->doping_stop),"#doping_stop");
	configh->doping_stop=confige->doping_stop;

	inp_search_gdouble(sim,&inp,&(confige->Tstart),"#Tstart");
	inp_search_gdouble(sim,&inp,&(confige->Tstop),"#Tstop");
	inp_search_int(sim,&inp,&(confige->Tsteps),"#Tpoints");

	configh->Tstart=confige->Tstart;
	configh->Tstop=confige->Tstop;
	configh->Tsteps=confige->Tsteps;

	inp_search_gdouble(sim,&inp,&(confige->nstart),"#nstart");
	inp_search_gdouble(sim,&inp,&(confige->nstop),"#nstop");
	inp_search_int(sim,&inp,&(confige->npoints),"#npoints");

	inp_search_gdouble(sim,&inp,&(configh->nstart),"#pstart");
	inp_search_gdouble(sim,&inp,&(configh->nstop),"#pstop");
	inp_search_int(sim,&inp,&(configh->npoints),"#ppoints");
	int bands=0;
	inp_search_int(sim,&inp,&(bands),"#srh_bands");
	confige->srh_bands=bands;
	configh->srh_bands=bands;

	inp_search_gdouble(sim,&inp,&(confige->srh_start),"#srh_start");
	configh->srh_start=confige->srh_start;

	confige->srh_stop=0.0;
	configh->srh_stop=confige->srh_stop;


	inp_search_gdouble(sim,&inp,&(confige->srh_sigman),"#srhsigman_e");
	confige->srh_sigman=fabs(confige->srh_sigman);

	inp_search_gdouble(sim,&inp,&(confige->srh_sigmap),"#srhsigmap_e");
	confige->srh_sigmap=fabs(confige->srh_sigmap);

	inp_search_gdouble(sim,&inp,&(confige->srh_vth),"#srhvth_e");
	confige->srh_vth=fabs(confige->srh_vth);
	if (confige->srh_vth<1e2) confige->srh_vth=1e2;

	inp_search_gdouble(sim,&inp,&(configh->srh_sigman),"#srhsigman_h");
	configh->srh_sigman=fabs(configh->srh_sigman);

	inp_search_gdouble(sim,&inp,&(configh->srh_sigmap),"#srhsigmap_h");
	configh->srh_sigmap=fabs(configh->srh_sigmap);

	inp_search_gdouble(sim,&inp,&(configh->srh_vth),"#srhvth_h");
	configh->srh_vth=fabs(configh->srh_vth);
	if (configh->srh_vth<1e2) configh->srh_vth=1e2;

	inp_search_gdouble(sim,&inp,&(confige->Nc),"#Nc");

	inp_search_gdouble(sim,&inp,&(confige->Nv),"#Nv");


	inp_search_gdouble(sim,&inp,&(confige->Xi),"#Xi");

	inp_search_gdouble(sim,&inp,&(confige->Eg),"#Eg");
	confige->Eg=fabs(confige->Eg);
	hard_limit(sim,"#Eg",&(confige->Eg));

	//confige->Eg=fabs(confige->Eg);
	//if (confige->Eg<1.0) configh->Eg=1.0;
	//if (confige->Eg>1.8) configh->Eg=1.8;


	//printf("rod >>%Le\n",configh->B);

	//gdouble hello;
	//inp_search_gdouble(sim,&inp,&(hello),"#free_to_free_recombination");
	//printf(">>%Le\n",hello);
	//getchar();

	//configh->B=-1;//fabs(confige->B);

	int Esteps=0;
	int Estep_div=0;

	if (bands>0)
	{
		inp_search_int(sim,&inp,&(Esteps),"#Esteps");
		Estep_div=(Esteps/bands)*bands;
		//if (Estep_div!=Esteps)
		//{
		//	printf_log(sim,"Esteps wanted= %d, given= %d \n",Esteps,Estep_div);
		//}
	}

	confige->Esteps=Estep_div;
	configh->Esteps=Estep_div;
	int dump;
	inp_search_int(sim,&inp,&dump,"#dump_band_structure");
	set_dump_status(sim,dump_band_structure, dump);

	inp_free(sim,&inp);

	configh->Xi=confige->Xi;
	configh->Eg=confige->Eg;

	confige->Nc=fabs(confige->Nc);
	confige->Nv=fabs(confige->Nv);
	if (confige->Nc<1e16) confige->Nc=1e16;
	if (confige->Nv<1e16) confige->Nv=1e16;

	/*printf("bing %Lf\n",confige->me);
	printf("%Lf\n",confige->mh);
	getchar();*/
	//confige->m=pow(confige->Nc/2.0,2.0/3.0)*hp*hp/kb/300.0/m0/2.0/PI;
	//configh->m=pow(confige->Nv/2.0,2.0/3.0)*hp*hp/kb/300.0/m0/2.0/PI;

	//(sqrt(E)/(4.0*PI*PI))*pow((2.0*in->m*m0)/(hbar*hbar),3.0/2.0)

	configh->Nc=confige->Nc;
	configh->Nv=confige->Nv;

	//The states are defined at 300K
	confige->me=(powl(confige->Nc/2.0,2.0/3.0)*hbar*hbar*2.0*PI)/(m0*kb*300.0);
	confige->mh=(powl(confige->Nv/2.0,2.0/3.0)*hbar*hbar*2.0*PI)/(m0*kb*300.0);

	configh->me=confige->me;
	configh->mh=confige->mh;

	inp_free(sim,&inp);
}
