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

/** @file mesh.c
@brief This builds the electrical mesh
*/

#include "device.h"
#include <mesh.h>
#include "inp.h"
#include "util.h"
#include "gpvdm_const.h"
#include "hard_limit.h"
#include <log.h>
#include <cal_path.h>
#include <circuit.h>
#include <matrix.h>
#include <memory.h>
#include <contacts.h>
#include <epitaxy_struct.h>
#include <epitaxy.h>
#include <exp.h>
#include <dump.h>
#include <lang.h>
#include <lang.h>
#include "lock.h"
#include "enabled_libs.h"
#include <umfpack.h>

void error_report(int status, const char *file, const char *func, int line)
{
	fprintf(stderr, "in %s: file %s, line %d: ", func, file, line);

	switch (status) {
		case UMFPACK_ERROR_out_of_memory:
			fprintf(stderr, "out of memory!\n");
			break;
		case UMFPACK_WARNING_singular_matrix:
			fprintf(stderr, "matrix is singular!\n");
			break;
		default:
			fprintf(stderr, "UMFPACK error code %d\n", status);
	}
}

void circuit_set_V(struct simulation * sim,struct circuit *cir,double V)
{
	int n=0;
	for (n=0;n<cir->nodes_len;n++)
	{
		//printf(">>>>>adding node %d\n", n);
		if (cir->nodes[n].type==CIR_CHANGE_X)
		{
			cir->nodes[n].V=V;
		}
	}
}



void circuit_solve(struct simulation * sim,struct circuit *cir,struct device *dev)
{

	circuit_apply_voltages(sim,cir,dev);
	circuit_apply_photo_generation(sim,cir,dev);
	circuit_build_and_solve_matrix(sim,cir,dev,-1);
}
//Notes:
//The circuit solver works in terms of resitances, and currents.
//Current densities are converted on the output not in the solver

int my_umfpack_solver(int col,int nz,int *Ti,int *Tj, long double *lTx,long double *lb)
{
int i;
void *Symbolic, *Numeric;
int status;
double *dtemp;
int *itemp;

	double *x =malloc(col*sizeof(double)*100);
	double *b = malloc(col*sizeof(double)*100);
	int *Ap = malloc((col+1)*sizeof(int)*100);
	int *Ai = malloc((nz)*sizeof(int)*100);
	double *Ax  = malloc((nz)*sizeof(double)*100);
	double *Tx  = malloc((nz)*sizeof(double)*100);



for (i=0;i<col;i++)
{
	b[i]=(double)lb[i];
}

for (i=0;i<nz;i++)
{
	Tx[i]=(double)lTx[i];
}

double Control [UMFPACK_CONTROL],Info [UMFPACK_INFO];

umfpack_di_defaults (Control) ;
Control[UMFPACK_BLOCK_SIZE]=20;
//Control [UMFPACK_STRATEGY]=UMFPACK_STRATEGY_AUTO;//
//Control [UMFPACK_STRATEGY]=UMFPACK_STRATEGY_SYMMETRIC;
//Control [UMFPACK_STRATEGY]=UMFPACK_STRATEGY_UNSYMMETRIC;
//Control [UMFPACK_ORDERING]=UMFPACK_ORDERING_NONE;//UMFPACK_ORDERING_BEST;//UMFPACK_ORDERING_AMD;//UMFPACK_ORDERING_BEST;//
//Control [UMFPACK_PIVOT_TOLERANCE]=0.0001;
//Control[UMFPACK_SINGLETONS]=1;
//Control[UMFPACK_SCALE]=3;
status = umfpack_di_triplet_to_col(col, col, nz, Ti, Tj, Tx, Ap, Ai, Ax, NULL);

if (status != UMFPACK_OK) {
	error_report(status, __FILE__, __func__, __LINE__);
	return EXIT_FAILURE;
}

// symbolic analysis
status = umfpack_di_symbolic(col, col, Ap, Ai, Ax, &Symbolic, Control, Info);

if (status != UMFPACK_OK) {
	error_report(status, __FILE__, __func__, __LINE__);
	return EXIT_FAILURE;
}

// LU factorization
umfpack_di_numeric(Ap, Ai, Ax, Symbolic, &Numeric, Control, Info);


if (status != UMFPACK_OK) {
	error_report(status, __FILE__, __func__, __LINE__);
	return EXIT_FAILURE;
}
// solve system

umfpack_di_free_symbolic(&Symbolic);


umfpack_di_solve(UMFPACK_A, Ap, Ai, Ax, x, b, Numeric, Control, Info);


if (status != UMFPACK_OK) {
	error_report(status, __FILE__, __func__, __LINE__);
	return EXIT_FAILURE;
}

umfpack_di_free_numeric(&Numeric);

for (i=0;i<col;i++)
{
lb[i]=(long double)x[i];
}


	free(x);
	free(b);
	free(Ap);
	free(Ai);
	free(Ax);
	free(Tx);

return 0;
}

void circuit_build_and_solve_matrix(struct simulation * sim,struct circuit *cir,struct device *dev,int dump_number)
{

	int n;
	int l;
	int x;
	int y;
	double dV=0.0;
	double error=1.0;
	//double A=0.0;
	//struct matrix *mx=&(cir->mx);
	struct circuit_link *links=(cir->links);
	struct circuit_node *nodes=cir->nodes;
	struct circuit_link *link;
	struct matrix mxa;
	matrix_init(&(mxa));
	mxa.M=cir->unknowns;		//col
	mxa.nz=1000;		//col
	mxa.complex_matrix=FALSE;
	mxa.build_from_non_sparse=TRUE;
	matrix_malloc(sim,&mxa);
	mxa.nz=0;		//col
	mxa.nz_max=1000;		//col

	struct matrix *mx=(&mxa);


	double mul=1.0;

	int ittr=0;
	int n0=0;
	int n1=0;
	double A=0.0;
	double Il;
	double dIn0=0.0;
	double dIn1=0.0;
	double deriv=0.0;
	int other_end=0;
	double old_error=1e6;

	do
	{
		//int test=0;
		mx->nz=0;
		matrix_zero_b(sim,mx);
		//printf("build\n");
		for (n=0;n<cir->nodes_len;n++)
		{
			//printf(">>>>>adding node %d\n", n);

			for (l=0;l<nodes[n].nlinks;l++)	//(l=0;l<cir->links_len;l++)
			{
				//mul=1.0;
				link=&(links[nodes[n].links[l]]);		//links[l];
				n0=link->start;
				n1=link->stop;
				other_end=link_get_other_end(cir,link,n);

				if (other_end!=-1)
				{

					//This defines the resistor
					if (link->type=='R')
					{
						Il=(nodes[n0].V-nodes[n1].V)/link->R;
						//printf("%le %le\n",link->rho,link->dl);
						//getchar();
						dIn0=(1.0)/link->R;
						dIn1=(-1.0)/link->R;
					}else
					if (link->type=='D')
					{
						A=Qe/(300.0*kb*link->n0);
						dV=(nodes[n0].V-nodes[n1].V);
						Il=link->I0*(exp(dV*A)-1.0)-link->Isc;
						/*if (link->Isc>0.0)
						{
							printf("%le\n",link->Isc);
							getchar();
						}*/
						dIn0=A*link->I0*exp(dV*A);
						dIn1=-A*link->I0*exp(dV*A);
					}else
					if (link->type=='C')
					{
						//printf("%d\n",dev->go_time);
						//getchar();
						if (dev->go_time==TRUE)
						{
							Il=link->C*((nodes[n0].V-nodes[n0].V_last)-(nodes[n1].V-nodes[n1].V_last))/dev->dt;
							//printf("%le %le\n",(nodes[n0].V-nodes[n0].V_last));
							//printf(">>>>>>>>>>%le %le\n",Il,link->C,nodes[n0].V);
							//getchar();
							dIn0=link->C*(1.0)/dev->dt;
							dIn1=link->C*(-1.0)/dev->dt;
						}else
						{
							double R=1e15;
							Il=(nodes[n0].V-nodes[n1].V)/R;
							dIn0=(1.0)/R;
							dIn1=(-1.0)/R;
						}
					}

					if (n==n0)
					{
						mul=-1.0;
						deriv=dIn0;
						//we are flowing out
					}else
					{
						mul=1.0;
						deriv=dIn1;
					}

							//printf("%d %d %le\n",n,n0,mul);
							//getchar();
					//printf("%d %d %le\n",x,y,link->dl);
					//getchar();
					if (nodes[n].type==CIR_UNKNOWN)
					{
						x=nodes[n].matrix_pos;
						y=nodes[n].matrix_pos;

						matrix_add_nz_item(sim,mx,x,y,mul*deriv);

						link->i=Il;
						mx->b[nodes[n].matrix_pos]-=mul*Il;

						if (cir->nodes[other_end].type==CIR_UNKNOWN)
						{

							if (other_end==n1)
							{
								//we are flowing out
								deriv=dIn1;
							}else
							{
								//we are flowing in
								deriv=dIn0;
							}

							x=nodes[other_end].matrix_pos;
							y=nodes[n].matrix_pos;

							matrix_add_nz_item(sim,mx,x,y,mul*deriv);

							//printf(">off-diag (%d,%d)\n",x,y);

						}


					}

					if (nodes[n].type!=CIR_UNKNOWN)
					{
						if (nodes[other_end].type!=CIR_UNKNOWN)
						{
							link->i=mul*Il;
						}
					}
				}
			}
			

		}

		if (cir->unknowns==0)
		{
			break;
		}

		//printf("build-end %d %d %d\n",test,mx->nz,mx->M);
		//getchar();
		matrix_convert_J_to_sparse(sim,mx);
		//matrix_dump(sim,mx);
		char file_name[200];

		if (dump_number!=-1)
		{
			sprintf(file_name,"%d_b.txt",dump_number);
			printf("%d %p\n",dump_number,mx->b);
			matrix_dump_to_file(sim,mx,file_name);
		}

		int *Ti=malloc(sizeof(int)*1000);
		int *Tj=malloc(sizeof(int)*1000);
		long double *Tx=malloc(sizeof(long double)*1000);
		long double *b=malloc(sizeof(long double)*1000);

		int r=0;

		if (dump_number!=-1)
		{

			//mx->nz=mx->M;
			for (r=0;r<mx->nz;r++)
			{
				Ti[r]=mx->Ti[r];
				Tj[r]=mx->Tj[r];
				Tx[r]=1.0;//(long double)mx->Tx[r];
			}

			for (r=0;r<mx->M;r++)
			{
				b[r]=1.0;//mx->b[r];
			}
			printf("%d %d %d\n",mxa.M, mxa.nz,	mxa.nz_max);

			my_umfpack_solver(mx->M,mx->nz,Ti,Tj, Tx,b);
			//my_umfpack_solver(mx->M,mx->nz,mx->Ti,mx->Tj, mx->Tx,mx->b);
			sprintf(file_name,"%d_a.txt",dump_number);

			//mx->nz=1000;
			mx->Ti=Ti;
			mx->Tj=Tj;
			mx->Tx=Tx;
			mx->b=b;

			matrix_dump_to_file(sim,mx,file_name);
			getchar();
			/*sprintf(file_name,"%d_2b.txt",dump_number);
			matrix_dump_to_file(sim,&(mxb),file_name);

			my_umfpack_solver(mxb.M,mxb.nz,mxb.Ti,mxb.Tj, mxb.Tx,mxb.b);
			sprintf(file_name,"%d_2a.txt",dump_number);
			matrix_dump_to_file(sim,&(mxb),file_name);
			*/
		}else
		{
			matrix_solve(sim,&(cir->msm),mx);
			printf("%d\n",dump_number);
		}
		//printf("solve-end\n");
		for (n=0;n<cir->nodes_len;n++)
		{
			if (nodes[n].type==CIR_UNKNOWN)
			{
				double update=mx->b[nodes[n].matrix_pos];
				if (isnan(update))
				{
					//matrix_dump(sim,mx);
					ewe(sim,"nan detected in newton solver");
				}

				//printf("%Le\n",dev->electrical_clamp);
				if (dev->electrical_clamp>20.0)
				{
					nodes[n].V+=update;///(1.0+fabs(update/dev->electrical_clamp/(300.0*kb/Q)));
				}else
				{
					nodes[n].V+=update/(1.0+fabs(update/dev->electrical_clamp/(300.0*kb/Qe)));
				}
			}

		}
		//circuit_print_nodes(sim,cir);
		old_error=error;
		error=matrix_cal_error(sim,mx);
		if (cir->quite==FALSE)
		{
			printf_log(sim,"Electrical solve %d f()=%le %le %le\n",ittr,error,old_error,fabs(error-old_error));
		}		

		ittr++;

		if (ittr>dev->max_electrical_itt)
		{
			//printf("here1\n");
			break;
		}

		if (fabs(error-old_error)<1e-16)
		{
			//printf("here2\n");
			break;
		}

		//printf("%Le %le %d\n",dev->min_cur_error,error,error>dev->min_cur_error);
	}while(error>dev->min_cur_error);


	//circuit_dump_I(sim,dev,"./");

	//circuit_print_nodes(sim,cir);
	//circuit_print_links(sim,cir);
	//getchar();
	//circuit_cal_device_resistance(sim,dev);
	//circuit_printf_links(sim,cir);
	//getchar();
}

double circuit_node_get_I(struct simulation * sim,struct circuit *cir, int n)
{
	int l;
	double ret=0.0;
	int n0=0;
	//double mul=0.0;
	int other_end=0;

	struct circuit_link *link;
	struct circuit_link *links=(cir->links);
	for (l=0;l<cir->links_len;l++)
	{
		link=&links[l];
		n0=link->start;
		//n1=link->stop;

		other_end=link_get_other_end(cir,&(links[l]),n);

		if (other_end!=-1)
		{
			if (n==n0)
			{
				ret-=link->i;
			}else
			{
				ret+=link->i;
			}

		}
		//printf(">>>>>>>%d %le\n",node->links[l],link->i);
	}

	return ret;
}


void circuit_apply_voltages(struct simulation * sim,struct circuit *cir,struct device *dev)
{
	int x=0;
	int y=0;
	int z=0;
	int c=0;
	int n=0;
	struct circuit_node *node;
	struct circuit_node *nodes=cir->nodes;
	struct dimensions *dim=&(dev->ns.dim);

	//printf("%d\n",cir->abstract_circuit);

	if (cir->abstract_circuit==FALSE)
	{
		for (z=0;z<dim->zlen;z++)
		{
			for (x=0;x<dim->xlen;x++)
			{
				y=0;
				c=dev->n_contact_y0[z][x];
				//printf("%d %Le\n",c,contact_get_voltage(sim,dev,c));
				if (c!=-1)
				{
					n=circuit_find_node_by_xyz(sim,cir,x,y,z);
					if (dev->contacts[c].active==TRUE)
					{
						nodes[n].type=CIR_CHANGE_X;
						//nodes[n].V=1.0;
					}else
					{
						nodes[n].type=CIR_KNOWN;
						//nodes[n].V=0.0;
					}
					nodes[n].V=contact_get_voltage(sim,dev,c); //put back in
					//printf("contact %d %d\n",c,dev->contacts[c].active);
					//getchar();
					//printf(">>%d %d %d\n",x,z,dev->n_contact_y0[z][x]);
				}

				y=circuit_cal_max_y_node_level(sim,cir,dev);
				c=dev->n_contact_y1[z][x];

				if (c!=-1)
				{
					n=circuit_find_node_by_xyz(sim,cir,x,y,z);
					if (dev->contacts[c].active==TRUE)
					{
						nodes[n].type=CIR_CHANGE_X;
						//nodes[n].V=1.0;
					}else
					{
						nodes[n].type=CIR_KNOWN;
						//nodes[n].V=0.0;
					}

					nodes[n].V=contact_get_voltage(sim,dev,c);
				}


			}
		}
	}else
	{
		for (n=0;n<cir->nodes_len;n++)
		{
			node=&(nodes[n]);
			if (node->type==CIR_CHANGE_X)
			{
				node->V=contact_get_active_contact_voltage(sim,dev);
				//printf(">>>%lf\n",node->V);
			}
		}
	}

	//getchar();
}

void circuit_time_step(struct simulation * sim,struct circuit *cir)
{
	int n=0;
	struct circuit_node *node;
	struct circuit_node *nodes=cir->nodes;


	if (cir->enabled==TRUE)
	{
		for (n=0;n<cir->nodes_len;n++)
		{
			node=&(nodes[n]);
			node->V_last=node->V;
		}
	}
}




double circuit_get_max_I(struct simulation * sim,struct circuit *cir)
{
	int l=0;
	double max_I=0.0;
	double fabs_I=0.0;
	struct circuit_link *links=cir->links;
	struct circuit_link *link;
	for (l=0;l<cir->links_len;l++)
	{
		link=&(links[l]);
		fabs_I=fabs(link->i);
		if (fabs_I>max_I)
		{
			max_I=fabs_I;
		}
	}

	return max_I;
}




