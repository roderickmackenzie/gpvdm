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

/** @file B.c
	@brief Math functions for solvers.
*/

#include <math.h>
#include <i.h>
#include <sim_struct.h>
#include <device.h>

//struct math_xy Btab;
//struct math_xy dBtab;

//Taken from mathworld and wikipidia
long double dB(long double x)
{
long double ret;
if (fabsl(x)>1e-40)
{
	ret=(expl(x)-1.0-x*expl(x))/(expl(x)-1)/(expl(x)-1);
}
else
{
	ret= -1.0/2.0+x/6.0-powl(x,3.0)/180.0+powl(x,5.0)/5040;
}

return ret;
}


long double B(long double x)
{
long double ret;
if (fabsl(x)>1e-40)
{
	ret=x/(expl(x)-1.0);
//From mathworld and wikipidia
}else
{
	ret=1-x/2.0+powl(x,2.0)/12.0-powl(x,4.0)/720.0+powl(x,6.0)/30240.0;

}
return ret;
}

void BdB(long double *B_ret,long double *dB_ret,long double x)
{
	*B_ret=B(x);
	*dB_ret=dB(x);
}

/*void init_BdB(struct simulation *sim,struct device *dev)
{
	int i;
	//printf("ok");
	//getchar();
	inter_init(sim,&(Btab));
	inter_init(sim,&(dBtab));
	int steps=10000;
	inter_malloc(&(Btab),steps);
	inter_malloc(&(dBtab),steps);
	long double start=-10.0;
	long double stop=10.0;
	long double x=start;
	long double dx=(stop-start)/steps;
	long double b;
	long double db;
	for (i=0;i<steps;i++)
	{
		b=B(x);
		db=dB(x);
		Btab.x[i]=x;
		Btab.data[i]=b;

		dBtab.x[i]=x;
		dBtab.data[i]=db;

		x+=dx;
	}
}*/


