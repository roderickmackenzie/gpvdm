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

#include <stdio.h>
#include <ray.h>
#include <gpvdm_const.h>
#include <math.h>
#include <stdlib.h>
#include <cal_path.h>
#include <log.h>
#include <dat_file.h>
#include <string.h>
#include <util.h>
#include <triangles.h>
#include <vectors.h>

/** @file triangles_reduce.c
	@brief Reduce the triangle count in a mesh
*/
int eq(double a,double b)
{
	double test=fabsl(a-b);
	if (test<1e-10)
	{
		return 1;
	}

	return 0;
}

int triangles_find_connecting_triangles(struct triangles *in,struct triangle **tri0,struct triangle **tri1,struct triangle **tri2,struct triangle *tri_orig)
{
	int i;
	int found=0;
	int sides=0;
	struct triangle *ret=NULL;
	struct triangle *tri;
	struct triangle build[3];
	sides=0;

	for (i=0;i<in->len;i++)
	{
		tri=&(in->data[i]);
		if (tri->deleted==FALSE)
		{
			if (tri->object_uid!=tri_orig->object_uid)
			{

				if (triangle_contains_zx_line(tri,&(tri_orig->xy0),&(tri_orig->xy1))==0)
				{
					*tri0=tri;
					sides++;
				}

				if (triangle_contains_zx_line(tri,&(tri_orig->xy1),&(tri_orig->xy2))==0)
				{
					*tri1=tri;
					sides++;
				}

				if (triangle_contains_zx_line(tri,&(tri_orig->xy2),&(tri_orig->xy0))==0)
				{
					*tri2=tri;
					sides++;
				}

			}

			//printf("%d\n",sides);
		}

		if (sides==3)
		{
			return 0;
		}
	}


	return -1;
}


int triangles_reduce_triangle(struct simulation *sim,struct triangles *in,int forwards)
{	
	int i;
	char temp[1000];
	struct triangle *tri;
	struct triangle *tri0;
	struct triangle *tri1;
	struct triangle *tri2;
	struct vec *tri0_point;
	struct vec *tri1_point;
	struct vec *tri2_point;
	int found;

	struct vec vec_max;
	struct vec vec_min;

	double height0=0.0;
	double height1=0.0;
	double height2=0.0;

	triangles_calculate_cog(in);
	triangles_find_max(&vec_max,in);
	triangles_find_min(&vec_min,in);

	int start_triangle_count=triangles_count(in);
	struct triangles out_list;
	struct triangles reduced_triangles;
	FILE *out;
	int replace=FALSE;
	i=0;

	if (forwards==FALSE)
	{
		i=in->len-1;
	}

	struct vectors vecs;
	vectors_init(&vecs);
	while(1)
	{
		tri=&(in->data[i]);

		found=triangles_find_connecting_triangles(in,&tri0,&tri1,&tri2,tri);


		if (found==0)
		{
			height0=(tri0->cog.y-vec_min.y)/(vec_max.y-vec_min.y);
			height1=(tri1->cog.y-vec_min.y)/(vec_max.y-vec_min.y);
			height2=(tri2->cog.y-vec_min.y)/(vec_max.y-vec_min.y);

			replace=FALSE;
			//printf("%lf %lf\n",vec_min.y,vec_max.y);
			if (fabs(height1-height0)==0.0)
			{
				if (fabs(height2-height0)==0.0)
				{
					replace=TRUE;
				}
			}

			if (replace==TRUE)
			{

				tri0_point=triangle_get_furthest_point(tri0,&(tri->cog));
				tri1_point=triangle_get_furthest_point(tri1,&(tri->cog));
				tri2_point=triangle_get_furthest_point(tri2,&(tri->cog));

				vec_cpy(&(tri->xy0),tri0_point);
				vec_cpy(&(tri->xy1),tri1_point);
				vec_cpy(&(tri->xy2),tri2_point);
				tri0->deleted=TRUE;
				tri1->deleted=TRUE;
				tri2->deleted=TRUE;
				triangle_cal_cog(tri);

				out=fopen("big_tri.dat","w");
				fprintf(out,"%le %le 0.0\n",tri0_point->z,tri0_point->x);
				fprintf(out,"%le %le 0.0\n",tri1_point->z,tri1_point->x);
				fprintf(out,"%le %le 0.0\n",tri2_point->z,tri2_point->x);
				fprintf(out,"%le %le 0.0\n",tri0_point->z,tri0_point->x);
				fclose(out);
			}





			//triangles_save(sim,"all_tri.dat",in);
			//getchar();
		}
		
		if (forwards==TRUE)
		{
			i++;
			if (i==in->len)
			{
				break;
			}
		}else
		{
			i--;
			if (i==-1)
			{
				break;
			}
		}

	}
	int end_triangle_count=triangles_count(in);
	printf("%d %d\n",end_triangle_count,start_triangle_count);
	triangles_save(sim,"all_tri.dat",in);

	return start_triangle_count-end_triangle_count;
}


void triangles_reduce(struct simulation *sim,struct triangles *in, double min_allowable_ang)
{
	int i;
	//triangles_reduce_nodes(sim,in);
	//return;

	/*while(1)
	{
		if (triangles_reduce_triangle(sim,in,TRUE)==0)
		{
			break;
		}

		if (triangles_reduce_triangle(sim,in,FALSE)==0)
		{
			break;
		}
		//return;

	}*/

	for (i=0;i<20;i++)
	{
		if (triangles_reduce_nodes(sim,in,i,min_allowable_ang)==0)
		{
			break;
		}
		//return;

	}

}
			/*out=fopen("tri.dat","w");
			fprintf(out,"%le %le 0.0\n",tri->xy0.z,tri->xy0.x);
			fprintf(out,"%le %le 0.0\n",tri->xy1.z,tri->xy1.x);
			fprintf(out,"%le %le 0.0\n",tri->xy2.z,tri->xy2.x);
			fprintf(out,"%le %le 0.0\n",tri->xy0.z,tri->xy0.x);
			fclose(out);

			out=fopen("tri0.dat","w");
			fprintf(out,"%le %le 0.0\n",tri0->xy0.z,tri0->xy0.x);
			fprintf(out,"%le %le 0.0\n",tri0->xy1.z,tri0->xy1.x);
			fprintf(out,"%le %le 0.0\n",tri0->xy2.z,tri0->xy2.x);
			fprintf(out,"%le %le 0.0\n",tri0->xy0.z,tri0->xy0.x);
			fclose(out);

			out=fopen("tri1.dat","w");
			fprintf(out,"%le %le 0.0\n",tri1->xy0.z,tri1->xy0.x);
			fprintf(out,"%le %le 0.0\n",tri1->xy1.z,tri1->xy1.x);
			fprintf(out,"%le %le 0.0\n",tri1->xy2.z,tri1->xy2.x);
			fprintf(out,"%le %le 0.0\n",tri1->xy0.z,tri1->xy0.x);
			fclose(out);

			out=fopen("tri2.dat","w");
			fprintf(out,"%le %le 0.0\n",tri2->xy0.z,tri2->xy0.x);
			fprintf(out,"%le %le 0.0\n",tri2->xy1.z,tri2->xy1.x);
			fprintf(out,"%le %le 0.0\n",tri2->xy2.z,tri2->xy2.x);
			fprintf(out,"%le %le 0.0\n",tri2->xy0.z,tri2->xy0.x);
			fclose(out);*/

