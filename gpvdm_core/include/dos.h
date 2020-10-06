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

/** @file dos.h
	@brief Headers for reading and getting values from the DoS.
*/


#ifndef dos_h
#define dos_h

#include <device.h>
#include <dos_struct.h>

void dos_config_load(struct simulation *sim,struct dosconfig *confige,struct dosconfig *configh,char * dos_file, char *lumo_file, char *homo_file);
long double get_dn_trap_den(long double top,long double T,int type,int band, int mat);
long double get_dp_trap_den(long double top,long double T,int type, int mat);

long double get_n_muz(struct epitaxy *epi,int mat);
long double get_p_muz(struct epitaxy *epi,int mat);

long double get_n_mux(struct epitaxy *epi,int mat);
long double get_p_mux(struct epitaxy *epi,int mat);

long double get_n_muy(struct epitaxy *epi,int mat);
long double get_p_muy(struct epitaxy *epi,int mat);



long double get_top_from_n(struct epitaxy *epi,long double n,long double T,int mat);
long double get_top_from_p(struct epitaxy *epi,long double p,long double T,int mat);
void get_n_den(struct epitaxy *epi,long double top,long double T,int mat,long double *n, long double *dn, long double *w);
void get_p_den(struct epitaxy *epi,long double top,long double T, int mat,long double *p, long double *dp, long double *w);
long double get_n_mu(struct epitaxy *epi,int mat);
long double get_p_mu(struct epitaxy *epi,int mat);
long double get_dpdT_den(struct epitaxy *epi,long double top,long double T,int mat);
long double get_dndT_den(struct epitaxy *epi,long double top,long double T,int mat);
void gen_dos_fd_gaus_n(struct simulation *sim,char * dos_file, char *lumo_file, char *homo_file);
void gen_dos_fd_gaus_p(struct simulation *sim,char * dos_file, char *lumo_file, char *homo_file);
//int hashget(long double *x,int N,long double find);

void safe_file(char *name);

long double get_dos_B(struct epitaxy *epi,int mat);


void dos_init(struct dos *mydos);
void dos_free(struct dos *mydos);
void dos_cpy(struct dos *out,struct dos *in);

//DoS dos.c
long double get_dos_filled_n(struct device *in);
long double get_dos_filled_p(struct device *in);
long double dos_srh_get_fermi_n(struct epitaxy *epi,long double n, long double p,int band,int mat,long double T);
long double dos_srh_get_fermi_p(struct epitaxy *epi,long double n, long double p,int band,int mat, long double T);
long double get_Nc_free(struct epitaxy *epi,int mat);
long double get_Nv_free(struct epitaxy *epi,int mat);
void load_txt_dos_file(struct simulation *sim,struct dos *dos_n,struct dos *dos_p,char *file);
void load_binary_dos_file(struct simulation *sim,struct dos *mydos,char *file);
long double get_dos_ion_density(struct epitaxy *epi,int mat);
long double get_dos_ion_mobility(struct epitaxy *epi,int mat);
long double get_dos_doping_start(struct epitaxy *epi,int mat);
long double get_dos_doping_stop(struct epitaxy *epi,int mat);
long double get_dos_epsilonr(struct epitaxy *epi,int mat);
long double dos_get_band_energy_n(struct epitaxy *epi,int band,int mat);
long double dos_get_band_energy_p(struct epitaxy *epi,int band,int mat);
long double get_dos_Eg(struct epitaxy *epi,int mat);
long double get_dos_Xi(struct epitaxy *epi,int mat);
void load_dos(struct simulation *sim,struct device *in,char *dos_file,int mat);
long double get_dos_E_n(struct epitaxy *epi,int band,int mat);
long double get_dos_E_p(struct epitaxy *epi,int band,int mat);
void get_n_srh(struct simulation *sim,struct epitaxy *epi,long double top,long double T,int trap,long double *nt,long double *srh1,long double *srh2,long double *srh3,long double *srh4,int mat);
void get_p_srh(struct simulation *sim,struct epitaxy *epi,long double top,long double T,int trap,long double *pt,long double *srh1,long double *srh2,long double *srh3,long double *srh4,int mat);
void get_dn_srh(struct simulation *sim,struct epitaxy *epi,long double top,long double T,int trap,long double *dnt,long double *srh1,long double *srh2,long double *srh3,long double *srh4,int mat);
void get_dp_srh(struct simulation *sim,struct epitaxy *epi,long double top,long double T,int trap,long double *dpt,long double *srh1,long double *srh2,long double *srh3,long double *srh4,int mat);
long double get_n_pop_srh(struct simulation *sim,struct epitaxy *epi,long double top,long double T,int trap,int mat);
long double get_p_pop_srh(struct simulation *sim,struct epitaxy *epi,long double top,long double T,int trap,int mat);
long double get_dn_pop_srh(struct simulation *sim,struct epitaxy *epi,long double top,long double T,int trap, int mat);
long double get_dp_pop_srh(struct simulation *sim,struct epitaxy *epi,long double top,long double T,int trap, int mat);
#endif
