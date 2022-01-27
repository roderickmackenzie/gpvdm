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

/** @file memory_basic.c
@brief memory functions for 3D arrays
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


void malloc_zxy_long_double(struct dimensions *dim, gdouble * (***var))
{
	malloc_3d((void****)var,dim->zlen, dim->xlen, dim->ylen,sizeof(long double));
}

void free_zxy_long_double(struct dimensions *dim, gdouble * (***var))
{
	free_3d((void****)var,dim->zlen, dim->xlen, dim->ylen,sizeof(long double));
}

void cpy_zxy_long_double(struct dimensions *dim, long double * (***out), long double * (***in))
{
	free_3d((void****)out,dim->zlen, dim->xlen, dim->ylen,sizeof(long double));
	if (*in==NULL)
	{
		return;
	}
	malloc_3d((void****)out,dim->zlen, dim->xlen, dim->ylen,sizeof(long double));
	cpy_3d((void****)out, (void****)in, dim->zlen, dim->xlen, dim->ylen, sizeof(long double));
}

void three_d_copy_gdouble(struct dimensions *dim, long double ***out, long double ***in)
{
	if (in==NULL)
	{
		printf("Warning copying null pointer\n");
	}

	if (out==NULL)
	{
		printf("Warning copying onto null pointer!\n");
		getchar();
	}

	cpy_3d((void****)&out, (void****)&in, dim->zlen, dim->xlen, dim->ylen, sizeof(long double));
}

long double zxy_min_gdouble(struct dimensions *dim, gdouble ***var)
{
	int x=0;
	int y=0;
	int z=0;

	long double min=var[0][0][0];

	for (z = 0; z < dim->zlen; z++)
	{
		for (x = 0; x < dim->xlen; x++)
		{
			for (y = 0; y < dim->ylen; y++)
			{
				if (var[z][x][y]<min)
				{
					min=var[z][x][y];
				}
			}
		}
	}

return min;
}

long double zxy_max_gdouble(struct dimensions *dim, gdouble ***var)
{
	int x=0;
	int y=0;
	int z=0;

	long double max=var[0][0][0];

	for (z = 0; z < dim->zlen; z++)
	{
		for (x = 0; x < dim->xlen; x++)
		{
			for (y = 0; y < dim->ylen; y++)
			{
				if (var[z][x][y]>max)
				{
					max=var[z][x][y];
				}
			}
		}
	}

return max;
}

long double zx_y_max_gdouble(struct dimensions *dim, gdouble ***var,int y)
{
	int x=0;
	int z=0;

	long double max=var[0][0][0];

	for (z = 0; z < dim->zlen; z++)
	{
		for (x = 0; x < dim->xlen; x++)
		{
			if (var[z][x][y]>max)
			{
				max=var[z][x][y];
			}
		}
	}

return max;
}

void zxy_set_gdouble(struct dimensions *dim, gdouble ***var, gdouble val)
{
int x=0;
int y=0;
int z=0;

	for (z = 0; z < dim->zlen; z++)
	{
		for (x = 0; x < dim->xlen; x++)
		{
			for (y = 0; y < dim->ylen; y++)
			{
				var[z][x][y]=val;
			}

		}
	}

}


void three_d_sub_gdouble(struct dimensions *dim, gdouble ***var, gdouble ***sub)
{
int x=0;
int y=0;
int z=0;

	for (z = 0; z < dim->zlen; z++)
	{
		for (x = 0; x < dim->xlen; x++)
		{
			for (y = 0; y < dim->ylen; y++)
			{
				var[z][x][y]-=sub[z][x][y];
			}

		}
	}

}



void three_d_add_gdouble(struct dimensions *dim, gdouble ***var, gdouble ***add)
{
int x=0;
int y=0;
int z=0;

	for (z = 0; z < dim->zlen; z++)
	{
		for (x = 0; x < dim->xlen; x++)
		{
			for (y = 0; y < dim->ylen; y++)
			{
				var[z][x][y]+=add[z][x][y];
			}

		}
	}

}

void zxy_mul_gdouble(struct dimensions *dim, gdouble ***src, gdouble val)
{
int x=0;
int y=0;
int z=0;

	for (z = 0; z < dim->zlen; z++)
	{
		for (x = 0; x < dim->xlen; x++)
		{
			for (y = 0; y < dim->ylen; y++)
			{
				src[z][x][y]*=val;
			}

		}
	}

}

void zxy_long_double_mul_by_zxy_long_double(struct dimensions *dim, gdouble ***a, gdouble ***b)
{
int x=0;
int y=0;
int z=0;

	for (z = 0; z < dim->zlen; z++)
	{
		for (x = 0; x < dim->xlen; x++)
		{
			for (y = 0; y < dim->ylen; y++)
			{
				a[z][x][y]*=b[z][x][y];
			}

		}
	}

}

void zxy_long_double_div_by_zxy_long_double(struct dimensions *dim, gdouble ***a, gdouble ***b)
{
int x=0;
int y=0;
int z=0;

	for (z = 0; z < dim->zlen; z++)
	{
		for (x = 0; x < dim->xlen; x++)
		{
			for (y = 0; y < dim->ylen; y++)
			{
				a[z][x][y]/=b[z][x][y];
			}

		}
	}

}

void zxy_div_gdouble(struct dimensions *dim, gdouble ***src, gdouble val)
{
int x=0;
int y=0;
int z=0;

	for (z = 0; z < dim->zlen; z++)
	{
		for (x = 0; x < dim->xlen; x++)
		{
			for (y = 0; y < dim->ylen; y++)
			{
				src[z][x][y]/=val;
			}

		}
	}

}

long double three_d_avg_raw(struct device *in, long double ***src)
{
int x=0;
int y=0;
int z=0;
long double sum=0.0;
long double ret=0.0;
if (src==NULL)
{
	return 0.0;
}

long double count=0.0;
struct dimensions *dim=&(in->ns.dim);

	for (z = 0; z < dim->zlen; z++)
	{
		for (x = 0; x < dim->xlen; x++)
		{
			for (y = 0; y < dim->ylen; y++)
			{

				sum+=src[z][x][y];
				count+=1.0;
//				printf("%Le %Le %Le %Le %Le %Le\n",dim->dx[x],dim->dy[y],dim->dz[z],in->zlen,in->xlen,in->ylen);
			}

		}
	}

ret=sum/count;
return ret;
}

long double three_d_avg(struct device *in, long double ***src)
{
int x=0;
int y=0;
int z=0;
long double sum=0.0;
long double ret=0.0;

long double dx=0.0;
long double dy=0.0;
long double dz=0.0;

if (src==NULL)
{
	return 0.0;
}

struct dimensions *dim=&(in->ns.dim);

	for (z = 0; z < dim->zlen; z++)
	{
		for (x = 0; x < dim->xlen; x++)
		{
			for (y = 0; y < dim->ylen; y++)
			{

				sum+=src[z][x][y]*dim->dX[x]*dim->dY[y]*dim->dZ[z];
//				printf("%Le %Le %Le %Le %Le %Le\n",dim->dx[x],dim->dy[y],dim->dZ[z],in->zlen,in->xlen,in->ylen);
			}

		}
	}

ret=sum/(in->zlen*in->xlen*in->ylen);
//printf("ret=%Le\n",ret);
return ret;
}


void three_d_printf(struct dimensions *dim, long double ***src)
{
int x=0;
int y=0;
int z=0;
long double sum=0.0;
long double ret=0.0;
	for (z = 0; z < dim->zlen; z++)
	{
		for (x = 0; x < dim->xlen; x++)
		{
			for (y = 0; y < dim->ylen; y++)
			{
				printf("%Le\n",src[z][x][y]);
			}

		}
	}

return;
}

long double three_d_avg_fabsl(struct device *in, long double ***src)
{
int x=0;
int y=0;
int z=0;
long double sum=0.0;
long double ret=0.0;

if (src==NULL)
{
	return 0.0;
}

struct dimensions *dim=&(in->ns.dim);
	for (z = 0; z < dim->zlen; z++)
	{
		for (x = 0; x < dim->xlen; x++)
		{
			for (y = 0; y < dim->ylen; y++)
			{
				sum+=fabsl(src[z][x][y])*dim->dX[x]*dim->dY[y]*dim->dZ[z];
			}

		}
	}

ret=sum/(in->zlen*in->xlen*in->ylen);
return ret;
}

long double three_d_integrate(struct dimensions *dim, long double ***src)
{
int x=0;
int y=0;
int z=0;
long double sum=0.0;

	for (z = 0; z < dim->zlen; z++)
	{
		for (x = 0; x < dim->xlen; x++)
		{
			for (y = 0; y < dim->ylen; y++)
			{
				sum+=src[z][x][y]*dim->dX[x]*dim->dY[y]*dim->dZ[z];
			}

		}
	}

return sum;
}

long double zxy_sum_gdouble(struct dimensions *dim, long double ***src)
{
int x=0;
int y=0;
int z=0;
long double sum=0.0;

	for (z = 0; z < dim->zlen; z++)
	{
		for (x = 0; x < dim->xlen; x++)
		{
			for (y = 0; y < dim->ylen; y++)
			{
				sum+=src[z][x][y];
			}

		}
	}

return sum;
}


void three_d_interpolate_gdouble(long double ***out, long double ***in, struct dimensions *dim_out, struct dimensions *dim_in)
{
int x=0;
int y=0;
int z=0;

int yi;
int xi;

long double y_out;
long double x_out;

long double y00;
long double y01;
long double yr;
long double y0;

long double y10;
long double y11;
long double y1;

long double x0;
long double x1;
long double xr;

long double c;

	z=0;
	for (x = 0; x < dim_out->xlen; x++)
	{

		x_out=dim_out->x[x];
		xi=hashget(dim_in->x,dim_in->xlen,x_out);

		for (y = 0; y < dim_out->ylen; y++)
		{
			y_out=dim_out->y[y];
			yi=hashget(dim_in->y,dim_in->ylen,y_out);

			y00=dim_in->y[yi];
			y01=dim_in->y[yi+1];
			yr=(y_out-y00)/(y01-y00);
			y0=in[z][xi][yi]+yr*(in[z][xi][yi+1]-in[z][xi][yi]);

			y10=dim_in->y[yi];
			y11=dim_in->y[yi+1];
			yr=(y_out-y10)/(y11-y10);
			y1=in[z][xi+1][yi]+yr*(in[z][xi+1][yi+1]-in[z][xi+1][yi]);

			x0=dim_in->x[xi];
			x1=dim_in->x[xi+1];
			xr=(x_out-x0)/(x1-x0);

			c=y0+xr*(y1-y0);
			out[z][x][y]=c;
		}

	}

}

void zx_y_quick_dump(char *file_name, long double ***in, struct dimensions *dim)
{
int x=0;
int y=0;
int z=0;
	FILE *out;
char full_name[200];

	for (y = 0; y < dim->ylen; y++)
	{
		sprintf(full_name,"%s.%d.dat",file_name,y);
		out=fopen(full_name,"w");

		for (z = 0; z < dim->zlen; z++)
		{

			for (x = 0; x < dim->xlen; x++)
			{


					fprintf(out,"%Le %Le %Le\n",dim->z[z],dim->x[x],in[z][x][y]);
				//}


			}
			fprintf(out,"\n");
			//fprintf(out,"\n\n");
		}
	fclose(out);

	}

}

void three_d_quick_dump(char *file_name, long double ***in, struct dimensions *dim)
{
int x=0;
int y=0;
int z=0;
	FILE *out=fopen(file_name,"w");

	for (z = 0; z < dim->zlen; z++)
	{

		for (x = 0; x < dim->xlen; x++)
		{

			//for (y = 0; y < dim->ylen; y++)
			//{
				fprintf(out,"%Le %Le %Le\n",dim->z[z],dim->x[x],in[z][x][2]);
			//}


		}
		fprintf(out,"\n");
		//fprintf(out,"\n\n");
	}

fclose(out);
}


void zxy_load_long_double(struct simulation *sim, struct dimensions *dim,long double **** data,char *file_name)
{
	char line[1000];
	FILE *file;
	int data_found=FALSE;
	int items_per_line=0;
	int x;
	int y;
	int z;
	long double x_val;
	long double y_val;
	long double z_val;
	long double val;
	long double ***dat=*data;
	struct dat_file d;
	dat_file_load_info(sim,&d,file_name);
	if ((d.x!=dim->xlen)||(d.y!=dim->ylen)||(d.z!=dim->zlen))
	{
		ewe(sim,"not matching dim should be (%d,%d,%d)\n",d.x,d.y,d.z);
	}

	//zxy_malloc_gdouble(dim, data);

	items_per_line++;

	if (d.y>1)
	{
		items_per_line++;
	}

	if (d.x>1)
	{
		items_per_line++;
	}

	if (d.z>1)
	{
		items_per_line++;
	}

	file=fopen(file_name,"r");
	x=0;
	y=0;
	z=0;
	int ret=0;
	do
	{
		memset(line,0,1000);
		ret=gpvdm_fgets(line, 1000, file);

		if (strcmp(line,"#end")==0)
		{
			break;
		}

		if (data_found==TRUE)
		{
			if (ret>0)
			{

				if (items_per_line==3)
				{
					sscanf(line,"%Le %Le %Le",&x_val,&z_val,&val);
					dat[z][x][y]=val;
					//printf("%Le %Le %Le %d %d %d\n",x_val,z_val,dat[z][x][y],z,x,y);
					//getchar();
				}else
				{
					ewe(sim,"I don't know how to read this type of file\n");
				}

				y++;
				if (y>=dim->ylen)
				{
					y=0;
					x++;
					if (x>=dim->xlen)
					{
						x=0;
						z++;
						if (z>=dim->zlen)
						{
							z=0;
						}
					}
				}



			}

		}

		if (strcmp(line,"#data")==0)
		{
			data_found=TRUE;
		}

	}while(!feof(file));
	fclose(file);
}

void flip_zxy_long_double_y(struct simulation *sim, struct dimensions *dim,long double *** data)
{
	int x=0;
	int y=0;
	int z=0;
	long double ***temp=NULL;

	malloc_zxy_long_double(dim, &temp);

	for (z=0;z<dim->zlen;z++)
	{

		for (x=0;x<dim->xlen;x++)
		{

			for (y=0;y<dim->ylen;y++)
			{
				temp[z][x][y]=data[z][x][y];
			}

		}
	}

	for (z=0;z<dim->zlen;z++)
	{
		for (x=0;x<dim->xlen;x++)
		{
			for (y=0;y<dim->ylen;y++)
			{
				data[z][x][dim->ylen-y-1]=temp[z][x][y];
			}
		}
	}


	free_zxy_long_double(dim, &temp);
}

//This shoudl be 3D interpolation but we are assuming the meshes are aligned.
long double interpolate_zxy_long_double(struct dimensions *dim, long double ***data,int z, int x, long double y_in)
{
	int y=0;
	long double x0=0.0;
	long double x1=0.0;
	long double y0=0.0;
	long double y1=0.0;

	long double ret;

	if (y_in<dim->y[0])
	{
		return 0.0;
	}


	if (y_in>=dim->y[dim->ylen-1])
	{
		//printf("here %Le %Le\n",y_in,dim->y[dim->ylen-1]);
		y=dim->ylen-1;
		x0=dim->y[y-1];
		x1=dim->y[y];
		y0=data[z][x][y-1];
		y1=data[z][x][y];

	}else
	{
		y=search(dim->y,dim->ylen,y_in);
		//printf("%d\n",y);
		x0=dim->y[y];
		x1=dim->y[y+1];

		y0=data[z][x][y];
		y1=data[z][x][y+1];
	}
	ret=y0+((y1-y0)/(x1-x0))*(y_in-x0);

return ret;

}
