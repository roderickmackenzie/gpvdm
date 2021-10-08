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
#include <gui_hooks.h>

/** @file triangles_reduce_nodes.c
	@brief A mesh reduction algorithm
*/

int triangles_simplify_node(struct simulation *sim,struct triangles *in, struct vec *remove, double min_allowable_ang)
{
	int t;
	double A_orig=0.0;
	double A_new=0.0;
	double delta_y=0.0;
	double delta_A=0.0;
	int removed=-1;
	struct triangles out_list;
	struct triangles reduced_triangles;
	struct vec *replace_with;
	struct vectors vec_list;

	struct vec vec_max;
	struct vec vec_min;

	triangles_calculate_cog(in);
	triangles_find_max(&vec_max,in);
	triangles_find_min(&vec_min,in);

	vectors_init(&vec_list);

	double height=vec_max.y-vec_min.y;
	double min_ang=0.0;

	//printf("%le %le\n",vec_max.y,vec_min.y);
	//getchar();
	triangles_init(&out_list);
	triangles_init(&reduced_triangles);

	triangles_malloc(&out_list);
	triangles_malloc(&reduced_triangles);

	triangles_find_by_vec(	&out_list, in, remove);
	int v;
	if (out_list.len!=0)
	{
		for (v=0;v<3;v++)
		{
			for (t=0;t<out_list.len;t++)
			{
				vectors_malloc(&vec_list);
				A_orig=triangles_get_area_zx(&out_list);
				//printf("%d\n",out_list.len);
				triangles_cpy(&reduced_triangles,&out_list);
				reduced_triangles.data[t].deleted=TRUE;

				if (v==0)
				{
					replace_with=&(reduced_triangles.data[t].xy0);
				}else
				if (v==1)
				{
					replace_with=&(reduced_triangles.data[t].xy1);
				}else
				{
					replace_with=&(reduced_triangles.data[t].xy2);
				}

				triangles_replace_vec(&reduced_triangles,remove,replace_with);
				triangles_delete_zero_area_triangles(&reduced_triangles);
				triangles_calculate_cog(&reduced_triangles);
				A_new=triangles_get_area_zx(&reduced_triangles);
				//printf("area=%le %le %le\n",A_new,A_orig,A_new-A_orig);

				//printf("%le\n",delta_y);
				delta_A=A_new-A_orig;
				//printf("%le %le\n",delta_y,height);
	
				if (delta_A==0.0)
				{
					triangles_cal_angles(&vec_list,&reduced_triangles);
					min_ang=vectors_min_value(&vec_list);
					if (min_ang>min_allowable_ang)
					{

						delta_y=triangles_get_delta(&out_list,&reduced_triangles);
						if ((delta_y/height)<=0.02)
						{

							triangles_remove_triangles(in, &out_list);
							//printf("OK!!!! %d\n",triangles_count(in));
							triangles_set_flag(&reduced_triangles,1);
							triangles_add_many(in, &reduced_triangles);
							removed=0;
							break;
						}
					}
				}
				triangles_free(&reduced_triangles);
				vectors_free(&vec_list);

			}

			if (removed==0)
			{
				break;
			}

		}

	}

	triangles_free(&out_list);

	return removed;
}
int triangles_reduce_nodes(struct simulation *sim,struct triangles *in, int pass, double min_allowable_ang)
{	
	int i;
	char temp[1000];
	char send_data[1000];
	struct triangle *tri;
	struct triangle *tri0;
	struct triangle *tri1;
	struct triangle *tri2;
	struct vec *tri0_point;
	struct vec *tri1_point;
	struct vec *tri2_point;
	int found;
	int end_triangle_count;


	int start_triangle_count=triangles_count(in);

	FILE *out;
	int replace=FALSE;
	i=0;


	//struct vectors vecs;
	//vectors_init(&vecs);

	//printf("here\n");

	for(i=0;i<in->len;i++)
	{
		triangles_set_flag(in,0);
		tri=&(in->data[i]);

		if (tri->flag==0)
		{
			if (triangles_simplify_node(sim,in,&(tri->xy0),min_allowable_ang)==0)
			{
				tri->flag=1;
			}
		}

		if (tri->flag==0)
		{
			if (triangles_simplify_node(sim,in,&(tri->xy1),min_allowable_ang)==0)
			{
				tri->flag=1;
			}
		}

		if (tri->flag==0)
		{
			if (triangles_simplify_node(sim,in,&(tri->xy2),min_allowable_ang)==0)
			{
				tri->flag=1;
			}
		}
		//getchar();

		end_triangle_count=triangles_count(in);
		if ((i%25)==0)
		{
			sprintf(send_data,"text:Pass %d removed %d/%d",pass,start_triangle_count-end_triangle_count,end_triangle_count);
			gui_send_data(sim,gui_sub,send_data);

			sprintf(send_data,"percent:%Lf",(long double)i/in->len);
			gui_send_data(sim,gui_sub,send_data);
		}
	}
	//triangles_save(sim,"patch_removed.dat",in);

	//printf("%d %d\n",end_triangle_count,start_triangle_count);
	//triangles_save(sim,"all_tri.dat",in);



	return start_triangle_count-end_triangle_count;
}


