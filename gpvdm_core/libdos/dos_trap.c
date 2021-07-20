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

/** @file dos_read_free.c
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
#include <memory.h>



void get_n_srh(struct simulation *sim,struct shape *s,long double top,long double T,int trap,long double *nt,long double *srh1,long double *srh2,long double *srh3,long double *srh4)
{
//long double ret=0.0;
long double c0=0.0;
long double c1=0.0;
long double x0=0.0;
long double x1=0.0;
long double t0=0.0;
long double t1=0.0;
long double c=0.0;
long double xr=0.0;
long double tr=0.0;
long double c00=0.0;
long double c01=0.0;
long double c10=0.0;
long double c11=0.0;
int t=0;
int x=0;

if ((s->dosn.x[0]>top)||(s->dosn.x[s->dosn.xlen-1]<top))
{
	errors_add(sim,"Electrons asking for %Le but range %Le %Le\n",top,s->dosn.x[0],s->dosn.x[s->dosn.xlen-1]);
}


x=hashget(s->dosn.x,s->dosn.xlen,top);
//if (x<0) x=0;


x0=s->dosn.x[x];
x1=s->dosn.x[x+1];
xr=(top-x0)/(x1-x0);

if (s->dosn.tlen>1)
{
	t=hashget(s->dosn.t,s->dosn.tlen,T);
	if (t<0) t=0;

	t0=s->dosn.t[t];
	t1=s->dosn.t[t+1];
	tr=(T-t0)/(t1-t0);
}else
{
	tr=0.0;
}


//switch (r)
//{
//case 1:
	c00=s->dosn.srh_r1[t][x][trap];
	c01=s->dosn.srh_r1[t][x+1][trap];
	c0=c00+xr*(c01-c00);

	if (s->dosn.tlen>1)
	{
		c10=s->dosn.srh_r1[t+1][x][trap];
		c11=s->dosn.srh_r1[t+1][x+1][trap];
		c1=c10+xr*(c11-c10);
	}
	c=c0+tr*(c1-c0);
	*srh1=c;
//break;

//case 2:
	c00=s->dosn.srh_r2[t][x][trap];
	c01=s->dosn.srh_r2[t][x+1][trap];
	c0=c00+xr*(c01-c00);

	if (s->dosn.tlen>1)
	{
		c10=s->dosn.srh_r2[t+1][x][trap];
		c11=s->dosn.srh_r2[t+1][x+1][trap];
		c1=c10+xr*(c11-c10);
	}
	c=c0+tr*(c1-c0);
	*srh2=c;
//break;

//case 3:
	c00=s->dosn.srh_r3[t][x][trap];
	c01=s->dosn.srh_r3[t][x+1][trap];
	c0=c00+xr*(c01-c00);

	if (s->dosn.tlen>1)
	{
		c10=s->dosn.srh_r3[t+1][x][trap];
		c11=s->dosn.srh_r3[t+1][x+1][trap];
		c1=c10+xr*(c11-c10);
	}
	c=c0+tr*(c1-c0);
	*srh3=c;
//break;

//case 4:
	c00=s->dosn.srh_r4[t][x][trap];
	c01=s->dosn.srh_r4[t][x+1][trap];
	c0=c00+xr*(c01-c00);

	if (s->dosn.tlen>1)
	{
		c10=s->dosn.srh_r4[t+1][x][trap];
		c11=s->dosn.srh_r4[t+1][x+1][trap];
		c1=c10+xr*(c11-c10);
	}
	c=c0+tr*(c1-c0);
	*srh4=c;
//break;
//}

	//carrier density
	c00=s->dosn.srh_c[t][x][trap];
	c01=s->dosn.srh_c[t][x+1][trap];
	c0=c00+xr*(c01-c00);

	if (s->dosn.tlen>1)
	{
		c10=s->dosn.srh_c[t+1][x][trap];
		c11=s->dosn.srh_c[t+1][x+1][trap];
		c1=c10+xr*(c11-c10);
	}

	c=c0+tr*(c1-c0);
	*nt=c;


}

void get_p_srh(struct simulation *sim,struct shape *s,long double top,long double T,int trap,long double *pt,long double *srh1,long double *srh2,long double *srh3,long double *srh4)
{
//long double ret=0.0;
long double c0=0.0;
long double c1=0.0;
long double x0=0.0;
long double x1=0.0;
long double t0=0.0;
long double t1=0.0;
long double c=0.0;
long double xr=0.0;
long double tr=0.0;
long double c00=0.0;
long double c01=0.0;
long double c10=0.0;
long double c11=0.0;
int t=0;
int x=0;

if ((s->dosp.x[0]>top)||(s->dosp.x[s->dosp.xlen-1]<top))
{
	errors_add(sim,"Holes asking for %e but range %e %e\n",top,s->dosp.x[0],s->dosp.x[s->dosp.xlen-1]);
}

x=hashget(s->dosp.x,s->dosp.xlen,top);
//if (x<0) x=0;

x0=s->dosp.x[x];
x1=s->dosp.x[x+1];
xr=(top-x0)/(x1-x0);


if (s->dosp.tlen>1)
{
	t=hashget(s->dosp.t,s->dosp.tlen,T);
	if (t<0) t=0;

	t0=s->dosp.t[t];
	t1=s->dosp.t[t+1];
	tr=(T-t0)/(t1-t0);
}else
{
	tr=0.0;
}

//switch (r)
//{
//	case 1:
		c00=s->dosp.srh_r1[t][x][trap];
		c01=s->dosp.srh_r1[t][x+1][trap];
		c0=c00+xr*(c01-c00);

		if (s->dosp.tlen>1)
		{
			c10=s->dosp.srh_r1[t+1][x][trap];
			c11=s->dosp.srh_r1[t+1][x+1][trap];
			c1=c10+xr*(c11-c10);
		}
		c=c0+tr*(c1-c0);
		*srh1=c;
//	break;

//	case 2:
		c00=s->dosp.srh_r2[t][x][trap];
		c01=s->dosp.srh_r2[t][x+1][trap];
		c0=c00+xr*(c01-c00);

		if (s->dosp.tlen>1)
		{
			c10=s->dosp.srh_r2[t+1][x][trap];
			c11=s->dosp.srh_r2[t+1][x+1][trap];
			c1=c10+xr*(c11-c10);
		}
		c=c0+tr*(c1-c0);
		*srh2=c;
//	break;

//	case 3:
		c00=s->dosp.srh_r3[t][x][trap];
		c01=s->dosp.srh_r3[t][x+1][trap];
		c0=c00+xr*(c01-c00);

		if (s->dosp.tlen>1)
		{
			c10=s->dosp.srh_r3[t+1][x][trap];
			c11=s->dosp.srh_r3[t+1][x+1][trap];
			c1=c10+xr*(c11-c10);
		}
		c=c0+tr*(c1-c0);
		*srh3=c;
//	break;

//	case 4:
		c00=s->dosp.srh_r4[t][x][trap];
		c01=s->dosp.srh_r4[t][x+1][trap];
		c0=c00+xr*(c01-c00);

		if (s->dosp.tlen>1)
		{
			c10=s->dosp.srh_r4[t+1][x][trap];
			c11=s->dosp.srh_r4[t+1][x+1][trap];
			c1=c10+xr*(c11-c10);
		}
		c=c0+tr*(c1-c0);
		*srh4=c;
//	break;
//}

	//carrier density
	c00=s->dosp.srh_c[t][x][trap];
	c01=s->dosp.srh_c[t][x+1][trap];
	c0=c00+xr*(c01-c00);

	if (s->dosp.tlen>1)
	{
		c10=s->dosp.srh_c[t+1][x][trap];
		c11=s->dosp.srh_c[t+1][x+1][trap];
		c1=c10+xr*(c11-c10);
	}


	c=c0+tr*(c1-c0);
	*pt=c;



//return ret;
}



void get_dn_srh(struct simulation *sim,struct shape *s,long double top,long double T,int trap,long double *dnt,long double *srh1,long double *srh2,long double *srh3,long double *srh4)
{
//long double ret=0.0;
long double c0=0.0;
long double c1=0.0;
long double x0=0.0;
long double x1=0.0;
long double t0=0.0;
long double t1=0.0;
long double c=0.0;
long double xr=0.0;
long double tr=0.0;
long double c00=0.0;
long double c01=0.0;
long double c10=0.0;
long double c11=0.0;


int t=0;
int x;

if ((s->dosn.x[0]>top)||(s->dosn.x[s->dosn.xlen-1]<top))
{
	errors_add(sim,"Electrons asking for %Le but range %Le %Le\n",top,s->dosn.x[0],s->dosn.x[s->dosn.xlen-1]);
}


x=hashget(s->dosn.x,s->dosn.xlen,top);

x0=s->dosn.x[x];
x1=s->dosn.x[x+1];
xr=1.0/(x1-x0);

if (s->dosn.tlen>1)
{
	t=hashget(s->dosn.t,s->dosn.tlen,T);
	t0=s->dosn.t[t];
	t1=s->dosn.t[t+1];
	tr=(T-t0)/(t1-t0);
}else
{
	tr=0.0;
}

//switch (r)
//{

//case 1:
	c00=s->dosn.srh_r1[t][x][trap];
	c01=s->dosn.srh_r1[t][x+1][trap];
	c0=xr*(c01-c00);

	if (s->dosn.tlen>1)
	{
		c10=s->dosn.srh_r1[t+1][x][trap];
		c11=s->dosn.srh_r1[t+1][x+1][trap];
		c1=xr*(c11-c10);
	}
	c=c0+tr*(c1-c0);
	*srh1=c;
//break;

//case 2:
	c00=s->dosn.srh_r2[t][x][trap];
	c01=s->dosn.srh_r2[t][x+1][trap];
	c0=xr*(c01-c00);

	if (s->dosn.tlen>1)
	{
		c10=s->dosn.srh_r2[t+1][x][trap];
		c11=s->dosn.srh_r2[t+1][x+1][trap];
		c1=xr*(c11-c10);
	}
	c=c0+tr*(c1-c0);
	*srh2=c;
//break;

//case 3:
	c00=s->dosn.srh_r3[t][x][trap];
	c01=s->dosn.srh_r3[t][x+1][trap];
	c0=xr*(c01-c00);

	if (s->dosn.tlen>1)
	{
		c10=s->dosn.srh_r3[t+1][x][trap];
		c11=s->dosn.srh_r3[t+1][x+1][trap];
		c1=xr*(c11-c10);
	}
	c=c0+tr*(c1-c0);
	*srh3=c;
//break;

//case 4:
	c00=s->dosn.srh_r4[t][x][trap];
	c01=s->dosn.srh_r4[t][x+1][trap];
	c0=xr*(c01-c00);

	if (s->dosn.tlen>1)
	{
		c10=s->dosn.srh_r4[t+1][x][trap];
		c11=s->dosn.srh_r4[t+1][x+1][trap];
		c1=xr*(c11-c10);
	}
	c=c0+tr*(c1-c0);
	*srh4=c;
//break;
//}

	c00=s->dosn.srh_c[t][x][trap];
	c01=s->dosn.srh_c[t][x+1][trap];
	c0=xr*(c01-c00);

	if (s->dosn.tlen>1)
	{
		c10=s->dosn.srh_c[t+1][x][trap];
		c11=s->dosn.srh_c[t+1][x+1][trap];
		c1=xr*(c11-c10);
	}


	c=c0+tr*(c1-c0);
	*dnt=c;

}


void get_dp_srh(struct simulation *sim,struct shape *s,long double top,long double T,int trap,long double *dpt,long double *srh1,long double *srh2,long double *srh3,long double *srh4)
{
//long double ret=0.0;
long double c0=0.0;
long double c1=0.0;
long double x0=0.0;
long double x1=0.0;
long double t0=0.0;
long double t1=0.0;
long double c=0.0;
long double xr=0.0;
long double tr=0.0;
long double c00=0.0;
long double c01=0.0;
long double c10=0.0;
long double c11=0.0;


int t=0;
int x;

if ((s->dosp.x[0]>top)||(s->dosp.x[s->dosp.xlen-1]<top))
{
	errors_add(sim,"Holes asking for %Le but range %Le %Le\n",top,s->dosp.x[0],s->dosp.x[s->dosp.xlen-1]);
}


x=hashget(s->dosp.x,s->dosp.xlen,top);

x0=s->dosp.x[x];
x1=s->dosp.x[x+1];
xr=1.0/(x1-x0);

if (s->dosp.tlen>1)
{
	t=hashget(s->dosp.t,s->dosp.tlen,T);
	t0=s->dosp.t[t];
	t1=s->dosp.t[t+1];
	tr=(T-t0)/(t1-t0);
}else
{
	tr=0.0;
}

//switch (r)
//{
//case 1:
	c00=s->dosp.srh_r1[t][x][trap];
	c01=s->dosp.srh_r1[t][x+1][trap];
	c0=xr*(c01-c00);

	if (s->dosp.tlen>1)
	{
		c10=s->dosp.srh_r1[t+1][x][trap];
		c11=s->dosp.srh_r1[t+1][x+1][trap];
		c1=xr*(c11-c10);
	}
	c=c0+tr*(c1-c0);
	*srh1=c;
//break;

//case 2:
	c00=s->dosp.srh_r2[t][x][trap];
	c01=s->dosp.srh_r2[t][x+1][trap];
	c0=xr*(c01-c00);

	if (s->dosp.tlen>1)
	{
		c10=s->dosp.srh_r2[t+1][x][trap];
		c11=s->dosp.srh_r2[t+1][x+1][trap];
		c1=xr*(c11-c10);
	}
	c=c0+tr*(c1-c0);
	*srh2=c;
//break;

//case 3:
	c00=s->dosp.srh_r3[t][x][trap];
	c01=s->dosp.srh_r3[t][x+1][trap];
	c0=xr*(c01-c00);

	if (s->dosp.tlen>1)
	{
		c10=s->dosp.srh_r3[t+1][x][trap];
		c11=s->dosp.srh_r3[t+1][x+1][trap];
		c1=xr*(c11-c10);
	}
	c=c0+tr*(c1-c0);
	*srh3=c;
//break;

//case 4:
	c00=s->dosp.srh_r4[t][x][trap];
	c01=s->dosp.srh_r4[t][x+1][trap];
	c0=xr*(c01-c00);

	if (s->dosp.tlen>1)
	{
		c10=s->dosp.srh_r4[t+1][x][trap];
		c11=s->dosp.srh_r4[t+1][x+1][trap];
		c1=xr*(c11-c10);
	}
	c=c0+tr*(c1-c0);
	*srh4=c;
//break;
//}
	c00=s->dosp.srh_c[t][x][trap];
	c01=s->dosp.srh_c[t][x+1][trap];
	c0=xr*(c01-c00);

	if (s->dosp.tlen>1)
	{
		c10=s->dosp.srh_c[t+1][x][trap];
		c11=s->dosp.srh_c[t+1][x+1][trap];
		c1=xr*(c11-c10);
	}


	c=c0+tr*(c1-c0);
	*dpt=c;

}



/////////////////////////////////////////////////////trap

long double get_n_pop_srh(struct simulation *sim,struct shape *s,long double top,long double T,int trap)
{
long double ret=0.0;
long double c0=0.0;
long double c1=0.0;
long double x0=0.0;
long double x1=0.0;
long double t0=0.0;
long double t1=0.0;
long double c=0.0;
long double xr=0.0;
long double tr=0.0;
long double c00=0.0;
long double c01=0.0;
long double c10=0.0;
long double c11=0.0;
int t=0;
int x=0;

if ((s->dosn.x[0]>top)||(s->dosn.x[s->dosn.xlen-1]<top))
{
	errors_add(sim,"Electrons asking for %Le but range %Le %Le\n",top,s->dosn.x[0],s->dosn.x[s->dosn.xlen-1]);
}

x=hashget(s->dosn.x,s->dosn.xlen,top);


x0=s->dosn.x[x];
x1=s->dosn.x[x+1];
xr=(top-x0)/(x1-x0);
if (s->dosn.tlen>1)
{
	t=hashget(s->dosn.t,s->dosn.tlen,T);

	t0=s->dosn.t[t];
	t1=s->dosn.t[t+1];
	tr=(T-t0)/(t1-t0);
}else
{
tr=0.0;
}


c00=s->dosn.srh_c[t][x][trap];
c01=s->dosn.srh_c[t][x+1][trap];
c0=c00+xr*(c01-c00);

if (s->dosn.tlen>1)
{
	c10=s->dosn.srh_c[t+1][x][trap];
	c11=s->dosn.srh_c[t+1][x+1][trap];
	c1=c10+xr*(c11-c10);
}

c=c0+tr*(c1-c0);
ret=c;

return ret;
}

long double get_p_pop_srh(struct simulation *sim,struct shape *s,long double top,long double T,int trap)
{
long double ret=0.0;
long double c0=0.0;
long double c1=0.0;
long double x0=0.0;
long double x1=0.0;
long double t0=0.0;
long double t1=0.0;
long double c=0.0;
long double xr=0.0;
long double tr=0.0;
long double c00=0.0;
long double c01=0.0;
long double c10=0.0;
long double c11=0.0;
int t=0;
int x=0;

if ((s->dosp.x[0]>top)||(s->dosp.x[s->dosp.xlen-1]<top))
{
	errors_add(sim,"Holes asking for %Le but range %Le %Le\n",top,s->dosp.x[0],s->dosp.x[s->dosp.xlen-1]);
}


x=hashget(s->dosp.x,s->dosp.xlen,top);

x0=s->dosp.x[x];
x1=s->dosp.x[x+1];
xr=(top-x0)/(x1-x0);
if (s->dosp.tlen>1)
{
	t=hashget(s->dosp.t,s->dosp.tlen,T);
	t0=s->dosp.t[t];
	t1=s->dosp.t[t+1];
	tr=(T-t0)/(t1-t0);
}else
{
	tr=0.0;
}


c00=s->dosp.srh_c[t][x][trap];
c01=s->dosp.srh_c[t][x+1][trap];
c0=c00+xr*(c01-c00);

if (s->dosp.tlen>1)
{
	c10=s->dosp.srh_c[t+1][x][trap];
	c11=s->dosp.srh_c[t+1][x+1][trap];
	c1=c10+xr*(c11-c10);
}


c=c0+tr*(c1-c0);
ret=c;

return ret;
}



long double get_dn_pop_srh(struct simulation *sim,struct shape *s,long double top,long double T,int trap)
{
long double ret=0.0;
long double c0=0.0;
long double c1=0.0;
long double x0=0.0;
long double x1=0.0;
long double t0=0.0;
long double t1=0.0;
long double c=0.0;
long double xr=0.0;
long double tr=0.0;
long double c00=0.0;
long double c01=0.0;
long double c10=0.0;
long double c11=0.0;


int t=0;
int x;
//errors_add(sim,"boo");

if ((s->dosn.x[0]>top)||(s->dosn.x[s->dosn.xlen-1]<top))
{
	errors_add(sim,"Electrons asking for %Le but range %Le %Le\n",top,s->dosn.x[0],s->dosn.x[s->dosn.xlen-1]);
}


x=hashget(s->dosn.x,s->dosn.xlen,top);

x0=s->dosn.x[x];
x1=s->dosn.x[x+1];
xr=1.0/(x1-x0);

if (s->dosn.tlen>1)
{
	t=hashget(s->dosn.t,s->dosn.tlen,T);
	t0=s->dosn.t[t];
	t1=s->dosn.t[t+1];
	tr=(T-t0)/(t1-t0);
}else
{
	tr=0.0;
}


c00=s->dosn.srh_c[t][x][trap];
c01=s->dosn.srh_c[t][x+1][trap];
c0=xr*(c01-c00);

if (s->dosn.tlen>1)
{
	c10=s->dosn.srh_c[t+1][x][trap];
	c11=s->dosn.srh_c[t+1][x+1][trap];
	c1=xr*(c11-c10);
}


c=c0+tr*(c1-c0);

ret=c;

return ret;
}


long double get_dp_pop_srh(struct simulation *sim,struct shape *s,long double top,long double T,int trap)
{
long double ret=0.0;
long double c0=0.0;
long double c1=0.0;
long double x0=0.0;
long double x1=0.0;
long double t0=0.0;
long double t1=0.0;
long double c=0.0;
long double xr=0.0;
long double tr=0.0;
long double c00=0.0;
long double c01=0.0;
long double c10=0.0;
long double c11=0.0;


int t=0;
int x;

if ((s->dosp.x[0]>top)||(s->dosp.x[s->dosp.xlen-1]<top))
{
	errors_add(sim,"Holes asking for %Le but range %Le %Le\n",top,s->dosp.x[0],s->dosp.x[s->dosp.xlen-1]);
	//if (get_dump_status(dump_exit_on_dos_error)==TRUE) server_stop_and_exit();
	//exit(0);
}


x=hashget(s->dosp.x,s->dosp.xlen,top);

x0=s->dosp.x[x];
x1=s->dosp.x[x+1];
xr=1.0/(x1-x0);

if (s->dosp.tlen>1)
{
	t=hashget(s->dosp.t,s->dosp.tlen,T);
	t0=s->dosp.t[t];
	t1=s->dosp.t[t+1];
	tr=(T-t0)/(t1-t0);
}else
{
	tr=0.0;
}


c00=s->dosp.srh_c[t][x][trap];
c01=s->dosp.srh_c[t][x+1][trap];
c0=xr*(c01-c00);

if (s->dosp.tlen>1)
{
	c10=s->dosp.srh_c[t+1][x][trap];
	c11=s->dosp.srh_c[t+1][x+1][trap];
	c1=xr*(c11-c10);
}


c=c0+tr*(c1-c0);

ret=c;

return ret;
}

