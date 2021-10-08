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

/** @file dos_config_load.c
	@brief Load a DoS configuration file
*/

#include <stdio.h>
#include <stdlib.h>
#include <code_ctrl.h>
#include <sim.h>
#include <util.h>
#include <hard_limit.h>
#include <lang.h>
#include <dos.h>

static int unused __attribute__((unused));

#include "dos_an.h"

#include "dump.h"
#include "log.h"
#include "checksum.h"
#include "cal_path.h"


void dos_config_load(struct simulation *sim,struct dosconfig *confige,struct dosconfig *configh,char * dos_file, struct json_obj *json_dos)
{
	int bands=0;
	int Esteps=0;
	int Estep_div=0;
	int dump;
	strcpy(confige->dos_name,dos_file);

	strcpy(configh->dos_name,dos_file);

	json_get_english(sim,json_dos, &(confige->dostype),"dostype");
	configh->dostype=confige->dostype;

	json_get_english(sim,json_dos, &(confige->dos_free_carrier_stats),"dos_free_carrier_stats");
	configh->dos_free_carrier_stats=confige->dos_free_carrier_stats;

	json_get_long_double(sim,json_dos, &confige->Nt,"Ntrape");
	json_get_long_double(sim,json_dos, &configh->Nt,"Ntraph");

	confige->Nt=fabs(confige->Nt);
	configh->Nt=fabs(configh->Nt);

	if (confige->Nt<1e7) confige->Nt=1e7;
	if (configh->Nt<1e7) configh->Nt=1e7;

	json_get_long_double(sim,json_dos, &confige->Et,"Etrape");
	json_get_long_double(sim,json_dos, &configh->Et,"Etraph");

	confige->Et=fabs(confige->Et);
	configh->Et=fabs(configh->Et);

	if (confige->Et<2e-3) confige->Et=2e-3;
	if (configh->Et<2e-3) configh->Et=2e-3;

	if (confige->Et>200e-3) confige->Et=200e-3;
	if (configh->Et>200e-3) configh->Et=200e-3;

	json_get_long_double(sim,json_dos, &(confige->ion_density),"ion_density");
	configh->ion_density=confige->ion_density;

	//json_dump_obj(json_dos);

	json_get_long_double(sim,json_dos, &(confige->ion_mobility),"ion_mobility");
	configh->ion_mobility=confige->ion_mobility;

	json_get_long_double(sim,json_dos, &(confige->epsilonr),"epsilonr");
	confige->epsilonr=fabs(confige->epsilonr);
	hard_limit_do(sim,"epsilonr",&(confige->epsilonr));

	confige->epsilonr=fabs(confige->epsilonr);
	configh->epsilonr=fabs(confige->epsilonr);

	json_get_long_double(sim,json_dos, &(confige->Na0),"Na0");
	confige->Na0=fabs(confige->Na0);
	configh->Na0=confige->Na0;

	json_get_long_double(sim,json_dos, &(confige->Na1),"Na1");
	confige->Na1=fabs(confige->Na1);
	configh->Na1=confige->Na1;

	json_get_long_double(sim,json_dos, &(confige->Nd0),"Nd0");
	confige->Nd0=fabs(confige->Nd0);
	configh->Nd0=confige->Nd0;

	json_get_long_double(sim,json_dos, &(confige->Nd1),"Nd1");
	confige->Nd1=fabs(confige->Nd1);
	configh->Nd1=confige->Nd1;

	json_get_long_double(sim,json_dos, &(confige->Tstart),"Tstart");
	json_get_long_double(sim,json_dos, &(confige->Tstop),"Tstop");
	json_get_int(sim,json_dos, &(confige->Tsteps),"Tpoints");

	configh->Tstart=confige->Tstart;
	configh->Tstop=confige->Tstop;
	configh->Tsteps=confige->Tsteps;

	json_get_long_double(sim,json_dos, &(confige->nstart),"nstart");
	json_get_long_double(sim,json_dos, &(confige->nstop),"nstop");
	json_get_int(sim,json_dos, &(confige->npoints),"npoints");

	json_get_long_double(sim,json_dos, &(configh->nstart),"pstart");
	json_get_long_double(sim,json_dos, &(configh->nstop),"pstop");
	json_get_int(sim,json_dos, &(configh->npoints),"ppoints");

	json_get_int(sim,json_dos,&bands,"srh_bands");
	confige->srh_bands=bands;
	configh->srh_bands=bands;

	json_get_long_double(sim,json_dos, &(confige->srh_start),"srh_start");
	configh->srh_start=confige->srh_start;

	confige->srh_stop=0.0;
	configh->srh_stop=confige->srh_stop;


	json_get_long_double(sim,json_dos, &(confige->srh_sigman),"srhsigman_e");
	confige->srh_sigman=fabs(confige->srh_sigman);

	json_get_long_double(sim,json_dos, &(confige->srh_sigmap),"srhsigmap_e");
	confige->srh_sigmap=fabs(confige->srh_sigmap);

	json_get_long_double(sim,json_dos, &(confige->srh_vth),"srhvth_e");
	confige->srh_vth=fabs(confige->srh_vth);
	if (confige->srh_vth<1e2) confige->srh_vth=1e2;

	json_get_long_double(sim,json_dos, &(configh->srh_sigman),"srhsigman_h");
	configh->srh_sigman=fabs(configh->srh_sigman);

	json_get_long_double(sim,json_dos, &(configh->srh_sigmap),"srhsigmap_h");
	configh->srh_sigmap=fabs(configh->srh_sigmap);


	json_get_long_double(sim,json_dos, &(configh->srh_vth),"srhvth_h");
	configh->srh_vth=fabs(configh->srh_vth);
	if (configh->srh_vth<1e2) configh->srh_vth=1e2;

	json_get_long_double(sim,json_dos, &(confige->Nc),"Nc");
	json_get_long_double(sim,json_dos, &(confige->Nv),"Nv");

	confige->Nc=fabs(confige->Nc);
	confige->Nv=fabs(confige->Nv);
	if (confige->Nc<1e16) confige->Nc=1e16;
	if (confige->Nv<1e16) confige->Nv=1e16;
	configh->Nc=confige->Nc;
	configh->Nv=confige->Nv;

	json_get_long_double(sim,json_dos, &(confige->Xi),"Xi");

	json_get_long_double(sim,json_dos, &(confige->Eg),"Eg");

	//printf_log(sim,"Eg0=%Le\n",confige->Eg);
	confige->Eg=fabs(confige->Eg);
	//printf_log(sim,"Eg1=%Le\n",confige->Eg);
	hard_limit_do(sim,"Eg",&(confige->Eg));
	//printf_log(sim,"Eg2=%Le\n",confige->Eg);
	//printf_log(sim,"Eg3=%Le\n",configh->Eg);
	configh->Xi=confige->Xi;
	configh->Eg=confige->Eg;


	if (bands>0)
	{
		json_get_int(sim,json_dos, &(Esteps),"Esteps");
		Estep_div=(Esteps/bands)*bands;
	}

	confige->Esteps=Estep_div;
	configh->Esteps=Estep_div;

	json_get_int(sim,json_dos, &(dump),"dump_band_structure");
	set_dump_status(sim,dump_band_structure, dump);


	/*printf("bing %Lf\n",confige->me);
	printf("%Lf\n",confige->mh);
	getchar();*/
	//confige->m=pow(confige->Nc/2.0,2.0/3.0)*hp*hp/kb/300.0/m0/2.0/PI;
	//configh->m=pow(confige->Nv/2.0,2.0/3.0)*hp*hp/kb/300.0/m0/2.0/PI;

	//(sqrt(E)/(4.0*PI*PI))*pow((2.0*in->m*m0)/(hbar*hbar),3.0/2.0)



	//The states are defined at 300K
	confige->me=(powl(confige->Nc/2.0,2.0/3.0)*hbar*hbar*2.0*PI)/(m0*kb*300.0);
	confige->mh=(powl(confige->Nv/2.0,2.0/3.0)*hbar*hbar*2.0*PI)/(m0*kb*300.0);

	configh->me=confige->me;
	configh->mh=confige->mh;

}
