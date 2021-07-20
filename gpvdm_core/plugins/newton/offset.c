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

/** @file newton.c
	@brief Standard newton solver.
*/


#include <string.h>
#include <log.h>
#include <gpvdm_const.h>
#include "newton.h"
#include <dll_export.h>
#include <util.h>
#include <exp.h>
#include <advmath.h>
#include <dump.h>
#include <cal_path.h>
#include <dos.h>
#include <sim.h>
#include <solver_interface.h>
#include <contacts.h>
#include <memory.h>
#include <newton_tricks.h>
#include <timer.h>
#include <perovskite.h>


int get_offset_Je(struct device *dev)
{
	int offset=0;
	struct newton_state *ns=&(dev->ns);
	struct dimensions *dim=&(ns->dim);
	int contact_left=dev->contacts[dev->n_contact_y0[0][0]].type;

	offset=dim->ylen;

	if (contact_left==contact_schottky)
	{
		offset++;
	}

	return offset;
}

int get_offset_Jh(struct device *dev)
{
	int offset=0;
	struct newton_state *ns=&(dev->ns);
	struct dimensions *dim=&(ns->dim);
	int contact_right=dev->contacts[dev->n_contact_y1[0][0]].type;
	offset=get_offset_Je(dev);
	offset+=dim->ylen;

	/*if (contact_right==contact_schottky)
	{
		offset++;
	}*/

	return offset;
}

int get_offset_srh_e(struct device *dev)
{
	int offset=0;
	struct newton_state *ns=&(dev->ns);
	struct dimensions *dim=&(ns->dim);

	offset=get_offset_Jh(dev);
	offset+=dim->ylen;

	return offset;
}

int get_offset_srh_h(struct device *dev)
{
	int offset=0;
	struct newton_state *ns=&(dev->ns);
	struct dimensions *dim=&(ns->dim);

	offset=get_offset_srh_e(dev);

	if (dev->ntrapnewton==TRUE)
	{
		offset+=dim->ylen*dim->srh_bands;	//srh electron
	}

	return offset;
}

int get_offset_kcl(struct device *dev)
{
	int offset=0;
	struct newton_state *ns=&(dev->ns);
	struct dimensions *dim=&(ns->dim);

	offset=get_offset_srh_h(dev);

	if (dev->ptrapnewton==TRUE)
	{
		offset+=dim->ylen*dim->srh_bands;	//srh hole
	}

	return offset;
}

int get_offset_nion(struct device *dev)
{
	int offset=0;

	offset=get_offset_kcl(dev);

	if (dev->kl_in_newton==TRUE)
	{
		offset++;
	}

	return offset;
}
