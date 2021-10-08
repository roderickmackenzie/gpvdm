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

/** @file newton_tricks.h
@brief functions to solver for an external voltage
*/

#ifndef newton_tricks_h
#define newton_tricks_h

struct newton_math_state
{
	int max_electrical_itt;
	gdouble min_cur_error;
	int newton_min_itt;
	gdouble electrical_clamp;
	int newton_clever_exit;
};


void newton_push_state(struct device *in);
void newton_pop_state(struct device *in);
gdouble sim_externalv(struct simulation *sim,struct device *in,gdouble wantedv);
gdouble sim_i(struct simulation *sim,struct device *in,gdouble wantedi);
void auto_ramp_contacts(struct simulation *sim,struct device *in);
void ramp_externalv(struct simulation *sim,struct device *in,gdouble from,gdouble to);
void set_ntricks_fast(int val);
gdouble sim_voc(struct device *in);
void newton_sim_simple(struct simulation  *sim,struct device *in);
void ntricks_auto_ramp_contacts(struct simulation *sim,struct device *in);


void newton_externv_aux(struct simulation *sim,struct device *in,gdouble V,gdouble* i,gdouble* didv,gdouble* didphi,gdouble* didxil,gdouble* didxipl,gdouble* didphir,gdouble* didxir,gdouble* didxipr);
long double newton_externv(struct simulation *sim,struct device *in,long double Vtot);
gdouble newton_externalv_simple(struct simulation *sim,struct device *in,gdouble V);
long double sim_externalv_ittr(struct simulation *sim,struct device *in,gdouble wantedv);

void state_cache_init(struct simulation *sim,struct device *in);
void hash_dir(struct simulation *sim,char *out);
//void state_gen_vector(struct simulation *sim,struct device *in);
//int state_find_vector(struct simulation *sim,struct device *in,char *out);

//newton state
void newton_state_init(struct newton_state *ns);
void newton_state_alloc_mesh(struct newton_state *ns,struct dimensions *dim, int alloc_dim);
void newton_state_alloc_traps(struct newton_state *ns,struct dimensions *dim);
void newton_state_free(struct newton_state *ns);
void newton_state_cpy(struct newton_state *out,struct newton_state *in);
void newton_state_save(struct simulation *sim,char *file_name,struct newton_state *ns);
int newton_state_load(struct simulation *sim,struct newton_state *ns,char *file_name);
void newton_state_update_device(struct simulation *sim,struct device *in, struct newton_state *ns);
void newton_state_set_last_error(struct simulation *sim, struct newton_state *ns,long double error);
void newton_state_reset_error(struct simulation *sim, struct newton_state *ns);
int newton_state_clever_exit(struct simulation *sim, struct newton_state *ns);

//newton state complex
void newton_state_complex_init(struct newton_state_complex *ns);
void newton_state_complex_alloc_mesh(struct newton_state_complex *ns,struct dimensions *dim);
void newton_state_complex_alloc_traps(struct newton_state_complex *ns,struct dimensions *dim);
void newton_state_complex_free(struct newton_state_complex *ns);
#endif

