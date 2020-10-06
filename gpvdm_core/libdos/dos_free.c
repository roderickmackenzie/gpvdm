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



long double get_dpdT_den(struct epitaxy *epi,long double top,long double T,int mat)
{
long double ret=0.0;
long double N=epi->dosp[mat].config.Nv;
ret= -((top*Q)/kb)*N*gexp((top*Q)/(kb*T))*gpow(T,-2.0);
return ret;
}

long double get_dndT_den(struct epitaxy *epi,long double top,long double T,int mat)
{
long double ret=0.0;
long double N=epi->dosn[mat].config.Nc;
ret= -((top*Q)/kb)*N*gexp((top*Q)/(kb*T))*gpow(T,-2.0);
return ret;
}

long double get_top_from_n(struct epitaxy *epi,long double n,long double T,int mat)
{
long double ret=0.0;
long double c0=0.0;
long double c1=0.0;
long double x0=0.0;
long double x1=0.0;
long double cll=0.0;
int xx=0;
long double xr=0.0;

int t=0;
int x=0;


	if (epi->dosn[mat].config.dos_free_carrier_stats==mb_equation)
	{
		ret=(kb*T/Q)*log((fabs(n))/epi->dosn[mat].config.Nc);
	}else
	{
		if (epi->dosn[mat].tlen>1)
		{
			for (t=0;t<epi->dosn[mat].tlen-1;t++)
			{
				if (epi->dosn[mat].t[t]>T) break;
			}
			t--;

		}else
		{
			t=0;
		}

		xx=0;


		for (x=0;x<epi->dosn[mat].xlen-1;x++)
		{
			if (epi->dosn[mat].c[t][x]>n) break;
		}

		x--;

		if (x<0) x=0;
		if (xx<0) xx=0;
		if (t<0) t=0;

		x0=epi->dosn[mat].c[t][x];
		x1=epi->dosn[mat].c[t][x+1];
		xr=(n-x0)/(x1-x0);
		if (xr>1) xr=1;
		c0=epi->dosn[mat].x[x];
		c1=epi->dosn[mat].x[x+1];

		cll=c0+xr*(c1-c0);

		ret=cll;

	}


return ret;
}


long double get_top_from_p(struct epitaxy *epi,long double p,long double T,int mat)
{
long double ret=0.0;
long double c0=0.0;
long double c1=0.0;
long double x0=0.0;
long double x1=0.0;
long double cll=0.0;
int xx=0;
long double xr=0.0;

long double c;
int t;
int x;

	if (epi->dosp[mat].config.dos_free_carrier_stats==mb_equation)
	{
		ret=(kb*T/Q)*log((fabs(p))/epi->dosp[mat].config.Nv);
	}else
	{

		for (t=0;t<epi->dosp[mat].tlen-1;t++)
		{
		if (epi->dosp[mat].t[t]>T) break;
		}
		t--;

		if (t<0) t=0;

		xx=0;

		for (x=0;x<epi->dosp[mat].xlen-1;x++)
		{
			if (epi->dosp[mat].c[t][x]>p) break;
		}

		x--;

		if (x<0) x=0;
		if (xx<0) xx=0;

		x0=epi->dosp[mat].c[t][x];
		x1=epi->dosp[mat].c[t][x+1];
		xr=(p-x0)/(x1-x0);
		c0=epi->dosp[mat].x[x];
		c1=epi->dosp[mat].x[x+1];
		if (xr>1) xr=1;
		cll=c0+xr*(c1-c0);

		c=cll;

		ret=c;
//		printf(">>>>>> %Le %Le\n",ret ,(kb*T/Q)*log((fabs(p))/epi->dosp[mat].config.Nv));
//		getchar();
	}

return ret;
}

void get_n_den(struct epitaxy *epi,long double top,long double T,int mat,long double *n, long double *dn,long double *w)
{
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
	long double ret=0.0;

	if (epi->dosn[mat].config.dos_free_carrier_stats==mb_equation)
	{
		long double Tkb=T*kb;
		*n=epi->dosn[mat].config.Nc*gexp((Q*top)/(Tkb));

		if (dn!=NULL)
		{
			//*dn=(Q/(T*kb))*epi->dosn[mat].config.Nc*gexp((Q*top)/(T*kb));
			*dn=(Q/(Tkb))*(*n);
		}

		if (w!=NULL)
		{
			*w=(3.0/2.0)*Tkb;
		}
	}else
	{

		#ifdef dos_warn
		if ((epi->dosn[mat].x[0]>top)||(epi->dosn[mat].x[epi->dosn[mat].xlen-1]<top))
		{
			errors_add(sim,"Free electrons asking for %Le but range %Le %Le\n",top,epi->dosn[mat].x[0],epi->dosn[mat].x[epi->dosn[mat].xlen-1]);
		}
		#endif

		t=search(epi->dosn[mat].t,epi->dosn[mat].tlen,T);
		x=search(epi->dosn[mat].x,epi->dosn[mat].xlen,top);

		if (x<0) x=0;
		if (t<0) t=0;

		//n
		x0=epi->dosn[mat].x[x];
		x1=epi->dosn[mat].x[x+1];
		xr=(top-x0)/(x1-x0);

		if (epi->dosn[mat].tlen>1)
		{
			t0=epi->dosn[mat].t[t];
			t1=epi->dosn[mat].t[t+1];
			tr=(T-t0)/(t1-t0);
		}else
		{
			tr=0.0;
		}

		c00=epi->dosn[mat].c[t][x];
		c01=epi->dosn[mat].c[t][x+1];
		c0=c00+xr*(c01-c00);

		if (epi->dosn[mat].tlen>1)
		{
			c10=epi->dosn[mat].c[t+1][x];
			c11=epi->dosn[mat].c[t+1][x+1];
			c1=c10+xr*(c11-c10);
		}

		c=c0+tr*(c1-c0);
		*n=c;

			//double N=2.0*pow(((2.0*pi*kb*T*m*m0)/(hp*hp)),1.5);
			//long double test=epi->dosn[mat].config.Nc*gexp((Q*top)/(T*kb));
			//printf("test = %Le %Le\n",test,ret);
			//getchar();
		//printf("%Lf %Lf %Lf\n",epi->dosn[mat].x[x],epi->dosn[mat].x[x+1],top);
		//printf(">>%Le %Le\n",epi->dosn[mat].config.Nc*gexp((Q*top)/(T*kb)),ret);
		//printf("%Le %Le\n",epi->dosn[mat].config.Nc*gexp((Q*epi->dosn[mat].x[x])/(T*kb)),epi->dosn[mat].c[0][x]);
		//getchar();

		//dn
		if (dn!=NULL)
		{
			x0=epi->dosn[mat].x[x];
			x1=epi->dosn[mat].x[x+1];
			xr=1.0/(x1-x0);

			if (epi->dosn[mat].tlen>1)
			{
				t0=epi->dosn[mat].t[t];
				t1=epi->dosn[mat].t[t+1];
				tr=(T-t0)/(t1-t0);
			}else
			{
				tr=0.0;
			}

			c00=epi->dosn[mat].c[t][x];
			c01=epi->dosn[mat].c[t][x+1];
			c0=xr*(c01-c00);

			if (epi->dosn[mat].tlen>1)
			{
				c10=epi->dosn[mat].c[t+1][x];
				c11=epi->dosn[mat].c[t+1][x+1];
				c1=xr*(c11-c10);
			}

			c=c0+tr*(c1-c0);

			*dn=c;
		}

		if (w!=NULL)
		{
			x0=epi->dosn[mat].x[x];
			x1=epi->dosn[mat].x[x+1];
			xr=(top-x0)/(x1-x0);

			if (epi->dosn[mat].tlen>1)
			{
				t0=epi->dosn[mat].t[t];
				t1=epi->dosn[mat].t[t+1];
				tr=(T-t0)/(t1-t0);
			}else
			{
				tr=0.0;
			}

			c00=epi->dosn[mat].w[t][x];
			c01=epi->dosn[mat].w[t][x+1];

			c0=c00+xr*(c01-c00);

			if (epi->dosn[mat].tlen>1)
			{
				c10=epi->dosn[mat].w[t+1][x];
				c11=epi->dosn[mat].w[t+1][x+1];
				c1=c10+xr*(c11-c10);
			}

			c=c0+tr*(c1-c0);
			*w=c;
		}

		}
	//ret=epi->dosn[mat].config.Nc*gexp((Q*top)/(T*kb));

}

void get_p_den(struct epitaxy *epi,long double top,long double T, int mat,long double *p, long double *dp, long double *w)
{
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
	long double ret=0.0;

	if (epi->dosp[mat].config.dos_free_carrier_stats==mb_equation)
	{
		long double Tkb=T*kb;
		*p=epi->dosp[mat].config.Nv*gexp((Q*top)/(Tkb));

		if (dp!=NULL)
		{
			*dp=(Q/(Tkb))*(*p);
		}

		if (w!=NULL)
		{
			*w=(3.0/2.0)*Tkb;
		}


		//ret=(Q/(T*kb))*epi->dosp[mat].config.Nv*gexp((Q*top)/(T*kb));
	}else
	{

		#ifdef dos_warn
		if ((epi->dosp[mat].x[0]>top)||(epi->dosp[mat].x[epi->dosp[mat].xlen-1]<top))
		{
			errors_add(sim,"Free holes asking for %Le but range %Le %Le\n",top,epi->dosp[mat].x[0],epi->dosp[mat].x[epi->dosp[mat].xlen-1]);
		}
		#endif

		t=search(epi->dosp[mat].t,epi->dosp[mat].tlen,T);
		x=search(epi->dosp[mat].x,epi->dosp[mat].xlen,top);

		if (x<0) x=0;
		if (t<0) t=0;

		x0=epi->dosp[mat].x[x];
		x1=epi->dosp[mat].x[x+1];

		xr=(top-x0)/(x1-x0);

		if (epi->dosp[mat].tlen>1)
		{
			t0=epi->dosp[mat].t[t];
			t1=epi->dosp[mat].t[t+1];
			tr=(T-t0)/(t1-t0);
		}else
		{
			tr=0.0;
		}

		c00=epi->dosp[mat].c[t][x];
		c01=epi->dosp[mat].c[t][x+1];
		c0=c00+xr*(c01-c00);

		if (epi->dosp[mat].tlen>1)
		{
			c10=epi->dosp[mat].c[t+1][x];
			c11=epi->dosp[mat].c[t+1][x+1];
			c1=c10+xr*(c11-c10);
		}

		c=c0+tr*(c1-c0);
		*p=c;

			//double N=2.0*pow(((2.0*pi*kb*T*m*m0)/(hp*hp)),1.5);
			//long double test=epi->dosp[mat].config.Nv*gexp((Q*top)/(T*kb));
			//printf("test h = %Le %Le\n",test,ret);
			//getchar();

		//dp
		if (dp!=NULL)
		{
			x0=epi->dosp[mat].x[x];
			x1=epi->dosp[mat].x[x+1];
			xr=1.0/(x1-x0);

			if (epi->dosp[mat].tlen>1)
			{
				t0=epi->dosp[mat].t[t];
				t1=epi->dosp[mat].t[t+1];
				tr=(T-t0)/(t1-t0);
			}else
			{
				tr=0.0;
			}

			c00=epi->dosp[mat].c[t][x];
			c01=epi->dosp[mat].c[t][x+1];
			c0=xr*(c01-c00);

			if (epi->dosp[mat].tlen>1)
			{
				c10=epi->dosp[mat].c[t+1][x];
				c11=epi->dosp[mat].c[t+1][x+1];
				c1=xr*(c11-c10);
			}

			c=c0+tr*(c1-c0);
			*dp=c;
		}

		if (w!=NULL)
		{
			x0=epi->dosp[mat].x[x];
			x1=epi->dosp[mat].x[x+1];
			xr=(top-x0)/(x1-x0);

			if (epi->dosp[mat].tlen>1)
			{
				t0=epi->dosp[mat].t[t];
				t1=epi->dosp[mat].t[t+1];
				tr=(T-t0)/(t1-t0);
			}else
			{
				tr=0.0;
			}

			c00=epi->dosp[mat].w[t][x];
			c01=epi->dosp[mat].w[t][x+1];

			c0=c00+xr*(c01-c00);

			if (epi->dosp[mat].tlen>1)
			{
				c10=epi->dosp[mat].w[t+1][x];
				c11=epi->dosp[mat].w[t+1][x+1];
				c1=c10+xr*(c11-c10);
			}

			c=c0+tr*(c1-c0);
			*w=c;
		}
	}

}

long double get_n_muz(struct epitaxy *epi,int mat)
{
	return epi->dosn[mat].muz;
}

long double get_p_muz(struct epitaxy *epi,int mat)
{
	return epi->dosp[mat].muz;
}

long double get_n_mux(struct epitaxy *epi,int mat)
{
	return epi->dosn[mat].mux;
}

long double get_p_mux(struct epitaxy *epi,int mat)
{
	return epi->dosp[mat].mux;
}

long double get_n_muy(struct epitaxy *epi,int mat)
{
	return epi->dosn[mat].muy;
}

long double get_p_muy(struct epitaxy *epi,int mat)
{
	return epi->dosp[mat].muy;
}

long double get_dos_B(struct epitaxy *epi,int mat)
{
return epi->dosn[mat].B;
}


long double get_dp_den(struct epitaxy *epi,long double top,long double T, int mat)
{
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

	int t;
	int x;

	long double ret=0.0;

	if (epi->dosp[mat].config.dos_free_carrier_stats==mb_equation)
	{
		ret=(Q/(T*kb))*epi->dosp[mat].config.Nv*gexp((Q*top)/(T*kb));
	}else
	{
		#ifdef dos_warn
		if ((epi->dosp[mat].x[0]>top)||(epi->dosp[mat].x[epi->dosp[mat].xlen-1]<top))
		{
			errors_add(sim,"Free electrons Asking for %e but range %e %e\n",top,epi->dosp[mat].x[0],epi->dosp[mat].x[epi->dosp[mat].xlen-1]);
		}
		#endif

		t=search(epi->dosp[mat].t,epi->dosp[mat].tlen,T);
		x=search(epi->dosp[mat].x,epi->dosp[mat].xlen,top);

		x0=epi->dosp[mat].x[x];
		x1=epi->dosp[mat].x[x+1];
		xr=1.0/(x1-x0);

		if (epi->dosp[mat].tlen>1)
		{
			t0=epi->dosp[mat].t[t];
			t1=epi->dosp[mat].t[t+1];
			tr=(T-t0)/(t1-t0);
		}else
		{
			tr=0.0;
		}

		c00=epi->dosp[mat].c[t][x];
		c01=epi->dosp[mat].c[t][x+1];
		c0=xr*(c01-c00);

		if (epi->dosp[mat].tlen>1)
		{
			c10=epi->dosp[mat].c[t+1][x];
			c11=epi->dosp[mat].c[t+1][x+1];
			c1=xr*(c11-c10);
		}

		c=c0+tr*(c1-c0);

		ret=c;

		//printf(">>%Le %Le\n",ret,(Q/(T*kb))*epi->dosp[mat].config.Nv*gexp((Q*top)/(T*kb)));
		//getchar();
	}
return ret;
}



