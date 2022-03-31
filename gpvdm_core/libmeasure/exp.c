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

/** @file exp.c
	@brief Pefrorm experiments on the device, such as count charge density, calculate average position of electrons/holes etc...
*/

#include <exp.h>
#include "sim.h"
#include "i.h"
#include "dat_file.h"
#include "contacts.h"
#include <memory.h>

/**
* @brief Average position of electron and holes (not updated)
*/
void get_avg_np_pos(struct device *in,gdouble *nx,gdouble *px)
{
	int x;
	int y;
	int z;

	gdouble navg=0.0;
	gdouble pavg=0.0;
	gdouble nsum=0.0;
	gdouble psum=0.0;
	struct newton_state *ns=&in->ns;
	struct dimensions *dim=&in->ns.dim;

	for (z=0;z<dim->zlen;z++)
	{
		for (x=0;x<dim->xlen;x++)
		{
			for (z=0;z<dim->zlen;z++)
			{
				navg+=(in->n[z][x][y]+in->nt_all[z][x][y]-in->n_orig[z][x][y])*dim->y[y];
				pavg+=(in->p[z][x][y]+in->pt_all[z][x][y]-in->p_orig[z][x][y])*dim->y[y];
				nsum+=(in->n[z][x][y]+in->nt_all[z][x][y]-in->n_orig[z][x][y]);
				psum+=in->p[z][x][y]+in->pt_all[z][x][y]-in->p_orig[z][x][y];
			}
		}
	}

	if (nsum!=0.0)
	{
		*nx=navg/nsum;
	}else
	{
		*nx=0.0;
	}

	if (psum!=0.0)
	{
	*px=pavg/psum;
	}else
	{
	*px=0.0;
	}



}


/**
* @brief Get the average recombination density
*/
gdouble get_tot_photons_abs(struct device *in)
{
	long double ret=0.0;

	ret=three_d_integrate(&(in->ns.dim), in->Gn);

	return ret;
}




/**
* @brief Get the average electron relaxation rate
*/
gdouble get_avg_relax_n(struct device *in)
{
	long double ret=0.0;

	ret=three_d_avg(in, in->nrelax);

	return ret;
}

/**
* @brief Get the average hole relaxation rate
*/
gdouble get_avg_relax_p(struct device *in)
{
	long double ret=0.0;

	ret=three_d_avg(in, in->prelax);

	return ret;
}


/**
* @brief Calculate J from the recombination rate
* Not updated for variable mesh
*/
gdouble get_J_recom(struct device *in)
{
	int x=0;
	int y=0;
	int z=0;

	gdouble Rtot=0.0;
	gdouble add=0.0;
	struct newton_state *ns=&in->ns;
	struct dimensions *dim=&in->ns.dim;

	gdouble dx=dim->y[1]-dim->y[0];



	for (z=0;z<dim->zlen;z++)
	{
		for (x=0;x<dim->xlen;x++)
		{
			for (y=0;y<dim->ylen;y++)
			{

				add=(in->Rn[z][x][y]-in->Gn[z][x][y]-in->Rp[z][x][y]-in->Gn[z][x][y])*dx/2.0;

				Rtot+=add;
			}
		}
	}

	return Rtot*Qe;
}


/**
* @brief Calculate the average J but ignores and the std of J
* throw away values of J which are outside a given std range of J
* used when the current does not converge very well.
* Not updated for variable mesh
*/
gdouble get_avg_J_std(struct device *in)
{
	int x=0;
	int y=0;
	int z=0;

	gdouble J=0.0;
	gdouble Javg=0.0;
	gdouble Jstd_dev=0.0;
	gdouble Jtot=0.0;

	struct dimensions *dim=&in->ns.dim;

	for (z=0;z<dim->zlen;z++)
	{
		for (x=0;x<dim->xlen;x++)
		{
			for (y=0;y<dim->ylen;y++)
			{
				Javg+=in->Jn[z][x][y]+in->Jp[z][x][y];
			}
		}
	}

	Javg/=(gdouble)dim->ylen;

	for (z=0;z<dim->zlen;z++)
	{
		for (x=0;x<dim->xlen;x++)
		{
			for (y=0;y<dim->ylen;y++)
			{
				Jstd_dev+=pow(((in->Jn[z][x][y]+in->Jp[z][x][y])-Javg),2.0);
			}
		}
	}

	Jstd_dev/=(gdouble)dim->ylen;
	Jstd_dev=sqrt(Jstd_dev);

	for (z=0;z<dim->zlen;z++)
	{
		for (x=0;x<dim->xlen;x++)
		{
			for (y=0;y<dim->ylen;y++)
			{
				J=in->Jn[z][x][y]+in->Jp[z][x][y];
				if (fabs(J-Javg)<Jstd_dev*0.05)
				{
					Jtot+=in->Jn[z][x][y]+in->Jp[z][x][y];


				}
			}
		}
	}
	Jtot/=(gdouble)dim->xlen;
	Jtot/=(gdouble)dim->ylen;
	Jtot/=(gdouble)dim->zlen;


	return Javg;
}

/**
* @brief Calculate the average J but ignores and the std of J
* throw away values of J which are outside a given std range of J
* used when the current does not converge very well.
* Not updated for variable mesh
*/
gdouble get_avg_J(struct device *in)
{
	gdouble Javg=0.0;
	Javg+=three_d_avg(in, in->Jn);
	Javg+=three_d_avg(in, in->Jp);
	Javg+=three_d_avg(in, in->Jion);

	return Javg;
}

/**
* @brief Calculate the average Jn
*/
gdouble get_jn_avg(struct device *in)
{
long double ret=0.0;

ret=three_d_avg(in, in->Jn);

ret*=in->flip_current;

return ret;
}

/**
* @brief Calculate the average Jp
*/
gdouble get_jp_avg(struct device *in)
{
long double ret=0.0;

ret=three_d_avg(in, in->Jp);

ret*=in->flip_current;

return ret;
}


/**
* @brief Calculate the maximum possible Jsc value from the generation rate.
*/
gdouble get_max_Jsc(struct device *in)
{
int x=0;
int y=0;
int z=0;

gdouble tot=0.0;
struct dimensions *dim=&in->ns.dim;

for (z=0;z<dim->zlen;z++)
{
	for (x=0;x<dim->xlen;x++)
	{
		for (y=0;y<dim->ylen;y++)
		{
			tot+=in->Gn[z][x][y]*dim->dY[y];
		}
	}
}

return -Qe*tot;
}



/**
* @brief Calculate the value of k
* Not updated for a variable mesh
*/
gdouble get_extracted_k(struct device *in)
{
int x=0;
int y=0;
int z=0;

gdouble tot=0.0;
gdouble n=0.0;
struct dimensions *dim=&in->ns.dim;


for (z=0;z<dim->zlen;z++)
{
	for (x=0;x<dim->xlen;x++)
	{
		for (y=0;y<dim->ylen;y++)
		{
			tot+=in->Rfree[z][x][y];
			n+=in->n[z][x][y]*in->p[z][x][y];
		}
	}
}

return tot/n;
}

/**
* @brief Calculate the average generation rate
*/
gdouble get_avg_gen(struct device *in)
{
long double ret_Gn=0.0;
long double ret_Gp=0.0;

ret_Gn=three_d_avg(in, in->Gn);
ret_Gp=three_d_avg(in, in->Gp);

return (ret_Gn+ret_Gp)/2.0;
}




/**
* @brief Save the device parameters
*/
void reset_np_save(struct device *in)
{
int x=0;
int y=0;
int z=0;

int band;

struct newton_state *ns=&(in->ns);
struct dimensions *dim=&in->ns.dim;

for (z=0;z<dim->zlen;z++)
{
	for (x=0;x<dim->xlen;x++)
	{
		for (y=0;y<dim->ylen;y++)
		{
			in->nf_save[z][x][y]=in->n[z][x][y];
			in->pf_save[z][x][y]=in->p[z][x][y];
			in->nt_save[z][x][y]=in->nt_all[z][x][y];
			in->pt_save[z][x][y]=in->pt_all[z][x][y];
			in->phi_save[z][x][y]=ns->phi[z][x][y];
			for (band=0;band<dim->srh_bands;band++)
			{
				in->ntb_save[z][x][y][band]=in->nt[z][x][y][band];
				in->ptb_save[z][x][y][band]=in->pt[z][x][y][band];
			}
		}
	}
}

}

/**
* @brief Reset np equlib
*/
void reset_npequlib(struct device *in)
{

	int x=0;
	int y=0;
	int z=0;

	struct dimensions *dim=&in->ns.dim;

	for (z=0;z<dim->zlen;z++)
	{
		for (x=0;x<dim->xlen;x++)
		{
			for (y=0;y<dim->ylen;y++)
			{
				in->nfequlib[z][x][y]=in->n[z][x][y];
				in->pfequlib[z][x][y]=in->p[z][x][y];
				in->ntequlib[z][x][y]=in->nt_all[z][x][y];
				in->ptequlib[z][x][y]=in->pt_all[z][x][y];
			}
		}
	}
}



/**
* @brief Calculate the external current using internal recombination
*/
gdouble get_I_recomb(struct device *in)
{
gdouble ret=0.0;

ret=(get_J_recom(in))*(in->xlen*in->zlen)/2.0;

return ret;
}

/**
* @brief Calculate the external current using the values on the contacts
*/
gdouble get_I(struct device *in)
{
gdouble ret=0.0;
ret+=(get_J_left(in)+get_J_right(in))*(in->xlen*in->zlen)/2.0;

return ret;
}

/**
* @brief Calculate i by intergrating across the device.
*/
gdouble get_i_intergration(struct device *in)
{
long double tot=0.0;
long double ret=0.0;

tot=three_d_avg(in, in->Jn);
tot+=three_d_avg(in, in->Jp);

ret=(in->xlen*in->zlen)*tot;
return ret;
}

/**
* @brief Calculate I including paracitic commponents
*/
gdouble get_equiv_I(struct simulation *sim,struct device *in)
{
gdouble Iout=0.0;
Iout=get_equiv_J(sim,in)*(in->xlen*in->zlen);
return Iout;
}

/**
* @brief Calculate J including paracitic commponents
*/
gdouble get_equiv_J(struct simulation *sim,struct device *in)
{
struct newton_state *ns=&(in->ns);
struct dimensions *dim=&in->ns.dim;
struct circuit *cir=&in->cir;

gdouble Vapplied=0.0;
Vapplied=contact_get_active_contact_voltage(sim,in);
gdouble J=0.0;

J=get_J(in);

if (in->circuit_simulation==TRUE)
{
	if ((dim->xlen>1)||(dim->zlen>1))
	{
		if (cir->abstract_circuit==FALSE)
		{
			J=contacts_get_Jright(in);
			//printf("%Le\n",J);
		}
	}
}

//if (in->lr_pcontact==RIGHT) J*= -1.0;
J+=Vapplied/in->Rshunt/in->area;

return J;
}

/**
* @brief Get I for charge extraction
*/
gdouble get_I_ce(struct simulation *sim,struct device *in)
{
gdouble Vapplied=0.0;
Vapplied=contact_get_active_contact_voltage(sim,in);

gdouble ret=Vapplied/(in->Rcontact+in->Rshort);
if (in->time<0.0)
{
	ret=0.0;
}
return ret;
}

/**
* @brief Get the average field across the device
*/
gdouble get_avg_field(struct device *in)
{
	struct newton_state *ns=&(in->ns);
	struct dimensions *dim=&in->ns.dim;
	return (ns->phi[0][0][dim->ylen-1]-ns->phi[0][0][0]);
}

/**
* @brief Calculate the drift and diffusion currents
* (Not updated for varialbe mesh)
*/
void cal_J_drift_diffusion(struct device *in)
{
return;
gdouble Ecl=0.0;
gdouble Ecr=0.0;
gdouble dEc=0.0;

gdouble Evl=0.0;
gdouble Evr=0.0;
gdouble dEv=0.0;

gdouble nl=0.0;
gdouble nr=0.0;
gdouble dn=0.0;
gdouble pl=0.0;
gdouble pr=0.0;
gdouble dp=0.0;
gdouble xl=0.0;
gdouble xr=0.0;
gdouble dx=0.0;

int x=0;
int y=0;
int z=0;
struct newton_state *ns=&in->ns;

struct dimensions *dim=&in->ns.dim;

for (z=0;z<dim->zlen;z++)
{
	for (x=0;x<dim->xlen;x++)
	{
		for (y=0;y<dim->ylen;y++)
		{
			if (y==0)
			{
				nl=in->n[z][x][0];
				pl=in->p[z][x][0];
				Ecl=in->Ec[z][x][0];
				Evl=in->Ev[z][x][0];
				xl=dim->y[0];
			}else
			{
				nl=in->n[z][x][y-1];
				pl=in->p[z][x][y-1];
				Ecl=in->Ec[z][x][y-1];
				Evl=in->Ev[z][x][y-1];
				xl=dim->y[y-1];
			}

			if (y==dim->ylen-1)
			{
				nr=in->n[z][x][dim->ylen-1];
				pr=in->p[z][x][dim->ylen-1];
				Ecr=in->Ec[z][x][dim->ylen-1];
				Evr=in->Ev[z][x][dim->ylen-1];
				xr=dim->y[dim->ylen-1];
			}else
			{
				nr=in->n[z][x][y+1];
				pr=in->p[z][x][y+1];
				Ecr=in->Ec[z][x][y+1];
				Evr=in->Ev[z][x][y+1];
				xr=dim->y[y+1];
			}
			dn=(nr-nl);
			dp=(pr-pl);
			dEc=Ecr-Ecl;
			dEv=Evr-Evl;
			dx=(xr-xl);
			in->Jn_diffusion[z][x][y]=Qe*in->Dn[z][x][y]*dn/dx;
			in->Jn_drift[z][x][y]=Qe*in->mun_y[z][x][y]*in->n[z][x][y]*dEc/dx;

			in->Jp_diffusion[z][x][y]= -Qe*in->Dp[z][x][y]*dp/dx;
			in->Jp_drift[z][x][y]=Qe*in->mup_y[z][x][y]*in->p[z][x][y]*dEv/dx;
		}
	}
}

}

/**
* @brief Jn diffusion
*/
gdouble get_Jn_diffusion(struct device *in)
{
long double J=0.0;

J=three_d_avg(in, in->Jn_diffusion);

return J;
}

/**
* @brief Jn drift
*/
gdouble get_Jn_drift(struct device *in)
{
long double J=0.0;


J=three_d_avg(in, in->Jn_drift);

return J;
}

/**
* @brief Jp diffusion
*/
gdouble get_Jp_diffusion(struct device *in)
{
long double J=0.0;

J=three_d_avg(in, in->Jp_diffusion);

return J;
}

/**
* @brief Jp drift
*/
gdouble get_Jp_drift(struct device *in)
{
long double J=0.0;

J=three_d_avg(in, in->Jp_drift);

return J;
}

/**
* @brief Get equivlent V
*/
gdouble get_equiv_V(struct simulation *sim,struct device *in)
{
gdouble J=0.0;
gdouble Vapplied=0.0;
Vapplied=contact_get_active_contact_voltage(sim,in);
//if (in->adv_sim==FALSE)
//{
//J=get_J_recom(in);
//}else
//{
J=get_equiv_J(sim,in);
//}

gdouble V=J*in->Rcontact*in->area+Vapplied;
return V;
}

/**
* @brief Get J
*/
gdouble get_J(struct device *in)
{
//int i;
gdouble ret=0.0;
//printf("%Le %Le\n",get_J_left(in),get_J_right(in));
ret=(get_J_left(in)+get_J_right(in))/2.0;

return ret;
}

/**
* @brief Get J left
*/
gdouble get_J_left(struct device *in)
{
long double ret=0.0;
ret = contacts_get_Jleft(in);
return ret;
}

/**
* @brief Get J right
*/
gdouble get_J_right(struct device *in)
{
long double ret=0.0;
ret = contacts_get_Jright(in);
return ret;
}

