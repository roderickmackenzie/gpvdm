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

/** @file matrix.c
@brief A struct for the matrix solver
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lang.h>
#include "sim.h"
#include "dump.h"
#include "mesh.h"
#include <math.h>
#include "log.h"
#include <solver_interface.h>
#include "memory.h"
#include "md5.h"
#include "cal_path.h"
#include <timer.h>

void matrix_free(struct simulation *sim,struct matrix *mx)
{
	int y;

	if (mx->Ti!=NULL)
	{
		free(mx->Ti);
		mx->Ti=NULL;
	}

	if (mx->Tj!=NULL)
	{
		free(mx->Tj);
		mx->Tj=NULL;
	}

	if (mx->Tx!=NULL)
	{
		free(mx->Tx);
		mx->Tx=NULL;
	}

	if (mx->b!=NULL)
	{
		free(mx->b);
		mx->b=NULL;
	}

	if (mx->Txz!=NULL)
	{
		free(mx->Txz);
		mx->Txz=NULL;
	}

	if (mx->bz!=NULL)
	{
		free(mx->bz);
		mx->bz=NULL;
	}

	if (mx->msort!=NULL)
	{
		free(mx->msort);
		mx->msort=NULL;
	}

	matrix_init(mx);
}
