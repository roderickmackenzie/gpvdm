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

/** @file contacts_free.c
@brief backend to handle complex contacts
*/

#include <string.h>
#include "epitaxy.h"
#include "inp.h"
#include "util.h"
#include "gpvdm_const.h"
#include <cal_path.h>
#include "contacts.h"
#include <log.h>
#include <dump.h>
#include <shape.h>
#include <shape_struct.h>


void contacts_cpy(struct simulation *sim,struct device *out,struct device *in)
{
	int i;

	if (in->contacts==NULL)
	{
		return;
	}

	out->contacts=(struct contact *)malloc(in->ncontacts*sizeof(struct contact));

	out->ncontacts=in->ncontacts;

	for (i=0;i<in->ncontacts;i++)
	{
		contact_cpy(sim,&(out->contacts[i]),&(in->contacts[i]));
	}

}

