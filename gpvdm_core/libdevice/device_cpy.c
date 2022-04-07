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


/** @file device.c
	@brief Initialize the device structure.
*/

#define _FILE_OFFSET_BITS 64
#define _LARGEFILE_SOURCE

#include <stdio.h>
#include <device.h>
#include <string.h>
#include <dump.h>
#include <mesh.h>
#include <sim.h>
#include <ray_fun.h>
#include <newton_tricks.h>
#include <memory.h>
#include <circuit.h>
#include <shape.h>
#include <heat.h>
#include <heat_fun.h>
#include <exciton.h>
#include <exciton_fun.h>
#include <gpvdm_const.h>
#include <lib_fxdomain.h>
#include <fxdomain_fun.h>
#include <device_fun.h>
#include <contacts.h>
#include <perovskite.h>
#include <light_fun.h>
#include <epitaxy.h>
#include <enabled_libs.h>
#include <light_srcs.h>

#include <world_struct.h>
#include <world.h>
#include <optical_mode_fun.h>

static int unused __attribute__((unused));
static char* unused_pchar __attribute__((unused));

void device_cpy(struct simulation *sim,struct device *out,struct device *in)
{
	struct newton_state *ns=&(in->ns);
	struct dimensions *dim=&ns->dim;
	int i;

	//Dimensions
		out->xlen=in->xlen;
		out->ylen=in->ylen;
		out->zlen=in->zlen;

		out->A= in->A;
		out->Vol= in->Vol;
		out->area= in->area;

	//Meshing
		out->remesh=in->remesh;
		out->newmeshsize=in->newmeshsize;
		out->dynamic_mesh=in->dynamic_mesh;

	//Current at contacts
		cpy_zx_long_double(dim, &(out->Jn_y0),&(in->Jn_y0));
		cpy_zx_long_double(dim, &(out->Jn_y1),&(in->Jn_y1));
		cpy_zx_long_double(dim, &(out->Jp_y0),&(in->Jp_y0));
		cpy_zx_long_double(dim, &(out->Jp_y1),&(in->Jp_y1));

	//Charge density at contacts
		cpy_zx_int(dim, &(out->n_contact_y0), &(in->n_contact_y0));
		cpy_zx_int(dim, &(out->n_contact_y1), &(in->n_contact_y1));
		cpy_zy_int(dim, &(out->n_contact_x0), &(in->n_contact_x0));
		cpy_zy_int(dim, &(out->n_contact_x1), &(in->n_contact_x1));

	//Contact fermi levels
		cpy_zx_long_double(dim, &(out->Fi0_y0),&(in->Fi0_y0));
		cpy_zx_long_double(dim, &(out->Fi0_y1),&(in->Fi0_y1));

		cpy_zy_long_double(dim, &(out->Fi0_x0),&(in->Fi0_x0));
		cpy_zy_long_double(dim, &(out->Fi0_x1),&(in->Fi0_x1));

	//Charge densities on surfaces even away from contacts
		cpy_zx_long_double(dim,&(out->electrons_y0),&(in->electrons_y0));
		cpy_zx_long_double(dim,&(out->holes_y0),&(in->holes_y0));

		cpy_zx_long_double(dim,&(out->electrons_y1),&(in->electrons_y1));
		cpy_zx_long_double(dim,&(out->holes_y1),&(in->holes_y1));

		cpy_zy_long_double(dim,&(out->electrons_x0),&(in->electrons_x0));
		cpy_zy_long_double(dim,&(out->holes_x0),&(in->holes_x0));

		cpy_zy_long_double(dim,&(out->electrons_x1),&(in->electrons_x1));
		cpy_zy_long_double(dim,&(out->holes_x1),&(in->holes_x1));


	//Built in potentials
		cpy_zx_long_double(dim, &(out->V_y0),&(in->V_y0));
		cpy_zx_long_double(dim, &(out->V_y1),&(in->V_y1));
		cpy_zy_long_double(dim, &(out->V_x0),&(in->V_x0));
		cpy_zy_long_double(dim, &(out->V_x1),&(in->V_x1));

	//Ions
		cpy_zxy_long_double(dim, &(out->Nad), &(in->Nad));
		cpy_zxy_long_double(dim, &(out->Nion), &(in->Nion));
		cpy_zxy_long_double(dim, &(out->dNion), &(in->dNion));
		cpy_zxy_long_double(dim, &(out->dNiondphi), &(in->dNiondphi));
		cpy_zxy_long_double(dim, &(out->Nion_last), &(in->Nion_last));

	//Singlet
		cpy_zxy_long_double(dim, &(out->Ns), &(in->Ns));
		cpy_zxy_long_double(dim, &(out->Nt), &(in->Nt));
		cpy_zxy_long_double(dim, &(out->Nsd), &(in->Nsd));
		cpy_zxy_long_double(dim, &(out->Ntd), &(in->Ntd));
		out->Nho=in->Nho;

		cpy_zxy_long_double(dim, &(out->dNs), &(in->dNs));
		cpy_zxy_long_double(dim, &(out->dNt), &(in->dNt));
		cpy_zxy_long_double(dim, &(out->dNsd), &(in->dNsd));
		cpy_zxy_long_double(dim, &(out->dNtd), &(in->dNtd));
		out->dNho=in->dNho;

		cpy_zxy_long_double(dim, &(out->Ns_last), &(in->Ns_last));
		cpy_zxy_long_double(dim, &(out->Nt_last), &(in->Nt_last));
		cpy_zxy_long_double(dim, &(out->Nsd_last), &(in->Nsd_last));
		cpy_zxy_long_double(dim, &(out->Ntd_last), &(in->Ntd_last));
		out->Nho_last=in->Nho_last;

		mode_cpy(sim,&(out->mode),&(in->mode));

	//Generation
		cpy_zxy_long_double(dim, &(out->G), &(in->G));
		cpy_zxy_long_double(dim, &(out->Gn), &(in->Gn));
		cpy_zxy_long_double(dim, &(out->Gp), &(in->Gp));

	//Free charges
		cpy_zxy_long_double(dim, &(out->n), &(in->n));
		cpy_zxy_long_double(dim, &(out->p), &(in->p));
		cpy_zxy_long_double(dim, &(out->dn), &(in->dn));
		cpy_zxy_long_double(dim, &(out->dndphi), &(in->dndphi));
		cpy_zxy_long_double(dim, &(out->dp), &(in->dp));
		cpy_zxy_long_double(dim, &(out->dpdphi), &(in->dpdphi));
		cpy_zxy_long_double(dim, &(out->Dn), &(in->Dn));
		cpy_zxy_long_double(dim, &(out->Dp), &(in->Dp));

		cpy_zxy_long_double(dim, &(out->Fn), &(in->Fn));
		cpy_zxy_long_double(dim, &(out->Fp), &(in->Fp));
		cpy_zxy_long_double(dim, &(out->Nc), &(in->Nc));
		cpy_zxy_long_double(dim, &(out->Nv), &(in->Nv));

		cpy_zxy_long_double(dim, &(out->nf_save), &(in->nf_save));
		cpy_zxy_long_double(dim, &(out->pf_save), &(in->pf_save));

		cpy_zxy_long_double(dim, &(out->nfequlib), &(in->nfequlib));
		cpy_zxy_long_double(dim, &(out->pfequlib), &(in->pfequlib));

		cpy_zxy_long_double(dim, &(out->nlast), &(in->nlast));
		cpy_zxy_long_double(dim, &(out->plast), &(in->plast));

		cpy_zxy_long_double(dim, &(out->wn), &(in->wn));
		cpy_zxy_long_double(dim, &(out->wp), &(in->wp));

		cpy_zxy_long_double(dim, &(out->n_orig), &(in->n_orig));
		cpy_zxy_long_double(dim, &(out->p_orig), &(in->p_orig));

		cpy_zxy_long_double(dim, &(out->n_orig_f), &(in->n_orig_f));
		cpy_zxy_long_double(dim, &(out->p_orig_f), &(in->p_orig_f));

		cpy_zxy_long_double(dim, &(out->n_orig_t), &(in->n_orig_t));
		cpy_zxy_long_double(dim, &(out->p_orig_t), &(in->p_orig_t));

		cpy_zxy_long_double(dim, &(out->t), &(in->t));
		cpy_zxy_long_double(dim, &(out->tp), &(in->tp));
		cpy_zxy_long_double(dim, &(out->t_ion), &(in->t_ion));

	//Fermi levels
		cpy_zxy_long_double(dim, &(out->Fi), &(in->Fi));

	//Bands
		cpy_zxy_long_double(dim, &(out->Eg), &(in->Eg));
		cpy_zxy_long_double(dim, &(out->Xi), &(in->Xi));
		cpy_zxy_long_double(dim, &(out->Ev), &(in->Ev));
		cpy_zxy_long_double(dim, &(out->Ec), &(in->Ec));

	//Recombination
		cpy_zxy_long_double(dim, &(out->Rfree), &(in->Rfree));
		cpy_zxy_long_double(dim, &(out->Rauger), &(in->Rauger));

		cpy_zxy_long_double(dim, &(out->Rn), &(in->Rn));
		cpy_zxy_long_double(dim, &(out->Rp), &(in->Rp));
		cpy_zxy_long_double(dim, &(out->Rnet), &(in->Rnet));

		cpy_zxy_long_double(dim, &(out->Rn_srh), &(in->Rn_srh));
		cpy_zxy_long_double(dim, &(out->Rp_srh), &(in->Rp_srh));

		cpy_zxy_long_double(dim, &(out->Rbi_k), &(in->Rbi_k));

		cpy_zxy_long_double(dim, &(out->B), &(in->B));

	//Interfaces
		out->interfaces_n=in->interfaces_n;
		out->interfaces_n_srh=in->interfaces_n_srh;
		cpy_zxy_int(dim,&(out->interface_type),&(in->interface_type));
		cpy_zxy_long_double(dim, &(out->interface_B), &(in->interface_B));
		cpy_zxy_long_double(dim, &(out->interface_Bt), &(in->interface_Bt));
		cpy_zxy_long_double(dim, &(out->interface_R), &(in->interface_R));
		//Tunnel
		out->interfaces_tunnels_e=in->interfaces_tunnels_e;
		out->interfaces_tunnels_h=in->interfaces_tunnels_h;
		cpy_zxy_long_double(dim, &(out->interface_Ge), &(in->interface_Ge));
		cpy_zxy_long_double(dim, &(out->interface_Gh), &(in->interface_Gh));

	//Rates
		cpy_zxy_long_double(dim, &(out->nrelax), &(in->nrelax));
		cpy_zxy_long_double(dim, &(out->ntrap_to_p), &(in->ntrap_to_p));
		cpy_zxy_long_double(dim, &(out->prelax), &(in->prelax));
		cpy_zxy_long_double(dim, &(out->ptrap_to_n), &(in->ptrap_to_n));

	//Mobility
		out->mun_symmetric=in->mun_symmetric;
		cpy_zxy_long_double(dim, &(out->mun_z), &(in->mun_z));
		cpy_zxy_long_double(dim, &(out->mun_x), &(in->mun_x));
		cpy_zxy_long_double(dim, &(out->mun_y), &(in->mun_y));

		out->mup_symmetric=in->mup_symmetric;
		cpy_zxy_long_double(dim, &(out->mup_z), &(in->mup_z));
		cpy_zxy_long_double(dim, &(out->mup_x), &(in->mup_x));
		cpy_zxy_long_double(dim, &(out->mup_y), &(in->mup_y));

		cpy_zxy_long_double(dim, &(out->muion), &(in->muion));

	//Auger
		cpy_zxy_long_double(dim, &(out->Cn), &(in->Cn));
		cpy_zxy_long_double(dim, &(out->Cp), &(in->Cp));
		out->auger_enabled=in->auger_enabled;

	//SS SRH
		out->ss_srh_enabled=in->ss_srh_enabled;
		cpy_zxy_long_double(dim, &(out->n1), &(in->n1));
		cpy_zxy_long_double(dim, &(out->p1), &(in->p1));
		cpy_zxy_long_double(dim, &(out->tau_n), &(in->tau_n));
		cpy_zxy_long_double(dim, &(out->tau_p), &(in->tau_p));

	//Electrostatics
		cpy_zxy_long_double(dim, &(out->epsilonr), &(in->epsilonr));
		cpy_zxy_long_double(dim, &(out->epsilonr_e0), &(in->epsilonr_e0));

		cpy_zxy_long_double(dim, &(out->phi_save), &(in->phi_save));

	//Temperature
		cpy_zxy_long_double(dim, &(out->Tl), &(in->Tl));
		cpy_zxy_long_double(dim, &(out->Te), &(in->Te));
		cpy_zxy_long_double(dim, &(out->Th), &(in->Th));

		cpy_zxy_long_double(dim, &(out->ke), &(in->ke));
		cpy_zxy_long_double(dim, &(out->kh), &(in->kh));

	//Heating
		cpy_zxy_long_double(dim, &(out->Hl), &(in->Hl));
		cpy_zxy_long_double(dim, &(out->H_recombination), &(in->H_recombination));
		cpy_zxy_long_double(dim, &(out->H_joule), &(in->H_joule));

		cpy_zxy_long_double(dim, &(out->He), &(in->He));
		cpy_zxy_long_double(dim, &(out->Hh), &(in->Hh));

	//Current
		cpy_zxy_long_double(dim, &(out->Jn), &(in->Jn));
		cpy_zxy_long_double(dim, &(out->Jp), &(in->Jp));
		cpy_zxy_long_double(dim, &(out->Jn_x), &(in->Jn_x));
		cpy_zxy_long_double(dim, &(out->Jp_x), &(in->Jp_x));

		cpy_zxy_long_double(dim, &(out->Jn_diffusion), &(in->Jn_diffusion));
		cpy_zxy_long_double(dim, &(out->Jn_drift), &(in->Jn_drift));

		cpy_zxy_long_double(dim, &(out->Jn_x_diffusion), &(in->Jn_x_diffusion));
		cpy_zxy_long_double(dim, &(out->Jn_x_drift), &(in->Jn_x_drift));

		cpy_zxy_long_double(dim, &(out->Jp_diffusion), &(in->Jp_diffusion));
		cpy_zxy_long_double(dim, &(out->Jp_drift), &(in->Jp_drift));

		cpy_zxy_long_double(dim, &(out->Jp_x_diffusion), &(in->Jp_x_diffusion));
		cpy_zxy_long_double(dim, &(out->Jp_x_drift), &(in->Jp_x_drift));

		cpy_zxy_long_double(dim, &(out->Jion), &(in->Jion));

	//Applied voltages
		cpy_zx_long_double(dim, &(out->Vapplied_y0),&(in->Vapplied_y0));
		cpy_zx_long_double(dim, &(out->Vapplied_y1),&(in->Vapplied_y1));

		cpy_zy_long_double(dim, &(out->Vapplied_x0),&(in->Vapplied_x0));
		cpy_zy_long_double(dim, &(out->Vapplied_x1),&(in->Vapplied_x1));

		out->Vbi= in->Vbi;

	//Passivation
		cpy_zx_int(dim, &(out->passivate_y0),&(in->passivate_y0));
		cpy_zx_int(dim, &(out->passivate_y1),&(in->passivate_y1));

		cpy_zy_int(dim, &(out->passivate_x0),&(in->passivate_x0));
		cpy_zy_int(dim, &(out->passivate_x1),&(in->passivate_x1));

	//Emission
		cpy_zxy_long_double(dim, &(out->Photon_gen), &(in->Photon_gen));

	//Device layout
		cpy_zxy_int(dim,&(out->imat_epitaxy),&(in->imat_epitaxy));
		cpy_zxy_int(dim,&(out->mask),&(in->mask));

		dim_alloc_zx_epitaxy(&(out->dim_epitaxy),in);
		cpy_zx_epitaxy_int(&(in->dim_epitaxy), &(out->mask_epitaxy),&(in->mask_epitaxy));

	//Trap control
		out->ntrapnewton=in->ntrapnewton;
		out->ptrapnewton= in->ptrapnewton;
		out->srh_sim= in->srh_sim;

	//Traps 3d n
		cpy_zxy_long_double(dim,&out->nt_all,&in->nt_all);
		cpy_zxy_long_double(dim,&out->tt,&in->tt);

		cpy_zxy_long_double(dim,&out->nt_save,&in->nt_save);
		cpy_zxy_long_double(dim,&out->pt_save,&in->pt_save);

	//Traps 3d p
		cpy_zxy_long_double(dim,&out->pt_all,&in->pt_all);
		cpy_zxy_long_double(dim,&out->tpt,&in->tpt);

		cpy_zxy_long_double(dim,&out->ntequlib,&in->ntequlib);
		cpy_zxy_long_double(dim,&out->ptequlib,&in->ptequlib);

	//Traps 4d n
		cpy_srh_long_double(dim, &out->nt, &in->nt);
		cpy_srh_long_double(dim, &out->ntlast, &in->ntlast);
		cpy_srh_long_double(dim, &out->dnt, &in->dnt);
		cpy_srh_long_double(dim, &out->srh_n_r1, &in->srh_n_r1);
		cpy_srh_long_double(dim, &out->srh_n_r2, &in->srh_n_r2);
		cpy_srh_long_double(dim, &out->srh_n_r3, &in->srh_n_r3);
		cpy_srh_long_double(dim, &out->srh_n_r4, &in->srh_n_r4);
		cpy_srh_long_double(dim, &out->dsrh_n_r1, &in->dsrh_n_r1);
		cpy_srh_long_double(dim, &out->dsrh_n_r2, &in->dsrh_n_r2);
		cpy_srh_long_double(dim, &out->dsrh_n_r3, &in->dsrh_n_r3);
		cpy_srh_long_double(dim, &out->dsrh_n_r4, &in->dsrh_n_r4);
		cpy_srh_long_double(dim, &out->Fnt, &in->Fnt);

		cpy_srh_long_double(dim, &out->nt_r1, &in->nt_r1);
		cpy_srh_long_double(dim, &out->nt_r2, &in->nt_r2);
		cpy_srh_long_double(dim, &out->nt_r3, &in->nt_r3);
		cpy_srh_long_double(dim, &out->nt_r4, &in->nt_r4);

		cpy_srh_long_double(dim, &out->ntb_save, &in->ntb_save);

	//Traps 4d p
		cpy_srh_long_double(dim, &out->pt, &in->pt);
		cpy_srh_long_double(dim, &out->ptlast, &in->ptlast);
		cpy_srh_long_double(dim, &out->dpt, &in->dpt);
		cpy_srh_long_double(dim, &out->srh_p_r1, &in->srh_p_r1);
		cpy_srh_long_double(dim, &out->srh_p_r2, &in->srh_p_r2);
		cpy_srh_long_double(dim, &out->srh_p_r3, &in->srh_p_r3);
		cpy_srh_long_double(dim, &out->srh_p_r4, &in->srh_p_r4);
		cpy_srh_long_double(dim, &out->dsrh_p_r1, &in->dsrh_p_r1);
		cpy_srh_long_double(dim, &out->dsrh_p_r2, &in->dsrh_p_r2);
		cpy_srh_long_double(dim, &out->dsrh_p_r3, &in->dsrh_p_r3);
		cpy_srh_long_double(dim, &out->dsrh_p_r4, &in->dsrh_p_r4);
		cpy_srh_long_double(dim, &out->Fpt, &in->Fpt);


		cpy_srh_long_double(dim, &out->pt_r1, &in->pt_r1);
		cpy_srh_long_double(dim, &out->pt_r2, &in->pt_r2);
		cpy_srh_long_double(dim, &out->pt_r3, &in->pt_r3);
		cpy_srh_long_double(dim, &out->pt_r4, &in->pt_r4);

		cpy_srh_long_double(dim, &out->ptb_save, &in->ptb_save);


	//Newton solver control
		out->max_electrical_itt= in->max_electrical_itt;
		out->electrical_clamp= in->electrical_clamp;
		out->min_cur_error= in->min_cur_error;

		out->max_electrical_itt0= in->max_electrical_itt0;
		out->electrical_clamp0= in->electrical_clamp0;
		out->electrical_error0= in->electrical_error0;

		out->math_enable_pos_solver= in->math_enable_pos_solver;

		strcpy(out->newton_name,in->newton_name);

		out->kl_in_newton=in->kl_in_newton;
		out->config_kl_in_newton=in->config_kl_in_newton;
		out->newton_aux=in->newton_aux;

		out->newton_clever_exit= in->newton_clever_exit;

		out->newton_only_fill_matrix=in->newton_only_fill_matrix;
		out->omega=in->omega;

		out->newton_min_itt= in->newton_min_itt;

		out->newton_last_ittr=in->newton_last_ittr;

	//Arrays used by newton solver
		cpy_1d_alloc((void**)&(out->newton_dntrap), (void**)&(in->newton_dntrap), dim->srh_bands, sizeof(long double));
		cpy_1d_alloc((void**)&(out->newton_dntrapdntrap), (void**)&(in->newton_dntrapdntrap), dim->srh_bands, sizeof(long double));
		cpy_1d_alloc((void**)&(out->newton_dntrapdn), (void**)&(in->newton_dntrapdn), dim->srh_bands, sizeof(long double));
		cpy_1d_alloc((void**)&(out->newton_dntrapdp), (void**)&(in->newton_dntrapdp), dim->srh_bands, sizeof(long double));
		cpy_1d_alloc((void**)&(out->newton_dJdtrapn), (void**)&(in->newton_dJdtrapn), dim->srh_bands, sizeof(long double));
		cpy_1d_alloc((void**)&(out->newton_dJpdtrapn), (void**)&(in->newton_dJpdtrapn), dim->srh_bands, sizeof(long double));

		cpy_1d_alloc((void**)&(out->newton_dptrapdp), (void**)&(in->newton_dptrapdp), dim->srh_bands, sizeof(long double));
		cpy_1d_alloc((void**)&(out->newton_dptrapdptrap), (void**)&(in->newton_dptrapdptrap), dim->srh_bands, sizeof(long double));
		cpy_1d_alloc((void**)&(out->newton_dptrap), (void**)&(in->newton_dptrap), dim->srh_bands, sizeof(long double));
		cpy_1d_alloc((void**)&(out->newton_dptrapdn), (void**)&(in->newton_dptrapdn), dim->srh_bands, sizeof(long double));
		cpy_1d_alloc((void**)&(out->newton_dJpdtrapp), (void**)&(in->newton_dJpdtrapp), dim->srh_bands, sizeof(long double));
		cpy_1d_alloc((void**)&(out->newton_dJpdtrapp_interface_right), (void**)&(in->newton_dJpdtrapp_interface_right), dim->srh_bands, sizeof(long double));
		cpy_1d_alloc((void**)&(out->newton_dJdtrapp), (void**)&(in->newton_dJdtrapp), dim->srh_bands, sizeof(long double));
		cpy_1d_alloc((void**)&(out->newton_dphidntrap), (void**)&(in->newton_dphidntrap), dim->srh_bands, sizeof(long double));
		cpy_1d_alloc((void**)&(out->newton_dphidptrap), (void**)&(in->newton_dphidptrap), dim->srh_bands, sizeof(long double));
		cpy_1d_alloc((void**)&(out->newton_ntlast), (void**)&(in->newton_ntlast), dim->srh_bands, sizeof(long double));
		cpy_1d_alloc((void**)&(out->newton_ptlast), (void**)&(in->newton_ptlast), dim->srh_bands, sizeof(long double));

	//Electrical components
		out->Rshunt= in->Rshunt;
		out->Rcontact= in->Rcontact;
		out->Rload= in->Rload;
		out->L= in->L;
		out->C= in->C;
		out->Rshort= in->Rshort;
		out->other_layers= in->other_layers;
		out->contact_charge= in->contact_charge;

	//Dump contorl
		out->dump_dynamic_pl_energy=in->dump_dynamic_pl_energy;

		out->snapshot_number=in->snapshot_number;

		out->map_start=in->map_start;
		out->map_stop=in->map_stop;

		out->timedumpcount=in->timedumpcount;

		strcpy(out->plot_file,in->plot_file);

	//Time
		out->go_time=in->go_time;
		out->dt=in->dt;
		out->time=in->time;
		out->Ilast= in->Ilast;
		out->start_stop_time=in->start_stop_time;

	//Run control
		out->stop=in->stop;
		out->onlypos=in->onlypos;
		out->odes=in->odes;
		out->stop_start=in->stop_start;
		out->dd_conv=in->dd_conv;
		out->high_voltage_limit=in->high_voltage_limit;
		out->stoppoint= in->stoppoint;
		out->drift_diffision_simulations_enabled=in->drift_diffision_simulations_enabled;
		out->electrical_simulation_enabled=in->electrical_simulation_enabled;
	//Matrix
		//printf("I'm doing the matrix copy!!\n");
		matrix_cpy(sim,&(out->mx),&(in->mx));
		//printf("I'm doing the matrix copy!! %p\n",out->mx.Ti);

	//mesh data	-- as long as we don't remesh we don't need to copy this 
		mesh_obj_init(&(out->mesh_data));
		mesh_obj_init(&(out->mesh_data_save));
		for (i=0;i<100;i++)
		{
			out->layer_start[i]=in->layer_start[i];
			out->layer_stop[i]=in->layer_stop[i];
		}
	//epitaxy
		epitaxy_cpy(sim,&(out->my_epitaxy),&(in->my_epitaxy));

	//plugins
		strcpy(out->simmode,in->simmode);

	//Newton states
		//struct newton_state *ns=&(in->ns);

		newton_state_cpy(&(out->ns),&(in->ns));
		newton_state_cpy(&(out->ns_save),&(in->ns_save));

	//Light
		light_cpy_memory(sim,&(out->mylight),&(in->mylight));
//		light_init(sim,&(out->mylight));	//we are going to try to not to copy the light structure
		light_init(sim,&(out->probe_modes));
		inter_init(sim,&(out->steady_stark));

		light_srcs_cpy(sim,&(out->lights),&(in->lights));

	//Emission
		out->emission_enabled=in->emission_enabled;
		out->pl_intensity= in->pl_intensity;
		out->pl_intensity_tot= in->pl_intensity_tot;
		out->pl_use_experimental_emission_spectra=in->pl_use_experimental_emission_spectra;

	//thermal
		#ifdef libheat_enabled
			heat_cpy(sim,&(out->thermal), &(in->thermal));
		#endif

	//exciton
		#ifdef libexciton_enabled
			exciton_cpy(sim,&(out->ex), &(in->ex));
		#endif

	//Preovskite
		#ifdef libperovskite_enabled
			perovskite_cpy(sim,&(out->mobileion),&(in->mobileion));
		#endif

	//Circuit
		#ifdef libcircuit_enabled
			out->circuit_simulation=in->circuit_simulation;
			circuit_cpy(sim,&(out->cir),&(in->cir));
		#endif

	//Ray tracing						
		ray_engine_init(&(out->my_image));	//We are not going to copy this
		shape_init(sim,&(out->big_box));	//We are not going to copy this

	//Contacts
		contacts_cpy(sim,out,in);
		out->ncontacts=in->ncontacts;
		out->active_contact=in->active_contact;
		out->boundry_y0=in->boundry_y0;

		out->flip_current=in->flip_current;

	//Objects
		cpy_zxy_p_object(dim, &(out->obj_zxy), &(in->obj_zxy));
		cpy_zx_layer_p_object(&(in->dim_epitaxy), &(out->obj_zx_layer), &(in->obj_zx_layer));


	//Time mesh
		time_mesh_cpy(&(out->tm),&(in->tm));

	//solver cache
		strcpy(out->solver_type,in->solver_type);
		device_cache_cpy(&(out->cache),&(in->cache));

	//fxdomain
		#ifdef libfxdomain_enabled
			fxdomain_cpy(sim,&(out->fxdomain_config),&(in->fxdomain_config));
		#endif

	//matrix solver memroy
		matrix_solver_memory_init(&(out->msm));
		matrix_solver_memory_load_dll(sim,&(out->msm));

	//json
		json_cpy(sim,&(out->config),&(in->config));

	//temp vars
		out->glob_wanted=in->glob_wanted;

	//paths
		strcpy(out->input_path,in->input_path);
		strcpy(out->output_path,in->output_path);

	//Newton solver dll
		out->dll_solve_cur=in->dll_solve_cur;
		out->dll_solver_realloc=in->dll_solver_realloc;
		out->dll_solver_free_memory=in->dll_solver_free_memory;
		out->dll_solver_handle=in->dll_solver_handle;
		out->solver_verbosity=in->solver_verbosity;

	//world
		world_cpy(sim,&(out->w),&(in->w));
}


