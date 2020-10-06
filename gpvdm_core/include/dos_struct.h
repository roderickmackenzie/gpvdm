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

/** @file dos_struct.h
	@brief Hold information about the DoS.
*/


#ifndef dos_struct_h
#define dos_struct_h

struct dosconfig
{
	char dos_name[20];
	char analytical_dos_file_name[20];
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
	long double doping_start;
	long double doping_stop;

	int Esteps;

	//outside binary dos file


	long double ion_density;
	long double ion_mobility;
};

struct dos
{
	int used;
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

	long double B;

	int mobility_symmetric;
};


#endif
