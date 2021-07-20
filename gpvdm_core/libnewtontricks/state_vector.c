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

/** @file ntricks.c
	@brief A collection of helper functions for the newton solver.
*/

//<stripall>

#include <stdio.h>
#include "sim.h"
#include "dump.h"
#include "newton_tricks.h"
#include <cal_path.h>
#include <contacts.h>
#include <dump.h>
#include <log.h>
#include <string.h>
#include <stdlib.h>
#include <inp.h>
#include <list.h>
#include <advmath.h>

static int unused __attribute__((unused));

/*void state_gen_vector(struct simulation *sim,struct device *in)
{
	int i;
	long double *buf;
	int len=0;
	int buf_pos=0;
	struct dimensions *dim=&in->ns.dim;
	struct epitaxy *epi =&(in->my_epitaxy);
	char cache_vector[PATH_MAX];
	join_path(3, cache_vector,get_cache_path(sim),in->cache.hash,"vector.dat");

	len+=5;
	len+=21*in->my_epitaxy.electrical_layers;
	len+=2*in->ncontacts;

	buf=malloc(sizeof(long double)*len);

	buf[buf_pos++]=dim->zlen;					//1
	buf[buf_pos++]=dim->xlen;					//2
	buf[buf_pos++]=dim->ylen;					//3
	buf[buf_pos++]=dim->srh_bands;					//4
	buf[buf_pos++]=epi->electrical_layers;	//5

	for (i=0;i<in->my_epitaxy.electrical_layers;i++)
	{
		buf[buf_pos++]=epi->dosn[i].muz;			//1
		buf[buf_pos++]=epi->dosp[i].muz;			//2

		buf[buf_pos++]=epi->dosn[i].mux;			//3
		buf[buf_pos++]=epi->dosp[i].mux;			//4

		buf[buf_pos++]=epi->dosn[i].muy;			//5
		buf[buf_pos++]=epi->dosp[i].muy;			//6

		buf[buf_pos++]=epi->dosn[i].config.doping_start;	//7
		buf[buf_pos++]=epi->dosn[i].config.doping_stop;	//8

		buf[buf_pos++]=epi->dosn[i].config.srh_sigman;	//9
		buf[buf_pos++]=epi->dosn[i].config.srh_sigmap;	//10
		buf[buf_pos++]=epi->dosp[i].config.srh_sigman;	//11
		buf[buf_pos++]=epi->dosp[i].config.srh_sigmap;	//12

		buf[buf_pos++]=epi->dosn[i].config.Nc;			//13
		buf[buf_pos++]=epi->dosn[i].config.Nv;			//14

		buf[buf_pos++]=epi->dosn[i].config.Eg;			//15
		buf[buf_pos++]=epi->dosn[i].config.Xi;			//16
		buf[buf_pos++]=epi->dosn[i].B;					//17

		buf[buf_pos++]=epi->dosn[i].config.Et;			//18
		buf[buf_pos++]=epi->dosp[i].config.Et;			//19

		buf[buf_pos++]=epi->dosn[i].config.Nt;			//20
		buf[buf_pos++]=epi->dosp[i].config.Nt;			//21
	}


	for (i=0;i<in->ncontacts;i++)
	{
		buf[buf_pos++]=in->contacts[i].np;			//1
		buf[buf_pos++]=in->contacts[i].charge_type;	//2
	}


	write_zip_buffer(sim,cache_vector,buf,len);

	free(buf);
}*/


