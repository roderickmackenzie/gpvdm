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

/** @file dos_ion.c
	@brief Perovskite calculations
*/

#include <enabled_libs.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>

	#include <zlib.h>
#include "code_ctrl.h"
#include "server.h"
#include "sim.h"
#include "dump.h"
#include "lang.h"
#include "log.h"
#include "cal_path.h"
#include "util.h"
#include <memory.h>



long double get_top_from_ion(long double n)
{
long double ret=0.0;
long double T=300.0;
	ret=(kb*T/Qe)*log((fabs(n))/1.0000e30);

return ret;
}


void get_ion_den(long double top,long double *n, long double *dn)
{
	long double T=300.0;
	long double Tkb=T*kb;
	*n=1.0000e30*gexp((Qe*top)/(Tkb));

	if (dn!=NULL)
	{
		*dn=(Qe/(Tkb))*(*n);
	}


}

long double get_top_from_singlet(long double n)
{
long double ret=0.0;
long double T=300.0;
	ret=(kb*T/Qe)*log((fabs(n))/1.0000e30);

return ret;
}


void get_singlet_den(long double top,long double *n, long double *dn)
{
	long double T=300.0;
	long double Tkb=T*kb;
	*n=1.0000e30*gexp((Qe*top)/(Tkb));

	if (dn!=NULL)
	{
		*dn=(Qe/(Tkb))*(*n);
	}


}
