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

/** @file anal.c
@brief do some analysis on the device, this belongs in the libmeasure really.
*/

#include <stdio.h>
#include <log.h>
#include "sim.h"

/*
int i;
struct math_xy xrange;
struct math_xy data;
struct math_xy intergrate;
struct math_xy left;
struct math_xy time;
struct math_xy tau;
struct math_xy loss;
struct math_xy lookup;

inter_load_a(&xrange,"timedata/CEVoccurrent_sane_axes.dat",0,0);
inter_load_a(&time,"timedata/TIME_for_CE.txt",0,0);
inter_load_a(&tau,"timedata/tpv/taulaser2nd_sane.csv",0,1);
	inter_save(&tau,"tausave.dat");
gdouble charge[xrange.len];
gdouble tottime=time.x[time.len-1]-time.x[0];
gdouble dt=tottime/((gdouble)time.len);
inter_rescale(&xrange,1e-3, 1.0);
//for (i=0;i<xrange.len;i++)
//{
//	printf_log("%e\n",xrange.x[i]);
//}
gdouble d=180e-9;
int x=0;
int loop=0;

inter_copy(&lookup,&xrange);

for (loop=0;loop<10;loop++)
{
printf_log("Dooing Loop %d\n",loop);
	for (x=0;x<xrange.len;x++)
	{
		gdouble area=0.06/100.0/100.0;
		inter_load_a(&data,"timedata/CEVoccurrent_sane.dat",x,xrange.len);
		inter_rescale(&data,1.0, 1.0/area);
		inter_save(&data,"testsave.dat");
		inter_copy(&intergrate,&data);
		inter_copy(&left,&data);
		inter_copy(&loss,&data);

		gdouble tot=0.0;
		for (i=0;i<intergrate.len;i++)
		{
		tot+=data.data[i]*dt;
		intergrate.data[i]=tot;
		}
		inter_save(&intergrate,"testsaveintergrate.dat");
		gdouble cap=3.8*epsilon0*area/d;
		gdouble capq=(xrange.x[x]*cap)/1.6e-19;	//charge on capasitor
		capq/=area;
		capq/=d;
		gdouble charge=tot/d;
		charge*=1.0/1.6e-19;
		tot=charge-capq;


		//rewrite as total charge left in the device
		gdouble mytau=0.0;
		for (i=0;i<left.len;i++)
		{


		//printf_log("%e\n",xrange.x[x]);
		left.data[i]=tot;
		if (loop!=0)
		{
		mytau=inter_get_hard(&lookup,tot);
		loss.data[i]=tot*(1.0-exp(-dt/mytau));
		}else
		{
		loss.data[i]=0.0;
		}

		tot-=data.data[i]*dt/d/1.6e-19;
		}
		gdouble sum_lost=inter_sum(&loss);

		lookup.x[x]=charge-capq-sum_lost;
		lookup.data[x]=inter_get_hard(&tau,xrange.x[x]);


		printf_log("%e %e %e %e %e %e\n",xrange.x[x],charge,capq,charge-capq,inter_get_hard(&tau,xrange.x[x]),sum_lost);
		inter_save(&left,"left.dat");
		inter_save(&loss,"loss.dat");

		inter_free(&loss);
		inter_free(&intergrate);
		inter_free(&data);
		inter_free(&left);

	}
inter_save(&lookup,"lookuptau.dat");
getchar();
}
inter_free(&xrange);
inter_free(&time);
inter_free(&tau);
exit(0);

*/


void process_ce_data(struct simulation *sim,int col,char *input,char *output)
{
int i;
struct math_xy data;

gdouble d=read_value(sim,"device.inp",0,12)+read_value(sim,"device.inp",0,39);
gdouble area=read_value(sim,"device.inp",0,21)*read_value(sim,"device.inp",0,23);

gdouble cap=read_value(sim,"blom_bulk.inp",0,84)*epsilon0*area/d;
gdouble capq=0.0;

gdouble dt=0.0;

int x=0;

FILE *out=fopen(output,"w");

	for (x=0;x<col;x++)
	{
		printf_log(sim,"loading.... %d\n",x);
		inter_load_by_col(sim,&data,input,x);


		gdouble tot=0.0;

		for (i=1;i<data.len-1;i++)
		{
			//if ((data.x[i]>1e-6)&&(data.x[i]<1e-5))
			{
				dt=(data.x[i+1]-data.x[i-1])/2.0;
				tot+=data.data[i]*dt;
			}
		}


		capq=(data.x[0]*cap)/Q;	//charge on capasitor
		capq/=area;
		capq/=d;

		tot/=area;
		tot/=d;
		tot/=Q;

		fprintf(out,"%Le %Le\n",data.data[0],tot-capq);
		inter_free(&data);
	}
//getchar();

fclose(out);

}


