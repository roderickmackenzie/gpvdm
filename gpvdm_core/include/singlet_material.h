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

/** @file singlet_material.h
@brief a structure for the singlet model
*/

#ifndef h_singlet_material
#define h_singlet_material

struct singlet_material
{
	long double singlet_k_fret;
	long double singlet_k_s;
	long double singlet_k_isc;
	long double singlet_k_ss;
	long double singlet_k_sp;
	long double singlet_k_st;
	long double singlet_k_dext;
	long double singlet_k_t;
	long double singlet_k_tp;
	long double singlet_k_tt;
	long double singlet_k_sd;
	long double singlet_k_iscd;
	long double singlet_k_spd;
	long double singlet_k_std;
	long double singlet_k_ssd;
	long double singlet_k_td;
	long double singlet_k_ttd;
	long double singlet_k_tpd;
	long double singlet_gamma;
	long double singlet_zeta;
	long double singlet_k_cav;
	long double singlet_beta_sp;
	long double singlet_C;
	long double singlet_N_dop;
	long double singlet_W;
};

#endif
