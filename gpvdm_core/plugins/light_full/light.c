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

/** @file light.c
	@brief Full optical pluugin solver.
*/


#include <stdio.h>
#include <stdlib.h>
#include <util.h>
#include <dump_ctrl.h>
#include <gpvdm_const.h>
#include <light.h>

#include <device.h>
#include <light_interface.h>

#include <functions.h>
#include <log.h>
#include <lang.h>
#include <matrix.h>
#include <memory.h>
#include <triangles.h>
#include <string.h>
//static long double min_light_error=1e-10;

EXPORT void light_dll_ver(struct simulation *sim)
{
        //printf_log(sim,"%s\n",_("Full transfer matrix based light model"));
}


EXPORT int light_dll_solve_lam_slice(struct simulation *sim,struct device *cell,struct light *li,long double *sun_E,int z, int x, int l,int w)
{
	int y;
	long double complex Epl=0.0+0.0*I;
	//long double complex Epc=0.0+0.0*I;
	//long double complex Epr=0.0+0.0*I;

	//long double complex Enl=0.0+0.0*I;
	//long double complex Enc=0.0+0.0*I;
	//long double complex Enr=0.0+0.0*I;

	long double complex rc=0.0+0.0*I;
	long double complex tc=0.0+0.0*I;
	long double complex rr=0.0+0.0*I;
	long double complex tr=0.0+0.0*I;

	long double complex nbar_c=0.0;
	long double complex xi_c=0.0;

	long double complex nbar_r=0.0;
	long double complex xi_r=0.0;

	long double complex rl=0.0+0.0*I;
	long double complex tl=0.0+0.0*I;
	long double complex nbar_l=0.0;
	long double complex xi_l=0.0;

	long double complex fp=0.0;
	long double complex fn=0.0;

	long double dy=0.0;
	//long double nc;

	long double lam=0;

	int ittr=0;
	int pos=0;

	double mul=1.0;

	int quit=FALSE;
	long double test=FALSE;

	long double update;
	struct vec my_vec;
	struct matrix *mx=&(li->mx[w]);
	struct dimensions *dim=&li->dim;

	if (sun_E[l]==0.0)
	{
		return 0;
	}

	lam=dim->l[l];


	dy=dim->y[2]-dim->y[1];

	//printf("%d\n",li->light_profile_tri);
	//getchar();

	if (strcmp(li->light_profile,"box")!=0)
	{
		if ((dim->xlen>1)&&(dim->zlen>1))
		{
			my_vec.x=dim->x[x];
			my_vec.z=dim->z[z];
			mul=triangles_interpolate(&li->light_profile_tri,&my_vec);
			if (mul<0.01)
			{
				return 0;
			}
			mul=sqrt(mul);
		}
	}
	//printf("%le %le %le\n",my_vec.x,my_vec.z,mul);
	//getchar();

	do
	{
		pos=0;

		for (y=0;y<dim->ylen;y++)
		{

			rc=li->r[z][x][y][l];
			tc=li->t[z][x][y][l];


			if (y==0)
			{
				rl=li->r[z][x][y][l];
				tl=li->t[z][x][y][l];

				nbar_l=li->nbar[z][x][y][l];
				Epl=sun_E[l]*mul+0.0*I;

			}else
			{
				rl=li->r[z][x][y-1][l];
				tl=li->t[z][x][y-1][l];

				nbar_l=li->nbar[z][x][y-1][l];
				Epl=0.0;
			}

			if (y==dim->ylen-1)
			{
				rr=li->r[z][x][y][l];
				tr=li->t[z][x][y][l];
				nbar_r=li->nbar[z][x][y][l];
			}else
			{
				rr=li->r[z][x][y+1][l];
				tr=li->t[z][x][y+1][l];
				nbar_r=li->nbar[z][x][y+1][l];
			}


			nbar_c=li->nbar[z][x][y][l];

			//printf("%d %d %e %e\n",l,y,li->n[z][x][y][l],li->alpha[z][x][y][l]);
			xi_l=((2*PI)/lam)*nbar_l;
			xi_c=((2*PI)/lam)*nbar_c;
			xi_r=((2*PI)/lam)*nbar_r;

			long double complex pa=-tl;
			long double complex pbp=cexp(xi_c*dy*I);

			long double complex pbn=rl*cexp(-xi_c*dy*I);

			long double complex na=-tc;//Enc
			long double complex nbp=rc*cexp(xi_r*dy*I);//Enr*
			long double complex nbn=cexp(-xi_r*dy*I);


			//getchar();
			fp=0.0+0.0*I;
			fn=0.0+0.0*I;

			if (y==0)
			{
				fp=Epl;
				pa=0.0+0.0*I;
			}else
			{
				fp=0.0+0.0*I;
			}


			//printf("%d %d\n",pos,mx->nz);
			//getchar();
			if (y!=0)
			{
				mx->Ti[pos]=y;
				mx->Tj[pos]=y-1;
				mx->Tx[pos]=gcreal(pa);
				mx->Txz[pos]=gcimag(pa);
				pos++;
			}

			mx->Ti[pos]=y;
			mx->Tj[pos]=y;
			mx->Tx[pos]=gcreal(pbp);
			mx->Txz[pos]=gcimag(pbp);
			pos++;

			mx->Ti[pos]=y;
			mx->Tj[pos]=dim->ylen+y;
			mx->Tx[pos]=gcreal(pbn);
			mx->Txz[pos]=gcimag(pbn);
			pos++;


			if (y!=dim->ylen-1)
			{
				mx->Ti[pos]=dim->ylen+y;
				mx->Tj[pos]=y+1;
				mx->Tx[pos]=gcreal(nbp);
				mx->Txz[pos]=gcimag(nbp);
				pos++;
			}

			mx->Ti[pos]=dim->ylen+y;
			mx->Tj[pos]=dim->ylen+y;
			mx->Tx[pos]=gcreal(na);
			mx->Txz[pos]=gcimag(na);
			pos++;

			if (y!=dim->ylen-1)
			{
				mx->Ti[pos]=dim->ylen+y;
				mx->Tj[pos]=dim->ylen+y+1;
				mx->Tx[pos]=gcreal(nbn);
				mx->Txz[pos]=gcimag(nbn);
				pos++;
			}

			mx->b[y]=gcreal(fp);
			mx->bz[y]=gcimag(fp);

			mx->b[dim->ylen+y]=gcreal(fn);
			mx->bz[dim->ylen+y]=gcimag(fn);


		}

		//complex_solver_print(sim,li->M,li->N,li->Ti,li->Tj, li->Tx, li->Txz,li->b,li->bz);
		//complex_solver_dump_matrix(li->M,li->N,li->Ti,li->Tj, li->Tx, li->Txz,li->b,li->bz);

		if (pos!=mx->nz)
		{
			printf_log(sim,"%s %d %d\n",_("I can't solve the matrix because I don't have enough equations."),pos,mx->nz);
			exit(0);
		}

		matrix_solve(sim,&(li->msm[w]),mx);

		//complex_solver(sim,li->M,li->N,li->Ti,li->Tj, li->Tx, li->Txz,li->b,li->bz);

		for (y=0;y<dim->ylen;y++)
		{

			update=mx->b[y];
			li->Ep[z][x][y][l]=update;

			update=mx->bz[y];
			li->Epz[z][x][y][l]=update;

			update=mx->b[dim->ylen+y];
			li->En[z][x][y][l]=update;

			update=mx->bz[dim->ylen+y];
			li->Enz[z][x][y][l]=update;
		}

		ittr++;

	if (test==TRUE)
	{
		if (ittr>1) quit=TRUE;
		getchar();
	}else
	{
		quit=TRUE;
	}
	}while(quit==FALSE);
	//getchar();
	//getchar();
return 0;
}


