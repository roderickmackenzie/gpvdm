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



long double get_dpdT_den(struct shape *s,long double top,long double T)
{
long double ret=0.0;
long double N=s->dosp.config.Nv;
ret= -((top*Qe)/kb)*N*gexp((top*Qe)/(kb*T))*gpow(T,-2.0);
return ret;
}

long double get_dndT_den(struct shape *s,long double top,long double T)
{
long double ret=0.0;
long double N=s->dosn.config.Nc;
ret= -((top*Qe)/kb)*N*gexp((top*Qe)/(kb*T))*gpow(T,-2.0);
return ret;
}

void check_fermi_inversion_n(struct simulation *sim,struct shape *s,long double n,int do_check)
{
	if (do_check==TRUE)
	{
		if (fabs(n)>s->dosn.config.Nc)
		{
			ewe(sim,"The carrier density at a contact is larger than the effective density of states.\nThis is probably non-physical as it will mean your Fermi level is above that of the band.\n Either decrease the charge density on the contacts, or increase the effective density of states.\n For most devices I would recommend an effective density of states of around 1x10^25 m-3.\nIf you know what you are doing you can disable this error message in the mathematical settings but then you should turn on Fermi-Dirac stats.\n");
		}
	}

}

void check_fermi_inversion_p(struct simulation *sim,struct shape *s,long double p,int do_check)
{
	if (do_check==TRUE)
	{
		if (fabs(p)>s->dosn.config.Nv)
		{
			ewe(sim,"The carrier density at a contact is larger than the effective density of states.\nThis is probably non-physical as it will mean your Fermi level is above that of the band.\n Either decrease the charge density on the contacts, or increase the effective density of states.\n For most devices I would recommend an effective density of states of around 1x10^25 m-3.\nIf you know what you are doing you can disable this error message in the mathematical settings but then you should turn on Fermi-Dirac stats.\n");
		}
	}

}
long double get_top_from_n(struct shape *s,long double n,long double T)
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


	if (s->dosn.config.dos_free_carrier_stats==mb_equation)
	{
		ret=(kb*T/Qe)*log((fabs(n))/s->dosn.config.Nc);
	}else
	{
		if (s->dosn.tlen>1)
		{
			for (t=0;t<s->dosn.tlen-1;t++)
			{
				if (s->dosn.t[t]>T) break;
			}
			t--;

		}else
		{
			t=0;
		}

		xx=0;


		for (x=0;x<s->dosn.xlen-1;x++)
		{
			if (s->dosn.c[t][x]>n) break;
		}

		x--;

		if (x<0) x=0;
		if (xx<0) xx=0;
		if (t<0) t=0;

		x0=s->dosn.c[t][x];
		x1=s->dosn.c[t][x+1];
		xr=(n-x0)/(x1-x0);
		if (xr>1) xr=1;
		c0=s->dosn.x[x];
		c1=s->dosn.x[x+1];

		cll=c0+xr*(c1-c0);

		ret=cll;

	}


return ret;
}


long double get_top_from_p(struct shape *s,long double p,long double T)
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

	if (s->dosp.config.dos_free_carrier_stats==mb_equation)
	{
		ret=(kb*T/Qe)*log((fabs(p))/s->dosp.config.Nv);
	}else
	{

		for (t=0;t<s->dosp.tlen-1;t++)
		{
		if (s->dosp.t[t]>T) break;
		}
		t--;

		if (t<0) t=0;

		xx=0;

		for (x=0;x<s->dosp.xlen-1;x++)
		{
			if (s->dosp.c[t][x]>p) break;
		}

		x--;

		if (x<0) x=0;
		if (xx<0) xx=0;

		x0=s->dosp.c[t][x];
		x1=s->dosp.c[t][x+1];
		xr=(p-x0)/(x1-x0);
		c0=s->dosp.x[x];
		c1=s->dosp.x[x+1];
		if (xr>1) xr=1;
		cll=c0+xr*(c1-c0);

		c=cll;

		ret=c;
//		printf(">>>>>> %Le %Le\n",ret ,(kb*T/Qe)*log((fabs(p))/s->dosp.config.Nv));
//		getchar();
	}

return ret;
}

void get_n_den(struct shape *s,long double top,long double T,long double *n, long double *dn,long double *w)
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
	//long double ret=0.0;

	if (s->dosn.config.dos_free_carrier_stats==mb_equation)
	{
		long double Tkb=T*kb;
		*n=s->dosn.config.Nc*gexp((Qe*top)/(Tkb));

		//if (*n>s->dosn.config.Nc)
		//{
		//	printf("%Le %Le %Le\n",s->dosn.config.Nc,*n,top);
		//}
		if (dn!=NULL)
		{
			//*dn=(Qe/(T*kb))*s->dosn.config.Nc*gexp((Qe*top)/(T*kb));
			*dn=(Qe/(Tkb))*(*n);
		}

		if (w!=NULL)
		{
			*w=(3.0/2.0)*Tkb;
		}
	}else
	{

		#ifdef dos_warn
		if ((s->dosn.x[0]>top)||(s->dosn.x[s->dosn.xlen-1]<top))
		{
			errors_add(sim,"Free electrons asking for %Le but range %Le %Le\n",top,s->dosn.x[0],s->dosn.x[s->dosn.xlen-1]);
		}
		#endif

		t=search(s->dosn.t,s->dosn.tlen,T);
		x=search(s->dosn.x,s->dosn.xlen,top);

		if (x<0) x=0;
		if (t<0) t=0;

		//n
		x0=s->dosn.x[x];
		x1=s->dosn.x[x+1];
		xr=(top-x0)/(x1-x0);

		if (s->dosn.tlen>1)
		{
			t0=s->dosn.t[t];
			t1=s->dosn.t[t+1];
			tr=(T-t0)/(t1-t0);
		}else
		{
			tr=0.0;
		}

		c00=s->dosn.c[t][x];
		c01=s->dosn.c[t][x+1];
		c0=c00+xr*(c01-c00);

		if (s->dosn.tlen>1)
		{
			c10=s->dosn.c[t+1][x];
			c11=s->dosn.c[t+1][x+1];
			c1=c10+xr*(c11-c10);
		}

		c=c0+tr*(c1-c0);
		*n=c;

			//double N=2.0*pow(((2.0*pi*kb*T*m*m0)/(hp*hp)),1.5);
			//long double test=s->dosn.config.Nc*gexp((Qe*top)/(T*kb));
			//printf("test = %Le %Le\n",test,ret);
			//getchar();
		//printf("%Lf %Lf %Lf\n",s->dosn.x[x],s->dosn.x[x+1],top);
		//printf(">>%Le %Le\n",s->dosn.config.Nc*gexp((Qe*top)/(T*kb)),ret);
		//printf("%Le %Le\n",s->dosn.config.Nc*gexp((Qe*s->dosn.x[x])/(T*kb)),s->dosn.c[0][x]);
		//getchar();

		//dn
		if (dn!=NULL)
		{
			x0=s->dosn.x[x];
			x1=s->dosn.x[x+1];
			xr=1.0/(x1-x0);

			if (s->dosn.tlen>1)
			{
				t0=s->dosn.t[t];
				t1=s->dosn.t[t+1];
				tr=(T-t0)/(t1-t0);
			}else
			{
				tr=0.0;
			}

			c00=s->dosn.c[t][x];
			c01=s->dosn.c[t][x+1];
			c0=xr*(c01-c00);

			if (s->dosn.tlen>1)
			{
				c10=s->dosn.c[t+1][x];
				c11=s->dosn.c[t+1][x+1];
				c1=xr*(c11-c10);
			}

			c=c0+tr*(c1-c0);

			*dn=c;
		}

		if (w!=NULL)
		{
			x0=s->dosn.x[x];
			x1=s->dosn.x[x+1];
			xr=(top-x0)/(x1-x0);

			if (s->dosn.tlen>1)
			{
				t0=s->dosn.t[t];
				t1=s->dosn.t[t+1];
				tr=(T-t0)/(t1-t0);
			}else
			{
				tr=0.0;
			}

			c00=s->dosn.w[t][x];
			c01=s->dosn.w[t][x+1];

			c0=c00+xr*(c01-c00);

			if (s->dosn.tlen>1)
			{
				c10=s->dosn.w[t+1][x];
				c11=s->dosn.w[t+1][x+1];
				c1=c10+xr*(c11-c10);
			}

			c=c0+tr*(c1-c0);
			*w=c;
		}

		}
	//ret=s->dosn.config.Nc*gexp((Qe*top)/(T*kb));

}

void get_p_den(struct shape *s,long double top,long double T, long double *p, long double *dp, long double *w)
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
	//long double ret=0.0;

	if (s->dosp.config.dos_free_carrier_stats==mb_equation)
	{
		long double Tkb=T*kb;
		*p=s->dosp.config.Nv*gexp((Qe*top)/(Tkb));

		if (dp!=NULL)
		{
			*dp=(Qe/(Tkb))*(*p);
		}

		if (w!=NULL)
		{
			*w=(3.0/2.0)*Tkb;
		}


		//ret=(Qe/(T*kb))*s->dosp.config.Nv*gexp((Qe*top)/(T*kb));
	}else
	{

		#ifdef dos_warn
		if ((s->dosp.x[0]>top)||(s->dosp.x[s->dosp.xlen-1]<top))
		{
			errors_add(sim,"Free holes asking for %Le but range %Le %Le\n",top,s->dosp.x[0],s->dosp.x[s->dosp.xlen-1]);
		}
		#endif

		t=search(s->dosp.t,s->dosp.tlen,T);
		x=search(s->dosp.x,s->dosp.xlen,top);

		if (x<0) x=0;
		if (t<0) t=0;

		x0=s->dosp.x[x];
		x1=s->dosp.x[x+1];

		xr=(top-x0)/(x1-x0);

		if (s->dosp.tlen>1)
		{
			t0=s->dosp.t[t];
			t1=s->dosp.t[t+1];
			tr=(T-t0)/(t1-t0);
		}else
		{
			tr=0.0;
		}

		c00=s->dosp.c[t][x];
		c01=s->dosp.c[t][x+1];
		c0=c00+xr*(c01-c00);

		if (s->dosp.tlen>1)
		{
			c10=s->dosp.c[t+1][x];
			c11=s->dosp.c[t+1][x+1];
			c1=c10+xr*(c11-c10);
		}

		c=c0+tr*(c1-c0);
		*p=c;

			//double N=2.0*pow(((2.0*pi*kb*T*m*m0)/(hp*hp)),1.5);
			//long double test=s->dosp.config.Nv*gexp((Qe*top)/(T*kb));
			//printf("test h = %Le %Le\n",test,ret);
			//getchar();

		//dp
		if (dp!=NULL)
		{
			x0=s->dosp.x[x];
			x1=s->dosp.x[x+1];
			xr=1.0/(x1-x0);

			if (s->dosp.tlen>1)
			{
				t0=s->dosp.t[t];
				t1=s->dosp.t[t+1];
				tr=(T-t0)/(t1-t0);
			}else
			{
				tr=0.0;
			}

			c00=s->dosp.c[t][x];
			c01=s->dosp.c[t][x+1];
			c0=xr*(c01-c00);

			if (s->dosp.tlen>1)
			{
				c10=s->dosp.c[t+1][x];
				c11=s->dosp.c[t+1][x+1];
				c1=xr*(c11-c10);
			}

			c=c0+tr*(c1-c0);
			*dp=c;
		}

		if (w!=NULL)
		{
			x0=s->dosp.x[x];
			x1=s->dosp.x[x+1];
			xr=(top-x0)/(x1-x0);

			if (s->dosp.tlen>1)
			{
				t0=s->dosp.t[t];
				t1=s->dosp.t[t+1];
				tr=(T-t0)/(t1-t0);
			}else
			{
				tr=0.0;
			}

			c00=s->dosp.w[t][x];
			c01=s->dosp.w[t][x+1];

			c0=c00+xr*(c01-c00);

			if (s->dosp.tlen>1)
			{
				c10=s->dosp.w[t+1][x];
				c11=s->dosp.w[t+1][x+1];
				c1=c10+xr*(c11-c10);
			}

			c=c0+tr*(c1-c0);
			*w=c;
		}
	}

}

long double get_n_muz(struct shape *s)
{
	return s->dosn.muz;
}

long double get_p_muz(struct shape *s)
{
	return s->dosp.muz;
}

long double get_n_mux(struct shape *s)
{
	return s->dosn.mux;
}

long double get_p_mux(struct shape *s)
{
	return s->dosp.mux;
}

long double get_n_muy(struct shape *s)
{
	return s->dosn.muy;
}

long double get_p_muy(struct shape *s)
{
	return s->dosp.muy;
}

//Auger
long double get_Cn(struct shape *s)
{
	if (s->dosn.auger_enabled==FALSE)
	{
		return 0.0;
	}

	return s->dosn.Cn;
}

long double get_Cp(struct shape *s)
{
	if (s->dosp.auger_enabled==FALSE)
	{
		return 0.0;
	}

	return s->dosp.Cp;
}

//Steady state SRH
long double get_ss_srh_n1(struct shape *s)
{
	if (s->dosn.ss_srh_enabled==FALSE)
	{
		return 0.0;
	}

	return s->dosn.n1;
}

long double get_ss_srh_p1(struct shape *s)
{
	if (s->dosn.ss_srh_enabled==FALSE)
	{
		return 0.0;
	}

	return s->dosn.p1;
}

long double get_ss_srh_tau_n(struct shape *s)
{
	if (s->dosn.ss_srh_enabled==FALSE)
	{
		return 0.0;
	}

	return s->dosn.tau_n;
}

long double get_ss_srh_tau_p(struct shape *s)
{
	if (s->dosn.ss_srh_enabled==FALSE)
	{
		return 0.0;
	}

	return s->dosn.tau_p;
}

long double get_dos_B(struct shape *s)
{
return s->dosn.B;
}


long double get_dp_den(struct shape *s,long double top,long double T)
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

	if (s->dosp.config.dos_free_carrier_stats==mb_equation)
	{
		ret=(Qe/(T*kb))*s->dosp.config.Nv*gexp((Qe*top)/(T*kb));
	}else
	{
		#ifdef dos_warn
		if ((s->dosp.x[0]>top)||(s->dosp.x[s->dosp.xlen-1]<top))
		{
			errors_add(sim,"Free electrons Asking for %e but range %e %e\n",top,s->dosp.x[0],s->dosp.x[s->dosp.xlen-1]);
		}
		#endif

		t=search(s->dosp.t,s->dosp.tlen,T);
		x=search(s->dosp.x,s->dosp.xlen,top);

		x0=s->dosp.x[x];
		x1=s->dosp.x[x+1];
		xr=1.0/(x1-x0);

		if (s->dosp.tlen>1)
		{
			t0=s->dosp.t[t];
			t1=s->dosp.t[t+1];
			tr=(T-t0)/(t1-t0);
		}else
		{
			tr=0.0;
		}

		c00=s->dosp.c[t][x];
		c01=s->dosp.c[t][x+1];
		c0=xr*(c01-c00);

		if (s->dosp.tlen>1)
		{
			c10=s->dosp.c[t+1][x];
			c11=s->dosp.c[t+1][x+1];
			c1=xr*(c11-c10);
		}

		c=c0+tr*(c1-c0);

		ret=c;

		//printf(">>%Le %Le\n",ret,(Qe/(T*kb))*s->dosp.config.Nv*gexp((Qe*top)/(T*kb)));
		//getchar();
	}
return ret;
}



