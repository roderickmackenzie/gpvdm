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


