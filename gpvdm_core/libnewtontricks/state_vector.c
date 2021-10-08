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


