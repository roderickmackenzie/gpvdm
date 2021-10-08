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
#include <ray_fun.h>
#include <util.h>

/** @file ray.c
	@brief Ray tracing for the optical model, this should really be split out into it's own library.
*/


int between(double v, double x0, double x1)
{
	double min=0.0;
	double max=0.0;
	if (x0>x1)
	{
		min=x1;
		max=x0;
	}else
	{
		min=x0;
		max=x1;
	}

	if ((v-min)>=-1e-12)
	{
		if ((v-max)<=1e-12)
		{
			return 0;
		}
	}

	return -1;
}


void ray_reset(struct ray_worker *worker)
{
	worker->nrays=0;
	worker->top_of_done_rays=0;
}

int add_ray(struct simulation *sim,struct ray_worker *worker,struct vec *start,struct vec *dir,double mag,int obj_uid,int parent)
{
	int ret=-1;

return ret;
}




int activate_rays(struct ray_worker *worker)
{
	int i=0;
	int changed=0;
	int found=FALSE;
	for (i=worker->top_of_done_rays;i<worker->nrays;i++)
	{
		if (worker->rays[i].state==WAIT)
		{
			worker->rays[i].state=READY;
			changed++;
		}

		if ((found==FALSE)&&(worker->rays[i].state!=DONE))
		{
			worker->top_of_done_rays=i;
			found=TRUE;
		}

	}

return changed;
}

int get_objects_from_tri(struct simulation *sim,struct device *dev,struct image *in,struct object **obj0,struct object **obj1,struct triangle *tri,struct ray *my_ray)
{
	struct world *w=&(dev->w);

	if ((my_ray->obj_uid_start==tri->obj_left)||(my_ray->obj_uid_start==tri->obj_right))
	{
		if (tri->obj_left==-1)
		{
			return -1;
		}

		if (tri->obj_right==-1)
		{
			return -1;
		}

		if (my_ray->obj_uid_start==tri->obj_left)
		{
			*obj0=&(w->obj[tri->obj_left]);
			*obj1=&(w->obj[tri->obj_right]);
		}else
		{
			*obj0=&(w->obj[tri->obj_right]);
			*obj1=&(w->obj[tri->obj_left]);
		}
	}else
	{
		printf("%s %s %s\n",w->obj[my_ray->obj_uid_start].name,w->obj[tri->obj_left].name,w->obj[tri->obj_right].name);
		triangle_print(tri);

		triangle_dump("tri.dat",tri);
		getchar();
		//ewe(sim,"oops\n");
	}
	//printf("%d %d\n",tri->obj_left,tri->obj_right);
	//printf("%d\n",my_ray->obj_uid_start);

return 0;
}

int get_objects(struct simulation *sim,struct device *dev,struct image *in,struct object **obj0,struct object **obj1,struct ray *my_ray)
{
	//printf("in\n");
	struct vec tmp;
	vec_init(&tmp);
	struct world *w=&(dev->w);
	//struct ray back;
	/*if (my_ray->obj_uid_start==-1)		//If the ray does not know where it is don't search
	{
		ray_cpy(&back,my_ray);
		vec_cpy(&tmp,&(my_ray->dir));
		vec_mul(&tmp,1e-10);
		vec_cpy(&(back.xy),&(my_ray->xy_end));
		vec_sub(&(back.xy),&(tmp));

		*obj0=ray_obj_search(sim,in,&back);
		if (*obj0==NULL)
		{
			return -1;
		}

	}
	else
	{*/
		*obj0=&(w->obj[my_ray->obj_uid_start]);
	//}

	struct ray fwd;
	ray_cpy(&fwd,my_ray);

	vec_cpy(&tmp,&(my_ray->dir));
	vec_mul(&tmp,1e-10);
	vec_cpy(&(fwd.xy),&(my_ray->xy_end));
	vec_add(&(fwd.xy),&(tmp));

	*obj1=ray_obj_search(sim,dev,&fwd);
	if (*obj1==NULL)
	{
		return -1;
	}

	//vec_print(&(my_ray->xy_end));
	//printf("obj0=%s \n",(*obj0)->name);
	//printf("obj1=%s\n",(*obj1)->name);
	//getchar();

	//if (my_ray->obj_uid_start!=-1)
	//{
	//	if (my_ray->obj_uid_start!=(*obj0)->uid)
	//	{
			//dump_ray_to_file(sim,in,my_ray);
			//dump_ray_to_file(sim,in,&(in->rays[my_ray->parent]));
			//printf("I would have saved a lookup %d %d\n",my_ray->obj_uid_start,(*obj0)->uid);
			//printf("I would have saved a lookup %s %s\n",dev->obj[my_ray->obj_uid_start].name,dev->obj[(*obj0)->uid].name);

			//printf("fuck!");
			//exit(0);
		//}//else
		//{
		//	printf("yes\n");
		//}
	//}

/*	if (my_ray->obj_uid_start!=-1)		//If the ray does not know where it is don't search
	{
		printf("I would have saved a lookup %d %d\n",my_ray->obj_uid_start,(*obj0)->uid);
	}else
	{
		printf("work\n");
	}*/

	return 0;

}

int propergate_next_ray(struct simulation *sim,struct device *dev,struct image *in,struct ray_worker *w)
{
return 0;
}


