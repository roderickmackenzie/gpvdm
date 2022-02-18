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
#include <lang.h>
#include <shape.h>
#include <epitaxy_struct.h>
#include <epitaxy.h>
#include <device_fun.h>
#include <ray_fun.h>
#include <json.h>

void mesh_cpy(struct simulation *sim,struct mesh *out,struct mesh *in)
{
	int i;
	int ii;

	mesh_free(out);

	out->nlayers= in->nlayers;
	out->remesh= in->remesh;


	out->layers = malloc (out->nlayers * sizeof(struct mesh_layer));

	for (i=0;i<out->nlayers;i++)
	{
		out->layers[i].len=in->layers[i].len;
		out->layers[i].n_points=in->layers[i].n_points;
		out->layers[i].mul=in->layers[i].mul;
		out->layers[i].left_right=in->layers[i].left_right;
		out->layers[i].dx=in->layers[i].dx;
		out->layers[i].start=in->layers[i].start;
		out->layers[i].end=in->layers[i].end;
	}


	for (i=0;i<out->nlayers;i++)
	{
		out->layers[i].n_points=in->layers[i].n_points;

		out->layers[i].dmesh=malloc ( out->layers[i].n_points * sizeof(long double));
		//printf("malloc %x\n",in->layers[0].dmesh);
		for (ii=0;ii>out->layers[i].n_points;ii++)
		{
			out->layers[i].dmesh[ii]=in->layers[i].dmesh[ii];
		}

	}

	out->tot_points=in->tot_points;

}

void mesh_check_y(struct simulation *sim,struct mesh *in,struct device *dev)
{
int y=0;
gdouble mesh_len=0.0;

	if (dev->circuit_simulation==TRUE)
	{
		return;
	}

	for (y=0;y<in->nlayers;y++)
	{
		mesh_len+=in->layers[y].len;
	}

	if (fabs(dev->ylen-mesh_len)>1e-14)
	{
		printf_log(sim,"calling remesh %Le %Le\n",dev->ylen,mesh_len);
		//getchar();
		mesh_remesh(sim,in,dev);
		printf_log(sim,"Warning: Length of epitaxy and computational mesh did not match, so I remesshed the device.\n");
	}
}

void mesh_remesh(struct simulation *sim,struct mesh *in,struct device *dev)
{
	//char device_file_path[1000];

	//if (in->remesh==TRUE)
	//{

		/*in->nlayers=1;
		in->layers[0].len=dev->ylen;
		if (dev->my_epitaxy.electrical_layers==1)
		{
			in->layers[0].n_points=10;
		}else
		{
			in->layers[0].n_points=40;
		}
		in->layers[0].mul=1.0;
		in->layers[0].left_right=LEFT;
		mesh_save(sim,"mesh_y.inp",in);

		device_free(sim,dev);
		mesh_obj_free(sim,&(dev->mesh_data));

		//join_path(2,device_file_path,get_input_path(sim),"epitaxy.inp");	//This is not obvious
		//epitaxy_free(sim,&(dev->my_epitaxy));
		//epitaxy_load(sim,&(dev->my_epitaxy),device_file_path);

		mesh_obj_load(sim,&(dev->mesh_data));

		dev->ns.dim.zlen=dev->mesh_data.meshdata_z.tot_points;
		dev->ns.dim.xlen=dev->mesh_data.meshdata_x.tot_points;
		dev->ns.dim.ylen=dev->mesh_data.meshdata_y.tot_points;

		device_get_memory(sim,dev);
		mesh_build(sim,dev);
		//printf("remesh\n");
		//getchar();
	//}else*/
	//{
		ewe(sim,"%s\n",_("The electrical mesh does not match the device length"));
	//}


}

/*void mesh_save(struct simulation *sim,char *file_name,struct mesh *in)
{
	int i=0;
	char buffer[2000];
	char temp[2000];
	char full_file_name[200];

	strcpy(buffer,"");
	strcat(buffer,"#remesh_enable\n");
	strcat(buffer,"True\n");
	strcat(buffer,"#mesh_layers\n");

	sprintf(temp,"%d\n",in->nlayers);
	strcat(buffer,temp);

	for (i=0;i<in->nlayers;i++)
	{
		strcat(buffer,"#mesh_layer_length0\n");

		sprintf(temp,"%Le\n",in->layers[i].len);
		strcat(buffer,temp);

		strcat(buffer,"#mesh_layer_points0\n");

		sprintf(temp,"%d\n",(int)(in->layers[i].n_points));
		strcat(buffer,temp);

		strcat(buffer,"#mesh_layer_mul0\n");

		sprintf(temp,"%d\n",(int)(in->layers[i].mul));
		strcat(buffer,temp);

		strcat(buffer,"#mesh_layer_left_right0\n");

		sprintf(temp,"%d\n",(int)(in->layers[i].left_right));
		strcat(buffer,temp);
	}

	strcat(buffer,"#ver\n");
	strcat(buffer,"1.0\n");
	strcat(buffer,"#end\n");

	join_path(2,full_file_name,get_input_path(sim),file_name);
	printf_log(sim,"Write new mesh to: %s\n",full_file_name);
	zip_write_buffer(sim,full_file_name,buffer, strlen(buffer));

}*/

void mesh_free(struct mesh *in)
{
	int i=0;
	//printf("free\n");
	//getchar();
	if (in->nlayers!=-1)
	{
		for (i=0;i<in->nlayers;i++)
		{
			if (in->layers[i].dmesh!=NULL)
			{
				free(in->layers[i].dmesh);
			}
		}

		free(in->layers);
		mesh_init(in);
	}


}

void mesh_init(struct mesh *in)
{
	in->layers= NULL;
	in->nlayers=-1;
	in->remesh=-1;
	in->start=0.0;
}


void mesh_malloc_sub_mesh(struct simulation * sim, struct mesh *in)
{
	int i;
	long double pos;

	pos=0.0;
	int points=0;
	long double dx=0.0;

	for (i=0;i<in->nlayers;i++)
	{
		dx=in->layers[i].len/((long double)in->layers[i].n_points);
		pos=0.0;
		in->layers[i].n_points=0;
		while(pos<in->layers[i].len)
		{
			pos+=dx/2.0;
			//printf("%Le %Le\n",pos,(*meshdata)[i].len);
			//getchar();
			if (pos>in->layers[i].len)
			{
				break;
			}

			in->layers[i].n_points++;
			points++;
			pos+=dx/2.0;
			dx=dx*in->layers[i].mul;
		}

		if (in->layers[i].n_points!=0)
		{
			in->layers[i].dmesh=malloc ( in->layers[i].n_points * sizeof(long double));
		}
	}

	in->tot_points=points;

}

void mesh_gen_simple(struct simulation * sim, struct mesh *in,long double len,int points)
{
	int i;

	if (points<=0)
	{
		ewe(sim,"%s\n",_("Can't generate a mesh with zero points"));
	}
	in->remesh=FALSE;
	in->nlayers=1;

	in->layers = malloc (in->nlayers * sizeof(struct mesh_layer));

	i=0;
	in->layers[i].len=len;
	in->layers[i].n_points=points;
	in->layers[i].dx=in->layers[i].len/((long double)in->layers[i].n_points);
	in->layers[i].mul=1.0;
	in->layers[i].left_right=TRUE;


	mesh_malloc_sub_mesh(sim, in);

}

void mesh_gen_graded(struct simulation * sim, struct mesh *in,long double len,int points)
{
	int i;

	in->remesh=FALSE;
	in->nlayers=2;
	len/=2.0;

	in->layers = malloc (in->nlayers * sizeof(struct mesh_layer));

	i=0;
	in->layers[i].len=len;
	in->layers[i].n_points=points;
	in->layers[i].dx=in->layers[i].len/((long double)in->layers[i].n_points);
	in->layers[i].mul=1.1;
	in->layers[i].left_right=FALSE;

	i++;
	in->layers[i].len=len;
	in->layers[i].n_points=points;
	in->layers[i].dx=in->layers[i].len/((long double)in->layers[i].n_points);
	in->layers[i].mul=1.1;
	in->layers[i].left_right=TRUE;

	mesh_malloc_sub_mesh(sim, in);

}

void mesh_layer_init(struct simulation * sim, struct mesh_layer *layer )
{
	layer->dx=-1.0;
	layer->len=-1.0;
	layer->mul=-1.0;
	layer->dmesh=NULL;
	layer->n_points=0;
	layer->left_right=FALSE;
	layer->start_at_edge_left=FALSE;
	layer->start_at_edge_right=FALSE;
}



void mesh_load_file(struct simulation * sim, struct mesh *in,struct json_obj *mesh_xyz)
{
	int i;
	char temp[200];
	struct json_obj *json_seg;

	json_get_int(sim, mesh_xyz, &in->nlayers,"segments");

	in->layers = malloc (in->nlayers * sizeof(struct mesh_layer));

	for (i=0;i<in->nlayers;i++)
	{
		sprintf(temp,"segment%d",i);
		json_seg=json_obj_find(mesh_xyz, temp);
		if (json_seg==NULL)
		{
			ewe(sim,"Object light not found\n");
		}
		mesh_layer_init(sim, &(in->layers[i]));

		json_get_long_double(sim, json_seg, &(in->layers[i].len),"len");
		json_get_int(sim, json_seg, &(in->layers[i].n_points),"points");

		in->layers[i].dx=in->layers[i].len/((long double)in->layers[i].n_points);

		json_get_long_double(sim, json_seg, &(in->layers[i].mul),"mul");
		json_get_english(sim, json_seg, &(in->layers[i].left_right),"left_right");

		in->layers[i].len=fabs(in->layers[i].len);

	}

	mesh_malloc_sub_mesh(sim, in);

}


long double mesh_to_lin_array(struct simulation *sim,long double *mesh, long double *dmesh, struct mesh *in)
{
	int pos=0;
	int i=0;
	int ii=0;
	long double dpos=0.0;
	long double dx=0.0;
	long double len=0.0;
	long double ret_len=0.0;

	//Build the sub meshes dmesh segments assuming left to right multiplication
	len=0.0;
	for (i=0;i<in->nlayers;i++)
	{
		pos=0;
		dpos=0.0;
		dx=in->layers[i].dx;
		//printf("going to build %d %Le\n",in->layers[i].n_points,in->layers[i].dx);
		//getchar();
		for (ii=0;ii<in->layers[i].n_points;ii++)
		{
			dpos+=dx/2.0;
			//printf("%d %d %d %x\n",i,ii,in->layers[i].n_points,in->layers[i].dmesh);
			in->layers[i].dmesh[ii]=dpos;
			dpos+=dx/2.0;
			dx*=in->layers[i].mul;
			pos++;
		}
		len+=in->layers[i].len;
	}

	ret_len=len;

	//Build the mesh forwards or backwards
	len=in->start;
	pos=0;
	for (i=0;i<in->nlayers;i++)
	{
		for (ii=0;ii<in->layers[i].n_points;ii++)
		{
			//printf("%d %d %d %Le\n",pos,in->layers[i].n_points,in->nlayers,in->layers[i].len);
			/*if ((in->layers[i].start_at_edge_left==TRUE)&&(ii==0))		//Insert extra point
			{
				printf("yes\n");
				getchar();
				mesh[pos]=0.0;
				pos++;
			}*/

			if (in->layers[i].left_right==LEFT)
			{
				mesh[pos]=len+in->layers[i].dmesh[ii];
			}else
			{
				mesh[pos]=len+in->layers[i].len-in->layers[i].dmesh[in->layers[i].n_points-1-ii];
			}
			//printf("%d %d\n",pos,in->layers[i].n_points);
			//printf("%c %ld %Le %d %d %ld\n",direction,pos,mesh[pos],i,ii,meshdata[i].n_points);
			pos++;
		}
		in->layers[i].start=len;
		len+=in->layers[i].len;
		in->layers[i].end=len;

	}

	//rebuild dmesh from main mesh
	long double last=in->start;
	long double next=0.0;
	int sub_mesh_pos=0;
	struct mesh_layer *layer;
	pos=0;

	for (i=0;i<in->nlayers;i++)
	{
		layer=&(in->layers[i]);

		for (sub_mesh_pos=0;sub_mesh_pos<layer->n_points;sub_mesh_pos++)
		{
			if (sub_mesh_pos==(layer->n_points-1))
			{
				next=layer->end;
			}else
			{
				next=(mesh[pos]+mesh[pos+1])/2.0;
			}

			if (sub_mesh_pos==0)
			{
				last=layer->start;
			}

			dx=next-last;
			dmesh[pos]=dx;
			last=next;
			pos++;
		}
	}


	return ret_len;
}

long double mesh_to_dim(struct simulation *sim,struct dimensions *dim, struct mesh *in,char xyz)
{

	long double *mesh;
	long double *dmesh;
	long double ret_len=0.0;

	dim_free_xyz(dim,xyz);

	if (xyz=='x')
	{
		dim->xlen=in->tot_points;
		dim_malloc_xyz(dim,'x');
		mesh=dim->x;
		dmesh=dim->dX;
	}else
	if (xyz=='y')
	{

		dim->ylen=in->tot_points;
		dim_malloc_xyz(dim,'y');
		mesh=dim->y;
		dmesh=dim->dY;
	}else
	if (xyz=='z')
	{
		dim->zlen=in->tot_points;
		dim_malloc_xyz(dim,'z');
		mesh=dim->z;
		dmesh=dim->dZ;
	}

	
	ret_len=mesh_to_lin_array(sim,mesh, dmesh,in);
	return ret_len;

}


long double mesh_to_dim_based_on_epitaxy(struct simulation *sim,struct dimensions *dim,struct device *in)
{
	int y;
	struct epitaxy *epi=&(in->my_epitaxy);

	dim->ylen=epi->layers+1;
	dim_malloc_xyz(dim,'y');

	for (y=0;y<epi->layers;y++)
	{
		dim->y[y]=epi->layer[y].y_start;
		//printf("%Le\n",dim->y[y]);
	}
	//getchar();
	dim->y[epi->layers]=epi->layer[epi->layers-1].y_stop;

	return epi->layer[epi->layers-1].y_stop-epi->layer[0].y_start;
}

void mesh_dump(struct simulation *sim,struct dimensions *dim)
{
	int x=0;

	for (x=0;x<dim->xlen;x++)
	{
		printf("%Le\n",dim->x[x]);
	}
}

void mesh_dump_y(struct simulation *sim,struct dimensions *dim)
{
	int y=0;

	for (y=0;y<dim->ylen;y++)
	{
		printf("%d %Le %Le\n",y,dim->y[y],dim->dY[y]);
	}
}


void mesh_build(struct simulation *sim,struct device *in)
{

	in->zlen=mesh_to_dim(sim, &(in->ns.dim), &(in->mesh_data.meshdata_z),'z');
	in->xlen=mesh_to_dim(sim, &(in->ns.dim), &(in->mesh_data.meshdata_x),'x');
	if (strcmp(in->newton_name,"newton_simple")!=0)
	{
		in->ylen=mesh_to_dim(sim, &(in->ns.dim), &(in->mesh_data.meshdata_y),'y');
	}else
	{
		in->ylen=mesh_to_dim_based_on_epitaxy(sim, &(in->ns.dim), in);
	}
	//dim_cpy(&(in->dim_max),&(in->ns.dim));

	//mesh_dump_y(sim,&(in->ns.dim));
	//getchar();
}


void mesh_numerate_points(struct simulation *sim,struct device *in)
{
	if (in->drift_diffision_simulations_enabled==FALSE)
	{
		return;
	}

	int z=0;
	int x=0;
	int y=0;

	struct dimensions *dim=&(in->ns.dim);

	gdouble dpos=0.0;

	//len=0.0;
	for (y=0;y<dim->ylen;y++)
	{
		dpos=dim->y[y];
		//in->imat[0][0][y]=epitaxy_get_electrical_material_layer(&(in->my_epitaxy),dpos);
		in->imat_epitaxy[0][0][y]=epitaxy_get_layer(&(in->my_epitaxy),in->my_epitaxy.device_start+dpos);
		//epitaxy_get_epitaxy_layer_using_electrical_pos(&(in->my_epitaxy),dpos);

		for (z=0;z<dim->zlen;z++)
		{
			for (x=0;x<dim->xlen;x++)
			{
				//in->imat[z][x][y]=in->imat[0][0][y];
				in->imat_epitaxy[z][x][y]=in->imat_epitaxy[0][0][y];
				//shape=shape_get_index(sim,&(in->my_epitaxy),dim->xmesh[x],dim->y[y],dim->zmesh[z]);
				//if (shape!=-1)
				//{
				//	in->imat[z][x][y]=shape;
				//}
				//printf("%d\n",shape);
			}
		}

	}


}


void mesh_cal_layer_widths(struct device *in)
{
int i;
int cur_i=in->imat_epitaxy[0][0][0];

in->layer_start[cur_i]=0.0;
struct dimensions *dim=&in->ns.dim;

for (i=0;i<dim->ylen;i++)
{
	if ((in->imat_epitaxy[0][0][i]!=cur_i)||(i==(dim->ylen-1)))
	{
		in->layer_stop[cur_i]=dim->y[i-1];//+(dim->y[i]-dim->y[i-1])/2;
		if (i==(dim->ylen-1))
		{
			break;
		}
		cur_i=in->imat_epitaxy[0][0][i];
		in->layer_start[cur_i]=dim->y[i];//-(dim->y[i]-dim->y[i-1])/2;
	}
//printf_log("%d\n",in->imat_epitaxy[i]);
}
}
