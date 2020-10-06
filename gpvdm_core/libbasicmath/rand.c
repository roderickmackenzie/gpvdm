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

/** @file rand.c
	@brief Randomize input files, for restarting a simulation after a crash during fitting.
*/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "inp.h"

#include "util.h"
#include <gpvdm_const.h>
#include "rand.h"


int random_int_range(struct simulation *sim,int start_in,int stop_in)
{
int start=start_in;
int stop=stop_in;

int temp=0;

if (start>stop)
{
	temp=start;
	start=stop;
	stop=temp;
}
int ret=0;
int delta=(stop-start);

ret=random_int(sim,delta);

return start+ret;
}

int random_int(struct simulation *sim, int in)
{
if (in==0) return 0;
int out=0;
int random;
FILE *fp = fopen("/dev/urandom", "r");
if (fread(&random, sizeof(int), 1, fp)==0)
{
	ewe(sim,"No data read from urandom\n");
}
random=fabs(random);
out = random % (in+1);

fclose(fp);
return out;
}

void randomize_input_files(struct simulation *sim)
{
struct inp_file inp;
inp_init(sim,&inp);

struct inp_file ifile;
inp_init(sim,&ifile);

inp_load(sim,&inp,"random.inp");
inp_check(sim,&inp,1.0);
inp_reset_read(sim,&inp);
char *data;
char file[100];
char token[100];
double man_min=0.0;
double man_max=0.0;
double exp_min=0.0;
double exp_max=0.0;
double a=0.0;
double b=0.0;
double value=0.0;
char value_string[100];
do
{
	data=inp_get_string(sim,&inp);

	if (strcmp(data,"#ver")==0)
	{
		break;
	}

	sscanf(data,"%s %s %le %le %le %le",file,token,&man_min,&man_max,&exp_min,&exp_max);

	a=(double)random_int_range(sim,man_min,man_max);
	b=(double)random_int_range(sim,exp_min,exp_max);
	value=a*pow(10,b);
	sprintf(value_string,"%le",value);
	inp_load(sim,&ifile,file);
	inp_replace(sim,&ifile,token, value_string);
	inp_free(sim,&ifile);


}while(1);

inp_free(sim,&inp);


}

