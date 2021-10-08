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

void matrix_init(struct matrix *mx)
{
	mx->Ti = NULL;
	mx->Tj = NULL;
	mx->Tx = NULL;
	mx->Txz = NULL;
	mx->b = NULL;
	mx->bz = NULL;
	mx->Tdebug = NULL;
	mx->nz= 0;
	mx->nz_max= 0;
	
	mx->M= 0;

	mx->use_cache=FALSE;
	strcpy(mx->hash,"");
	strcpy(mx->cache_file_path,"");

	mx->ittr=0;
	mx->complex_matrix=FALSE;

	mx->build_from_non_sparse=FALSE;
	//mx->J=NULL;

	//stats
	mx->tot_time=0;
	mx->call=0;
	strcpy(mx->dump_name,"");
	mx->msort = NULL;
	mx->msort_len=-1;
}

void matrix_stats(struct simulation *sim,struct matrix *mx)
{
	printf("matrix solver time=%d ms \n",mx->tot_time);
	mx->tot_time=0;
}

void matrix_cache_reset(struct simulation *sim,struct matrix *mx)
{
	strcpy(mx->hash,"");
	mx->ittr=0;
}

void matrix_dump(struct simulation *sim,struct matrix *mx)
{
int i;
	printf("J:\n");
	for (i=0;i<mx->nz;i++)
	{
		printf_log(sim,"%ld %ld %Le\n",mx->Tj[i],mx->Ti[i],mx->Tx[i]);
	}

	printf("b:");
	
	for (i=0;i<mx->M;i++)
	{
		printf_log(sim,"%Le\n",mx->b[i]);
	}

}

void matrix_load_from_file(struct simulation *sim,struct matrix *mx,char *file_name)
{
int i;
	FILE *in;
	char temp[200];
	in=fopen(file_name,"r");
	fscanf(in,"%d",&mx->M);
	fscanf(in,"%d",&mx->nz);
	matrix_malloc(sim,mx);
	fscanf(in,"%s",temp);
	for (i=0;i<mx->nz;i++)
	{
		fscanf(in,"%d %d %Le",&mx->Tj[i],&mx->Ti[i],&mx->Tx[i]);
	}

	fscanf(in,"%s",temp);
	
	for (i=0;i<mx->M;i++)
	{
		fscanf(in,"%Le\n",&mx->b[i]);
	}

	fclose(in);
}

void matrix_dump_to_file(struct simulation *sim,struct matrix *mx,char *file_name)
{
int i;
	FILE *out;
	out=fopen(file_name,"w");
	fprintf(out,"%d\n",mx->M);
	fprintf(out,"%d\n",mx->nz);
	fprintf(out,"J:\n");
	for (i=0;i<mx->nz;i++)
	{
		fprintf(out,"%d %d %Le\n",mx->Tj[i],mx->Ti[i],mx->Tx[i]);
	}

	fprintf(out,"b:\n");
	
	for (i=0;i<mx->M;i++)
	{
		fprintf(out,"%Le\n",mx->b[i]);
	}

	fclose(out);
}

int matrix_cmp_to_file(struct simulation *sim,struct matrix *mx,char *file_name)
{
	int i;
	char temp[100];
	char Tx_file[100];
	char Tx[100];
	char b[100];
	char b_file[100];
	int Tj,Ti;
	FILE *out;

	out=fopen(file_name,"r");

	fscanf(out,"%s",temp);
	for (i=0;i<mx->nz;i++)
	{
		fscanf(out,"%d %d %s",&Tj,&Ti,Tx_file);


		sprintf(Tx,"%Le",mx->Tx[i]);
		if (strcmp(Tx,Tx_file)!=0)
		{
			printf("J: %d %d %s!=%s\n",Ti,Tj,Tx,Tx_file);
			fclose(out);
			return -1;
		}		

	}

	fscanf(out,"%s",temp);
	
	for (i=0;i<mx->M;i++)
	{
		fscanf(out,"%s",b);
		sprintf(b_file,"%Le",mx->b[i]);
		if (strcmp(b,b_file)!=0)
		{
			printf("b: %d %s!=%s\n",i,b,b_file);
			fclose(out);
			return -1;
		}		
	}

	fclose(out);
	return 0;
}

void matrix_dump_b(struct simulation *sim,struct matrix *mx)
{
int i;
	if (mx->complex_matrix==TRUE)
	{
		for (i=0;i<mx->M;i++)
		{
			printf_log(sim,"%Le %Le\n",mx->b[i],mx->bz[i]);
		}
	}else
	{
		for (i=0;i<mx->M;i++)
		{
			printf_log(sim,"%Le\n",mx->b[i]);
		}
	}
}

void matrix_dump_J(struct simulation *sim,struct matrix *mx)
{
int i;
	if (mx->complex_matrix==TRUE)
	{
		for (i=0;i<mx->nz;i++)
		{
			printf_log(sim,"%ld %ld %Le %Le\n",mx->Ti[i],mx->Tj[i],mx->Tx[i],mx->Txz[i]);
		}
	}else
	{

		for (i=0;i<mx->nz;i++)
		{
			printf_log(sim,"%ld %ld %Le\n",mx->Ti[i],mx->Tj[i],mx->Tx[i]);
		}
	}
}


long double matrix_cal_error(struct simulation *sim,struct matrix *mx)
{
int i;
long double sum=0.0;
	for (i=0;i<mx->M;i++)
	{
		sum+=fabsl(mx->b[i]);
	}

return sum;
}

int matrix_solve(struct simulation *sim,struct matrix_solver_memory *msm,struct matrix *mx)
{
char out[100];
char temp[200];
struct md5 hash;
int dump_matrix_check=FALSE;
int matrix_check=FALSE;
int start_time=timer_get_time_in_ms();
	
	if (mx->use_cache==TRUE)
	{
		if (mx->ittr==0)
		{
			md5_init(&hash);
			md5_update(&hash,(char*)mx->Ti,mx->nz*sizeof(int));
			md5_update(&hash,(char*)mx->Tj,mx->nz*sizeof(int));
			md5_update(&hash,(char*)mx->Tx,mx->nz*sizeof(long double));
			md5_update(&hash,(char*)mx->b,mx->M*sizeof(long double));
			md5_to_str(mx->hash,&hash);

			join_path(2,mx->cache_file_path,get_cache_path(sim),mx->hash);

			if (isfile(mx->cache_file_path)==0)
			{
				return 0;
			}
		}

	}

	if (mx->complex_matrix==FALSE)
	{
		/*printf("here %d %d\n",mx->M,mx->nz);
		int i;

		for (i=0;i<mx->nz;i++)
		{
			printf("%d %Le\n",i,mx->b[i]);
		}

		for (i=0;i<mx->M;i++)
		{
			printf("%d %Le\n",i,mx->b[i]);
		}

		getchar();*/
		if (sim->dll_matrix_solve==NULL)
		{
			ewe(sim,"The matrix solver dll is not loaded\n");
		}

		matrix_solver_memory_check_memory(sim,msm,mx->M,mx->nz);
		(*sim->dll_matrix_solve)(msm,mx->M,mx->nz,mx->Ti,mx->Tj,mx->Tx,mx->b);

		//printf("fin\n");
	}else
	{
		(*sim->dll_complex_matrix_solve)(msm,mx->M,mx->nz,mx->Ti,mx->Tj,mx->Tx,mx->Txz,mx->b,mx->bz);
	}

	mx->tot_time+=(timer_get_time_in_ms()-start_time);
	mx->call++;
return -1;
}

void matrix_malloc(struct simulation *sim,struct matrix *mx)
{
	malloc_1d((void **)(&mx->Ti), mx->nz, sizeof(int));

	malloc_1d((void **)(&mx->Tj), mx->nz, sizeof(int));

	malloc_1d((void **)(&mx->Tx), mx->nz, sizeof(long double));

	malloc_1d((void **)(&mx->b), mx->M, sizeof(long double));


	if (mx->complex_matrix==TRUE)
	{
		malloc_1d((void **)(&mx->Txz), mx->nz, sizeof(long double));

		malloc_1d((void **)(&mx->bz), mx->M, sizeof(long double));
	}

	/*if (mx->build_from_non_sparse==TRUE)
	{
		malloc_2d((void ***)(&mx->J), mx->M, mx->M, sizeof(long double));
	}*/

}

int matrix_sort_comparator (const void *in_m0, const void *in_m1)
{
	struct matrix_sort *val0=(struct matrix_sort *)in_m0;
	struct matrix_sort *val1=(struct matrix_sort *)in_m1;

	if (val0->Ti<val1->Ti)
	{
		return -1;
	}

	if (val0->Ti>val1->Ti)
	{
		return 1;
	}

	if (val0->Tj<val1->Tj)
	{
		return -1;
	}

	if (val0->Tj>val1->Tj)
	{
		return 1;
	}

	return 0;
}

void matrix_convert_J_to_sparse(struct simulation *sim,struct matrix *mx)
{
	int x;
	int y;
	int n;
	int nn;
	int write_pos=0;
	int read_pos=0;
	int read_Ti=0;
	int read_Tj=0;
	int t=0;

	struct matrix_sort *ms;
	if (mx->msort_len<mx->nz)
	{
		mx->msort=realloc(mx->msort,sizeof(struct matrix_sort)*mx->nz);
		mx->msort_len=mx->nz;
	}

	for (n=0;n<mx->nz;n++)
	{
		ms=&(mx->msort[n]);
		ms->Ti=mx->Ti[n];
		ms->Tj=mx->Tj[n];
		ms->Tx=mx->Tx[n];
	}

	/*for (n=0;n<mx->nz;n++)
	{
		ms=&(mx->msort[n]);
		printf("b %d %d %Le\n",ms->Ti,ms->Tj,ms->Tx);
	}*/

	qsort(mx->msort, mx->nz,sizeof(struct matrix_sort),matrix_sort_comparator);
	/*printf("\n");
	for (n=0;n<mx->nz;n++)
	{
		ms=&(mx->msort[n]);
		printf("a %d %d %Le\n",ms->Ti,ms->Tj,ms->Tx);
	}*/
	

	//printf("\n");
	int last_Ti=-1;
	int last_Tj=-1;
	int pos=0;
	for (n=0;n<mx->nz;n++)
	{
		ms=&(mx->msort[n]);
		//printf("c %d %d %Le %d\n",ms->Ti,ms->Tj,ms->Tx,pos);
		//printf("%d %d\n",last_Ti,last_Tj);
		if ((ms->Ti!=-1)&&(ms->Tj!=-1))
		{
			if ((ms->Ti!=last_Ti)||(ms->Tj!=last_Tj))
			{
				last_Ti=ms->Ti;
				last_Tj=ms->Tj;
				pos=n;
			}else
			{
				mx->msort[pos].Tx+=ms->Tx;
				ms->Ti=-1;
				ms->Tj=-1;
			}

		}
	}

	/*for (n=0;n<mx->nz;n++)
	{
		ms=&(mx->msort[n]);
		printf("c %d %d %Le\n",ms->Ti,ms->Tj,ms->Tx);
	}
	printf("\n");*/
	pos=0;
	for (n=0;n<mx->nz;n++)
	{
		ms=&(mx->msort[n]);
		if (ms->Ti!=-1)
		{
			mx->Ti[pos]=ms->Ti;
			mx->Tj[pos]=ms->Tj;
			mx->Tx[pos]=ms->Tx;
			pos++;
		}
	}
	//printf("\n");
	mx->nz=pos;


///////////////////////Right
	/*for (n=0;n<mx->nz;n++)
	{

		if (mx->Ti[read_pos]!=-1)
		{
			read_Ti=mx->Ti[read_pos];
			read_Tj=mx->Tj[read_pos];

			for (nn=read_pos;nn<mx->nz;nn++)
			{


				if ((read_Ti==mx->Ti[nn])&&(mx->Ti[nn]!=-1))
				{
					if ((read_Tj==mx->Tj[nn])&&(mx->Tj[nn]!=-1))
					{
						if (write_pos!=nn)
						{
							mx->Tx[write_pos]+=mx->Tx[nn];
							mx->Ti[write_pos]=mx->Ti[nn];
							mx->Tj[write_pos]=mx->Tj[nn];
							mx->Ti[nn]=-1;
							mx->Tj[nn]=-1;
							mx->Tx[nn]=0.0;
						}

					}
				}


			}

			write_pos++;
		}



		read_pos++;



	}



	int found=FALSE;
	for (n=0;n<mx->nz;n++)
	{
		ms=&(mx->msort[n]);
		if (ms->Ti!=-1)
		{
			found=FALSE;
			for (nn=0;nn<mx->nz;nn++)
			{
				if (mx->Ti[nn]==ms->Ti)
				{
					if (mx->Tj[nn]==ms->Tj)
					{
						if (mx->Tx[nn]!=ms->Tx)
						{
							printf("error!\n");
						}else
						{
							found=TRUE;
							printf("OK\n");
						}
						break;
					}
				}

			}

			if (found==FALSE)
			{
				printf("error!\n");
			}
		}
	}*/
			//	mx->Ti[nn]=ms->Ti;
			//	mx->Tj[nn]=ms->Tj;
			//	mx->Tx[pos]=ms->Tx;

	//mx->nz=write_pos;
}

void matrix_add_nz_item(struct simulation *sim,struct matrix *mx,int x,int y,long double val)
{
	//int i;
	//(printf("search =%d %d\n",x,y);
	//mx->J[y][x]+=val;
	
	/*for (i=0;i<mx->nz;i++)
	{
		if ((x==mx->Tj[i])&&(y==mx->Ti[i]))
		{
			//printf("found\n");
			mx->Tx[i]+=val;
			return;
		}
	}*/

	if (mx->nz>=mx->nz_max)
	{
		mx->nz_max+=1000;
		mx->Ti=realloc(mx->Ti,mx->nz_max*sizeof(int));
		mx->Tj=realloc(mx->Tj,mx->nz_max*sizeof(int));
		mx->Tx=realloc(mx->Tx,mx->nz_max*sizeof(long double));
	}

	mx->Ti[mx->nz]=y;
	mx->Tj[mx->nz]=x;
	mx->Tx[mx->nz]=val;
	mx->nz++;

}
void matrix_realloc(struct simulation *sim,struct matrix *mx)
{
	gdouble *dtemp=NULL;
	int *itemp=NULL;
	itemp = realloc(mx->Ti,mx->nz*sizeof(int));
	if (itemp==NULL)
	{
		ewe(sim,"mx->Ti - memory error\n");
	}else
	{
		mx->Ti=itemp;
		memset(mx->Ti, 0, mx->nz*sizeof(int));
	}

	itemp = realloc(mx->Tj,mx->nz*sizeof(int));
	if (itemp==NULL)
	{
		ewe(sim,"mx->Tj - memory error\n");
	}else
	{
		mx->Tj=itemp;
		memset(mx->Tj, 0, mx->nz*sizeof(int));
	}

	dtemp = realloc(mx->Tx,mx->nz*sizeof(long double));
	if (dtemp==NULL)
	{
		ewe(sim,"mx->Tx - memory error\n");
	}else
	{
		mx->Tx=dtemp;
		memset(mx->Tx, 0, mx->nz*sizeof(long double));
	}

	dtemp = realloc(mx->b,mx->M*sizeof(long double));

	if (dtemp==NULL)
	{
		ewe(sim,"in->b - memory error\n");
	}else
	{
		mx->b=dtemp;
		memset(mx->b, 0, mx->M*sizeof(long double));
	}

	if (mx->complex_matrix==TRUE)
	{
		dtemp = realloc(mx->Txz,mx->nz*sizeof(long double));
		if (dtemp==NULL)
		{
			ewe(sim,"mx->Txz - memory error\n");
		}else
		{
			mx->Txz=dtemp;
			memset(mx->Txz, 0, mx->nz*sizeof(long double));
		}

		dtemp = realloc(mx->bz,mx->M*sizeof(long double));

		if (dtemp==NULL)
		{
			ewe(sim,"in->bz - memory error\n");
		}else
		{
			mx->bz=dtemp;
			memset(mx->bz, 0, mx->M*sizeof(long double));
		}

	}

	/*if (mx->build_from_non_sparse==TRUE)
	{
		int y;
		for (y=0;y<mx->M;y++)
		{
			free(mx->J[y]);
		}

		free(mx->J);

		mx->J=malloc(mx->M*sizeof(long double*));
		for (y=0;y<mx->M;y++)
		{
			mx->J[y]=malloc(mx->M*sizeof(long double));
			memset(mx->J[y], 0, mx->M*sizeof(long double));
		}
	}*/
}

void matrix_zero_b(struct simulation *sim,struct matrix *mx)
{
	memset(mx->b, 0, mx->M*sizeof(long double));

	if (mx->complex_matrix==TRUE)
	{
		memset(mx->bz, 0, mx->M*sizeof(long double));
	}

	/*if (mx->build_from_non_sparse==TRUE)
	{
		int y;

		for (y=0;y<mx->M;y++)
		{
			memset(mx->J[y], 0, mx->M*sizeof(long double));
		}
	}*/
}

void matrix_save(struct simulation *sim,struct matrix *mx)
{
	char cache_file[PATH_MAX];
	FILE *file;

	join_path(2, cache_file,get_cache_path(sim),mx->hash);

	file=fopen(cache_file,"wb");

	fwrite ((char*)mx->Ti,mx->nz*sizeof(int),1,file);
	fwrite ((char*)mx->Tj,mx->nz*sizeof(int),1,file);
	fwrite ((char*)mx->Tx,mx->nz*sizeof(long double),1,file);
	fwrite ((char*)mx->b,mx->M*sizeof(long double),1,file);

	if (mx->complex_matrix==TRUE)
	{
		fwrite ((char*)mx->Txz,mx->nz*sizeof(long double),1,file);
		fwrite ((char*)mx->bz,mx->M*sizeof(long double),1,file);
	}

	fclose(file);

}

int matrix_load(struct simulation *sim,struct matrix *mx)
{
	char cache_file[PATH_MAX];
	FILE *file;

	join_path(2, cache_file,get_cache_path(sim),mx->hash);

	file=fopen(cache_file,"rb");
	if (file==NULL)
	{
		return -1;
	}

	fread((int*)mx->Ti, mx->nz*sizeof(int), 1, file);
	fread((int*)mx->Tj, mx->nz*sizeof(int), 1, file);
	fread((long double *)mx->Tx, mx->nz*sizeof(long double), 1, file);
	fread((long double *)mx->b, mx->M*sizeof(long double), 1, file);

	if (mx->complex_matrix==TRUE)
	{
		fread((long double *)mx->Txz, mx->nz*sizeof(long double), 1, file);
		fread((long double *)mx->bz, mx->M*sizeof(long double), 1, file);
	}
	fclose(file);

	return 0;

}



/*
for (n=0;n<mx->nz;n++)
	{

		if (mx->Ti[read_pos]!=-1)
		{
			read_Ti=mx->Ti[read_pos];
			read_Tj=mx->Tj[read_pos];

			//mx->Tx[write_pos]=0.0;
			for (nn=read_pos;nn<mx->nz;nn++)
			{
				//for (t=0;t<mx->nz;t++)
				//{
				//	printf("%d %d %Lf",mx->Ti[t],mx->Tj[t],mx->Tx[t]);
				//	if (t==write_pos)
				//	{
				//		printf(" W");
				//	}

				//	if (t==read_pos)
				//	{
				//		printf(" R");
				//	}

				//	if (t==nn)
				//	{
				//		printf(" *");
				//	}

				//	printf("\n");
				//

				if ((read_Ti==mx->Ti[nn])&&(mx->Ti[nn]!=-1))
				{
					if ((read_Tj==mx->Tj[nn])&&(mx->Tj[nn]!=-1))
					{
						if (write_pos!=nn)
						{
							mx->Tx[write_pos]+=mx->Tx[nn];
							mx->Ti[write_pos]=mx->Ti[nn];
							mx->Tj[write_pos]=mx->Tj[nn];
							mx->Ti[nn]=-1;
							mx->Tj[nn]=-1;
							mx->Tx[nn]=0.0;
							//printf("%d %d\n",nn,write_pos);
						}

					}
				}

				//Think I need to replace with hashing
				//if ((read_Ti<mx->Ti[nn])&&(mx->Ti[nn]!=-1))
				//{
				//	if ((read_Tj<mx->Tj[nn])&&(mx->Tj[nn]!=-1))
				//	{
				//		//printf("break!");
				//		break;
				//	}
				//}

				//printf("\n");




				//getchar();
			}

			write_pos++;
		}



		read_pos++;



	}

*/

