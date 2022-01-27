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

/** @file contacts.c
@brief backend to handle complex contacts
*/

#include <string.h>
#include "epitaxy.h"
#include "inp.h"
#include "util.h"
#include "gpvdm_const.h"
#include <cal_path.h>
#include "contacts.h"
#include <log.h>
#include <dump.h>
#include <shape.h>
#include <shape_struct.h>
#include <json.h>

int contacts_find_ground_contact(struct simulation *sim,struct device *in)
{
	int i;

	for (i=0;i<in->ncontacts;i++)
	{
		if (in->contacts[i].ground==TRUE)
		{
			return i;
		}
	}

ewe(sim,"No ground contact found\n");
}

int contacts_get_lcharge_type(struct simulation *sim,struct device *in)
{
	int i;

	for (i=0;i<in->ncontacts;i++)
	{
		if (in->contacts[i].position==TOP)
		{
			return in->contacts[i].charge_type;
		}
	}

ewe(sim,"No top contact found\n");
}

int contacts_get_rcharge_type(struct simulation *sim,struct device *in)
{
	int i;

	for (i=0;i<in->ncontacts;i++)
	{
		if (in->contacts[i].position==BOTTOM)
		{
			return in->contacts[i].charge_type;
		}
	}

ewe(sim,"No bottom contact found\n");
}

long double contacts_get_lcharge(struct simulation *sim,struct device *in)
{
	int i;

	for (i=0;i<in->ncontacts;i++)
	{
		if (in->contacts[i].position==TOP)
		{
			return in->contacts[i].np;
		}
	}

ewe(sim,"No top contact found\n");
}

long double contacts_get_rcharge(struct simulation *sim,struct device *in)
{
	int i;

	for (i=0;i<in->ncontacts;i++)
	{
		if (in->contacts[i].position==BOTTOM)
		{
			return in->contacts[i].np;
		}
	}
ewe(sim,"No bottom contact found\n");
}

void contacts_cal_std_resistance(struct simulation *sim,struct device *dev)
{
	return;
	int i;
	int ground=-1;
	int active=-1;
	for (i=0;i<dev->ncontacts;i++)
	{
		if (dev->contacts[i].applied_voltage_type,"ground")
		{
			ground=i;
			break;
		}
	}

	if (ground==-1)
	{
		ewe(sim,"No ground contact found\n");
	}

	for (i=0;i<dev->ncontacts;i++)
	{
		if (dev->contacts[i].active==TRUE)
		{
			active=i;
			break;
		}
	}

	if (active==-1)
	{
		ewe(sim,"No active contact found\n");
	}

	//dev->Rshunt=dev->contacts[active].shunt_resistance_sq/dev->area;
	//dev->Rcontact=(dev->contacts[active].contact_resistance_sq+dev->contacts[ground].contact_resistance_sq)/dev->area;
	//printf("Rshunt=%Le Rcontact=%Le\n",dev->Rshunt,dev->Rcontact);
	//getchar();

	for (i=0;i<dev->ncontacts;i++)
	{
		printf_log(sim,"%s\tRcontact=%.2Le\t(Ohms) Rshunt=%.2Le\t(Ohms)\n",dev->contacts[i].name,dev->contacts[i].contact_resistance_sq/dev->contacts[active].area,dev->contacts[i].shunt_resistance_sq/dev->contacts[i].area);
	}
	//getchar();
}

void contacts_time_step(struct simulation *sim,struct device *in)
{
	int i;

	for (i=0;i<in->ncontacts;i++)
	{
		in->contacts[i].voltage_last=in->contacts[i].voltage;
	}
}

int contacts_itterate_to_desired_voltage(struct simulation *sim,struct device *in,char *text)
{
int i;
char temp[400];
static long double dV=0.1;
int up=TRUE;
int changed=FALSE;
struct newton_state *ns=&(in->ns);

	strcpy(text,"Ramping voltage: ");

	for (i=0;i<in->ncontacts;i++)
	{

		up=TRUE;

		if (in->contacts[i].voltage_want!=in->contacts[i].voltage)
		{
			changed=TRUE;
			if ((in->contacts[i].voltage_want-in->contacts[i].voltage)<0.0)
			{
				up=FALSE;
			}

			if (ns->last_ittr<16)
			{
				dV=dV*(1.0+0.005);
			}

			if (ns->last_ittr>18)
			{
				dV*=(1.0-0.001);
			}

			if (dV<0.1)
			{
				dV=0.1;
			}

			/*if (in->contacts[i].voltage<5.0)
			{
				dV=0.1;
			}else
			{
				dV=0.1+fabs(in->contacts[i].voltage)*0.005;
			}*/

			if (up==TRUE)
			{
				if ((in->contacts[i].voltage+dV)>=in->contacts[i].voltage_want)
				{
					in->contacts[i].voltage=in->contacts[i].voltage_want;
				}else
				{
					in->contacts[i].voltage+=dV;
				}

			}else
			{
				if ((in->contacts[i].voltage-dV)<=in->contacts[i].voltage_want)
				{
					in->contacts[i].voltage=in->contacts[i].voltage_want;
				}else
				{
					in->contacts[i].voltage-=dV;
				}
			}

			printf_log(sim,"Ramping: %s %.2Lf %.2Lf dV=%Lf ittr=%d (%lf ms)\n",in->contacts[i].name,in->contacts[i].voltage,in->contacts[i].voltage_want,dV,ns->last_ittr,ns->last_time);

			sprintf(temp,"%s %.2Lf V/%.2Lf V",in->contacts[i].name,in->contacts[i].voltage,in->contacts[i].voltage_want);
			strcat(text,temp);
		}


	}

	if (changed==TRUE)
	{
		contacts_update(sim,in);
	}

	return changed;
}


void contacts_state_to_string(struct simulation *sim,char *out, struct device *dev)
{
	int i;
	char temp[200];
	long double voltage=0.0;
	strcpy(out,"");
	voltage=contact_get_active_contact_voltage(sim,dev);

	for (i=0;i<dev->ncontacts;i++)
	{
		sprintf(temp,"%s= %.2Lf V, %.2Le A/m^2",dev->contacts[i].name,dev->contacts[i].voltage,dev->contacts[i].J);
		strcat(out,temp);
		if (i<dev->ncontacts-1)
		{
			strcat(out,",\t");
		}
	}
}

void contacts_force_to_zero(struct simulation *sim,struct device *in)
{
int y;
int x;
int z;
struct dimensions *dim=&in->ns.dim;

for (z=0;z<dim->zlen;z++)
{
	for (x=0;x<dim->xlen;x++)
	{
			in->Vapplied_y0[z][x]=0.0;
			in->Vapplied_y1[z][x]=0.0;
	}

	for (y=0;y<dim->ylen;y++)
	{
			in->Vapplied_x0[z][y]=0.0;
			in->Vapplied_x1[z][y]=0.0;
	}

}

}


void contacts_dump(struct simulation *sim,struct device *in)
{
	int i;
	int y=0;
	int x=0;
	int z=0;

	struct dimensions *dim=&in->ns.dim;

	if (get_dump_status(sim,dump_print_text)==TRUE)
	{
		for (i=0;i<in->ncontacts;i++)
		{
			printf_log(sim,"%-10s\tV=%Le\tA=%Le\n",in->contacts[i].name,in->contacts[i].voltage,in->contacts[i].area);
		}

	}



	printf("top-btm\n");
	for (z = 0; z < dim->zlen; z++)
	{
		for (x = 0; x < dim->xlen; x++)
		{
			printf("%d passivate=%d (%Le) %d\n",in->n_contact_y0[z][x],in->passivate_y0[z][x],in->Vapplied_y0[z][x],in->n_contact_y1[z][x]);
		}
	}

	printf("left-right\n");
	for (z = 0; z < dim->zlen; z++)
	{
		for (y = 0; y < dim->ylen; y++)
		{
			printf("%d %d\n",in->n_contact_x0[z][y],in->n_contact_x1[z][y]);
			//printf("%d %.2Le %.2Lf %.2Lf (%.2Lf %.2Lf %Le %Le %Le %Le)\n",y,dim->y[y],in->Vapplied_x0[z][y],in->Vapplied_x1[z][y],in->V_x0[z][y],in->V_x1[z][y],in->electrons_x0[z][y],in->holes_x0[z][y],in->electrons_x1[z][y],in->holes_x1[z][y]);
		}
	}
}

int contact_within_zx(struct contact *c, long double z,long double x)
{
	if (x>=c->shape.x0)
	{
		if (x<=(c->shape.x0+c->shape.dx))
		{
			return 0;
		}
	}

	return -1;
}

int contact_within_zy(struct contact *c, long double z, long double y)
{

	if (y>=c->shape.y0)
	{
		if (y<(c->shape.y0+c->shape.dy))
		{
			return 0;
		}
	}

	return -1;
}

void contacts_update(struct simulation *sim,struct device *in)
{
int i;
int y;
int x;
int z;
int n;
int found=FALSE;

gdouble value=0.0;
struct newton_state *ns=&in->ns;
struct dimensions *dim=&in->ns.dim;
struct contact *c;
int electrical_y0_boundry;
long double Voltage_electrical_y0;
struct json_obj *obj_contacts;
struct json_obj *obj_contact;


if (in->Vapplied_y0==NULL) return;

if (dim->xlen==1)
{
	for (z=0;z<dim->zlen;z++)
	{

		in->passivate_y0[z][0]=FALSE;
		in->passivate_y1[z][0]=FALSE;

		for (i=0;i<in->ncontacts;i++)
		{
			c=&(in->contacts[i]);
			if ((c->position==TOP)&&(c->active==TRUE))
			{
				in->Vapplied_y0[z][0]=c->voltage;
				in->n_contact_y0[z][0]=i;
			}else
			{
				in->Vapplied_y1[z][0]=c->voltage;
				in->n_contact_y1[z][0]=i;
			}
		}
	}

	//contacts_dump(sim,in);
	//getchar();
	return;
}

obj_contacts=json_obj_find_by_path(sim,&(in->config.obj), "electrical_solver.boundary");

json_get_english(sim,obj_contacts, &electrical_y0_boundry,"electrical_y0_boundry");
json_get_long_double(sim,obj_contacts, &Voltage_electrical_y0,"electrical_y0");


//Reset contacts
for (z=0;z<dim->zlen;z++)
{
	for (x=0;x<dim->xlen;x++)
	{
		in->Vapplied_y0[z][x]=0.0;
		in->Vapplied_y1[z][x]=0.0;

		in->n_contact_y0[z][x]=-1;
		in->n_contact_y1[z][x]=-1;

		in->passivate_y0[z][x]=electrical_y0_boundry;
		in->Vapplied_y0[z][x]=Voltage_electrical_y0;
		in->passivate_y1[z][x]=NEUMANN;
	}

	for (y=0;y<dim->ylen;y++)
	{
		in->Vapplied_x0[z][y]=0.0;
		in->Vapplied_x1[z][y]=0.0;

		in->n_contact_x0[z][y]=-1;
		in->n_contact_x1[z][y]=-1;

		in->passivate_x0[z][y]=NEUMANN;
		in->passivate_x1[z][y]=NEUMANN;
	}

}

	for (i=0;i<in->ncontacts;i++)
	{
		c=&(in->contacts[i]);
		if (c->position==TOP)
		{
			for (z=0;z<dim->zlen;z++)
			{
				for (x=0;x<dim->xlen;x++)
				{

					if (contact_within_zx(c, dim->z[z],dim->x[x])==0)
					{

						if (in->n_contact_y0[z][x]!=-1)
						{
							ewe(sim,"You have overlapping contacts\n");
						}

						in->Vapplied_y0[z][x]=c->voltage;
						in->n_contact_y0[z][x]=i;
						in->passivate_y0[z][x]=CONSTANT;
					}
				}
			}
		}else
		if (c->position==BOTTOM)
		{
			//printf("btm %d\n",in->contacts[i].position);
			for (z=0;z<dim->zlen;z++)
			{
				for (x=0;x<dim->xlen;x++)
				{
					//printf("%d %Le %Le %d\n",i,dim->z[z],dim->x[x],contact_within_zx(c, dim->z[z],dim->x[x]));
					//getchar();
					//printf("%Le\n",in->contacts[1].shape.x0);
					if (contact_within_zx(c, dim->z[z],dim->x[x])==0)
					{

						if (in->n_contact_y1[z][x]!=-1)
						{
							ewe(sim,"You have overlapping contacts\n");
						}

						in->Vapplied_y1[z][x]=c->voltage;
						in->n_contact_y1[z][x]=i;
						in->passivate_y1[z][x]=FALSE;
					}
				}
			}
			//getchar();
		}else
		if (c->position==LEFT)
		{
			for (z=0;z<dim->zlen;z++)
			{
				for (y=0;y<dim->ylen;y++)
				{


					if (contact_within_zy(c, dim->z[z], dim->y[y]+in->my_epitaxy.device_start)==0)
					{

						if (in->n_contact_x0[z][y]!=-1)
						{
							ewe(sim,"You have overlapping contacts\n");
						}

						in->Vapplied_x0[z][y]=c->voltage;
						in->n_contact_x0[z][y]=i;
						in->passivate_x0[z][y]=FALSE;
					}
				}
			}
		}else
		if (c->position==RIGHT)
		{
			for (z=0;z<dim->zlen;z++)
			{
				for (y=0;y<dim->ylen;y++)
				{

					if (contact_within_zy(c, dim->z[z], dim->y[y]+in->my_epitaxy.device_start)==0)
					{

						if (in->n_contact_x1[z][y]!=-1)
						{
							ewe(sim,"You have overlapping contacts\n");
						}

						in->Vapplied_x1[z][y]=c->voltage;
						in->n_contact_x1[z][y]=i;
						in->passivate_x1[z][y]=FALSE;
					}
				}
			}
		}
	}

contacts_interpolate(sim,in,in->Vapplied_y0);
//contacts_dump(sim,in);
//getchar();
}

//This will interpolate physical constatns across the top of the device
void contacts_interpolate(struct simulation *sim,struct device *in, long double **var_zx)
{
	int x=0;
	int xx=0;
	int z=0;
	int last=0;
	long double x0=0.0;
	long double x1=0.0;
	long double v0=0.0;
	long double v1=0.0;
	int c;
	int cc;
	int interpolate=FALSE;
	int last_x=0;
	int electrical_y0_boundry;

	struct json_obj *obj_contacts;
	struct json_obj *obj_contact;
	char contact_name[200];

	obj_contacts=json_obj_find_by_path(sim,&(in->config.obj), "electrical_solver.boundary");

	json_get_english(sim,obj_contacts, &electrical_y0_boundry,"electrical_y0_boundry");

	if (electrical_y0_boundry!=INTERPOLATE)
	{
		return;
	}
	//struct newton_state *ns=&in->ns;
	struct dimensions *dim=&in->ns.dim;
	for (z=0;z<dim->zlen;z++)
	{
		/*for (x=0;x<dim->xlen;x++)
		{
			printf("before %d %Le %d %d\n",x,var_zx[z][x],in->passivate_y0[z][x],in->n_contact_y0[z][x]);
		}*/

		x=0;
		while(x<dim->xlen)
		{
			c=in->n_contact_y0[z][x];
			if ((last!=-1)&&(c==-1))
			{
				for (xx=x;xx<dim->xlen;xx++)
				{
					cc=in->n_contact_y0[z][xx];
					//we have found the next contact
					if (cc!=-1)
					{
						x0=dim->x[last_x];
						x1=dim->x[xx];
						v0=var_zx[z][last_x];//in->contacts[last].voltage;
						v1=var_zx[z][xx];//in->contacts[cc].voltage;
						interpolate=TRUE;
						break;
					}
				}

			}

			if ((interpolate==TRUE)&&(c==-1))
			{
				//printf("%Le %Le %Le %Le\n",x0,x1,v0,v1);
				var_zx[z][x]=v0+(dim->x[x]-x0)*((v1-v0)/(x1-x0));
				in->passivate_y0[z][x]=INTERPOLATE;
			}

			if (c!=-1)
			{
				interpolate=FALSE;
			}
			last=c;
			last_x=x;
			x++;
		}

		/*for (x=0;x<dim->xlen;x++)
		{
			printf("after %d %Le %d %d\n",x,var_zx[z][x],in->passivate_y0[z][x],in->n_contact_y0[z][x]);
		}*/
	}

}

gdouble contact_get_voltage_last(struct simulation *sim,struct device *in,int contact)
{
	return in->contacts[contact].voltage_last;
}

gdouble contact_get_voltage(struct simulation *sim,struct device *in,int contact)
{
	return in->contacts[contact].voltage;
}

void contact_set_voltage(struct simulation *sim,struct device *in,int contact,gdouble voltage)
{
	in->contacts[contact].voltage=voltage;
	contacts_update(sim,in);
}

void contact_set_wanted_active_contact_voltage(struct simulation *sim,struct device *in,gdouble voltage)
{

	int i=0;
	int found=FALSE;
	for (i=0;i<in->ncontacts;i++)
	{
		if (in->contacts[i].active==TRUE)
		{
			found=TRUE;
			in->contacts[i].voltage_want=voltage;
		}
	}

	if (found==FALSE)
	{
		ewe(sim,"There is no active contact");
	}
	contacts_update(sim,in);

}

void contact_set_active_contact_voltage(struct simulation *sim,struct device *in,gdouble voltage)
{
	int i=0;
	int found=FALSE;


	for (i=0;i<in->ncontacts;i++)
	{
		if (in->contacts[i].active==TRUE)
		{
			found=TRUE;
			in->contacts[i].voltage=voltage;
		}
	}

	if (found==FALSE)
	{
		ewe(sim,"There is no active contact");
	}

	contacts_update(sim,in);
	//contacts_dump(sim,in);
}

void contact_set_flip_current(struct simulation *sim,struct device *in)
{
	int i=0;
	in->flip_current=1.0;
	if (in->ncontacts==2)
	{
		for (i=0;i<in->ncontacts;i++)
		{
			//printf("%d %d %d\n",in->contacts[i].active,in->contacts[i].position,BOTTOM);
			//getchar();

			if ((in->contacts[i].active==TRUE)&&(in->contacts[i].position==BOTTOM))
			{
				in->flip_current=-1.0;
			}
		}
	}
}

gdouble contact_get_active_contact_voltage_last(struct simulation *sim,struct device *in)
{
	int i=0;
	int found=FALSE;
	for (i=0;i<in->ncontacts;i++)
	{
		if (in->contacts[i].active==TRUE)
		{
			found=TRUE;
			return in->contacts[i].voltage_last;
		}
	}

	if (found==FALSE)
	{
		ewe(sim,"There is no active contact");
	}
}

int contact_get_active_contact_index(struct simulation *sim,struct device *in)
{
	int i=0;
	int found=FALSE;

	for (i=0;i<in->ncontacts;i++)
	{
		if (in->contacts[i].active==TRUE)
		{
			found=TRUE;
			return i;
		}
	}

	if (found==FALSE)
	{
		ewe(sim,"There is no active contact");
	}
}


long double contact_get_active_contact_voltage(struct simulation *sim,struct device *in)
{
	int i=0;
	int found=FALSE;

	for (i=0;i<in->ncontacts;i++)
	{
		if (in->contacts[i].active==TRUE)
		{
			found=TRUE;
			return in->contacts[i].voltage;
		}
	}

	if (found==FALSE)
	{
		ewe(sim,"There is no active contact");
	}
}

long double contact_get_wanted_active_contact_voltage(struct simulation *sim,struct device *in)
{
	int i=0;
	int found=FALSE;

	for (i=0;i<in->ncontacts;i++)
	{
		if (in->contacts[i].active==TRUE)
		{
			found=TRUE;
			return in->contacts[i].voltage_want;
		}
	}

	if (found==FALSE)
	{
		ewe(sim,"There is no active contact");
	}
}


void contact_set_all_voltages(struct simulation *sim,struct device *in,gdouble voltage)
{
int i;
	for (i=0;i<in->ncontacts;i++)
	{
		in->contacts[i].voltage=voltage;
	}

	contacts_update(sim,in);
}

long double contacts_get_Jleft(struct device *in)
{
int i;
int x;
int z;

long double tot=0.0;
long double count=0.0;
struct dimensions *dim=&in->ns.dim;

for (x=0;x<dim->xlen;x++)
{
		for (z=0;z<dim->zlen;z++)
		{
			if (in->n_contact_y0[z][x]>=0)
			{
				tot+=in->Jp_y0[z][x]+in->Jn_y0[z][x];
				//printf("%Le %Le\n",in->Jp_y0[z][x],in->Jn_y0[z][x]);
				count=count+1.0;						//this will need updating for meshes which change
			}
		}
}

tot=tot/count;
tot*=in->flip_current;

return tot;
}

long double contacts_get_Jright(struct device *in)
{
int i;
int x;
int z;

long double tot=0.0;
long double count=0.0;
struct dimensions *dim=&in->ns.dim;

for (x=0;x<dim->xlen;x++)
{
		for (z=0;z<dim->zlen;z++)
		{
			if (in->n_contact_y1[z][x]>=0)
			{
				tot+=in->Jp_y1[z][x]+in->Jn_y1[z][x];
				count=count+1.0;
			}
		}
}

tot=tot/count;

tot*=in->flip_current;

return tot;
}

int contacts_get_active_contact_left_right(struct device *in)
{
int i;
	for (i=0;i<in->ncontacts;i++)
	{
		if (in->contacts[i].active==TRUE)
		{
			if (in->contacts[i].position==TOP)
			{
				return LEFT;
			}else
			{
				return RIGHT;
			}
		}

	}

return -1;
}

void contacts_cal_area(struct simulation *sim,struct device *in)
{
int i;
int x;
int z;

if (in->n_contact_y1==NULL) return;

struct dimensions *dim=&in->ns.dim;

for (i=0;i<in->ncontacts;i++)
{
	in->contacts[i].area=0.0;
}

for (x=0;x<dim->xlen;x++)
{
		for (z=0;z<dim->zlen;z++)
		{
			i=in->n_contact_y1[z][x];
			if (i!=-1)
			{
				in->contacts[i].area+=dim->dX[x]*dim->dZ[z];
			}

			i=in->n_contact_y0[z][x];
			if (i!=-1)
			{
				in->contacts[i].area+=dim->dX[x]*dim->dZ[z];
			}

		}
}

}

void contacts_detailed_dump(struct device *in)
{
int i;
int x;
int z;
struct dimensions *dim=&in->ns.dim;

for (x=0;x<dim->xlen;x++)
{
		for (z=0;z<dim->zlen;z++)
		{
			printf("%d %Le %Le\n",in->n_contact_y0[z][x],in->Jp_y0[z][x],in->Jn_y0[z][x]);
		}
}

}

//Average the current over both contacts
void contacts_cal_J_and_i(struct simulation *sim,struct device *dev)
{
int c;
int i;
int x;
int z;

long double tot=0.0;
struct dimensions *dim=&dev->ns.dim;
long double count[10];

for (c=0;c<dev->ncontacts;c++)
{
	dev->contacts[c].J=0.0;
	dev->contacts[c].i=0.0;
	count[c]=0;
}

for (x=0;x<dim->xlen;x++)
{
		for (z=0;z<dim->zlen;z++)
		{
			c=dev->n_contact_y1[z][x];
			if (c>=0)
			{
				dev->contacts[c].J+=dev->Jp_y1[z][x]+dev->Jn_y1[z][x];
				count[c]+=1.0;						//this will need updating for meshes which change
			}

			c=dev->n_contact_y0[z][x];
			if (c>=0)
			{
				dev->contacts[c].J+=dev->Jp_y0[z][x]+dev->Jn_y0[z][x];
				count[c]+=1.0;						//this will need updating for meshes which change
			}
		}
}

for (c=0;c<dev->ncontacts;c++)
{
	dev->contacts[c].J/=count[c];
	dev->contacts[c].J*=dev->flip_current;
	dev->contacts[c].i=dev->contacts[c].J*dev->contacts[c].area;
}

}

void contacts_passivate(struct simulation *sim,struct device *in)
{
int i;
int x;
int y;
int z;
return;
struct newton_state *ns=&in->ns;
struct dimensions *dim=&in->ns.dim;

//passivate under each contact
for (x=0;x<dim->xlen;x++)
{
	for (y=0;y<dim->ylen;y++)
	{
		for (z=0;z<dim->zlen;z++)
		{

			for (i=0;i<in->ncontacts;i++)
			{
				if (in->contacts[i].position==TOP)
				{
					//if ((in->ylen-dim->ymesh[y]<=in->contacts[i].depth)&&(dim->xmesh[x]>in->contacts[i].start)&&(dim->xmesh[x]<in->contacts[i].start+in->contacts[i].width))
					{
						in->mun_z[z][x][y]=1e-15;
						in->mup_z[z][x][y]=1e-15;
						in->mun_x[z][x][y]=1e-15;
						in->mup_x[z][x][y]=1e-15;
						in->mun_y[z][x][y]=1e-15;
						in->mup_y[z][x][y]=1e-15;

					}
				}

				if (in->contacts[i].position==BOTTOM)
				{
					//if ((dim->ymesh[y]<=in->contacts[i].depth)&&(dim->xmesh[x]>in->contacts[i].start)&&(dim->xmesh[x]<in->contacts[i].start+in->contacts[i].width))
					{
						in->mun_z[z][x][y]=1e-15;
						in->mup_z[z][x][y]=1e-15;
						in->mun_x[z][x][y]=1e-15;
						in->mup_x[z][x][y]=1e-15;
						in->mun_y[z][x][y]=1e-15;
						in->mup_y[z][x][y]=1e-15;
					}
				}
			}
		}
	}
}

for (x=0;x<dim->xlen;x++)
{
	for (z=0;z<dim->zlen;z++)
	{
		i=in->n_contact_y1[z][x];
		if (i==-1)
		{
			in->mun_z[z][x][dim->ylen-1]=1e-15;
			in->mup_z[z][x][dim->ylen-1]=1e-15;
			in->mun_x[z][x][dim->ylen-1]=1e-15;
			in->mup_x[z][x][dim->ylen-1]=1e-15;
			in->mun_y[z][x][dim->ylen-1]=1e-15;
			in->mup_y[z][x][dim->ylen-1]=1e-15;

		}

		i=in->n_contact_y0[z][x];
		if (i==-1)
		{
			in->mun_z[z][x][0]=1e-15;
			in->mup_z[z][x][0]=1e-15;
			in->mun_x[z][x][0]=1e-15;
			in->mup_x[z][x][0]=1e-15;
			in->mun_y[z][x][0]=1e-15;
			in->mup_y[z][x][0]=1e-15;

		}
	}
}

}

void contacts_ingress(struct simulation *sim,struct device *in)
{
	int x=0;
	int y=0;
	int z=0;
	int c=0;
	struct newton_state *ns=&(in->ns);
	struct dimensions *dim=&in->ns.dim;
	struct shape *s;
	long double x_pos=0.0;
	long double y_pos=0.0;
	long double z_pos=0.0;

	for (z=0;z<dim->zlen;z++)
	{
		z_pos=dim->z[z];
		for (x=0;x<dim->xlen;x++)
		{
			x_pos=dim->x[x];
			for (y=0;y<dim->ylen;y++)
			{
				y_pos=dim->y[y];
				for (c=0;c<in->ncontacts;c++)
				{
					if (in->contacts[c].position==LEFT)		//I should not need this line if I fix up the other contacts
					{
						s=&(in->contacts[c].shape);
						if (shape_in_shape(sim,s,z_pos,x_pos,y_pos+in->my_epitaxy.device_start)==0)
						{
							in->mun_z[z][x][y]=1.0;
							in->mun_x[z][x][y]=1.0;
							in->mun_y[z][x][y]=1.0;

							in->mup_z[z][x][y]=1.0;
							in->mup_x[z][x][y]=1.0;
							in->mup_y[z][x][y]=1.0;
						}
					}

				}

				//printf("%d %d %Le\n",x,y,in->mun[z][x][y]);
			}
		}

	}

//getchar();
}
