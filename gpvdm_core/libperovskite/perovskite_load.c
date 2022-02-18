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

/** @file perovskite.c
	@brief A perovskite ion solver.
*/
#include <enabled_libs.h>
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
#include <perovskite.h>
#include <inp.h>
#include <plot.h>
#include <memory.h>
#include <json.h>

void perovskite_load(struct simulation *sim,struct device *in)
{
	int i=0;
	int z=0;
	int x=0;


	struct dimensions *dim=&in->ns.dim;
	struct newton_state *ns=&(in->ns);
	struct matrix *mx=&(in->mobileion.mx);

	if (ns->Nion_enabled==FALSE)
	{
		return;
	}

	struct json_obj *json_perovskite;

	if (in->drift_diffision_simulations_enabled==FALSE)
	{
		return;
	}

	json_perovskite=json_obj_find(&(in->config.obj),"perovskite");
	if (json_perovskite==NULL)
	{
		in->mobileion.perovskite_enabled=FALSE;
		return;
	}

	json_get_long_double(sim,json_perovskite, &(in->mobileion.perovskite_dt),"perovskite_dt");
	json_get_int(sim,json_perovskite, &(in->mobileion.perovskite_steps),"perovskite_steps");
	json_get_english(sim,json_perovskite, &(in->mobileion.perovskite_enabled),"perovskite_enabled");


	in->mobileion.ystart=-1;
	in->mobileion.ystop=dim->ylen;
	in->mobileion.ylen=dim->ylen;


	for (i=0;i<dim->ylen;i++)
	{
		if ((in->mobileion.ystart==-1)&&(in->Nion[z][x][i]!=0))
		{
			in->mobileion.ystart=i;
		}

		if (i>0)
		{
			if ((in->Nion[z][x][i]==0)&&(in->Nion[z][x][i-1]!=0))
			{
				in->mobileion.ystop=i;
			}
		}

		//printf("%d %Le\n",i,in->Nion[z][x][i]);
	}

	//printf("%d %d\n",in->mobileion.ystart,in->mobileion.ystop);
	//getchar();
	in->mobileion.ylen=in->mobileion.ystop-in->mobileion.ystart;

	if (in->mobileion.ystart==-1)
	{
		in->mobileion.perovskite_enabled=FALSE;
		ns->Nion_enabled=FALSE;
	}

	if (in->mobileion.perovskite_enabled==TRUE)
	{
		in->mobileion.full_newton=TRUE;
		
		if (in->mobileion.full_newton==FALSE)
		{
			matrix_init(mx);
			mx->nz=in->mobileion.ylen*3-2;
			mx->M=in->mobileion.ylen;
			matrix_malloc(sim,mx);
		}
	}

//perovskite_dump(sim,in);
//getchar();
}

