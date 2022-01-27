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

/** @file remesh.c
	@brief Remesh the solver.
*/


#include "sim.h"
#include "dump.h"
#include "remesh.h"
#include "newton_tricks.h"
#include <mesh.h>
#include <memory.h>
#include <contacts.h>
#include <string.h>

//<stripall>

struct remesh my_mesh;

void align_to_contact(struct simulation *sim,struct device *in)
{
struct dimensions *dim=&in->ns.dim;
long double c0;
int x=0;
int i=0;

	for (x=0;x<dim->xlen-1;x++)
	{
		for (i=0;i<in->ncontacts;i++)
		{
			c0=in->contacts[i].shape.x0+in->contacts[i].shape.dx;
			if ((dim->x[x]<c0)&&(dim->x[x+1]>c0))
			{
				dim->x[x]=c0-2e-9;
				dim->x[x-1]=(dim->x[x-2]+dim->x[x])/2;
				dim->x[x+1]=(dim->x[x]+dim->x[x+2])/2;

			}

			//printf("%Le\n",dim->xmesh[x]);
		}
	}
}

void project_state(struct simulation *sim,struct device *dev,struct newton_state *out, struct newton_state *in)
{
	int band=0;
	//int x=0;
	//int z=0;

	three_d_interpolate_gdouble(out->phi, in->phi, &(out->dim),&(in->dim));

	three_d_interpolate_gdouble(out->x, in->x, &(out->dim),&(in->dim));
	three_d_interpolate_gdouble(out->xp, in->xp, &(out->dim),&(in->dim));

	for (band=0;band<out->dim.srh_bands;band++)
	{
		three_d_interpolate_srh(out->xt, in->xt, &(out->dim),&(in->dim),band);
		three_d_interpolate_srh(out->xpt, in->xpt, &(out->dim),&(in->dim),band);
	}


	newton_state_update_device(sim,dev,in);
/*	for (z=0;z<out->dim.zlen;z++)
	{
		for (x=0;x<out->dim.xlen;x++)
		{
			update_y_array(sim,dev,z,x);
		}
	}*/
}

void remesh_shrink(struct simulation *sim,struct device *in)
{

	if ((in->mesh_data.meshdata_x.tot_points*in->mesh_data.meshdata_y.tot_points)<20)
	{
		in->dynamic_mesh=FALSE;
	}

	if (strcmp(in->newton_name,"newton_2d")!=0)
	{
		in->dynamic_mesh=FALSE;
	}

	

	if (in->dynamic_mesh==TRUE)
	{
		printf("mesh shrink\n");
		newton_state_cpy(&(in->ns_save),&(in->ns));
		mesh_obj_cpy(sim,&(in->mesh_data_save),&(in->mesh_data));
		mesh_free(&(in->mesh_data.meshdata_x));
		mesh_gen_simple(sim, &(in->mesh_data.meshdata_x),in->xlen,18);
		mesh_to_dim(sim,&(in->ns.dim), &(in->mesh_data.meshdata_x),'x');

		project_state(sim,in,&(in->ns), &(in->ns_save));
		mesh_numerate_points(sim,in);
		contacts_update(sim,in);
		get_initial(sim,in,FALSE);

	}

}

void remesh_expand_array_band(gdouble **y,int band,struct device *in)
{
	int i;
	int pos;
	gdouble *temp=(gdouble*)malloc(sizeof(gdouble)*in->ylen);
	for (i=0;i<in->ylen;i++)
	{
		temp[i]=y[i][band];
	}

	for (i=0;i<my_mesh.len;i++)
	{

		pos=in->ylen*(my_mesh.x[i]/in->ylen);
		if (pos>=in->ylen) pos=in->ylen-1;

		y[i][band]=temp[pos];
	}

	free(temp);
}


//three_d_quick_dump("phi_corse.dat", in->ns_save.phi, &(in->dim_save));
//srh_quick_dump("srh_corse.dat", in->ns_save.xt, &(in->dim_save),0);

//three_d_quick_dump("phi_fine.dat", in->ns.phi, &(in->dim));
//srh_quick_dump("srh_fine.dat", in->ns.xt, &(in->dim),0);

//sim_externalv(sim,in,voltage);
//sim_externalv(sim,in,voltage);
//three_d_quick_dump("phi_polish.dat", in->ns.phi, &(in->dim));
//srh_quick_dump("srh_polish.dat", in->ns.xt, &(in->dim),0);

void remesh_reset(struct simulation *sim,struct device *in,gdouble voltage)
{
	//in->dynamic_mesh=FALSE;
	if (in->dynamic_mesh==TRUE)
	{

		//srh_quick_dump("xt_corse.dat", in->ns.xt, &(in->ns.dim),0);
		//srh_quick_dump("xpt_corse.dat", in->ns.xpt, &(in->ns.dim),0);

		int x=in->mesh_data.meshdata_x.tot_points;
		int stop=FALSE;
		while (1)
		{
			three_d_quick_dump("phi_corse.dat", in->ns.phi, &(in->ns.dim));
			three_d_quick_dump("x_corse.dat", in->ns.x, &(in->ns.dim));
			three_d_quick_dump("xp_corse.dat", in->ns.xp, &(in->ns.dim));

			project_state(sim,in, &(in->ns_save), &(in->ns));
			mesh_obj_cpy(sim,&(in->mesh_data),&(in->mesh_data_save));
			mesh_free(&(in->mesh_data.meshdata_x));
			mesh_gen_simple(sim, &(in->mesh_data.meshdata_x),in->xlen,x);
			mesh_to_dim(sim,&(in->ns.dim), &(in->mesh_data.meshdata_x),'x');
			align_to_contact(sim,in);
			//
			project_state(sim,in, &(in->ns), &(in->ns_save));

			mesh_numerate_points(sim,in);
			contacts_update(sim,in);
			get_initial(sim,in,FALSE);

			newton_push_state(in);
			in->max_electrical_itt=1000;
			in->newton_min_itt=10;
			in->electrical_clamp=4.0;
			three_d_quick_dump("phi_initial.dat", in->ns.phi, &(in->ns.dim));
			three_d_quick_dump("x_initial.dat", in->ns.x, &(in->ns.dim));
			three_d_quick_dump("xp_initial.dat", in->ns.xp, &(in->ns.dim));
			//srh_quick_dump("xt_initial.dat", in->ns.xt, &(in->ns.dim),0);
			//srh_quick_dump("xpt_initial.dat", in->ns.xpt, &(in->ns.dim),0);

			newton_sim_simple(sim,in);
			three_d_quick_dump("phi_refine.dat", in->ns.phi, &(in->ns.dim));
			three_d_quick_dump("x_refine.dat", in->ns.x, &(in->ns.dim));
			three_d_quick_dump("xp_refine.dat", in->ns.xp, &(in->ns.dim));
			//srh_quick_dump("xt_refine.dat", in->ns.xt, &(in->ns.dim),0);
			//srh_quick_dump("xpt_refine.dat", in->ns.xpt, &(in->ns.dim),0);

			newton_pop_state(in);

			//x+=5;
			printf("%d %Le %d\n",x,in->ns.last_error,in->ns.last_ittr);

			if (stop==TRUE)
			{
				break;
			}

			x+=1;
			if (x>30)
			{
				x+=2;
			}

			if (x>=in->mesh_data_save.meshdata_x.tot_points)
			{
				x=in->mesh_data_save.meshdata_x.tot_points;
				stop=TRUE;
			}

			//getchar();
			//getchar();
		}
		//getchar();
		//newton_state_cpy(&(in->ns),&(in->ns_save));

		printf("Fine tuning\n");

		project_state(sim,in, &(in->ns_save), &(in->ns));
		newton_state_cpy(&(in->ns_save),&(in->ns));

		mesh_obj_cpy(sim,&(in->mesh_data),&(in->mesh_data_save));
		mesh_numerate_points(sim,in);
		contacts_update(sim,in);
		get_initial(sim,in,FALSE);

		newton_push_state(in);
		in->max_electrical_itt=1000;
		in->newton_min_itt=10;
		in->electrical_clamp=1.0;
		three_d_quick_dump("phi_initial.dat", in->ns.phi, &(in->ns.dim));
		three_d_quick_dump("x_initial.dat", in->ns.x, &(in->ns.dim));
		three_d_quick_dump("xp_initial.dat", in->ns.xp, &(in->ns.dim));
		//srh_quick_dump("xt_initial.dat", in->ns.xt, &(in->ns.dim),0);
		//srh_quick_dump("xpt_initial.dat", in->ns.xpt, &(in->ns.dim),0);

		newton_sim_simple(sim,in);
		three_d_quick_dump("phi_refine.dat", in->ns.phi, &(in->ns.dim));
		three_d_quick_dump("x_refine.dat", in->ns.x, &(in->ns.dim));
		three_d_quick_dump("xp_refine.dat", in->ns.xp, &(in->ns.dim));
		//srh_quick_dump("xt_refine.dat", in->ns.xt, &(in->ns.dim),0);
		//srh_quick_dump("xpt_refine.dat", in->ns.xpt, &(in->ns.dim),0);

		newton_pop_state(in);
		printf("finished\n");
		//getchar();
	}
}
