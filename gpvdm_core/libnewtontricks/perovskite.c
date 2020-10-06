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

/** @file perovskite.c
	@brief A perovskite ion solver.
*/

#include <string.h>
#include <stdlib.h>
#include <dump.h>
#include <dos.h>
#include "sim.h"
#include "solver_interface.h"
#include "dat_file.h"
#include "log.h"
#include <cal_path.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <lang.h>
//<strip>
#include <perovskite.h>
//</strip>
#include <inp.h>
#include <memory.h>
#include <newton_tricks.h>
#include <enabled_libs.h>

long double newton_externalv_simple_perovskite(struct simulation *sim,struct device *in,gdouble V)
{
	long double i0;
	//<strip>
	i0=newton_externalv_simple(sim,in,V);
	#ifdef libperovskite_enabled
		solve_perovskite(sim,in, 0, 0);
	#endif
	//</strip>
	return i0;
}

long double newton_externv_perovskite(struct simulation *sim,struct device *in,gdouble Vtot,int usecap)
{
	int i=0;
	int ii=0;
	long double i0;
	long double i0_last=1000.0;
	long double error=0.0;
	long double first_error=0.0;
	for (i==0;ii<10;ii++)
	{
		for (i=0;i<3;i++)
		{
			i0=newton_externv(sim,in,Vtot);
			//<strip>
			#ifdef libperovskite_enabled
				solve_perovskite(sim,in, 0, 0);
			#endif
			//</strip>
			error=fabsl(i0-i0_last);
			//			printf_log(sim,"%s %Le %d\n",_("Perovskite ion"),error,ii);printf_log(sim,"%s %Le %d\n",_("Perovskite ion"),error,ii);

			if (error<1e-6)
			{
				break;
			}

			i0_last=i0;
		}
		//getchar();
	}

	printf_log(sim,"%s %Le\n",_("Electrical+perovskite solver f(x)="),error);
	return i0;
}



