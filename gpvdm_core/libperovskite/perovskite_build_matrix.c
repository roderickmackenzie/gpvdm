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

/** @file perovskite_build_matrix.c
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
#include <perovskite.h>
#include <matrix.h>

void perovskite_build_matrix(struct simulation *sim,struct device *in,struct perovskite *config, int z, int x)
{
struct matrix *mx=&(config->mx);

int col=mx->M;
int nz=mx->nz;
int *Ti=mx->Ti;
int *Tj=mx->Tj;
long double *Tx=mx->Tx;
long double *b=mx->b;

int pos=0;

long double Tl=0.0;
long double Tc=0.0;
long double Tr=0.0;
long double Dpl=0.0;
long double Dpc=0.0;
long double Dpr=0.0;

long double mul=0.0;
long double muc=0.0;
long double mur=0.0;

long double xipl=0.0;
long double xipr=0.0;

long double phil=0.0;
long double phic=0.0;
long double phir=0.0;

long double pl=0.0;
long double pc=0.0;
long double pr=0.0;

long double Jpl=0.0;
long double Jpr=0.0;

long double build=0.0;

long double dylh=0.0;
long double dyrh=0.0;

long double dJpl;
long double dJpc;
long double dJpr;

long double dJpldpl=0.0;
long double dJpldpc=0.0;
long double dJpldpr=0.0;

long double dJprdpl=0.0;
long double dJprdpc=0.0;
long double dJprdpr=0.0;

long double yl;
long double yc;
long double yr;
long double dyl;
long double dyr;
long double dyc=0.0;

long double dt=in->dt;//config->perovskite_dt;

long double pc_last=0.0;

int i=0;
struct newton_state *ns=&(in->ns);
struct dimensions *dim=&in->ns.dim;
	pos=0;

	int ii=0;
	for (i=config->ystart;i<config->ystop;i++)
	{


		if (i==config->ystart)
		{
			phil=ns->phi[z][x][i];
			yl=dim->y[i]-(dim->y[i+1]-dim->y[i]);
			Tl=in->Tl[z][x][i];
			mul=in->muion[z][x][i];
			pl=in->Nion[z][x][i];//1.0;//0.0;//ns->Nion[z][x][0];
			//printf("start\n");
			//getchar();
		}else
		{
			phil=ns->phi[z][x][i-1];
			yl=dim->y[i-1];
			Tl=in->Tl[z][x][i-1];
			mul=in->muion[z][x][i-1];
			pl=in->Nion[z][x][i-1];
		}

		if (i==(config->ystop-1))
		{
			phir=ns->phi[z][x][i];
			yr=dim->y[i]+(dim->y[i]-dim->y[i-1]);
			Tr=in->Tl[z][x][i];
			mur=in->muion[z][x][i];
			pr=1.0;
			//printf("stop\n");
			//getchar();
		}else
		{
			phir=ns->phi[z][x][i+1];
			yr=dim->y[i+1];
			Tr=in->Tl[z][x][i+1];
			mur=in->muion[z][x][i+1];
			pr=in->Nion[z][x][i+1];
		}


		yc=dim->y[i];
		dyl=yc-yl;
		dyr=yr-yc;
		dyc=(dyl+dyr)/2.0;
		Tc=in->Tl[z][x][i];

		dylh=dyl/2.0;
		dyrh=dyr/2.0;

		muc=in->muion[z][x][i];
		phic=ns->phi[z][x][i];
		pc=in->Nion[z][x][i];

		pc_last=in->Nion_last[z][x][i];

		Dpl=((kb*Tl)/Qe)*mul;
		Dpc=((kb*Tc)/Qe)*muc;
		Dpr=((kb*Tr)/Qe)*mur;

		Tl=(Tl+Tc)/2.0;
		Tr=(Tr+Tc)/2.0;

		Dpl=(Dpl+Dpc)/2.0;
		Dpr=(Dpr+Dpc)/2.0;

		/*if (i==(dim->ylen-1))
		{
			Dpr*=0.0;
		}
		if (i==0)
		{
			Dpl*=0.0;
		}*/

		xipl=-Qe*(phic-phil)/(Tl*kb);
		xipr=-Qe*(phir-phic)/(Tr*kb);

		Jpl=(Dpl/dyl)*(B(-xipl)*pl-B(xipl)*pc);

		dJpldpl=(Dpl/dyl)*(B(-xipl));
		dJpldpc=(Dpl/dyl)*(-B(xipl));
		dJpldpr=0.0;

		if (i==config->ystart)
		{
			Jpl=0.0;
			//dJpldpl=0.0;
			//dJpldpc=0.0;
			dJpldpc+=dJpldpl;
			dJpldpl=0.0;
		}

		Jpr=(Dpr/dyr)*(B(-xipr)*pc-B(xipr)*pr);

		dJprdpl=0.0;
		dJprdpc=(Dpr/dyr)*(B(-xipr));
		dJprdpr=(Dpr/dyr)*(-B(xipr));


		if (i==(config->ystop-1))
		{
			Jpr=0.0;//(Dpr/dyr)*(B(-xipr)*pc-B(xipr)*pr);

			//dJprdpl=0.0;
			dJprdpc+=dJprdpr;//(Dpr/dyr)*(B(-xipr));
			dJprdpr=0.0;//(Dpr/dyr)*(-B(xipr));
		}

		build=0.0;
		build= (Jpr-Jpl)/(dylh+dyrh);

		if (in->go_time==TRUE)
		{
			build+= (pc-pc_last)/dt;
		}

		dJpl= (dJprdpl-dJpldpl)/(dylh+dyrh);
		dJpc= (dJprdpc-dJpldpc)/(dylh+dyrh);
		dJpr= (dJprdpr-dJpldpr)/(dylh+dyrh);

		if (in->go_time==TRUE)
		{
			dJpc+=(1.0)/dt;
		}

		b[ii]=-build;

		//printf("%d %Le %Le %Le %Le %Le\n",ii,build,dJpl,dJpc,dJpr,build);
		//getchar();

		if (i!=config->ystart)
		{
			Ti[pos]=ii;
			Tj[pos]=ii-1;
			Tx[pos]=dJpl;
			pos++;
		}

		Ti[pos]=ii;
		Tj[pos]=ii;
		Tx[pos]=dJpc;

		pos++;

		if (i!=(config->ystop-1))
		{
			Ti[pos]=ii;
			Tj[pos]=ii+1;
			Tx[pos]=dJpr;
			pos++;

		}

		ii++;

	}
}
