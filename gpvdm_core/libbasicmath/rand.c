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

double random_double()
{
	double r=0.0;
	r = rand();
	r=(double)r/(double)RAND_MAX;
	return r;
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
char data[4000];
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
int ret_val;
do
{
	ret_val=inp_get_string(sim,data,&inp);

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

