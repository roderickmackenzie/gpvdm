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

/** @file dos_struct.h
	@brief Hold information about the DoS.
*/


#ifndef dos_struct_h
#define dos_struct_h

struct dos_cache_obj
{
	int len;
	char id[100];
	char md5[100];
	char *dosn;
	char *dosp;
};

struct dos_cache
{
	int enabled;
	int setup;
	int len;
	int len_max;
	struct dos_cache_obj *objs;
};

struct dosconfig
{
	char dos_name[20];
	int dos_number;
	int dostype;
	int dos_free_carrier_stats;
	long double Nt;
	long double Et;
	long double nstart;
	long double nstop;
	int npoints;

	long double Tstart;
	long double Tstop;
	int Tsteps;
	int traps;
	long double dband;
	long double detrap;
	int srh_bands;
	long double srh_start;
	long double srh_stop;


	long double srh_sigman;
	long double srh_sigmap; 
	long double srh_vth;
	long double Nc;
	long double Nv;
	long double me;
	long double mh;

	long double Eg;
	long double Xi;
	long double epsilonr;
	long double Na0;
	long double Na1;
	long double Nd0;
	long double Nd1;

	int Esteps;

	//outside binary dos file


	long double ion_density;
	long double ion_mobility;
};

struct dos
{
	int enabled;
	long double *x;
	int xlen;
	int tlen;
	int srh_bands;
	long double *t;
	long double *srh_E;
	long double *srh_den;
	long double **c;
	long double **w;
	long double ***srh_r1;
	long double ***srh_r2;
	long double ***srh_r3;
	long double ***srh_r4;
	long double ***srh_c;
	struct dosconfig config;

	long double muz;
	long double mux;
	long double muy;

	//Auger
	int auger_enabled;
	long double Cn;
	long double Cp;

	//SS SRH
	int ss_srh_enabled;
	long double n1;
	long double p1;
	long double tau_n;
	long double tau_p;

	long double B;

	int mobility_symmetric;
};


#endif
