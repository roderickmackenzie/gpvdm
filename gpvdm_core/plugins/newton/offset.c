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
	//int contact_right=dev->contacts[dev->n_contact_y1[0][0]].type;
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

//singlet offset
int get_offset_singlet_Ns(struct device *dev)
{
	int offset=0;
	struct newton_state *ns=&(dev->ns);
	struct dimensions *dim=&(ns->dim);
	struct perovskite *ion=&(dev->mobileion);

	offset=get_offset_nion(dev);

	if (ion->full_newton==TRUE)
	{
		if (ns->Nion_enabled==TRUE)
		{
			offset+=ion->ylen;
		}
	}

	return offset;
}

int get_offset_singlet_Nt(struct device *dev)
{
	int offset=0;
	struct newton_state *ns=&(dev->ns);
	struct dimensions *dim=&(ns->dim);
	struct singlet *sing=&(dev->sing);

	offset=get_offset_singlet_Ns(dev);

	if (sing->full_newton==TRUE)
	{
		if (ns->singlet_enabled==TRUE)
		{
			offset+=dim->ylen;
		}
	}

	return offset;
}

int get_offset_singlet_Nsd(struct device *dev)
{
	int offset=0;
	struct newton_state *ns=&(dev->ns);
	struct dimensions *dim=&(ns->dim);
	struct singlet *sing=&(dev->sing);

	offset=get_offset_singlet_Nt(dev);

	if (sing->full_newton==TRUE)
	{
		if (ns->singlet_enabled==TRUE)
		{
			offset+=dim->ylen;
		}
	}

	return offset;
}

int get_offset_singlet_Ntd(struct device *dev)
{
	int offset=0;
	struct newton_state *ns=&(dev->ns);
	struct dimensions *dim=&(ns->dim);
	struct singlet *sing=&(dev->sing);

	offset=get_offset_singlet_Nsd(dev);

	if (sing->full_newton==TRUE)
	{
		if (ns->singlet_enabled==TRUE)
		{
			offset+=dim->ylen;
		}
	}

	return offset;
}

int get_offset_singlet_Nho(struct device *dev)
{
	int offset=0;
	struct newton_state *ns=&(dev->ns);
	struct dimensions *dim=&(ns->dim);
	struct singlet *sing=&(dev->sing);

	offset=get_offset_singlet_Ntd(dev);

	if (sing->full_newton==TRUE)
	{
		if (ns->singlet_enabled==TRUE)
		{
			offset+=dim->ylen;
		}
	}

	return offset;
}
