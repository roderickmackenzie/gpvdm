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

/** @file simplex_run.c
@brief Simplex run
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <simplex.h>
#include <log.h>
#include <sim_struct.h>

//#define simplex_verbose

void newton_start(struct simulation *sim,struct multimin *data)
{
int i;
int count=0;
double e0=0.0;
double e1=0.0;
double dx=0.0;
double dy=0.0;
data->s[0]=1e-8;
double clamp=0.01;
double deriv=0.0;
double step=0.0;

data->ptry[0]=data->x[0];

e0=data->fn((void *)data,data->ptry);
getchar();

step=0.1;
data->ptry[0]=data->x[0]+step;
e1=data->fn((void *)data,data->ptry);
getchar();

deriv=(e1-e0)/step;
step= -e1/deriv;

data->ptry[0]+=step;

	do
	{
		e0=e1;
		e1=data->fn((void *)data,data->ptry);
		getchar();

		deriv=(e1-e0)/step;
		step= -e1/deriv;
		//gdouble clamp=0.01;
		//if (e1<clamp) clamp=e1/100.0;
		//step=step/(1.0+fabs(step/clamp));
		step=step/(1.0+fabs(step/clamp));
		data->ptry[0]+=step;
		printf("%le %le %le\n",e1,data->ptry[0],step);
		getchar();
		if (count>1000) break;
		count++;
	}while(e1>1e-8);

}

int simplex_iterate(struct simulation *sim,struct multimin *data)
{
	int i=0;
	int j=0;

	double rtol=0.0;
	double sum=0.0;
	double swap=0.0;
	double ysave=0.0;
	int ittr=0;
	//printf("BOOM\n");
	//getchar();
	multimin_find_best(data);


	rtol=2.0*fabs(data->y[data->i_hi0]-data->y[data->i_lo])/(fabs(data->y[data->i_hi0])+fabs(data->y[data->i_lo])+TINY);


	if (data->ittr >= data->n_max)
	{
		printf(">>>>>>exit1\n");
		getchar();

		printf("Number of itterations exceeded");
		return SIMPLEX_MAX;
	}

	#ifdef simplex_verbose
	printf(">>>>>>MAIN REFLECT\n");
	//getchar();
	#endif
	double ytry_reflect=reflect(data,1.0);		//1.0
	multimin_find_best(data);
	multimin_cal_center(data);

	multimin_dump(sim,data);

	//getchar();
	double ytry_expand=0.0;

	if ((ytry_reflect < data->y[data->i_hi1]) && (ytry_reflect > data->y[data->i_lo]))		//3 reflection
	{
		#ifdef simplex_verbose
		printf(">>>>>>3 REFLECT\n");
		#endif
		//getchar();
		//printf("(ytry_reflect < data->y[data->i_hi1]) && (ytry_reflect > data->y[data->i_lo]) %lf %lf %lf %lf \n",ytry_reflect , data->y[data->i_hi1],ytry_reflect ,data->y[data->i_lo]);
		multimin_find_best(data);
		//printf("what?? %d\n",data->i_hi0);
		#ifdef simplex_verbose
		multimin_dump(sim,data);
		#endif
		//printf("not yet\n");
		multimin_sync(data,data->i_hi0);

		multimin_cal_center(data);

		//printf("Done second 3a\n");
		#ifdef simplex_verbose
		multimin_dump(sim,data);
		#endif
	}else
	if (ytry_reflect < data->y[data->i_lo])						//3 expansion
	{
		#ifdef simplex_verbose
		printf(">>>>>>EXPAND\n");
		#endif
		//getchar();
		#ifdef simplex_verbose
		multimin_dump(sim,data);
		#endif
		multimin_sync(data,data->i_hi0);

		#ifdef simplex_verbose
		multimin_dump(sim,data);
		#endif
		multimin_cal_center(data);

		#ifdef simplex_verbose
		multimin_dump(sim,data);
		#endif

		ytry_expand=expand(data,2.0);//4.0
		//printf("ytry_expand<ytry_reflect %f %f\n",ytry_expand,ytry_reflect);
		if (ytry_expand<ytry_reflect)
		{
			multimin_sync(data,data->i_hi0);
			multimin_cal_center(data);
			//printf("was much better\n");
			#ifdef simplex_verbose
			multimin_dump(sim,data);
			#endif
		}

	}else
	if (ytry_reflect >= data->y[data->i_hi1])
	{
		//printf(">>>>>>>>>>>here %f %f\n",ytry, data->y[data->i_hi1]);
		#ifdef simplex_verbose
		printf(">>>>>>CONTRACT\n");
		#endif
		//getchar();
		double ytry_contract=contract(data,0.25);	//contract 0.5
		if (ytry_contract<data->y[data->i_hi0])
		{
			multimin_sync(data,data->i_hi0);
			multimin_cal_center(data);
		}
		//printf(">>>>>>>>>>>>>>>>contracted %f %f", ytry_contract, ysave);

	}else
	{
		#ifdef simplex_verbose
		printf(">>>>>>SHRINK\n");
		#endif
		//	getchar();
			multimin_shrink(data);
			multimin_cal_center(data);
	}

	data->ittr++;

	simplex_copy_ans(data);


	if (data->error < data->stop_error)
	{
		#ifdef simplex_verbose
		printf(">>>>>>exit0 %e\n",rtol);
		getchar();
		#endif


		return SIMPLEX_CONVERGED;
	}

	#ifdef simplex_verbose
	multimin_dump(sim,data);
	#endif

	data->error_delta=(fabs(data->error_last-data->error)+data->error_delta)/2.0;

	data->error_last=data->error;
	return 0;
		
}
