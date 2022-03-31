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

/** @file jv.c
	@brief Simulate JV curve.
*/


#include <sim.h>
#include <exp.h>
#include "jv.h"
#include <dump.h>
#include <dynamic_store.h>
#include "newton_tricks.h"
#include <inp.h>
#include <dat_file.h>
#include <gui_hooks.h>
#include <pl.h>
#include <log.h>
#include <lang.h>
#include <remesh.h>
#include <plot.h>
#include <cal_path.h>
#include <contacts.h>
#include <contacts_vti_store.h>
#include "measure.h"
#include <light_fun.h>
#include <cache.h>
#include <ray_fun.h>
#include <enabled_libs.h>
#include <json.h>
#include <lock.h>
#include <device_fun.h>
#include <memory.h>

static int unused __attribute__((unused));

void jv_voltage_step(struct simulation *sim,struct device *in,long double *V, long double *Vstep)
{
	long double Vapplied=*V;
	long double Vtarget=*V+*Vstep;
	long double dV=0.05;
	int i;
	int steps=*Vstep/dV;
	long double Vexternal;
	long double J;
	if (in->dynamic_mesh==TRUE)
	{
		if (*Vstep>0.1)
		{
			remesh_shrink(sim,in);
			//printf("that's never going to work\n");
			for (i=0;i<steps;i++)
			{
				contact_set_active_contact_voltage(sim,in,Vapplied);
				Vapplied+=dV;
				//printf_log(sim,".");
				if (in->solver_verbosity>=SOLVER_VERBOSITY_AT_END)
				{
					Vexternal=get_equiv_V(sim,in);
					J=get_equiv_J(sim,in);
					printf_log(sim,"*%s=%Lf (%Lf) %s = %Le mA (%Le A/m^2) %Le\n",_("Voltage"),Vapplied,Vexternal,_("Current"),get_I(in)/1e-3,J,in->ns.last_error);
				}
				newton_sim_simple(sim,in);
			}

			remesh_reset(sim,in,0.0);
		}
	}
//	printf_log(sim,"\n");
	*V+=*Vstep;

}

int get_step_n(long double step0,long double step_mul,long double V)
{
int n=0;
long double pos=0;
long double dv=fabs(step0);

while(pos<fabs(V))
{
	pos+=dv;
	dv*=step_mul;
	n++;
}
return n;//roundl(log(1.0+(fabs(V)/fabs(step0))*log(step_mul))/log(step_mul));
}

void sim_jv(struct simulation *sim,struct device *in)
{

long double Vapplied=0.0;
int up=TRUE;
struct jv config;
int ittr=0;
long double J;
long double Pden;
int first=TRUE;
long double Vlast;
long double Jlast;
long double Pdenlast=-1e6;
long double Vexternal;
long double V=0.0;

long double mue=0.0;
long double muh=0.0;
 
struct json_obj *json_jv;
struct dimensions *dim=&in->ns.dim;

find_n0(sim,in);

light_solve_and_update(sim,in,&(in->mylight),0.0);

//printf("%Le\n",in->Gn[0][0][0]);
//getchar();

printf_log(sim,_("Running JV simulation\n"));
struct dat_file buf;
dat_file_init(&buf);

struct dynamic_store store;

dump_dynamic_init(sim,&store,in);

struct contacts_vti_store contact_store;
dump_contacts_init(sim,in,&contact_store);

json_jv=json_find_sim_struct(sim, &(in->config),in->simmode);

jv_load_config(sim,&config,json_jv);

if (config.jv_Rcontact!=-1.0)
{
	in->Rcontact=gfabs(config.jv_Rcontact);
}

if (config.jv_Rshunt!=-1.0)
{
	in->Rshunt=gfabs(config.jv_Rshunt);
}


long double Vstop=config.Vstop;
long double Vstep=config.Vstep;

struct math_xy ivexternal;
inter_init(sim,&ivexternal);

struct math_xy jvexternal;
inter_init(sim,&jvexternal);

struct math_xy jv_internal;
inter_init(sim,&jv_internal);

struct math_xy vjexternal;
inter_init(sim,&vjexternal);

struct math_xy power_den;
inter_init(sim,&power_den);

struct math_xy charge;
inter_init(sim,&charge);

struct math_xy charge_with_surface_charge;
inter_init(sim,&charge_with_surface_charge);

struct math_xy charge_tot;
inter_init(sim,&charge_tot);

struct math_xy klist;
inter_init(sim,&klist);

struct math_xy lv;
inter_init(sim,&lv);

struct math_xy lj;
inter_init(sim,&lj);

struct math_xy R_list;
inter_init(sim,&R_list);

struct math_xy n_list;
inter_init(sim,&n_list);

struct math_xy tau_list;
inter_init(sim,&tau_list);

struct math_xy tau_all_list;
inter_init(sim,&tau_all_list);

struct math_xy Tl;
inter_init(sim,&Tl);

struct math_xy v_optical_efficiency;
inter_init(sim,&v_optical_efficiency);

struct math_xy sigma;		//conductivity
inter_init(sim,&sigma);

//contact_set_active_contact_voltage(sim,in,Vapplied);
//char json_path[PATH_MAX];
//join_path(2,json_path,in->output_path,"json.dat");
//in->config.compact=TRUE;
//json_save_as(sim,json_path,&(in->config));

//printf("%d\n",in->cache.enabled);
state_cache_enable(sim,in);

if ((dim->zlen>1) || (dim->xlen>1))
{
	contact_set_wanted_active_contact_voltage(sim,in,config.Vstart);
	//contact_set_active_contact_voltage(sim,in,config.Vstart);
	ntricks_auto_ramp_contacts(sim,in);
}else
{
	if (gfabs(config.Vstart-Vapplied)>0.2)
	{
		ramp_externalv(sim,in,Vapplied,config.Vstart);
	}
}

//ray_solve_all(sim,in);
//exit(0);
//sim_externalv(in,in->Vapplied);

//contacts_dump(sim,in);
//printf("wait a\n");
//getchar();

remesh_reset(sim,in,Vapplied);
//if (in->remesh==TRUE)
//{
//
//}
//long double start_Q=(three_d_integrate(dim, in->n)+three_d_integrate(dim, in->nt_all))*Qe;
long double sun_orig=light_get_sun(&(in->mylight));
light_set_sun(&(in->mylight),sun_orig*config.jv_light_efficiency);
light_solve_and_update(sim,in,&(in->mylight),0.0);

struct light *li=&(in->mylight);
struct dimensions *ldim=&(li->dim);
int z;
int x;
int y;

/*for (z=0;z<ldim->zlen;z++)
{
	for (x=0;x<ldim->xlen;x++)
	{
		for (y=0;y<ldim->ylen;y++)
		{
			printf("%d %d %d %Le\n",z,x,y,li->Gn[z][x][y]);
		}
	}

}
getchar();*/
//contacts_dump(sim,in);
//printf("wait b\n");
//getchar();
newton_push_state(in);

newton_set_min_ittr(in,30);

Vapplied=config.Vstart;
//contacts_dump(sim,in);
//printf("wait c\n");
//getchar();

contact_set_active_contact_voltage(sim,in,Vapplied);
//contacts_dump(sim,in);
V=Vapplied;
newton_sim_simple(sim,in);
//printf("wait d\n");
//getchar();

newton_pop_state(in);
//newton_set_min_ittr(in,0);

//long double k_voc=0.0;
long double n_voc=-1.0;
long double r_voc=-1.0;
long double nsc=0.0;
long double n_trap_voc=-1.0;
long double p_trap_voc=-1.0;
long double n_free_voc=-1.0;
long double p_free_voc=-1.0;
long double np_voc_tot=-1.0;
long double r_pmax=0.0;
long double n_pmax=0.0;
long double mue_pmax=0.0;
long double muh_pmax=0.0;
long double mue_jsc=0.0;
long double muh_jsc=0.0;

//Device characterisation
long double Voc=-1.0;
long double Jsc=-1.0;
long double FF=-1.0;
long double Pmax=-1.0;
long double V_pmax=-1.0;
long double J_pmax=-1.0;


long double cal_step=0;

long double n_steps=0.0;
char send_data[200];
long double V_simple_last=-1.0;
int power_min_pos=0;
int dump_step=0;
long double optical_power_m2=0.0;

struct newton_state *ns=&(in->ns);
struct heat *thermal=&(in->thermal);



long double mu_pmax=-1.0;
long double mu_voc=-1.0;
long double mu_jsc=0.0;
long double mu_geom_pmax=-1.0;
long double mu_geom_voc=-1.0;
long double mu_geom_jsc=0.0;

long double mu_geom_micro_pmax=-1.0;
long double mu_geom_micro_voc=0.0;
long double mu_geom_micro_jsc=0.0;

long double tau_voc=-1.0;
long double tau_pmax=-1.0;
long double tau_all_voc=-1.0;
long double tau_all_pmax=-1.0;
long double tau=-1.0;
long double tau_all=-1.0;

long double theta_srh_free=-1.0;
long double theta_srh_free_trap=-1.0;
long double k_voc=-1.0;
long double pce=-1.0;

int dump_sim_info=FALSE;
int found_voc=FALSE;
int found_jsc=FALSE;
long double np_extracted;

n_steps=get_step_n(config.Vstep,config.jv_step_mul,config.Vstart);
n_steps+=get_step_n(config.Vstep,config.jv_step_mul,config.Vstop);

in->stop=FALSE;

	if (dump_can_i_dump(sim,in, "sim_info.dat")==0)
	{
		dump_sim_info=TRUE;
	}

up=TRUE;
if (config.Vstop<config.Vstart)
{
	up=FALSE;
}

	do
	{
		Vapplied=V;

		contact_set_active_contact_voltage(sim,in,Vapplied);

		newton_sim_simple(sim,in);

		contacts_cal_J_and_i(sim,in);

		J=get_equiv_J(sim,in);
		Vexternal=get_equiv_V(sim,in);

		sprintf(send_data,"percent:%Lf",(long double)ittr/n_steps);
		gui_send_data(sim,gui_sub,send_data);

		sprintf(send_data,"text:Voltage %.2Lf V/%.2Lf V",V,config.Vstop);
		gui_send_data(sim,gui_sub,send_data);
		if (ittr>0)
		{

			inter_append(&jvexternal,Vexternal,J);
			inter_append(&vjexternal,J,Vexternal);
			inter_append(&power_den,Vexternal,J*Vexternal);

			inter_append(&ivexternal,Vexternal,get_equiv_I(sim,in));
			inter_append(&jv_internal,Vapplied,get_J(in));

		}
		ittr++;
		//printf("%Le %Le\n",three_d_integrate(dim, in->n),three_d_integrate(dim, in->nt_all));
		//long double charge_now=(three_d_integrate(dim, in->n)+three_d_integrate(dim, in->nt_all))*Qe;
		//printf("%Le\n",charge_now-start_Q);
/*three_d_integrate(dim, in->nf_save)

	ret=three_d_avg(in, in->n);
	ret-=three_d_avg(in, in->nf_save);

	ret+=three_d_avg(in, in->nt_all);
	ret-=three_d_avg(in, in->nt_save);*/

		np_extracted=get_extracted_np(in);
		inter_append(&charge,Vexternal,np_extracted);
		inter_append(&charge_tot,Vexternal,get_np_tot(in));

		inter_append(&charge_with_surface_charge,Vexternal,np_extracted+(cal_contact_charge(in)-in->contact_charge));

		double R;
		double rho;
		R=Vexternal/(get_avg_J(in)*in->A);
		rho=R*in->A/in->ylen;
		inter_append(&sigma,Vexternal,1.0/rho);

		Pden=gfabs(J*Vexternal);

		plot_now(sim,in,"jv.plot");
		stop_start(sim,in);
		dump_dynamic_add_data(sim,&store,in,Vexternal);


		dump_contacts_add_data(sim,in,&contact_store);

		V_simple_last=V;
		if (in->solver_verbosity>=SOLVER_VERBOSITY_AT_END)
		{
			char mytemp[200];
			contacts_state_to_string(sim,mytemp, in);
			printf_log(sim," %s f()=%Le %lf ms\n",mytemp,ns->last_error, ns->last_time);
		}

		if (first==FALSE)
		{

			//check if we have crossed 0V
			if (dump_sim_info==TRUE)
			{
				//Jsc
				if ((Vlast<=0)&&(Vexternal>=0.0))
				{
					nsc=get_extracted_np(in);
					get_avg_mu(in,&mue_jsc,&muh_jsc);
					mu_jsc=(mue_jsc+muh_jsc)/2.0;
					mu_geom_jsc=sqrt(mue_jsc*muh_jsc);

					get_avg_geom_micro_mu(in,&mu_geom_micro_jsc);
					found_jsc=TRUE;
				}

				if (light_get_sun(&(in->mylight))!=0.0)
				{

					//Voc
					if ((Jlast<=0)&&(J>=0.0))
					{

						np_voc_tot=get_total_np(in);
						n_trap_voc=get_n_trapped_charge(in);
						n_free_voc=get_free_n_charge(in);
						p_trap_voc=get_p_trapped_charge(in);
						p_free_voc=get_free_p_charge(in);
						get_avg_mu(in,&mue,&muh);
						mu_voc=(mue+muh)/2.0;
						mu_geom_voc=sqrt(mue*muh);

						get_avg_geom_micro_mu(in,&mu_geom_micro_voc);

						found_voc=TRUE;
					}
				}

				//Pmax
				if ((Pden>Pdenlast)&&(Vexternal>0.0)&&(J<0.0))
				{
					get_avg_mu(in,&mue_pmax,&muh_pmax);
					mu_pmax=(mue_pmax+muh_pmax)/2.0;
					mu_geom_pmax=sqrt(mue_pmax*muh_pmax);

					get_avg_geom_micro_mu(in,&mu_geom_micro_pmax);
					Pdenlast=Pden;
				}

				inter_append(&R_list,Vexternal,get_avg_recom(in));
				inter_append(&n_list,Vexternal,get_extracted_np(in));
				get_tau(in,&tau,&tau_all);

				inter_append(&tau_list,Vexternal,tau);
				inter_append(&tau_all_list,Vexternal,tau_all);

			}

			if (thermal->newton_enable_external_thermal==TRUE)
			{
				inter_append(&Tl,Vexternal,get_avg_Tl(in));
			}

			//printf("%d\n",config.jv_use_external_voltage_as_stop);
			if (config.jv_use_external_voltage_as_stop==TRUE)
			{
				if (up==TRUE)
				{
					if (Vexternal>Vstop)
					{
						printf_log(sim,"%s %Le>%Le\n",_("Stopping because of Vexternal"),Vexternal,Vstop);
						break;
					}
				}else
				{
					if (Vexternal<Vstop)
					{
						printf_log(sim,"%s %Le>%Le\n",_("Stopping because of Vexternal"),Vexternal,Vstop);
						break;
					}
				}
			}

		}
		//printf("Vapplied=%Le %Le\n",Vapplied,contact_get_active_contact_voltage(sim,in));
		//getchar();

		Jlast=J;
		Vlast=Vexternal;

		first=FALSE;

		if (config.dump_verbosity!=0)
		{
			if ((dump_step>=config.dump_verbosity)||(config.dump_verbosity==1))
			{
				dump_write_to_disk(sim,in);

				if (config.dump_energy_space==ENERGY_SPACE_MAP)
				{
					dump_device_map(sim,in->output_path,in);
				}

				if (config.dump_energy_space==SINGLE_MESH_POINT)
				{
					dump_energy_slice(sim,in,config.dump_x, config.dump_y, config.dump_z);
				}

				dump_step=0;
			}
			dump_step++;
		}





		if (in->emission_enabled==TRUE)
		{
			#ifdef libemission_enabled
				optical_power_m2=calculate_photon_power_m2(sim,in);
			#endif

			inter_append(&lv,Vexternal,optical_power_m2);

			inter_append(&lj,J,optical_power_m2);
			inter_append(&v_optical_efficiency,Vexternal,100.0*optical_power_m2/(J*Vexternal));
		}
		//printf("%Le %le\n",get_avg_recom(in),in->my_image.avg_extract_eff);

		jv_voltage_step(sim,in,&V,&Vstep);
		if (config.jv_step_mul>1.0)
		{
			cal_step=get_step_n(config.Vstep,config.jv_step_mul,V);//roundl(log(1.0+(fabs(V)/config.Vstep)*log(config.jv_step_mul))/log(config.jv_step_mul));
			if (cal_step<0)
			{
				cal_step=1.0;
			}
			Vstep=config.Vstep*powl(config.jv_step_mul,cal_step);
			//printf("%Lf %Lf %Lf %Lf\n",Vstep,cal_step,config.Vstep,config.jv_step_mul);
			//getchar();
		}

		//Vstep=fabs(V)(pow(config.jv_step_mul;
		//dialog_set_progress ((in->Vstart+V)/(in->Vstop-in->Vstart));
		if ((up==TRUE)&&(V>Vstop))
		{
			in->stop=TRUE;
		}

		if ((up==FALSE)&&(V<Vstop))
		{
			in->stop=TRUE;
		}

		if (config.jv_use_external_voltage_as_stop==TRUE)
		{
			if (get_equiv_J(sim,in)>config.jv_max_j)
			{
				in->stop=TRUE;
			}
		}
		if (in->stop==TRUE)
		{
			break;
		}

		inter_append(&klist,get_extracted_np(in),get_avg_recom(in)/(pow(get_extracted_np(in),2.0)));

		stop_start(sim,in);


		poll_gui(sim);

		//contacts_detailed_dump(in);
		//contacts_dump(sim,in);
		//dump_contacts_save(sim,in,&contact_store);

		if (config.jv_single_point==TRUE)
		{
			break;
		}
	}while(1);

double sigma_out=inter_get(&sigma,0.3);

if (power_den.len>0)
{
	Jsc=inter_get(&jvexternal,0.0);
	//r_jsc=inter_get(&jvexternal,0.0);
	if (Jsc==0.0)
	{
		Jsc=inter_get_min(&jvexternal);
	}

	if (found_voc==TRUE)
	{
		power_min_pos=inter_get_min_pos(&power_den);
		Pmax=-1.0*power_den.data[power_min_pos];
		r_pmax=R_list.data[power_min_pos];
		n_pmax=n_list.data[power_min_pos];

		tau_pmax=tau_list.data[power_min_pos];
		tau_all_pmax=tau_all_list.data[power_min_pos];

		V_pmax=power_den.x[power_min_pos];
		J_pmax=jvexternal.data[power_min_pos];
		Voc=inter_get(&vjexternal,0.0);
		r_voc=inter_get(&R_list,Voc);
		n_voc=inter_get(&n_list,Voc);

		tau_voc=inter_get(&tau_list,Voc);
		tau_all_voc=inter_get(&tau_all_list,Voc);

		k_voc=r_voc/n_voc;
		FF=gfabs(Pmax/(Jsc*Voc));
		pce=gfabs(100.0*Pmax/(1000.0*light_get_sun(&(in->mylight))));
	}

}
printf_log(sim,"Max possible Jsc = %Lf\n",get_max_Jsc(in));
if (get_dump_status(sim,dump_print_text)==TRUE)
{

	if (Voc!=-1.0)
	{
		printf_log(sim,"Voc= %Lf (V)\n",Voc);
	}
	printf_log(sim,"Jsc= %Lf (A/m^2)\n",Jsc);
	if (Pmax!=-1.0)
	{
		printf_log(sim,"Pmax= %Lf (W/m^2)\n",Pmax);
		printf_log(sim,"Pmax %s= %Lf (V)\n",_("Voltage"),V_pmax);
		printf_log(sim,"%s= %Lf percent\n",_("Efficiency"),gfabs(Pmax/light_get_sun(&(in->mylight))/1000)*100.0);
	}

	if (FF!=-1.0)
	{
		printf_log(sim,"FF= %Lf\n",FF*100.0);
	}

}

long double added=0.0;
added=get_tot_photons_abs(in);
printf("photon density= %Le\n", added);
printf("%Le %Le",Pmax,light_get_sun(&(in->mylight)));
//getchar();
if (in->circuit_simulation==FALSE)
{
	theta_srh_free=((in->mun_y[0][0][0]+in->mup_y[0][0][0])/2.0)*tau_pmax*Voc/(dim->ylen*dim->ylen);
	theta_srh_free=sqrt(theta_srh_free);

	theta_srh_free_trap=((mue_pmax+muh_pmax)/2.0)*tau_pmax*Voc/(dim->ylen*dim->ylen);
	theta_srh_free_trap=sqrt(theta_srh_free_trap);
}

if (config.dump_verbosity>=0)
{
	if (buffer_set_file_name(sim,in,&buf,"sim_info.dat")==0)
	{
		FILE *out;
		out=fopena(in->output_path,"sim_info.dat","w");
		if (out!=NULL)
		{
			fprintf(out,"{\n");
			fprintf(out,"\t\"icon\":%s,\n","\"json_file\"");
			fprintf(out,"\t\"ff\":\"%Lf\",\n",FF);
			fprintf(out,"\t\"pce\":\"%Lf\",\n",pce);
			fprintf(out,"\t\"Pmax\":\"%Lf\",\n",Pmax);
			fprintf(out,"\t\"voc\":\"%Lf\",\n",Voc);
			fprintf(out,"\t\"voc_R\":\"%Le\",\n",r_voc);
			fprintf(out,"\t\"k_voc\":\"%Le\",\n",k_voc);
			fprintf(out,"\t\"jv_pmax_n\":\"%Le\",\n",n_pmax);
			fprintf(out,"\t\"voc_nt\":\"%Le\",\n",n_trap_voc);
			fprintf(out,"\t\"voc_pt\":\"%Le\",\n",p_trap_voc);
			fprintf(out,"\t\"voc_nf\":\"%Le\",\n",n_free_voc);
			fprintf(out,"\t\"voc_pf\":\"%Le\",\n",p_free_voc);
			fprintf(out,"\t\"jsc\":\"%Le\",\n",Jsc);
			fprintf(out,"\t\"jv_jsc_n\":\"%Le\",\n",nsc);
			fprintf(out,"\t\"jv_vbi\":\"%Le\",\n",in->Vbi);
			fprintf(out,"\t\"jv_gen\":\"%Le\",\n",get_avg_gen(in));
			fprintf(out,"\t\"voc_np_tot\":\"%Le\",\n",np_voc_tot);
			fprintf(out,"\t\"j_pmax\":\"%Le\",\n",J_pmax);
			fprintf(out,"\t\"v_pmax\":\"%Le\",\n",V_pmax);
			fprintf(out,"\t\"mu_jsc\":\"%Le\",\n",mu_jsc);
			fprintf(out,"\t\"mu_pmax\":\"%Le\",\n",mu_pmax);
			fprintf(out,"\t\"mu_voc\":\"%Le\",\n",mu_voc);
			fprintf(out,"\t\"mu_geom_jsc\":\"%Le\",\n",mu_geom_jsc);
			fprintf(out,"\t\"mu_geom_pmax\":\"%Le\",\n",mu_geom_pmax);
			fprintf(out,"\t\"mu_geom_voc\":\"%Le\",\n",mu_geom_voc);
			fprintf(out,"\t\"mu_geom_micro_jsc\":\"%Le\",\n",mu_geom_micro_jsc);
			fprintf(out,"\t\"mu_geom_micro_pmax\":\"%Le\",\n",mu_geom_micro_pmax);
			fprintf(out,"\t\"mu_geom_micro_voc\":\"%Le\",\n",mu_geom_micro_voc);
			fprintf(out,"\t\"mue_pmax\":\"%Le\",\n",mue_pmax);
			fprintf(out,"\t\"muh_pmax\":\"%Le\",\n",muh_pmax);
			fprintf(out,"\t\"mue_jsc\":\"%Le\",\n",mue_jsc);
			fprintf(out,"\t\"muh_jsc\":\"%Le\",\n",muh_jsc);
			fprintf(out,"\t\"tau_voc\":\"%Le\",\n",tau_voc);
			fprintf(out,"\t\"tau_pmax\":\"%Le\",\n",tau_pmax);
			fprintf(out,"\t\"tau_all_voc\":\"%Le\",\n",tau_all_voc);
			fprintf(out,"\t\"tau_all_pmax\":\"%Le\",\n",tau_all_pmax);

			fprintf(out,"\t\"sigma\":\"%le\",\n",sigma_out);

			fprintf(out,"\t\"theta_srh_free\":\"%Le\",\n",theta_srh_free);
			fprintf(out,"\t\"theta_srh_free_trap\":\"%Le\",\n",theta_srh_free_trap);
			fprintf(out,"\t\"device_C\":\"%Le\"\n",in->C);
			fprintf(out,"}");
			fclose(out);
		}
	}
}

if (config.dump_verbosity==1)
{
	if (buffer_set_file_name(sim,in,&buf,"jv_internal.csv")==0)
	{
		buffer_malloc(&buf);
		buf.y_mul=1.0;
		buf.data_mul=1.0;
		sprintf(buf.title,"%s",_("Voltage/Current across just the ideal diode"));
		strcpy(buf.type,"xy");
		strcpy(buf.y_label,_("Applied Voltage"));
		strcpy(buf.data_label,_("Current density"));
		strcpy(buf.y_units,"Volts");
		strcpy(buf.data_units,"A m^{-2}");
		buf.logscale_x=0;
		buf.logscale_y=0;
		dat_file_add_xy_data(sim,&buf,jv_internal.x, jv_internal.data, jv_internal.len);
		buffer_dump_path(sim,in->output_path,NULL,&buf);
		buffer_free(&buf);
	}
}

if (buffer_set_file_name(sim,in,&buf,"k.csv")==0)
{
	buffer_malloc(&buf);
	buf.y_mul=1.0;
	buf.x_mul=1.0;
	sprintf(buf.title,"%s - %s",_("Recombination prefactor"),_("Applied voltage"));
	strcpy(buf.type,"xy");
	strcpy(buf.y_label,_("Applied Voltage"));
	strcpy(buf.data_label,_("Recombination prefactor"));
	strcpy(buf.y_units,"Volts");
	strcpy(buf.data_units,"m^{-6}s^{-1}");
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.logscale_z=0;
	dat_file_add_xy_data(sim,&buf,klist.x, klist.data, klist.len);
	buffer_dump_path(sim,in->output_path,NULL,&buf);
	buffer_free(&buf);
}

if (buffer_set_file_name(sim,in,&buf,"charge.dat")==0)
{
	buffer_malloc(&buf);
	buf.y_mul=1.0;
	buf.data_mul=1.0;
	sprintf(buf.title,"%s - %s",_("Excess charge density above equilibrium"),_("Applied voltage"));
	strcpy(buf.type,"xy");
	strcpy(buf.y_label,_("Applied Voltage"));
	strcpy(buf.data_label,_("Charge density"));
	strcpy(buf.y_units,"Volts");
	strcpy(buf.data_units,"m^{-3}");
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.logscale_z=0;
	dat_file_add_xy_data(sim,&buf,charge.x, charge.data, charge.len);
	buffer_dump_path(sim,in->output_path,NULL,&buf);
	buffer_free(&buf);
}

if (buffer_set_file_name(sim,in,&buf,"charge_with_surface.csv")==0)
{
	buffer_malloc(&buf);
	buf.y_mul=1.0;
	buf.data_mul=1.0;
	sprintf(buf.title,"%s - %s",_("Charge density with correction for surface charge"),_("Applied voltage"));
	strcpy(buf.type,"xy");
	strcpy(buf.y_label,_("Applied Voltage"));
	strcpy(buf.data_label,_("Charge density"));
	strcpy(buf.y_units,"Volts");
	strcpy(buf.data_units,"m^{-3}");
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.logscale_z=0;
	dat_file_add_xy_data(sim,&buf,charge_with_surface_charge.x, charge_with_surface_charge.data, charge_with_surface_charge.len);
	buffer_dump_path(sim,in->output_path,NULL,&buf);
	buffer_free(&buf);
}


if (buffer_set_file_name(sim,in,&buf,"charge_tot.csv")==0)
{
	buffer_malloc(&buf);
	buf.y_mul=1.0;
	buf.data_mul=1.0;
	sprintf(buf.title,"%s - %s",_("Total charge density"),_("Applied voltage"));
	strcpy(buf.type,"xy");
	strcpy(buf.y_label,_("Applied Voltage"));
	strcpy(buf.data_label,_("Total charge density"));
	strcpy(buf.y_units,"Volts");
	strcpy(buf.data_units,"m^{-3}");
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.logscale_z=0;
	dat_file_add_xy_data(sim,&buf,charge_tot.x, charge_tot.data, charge.len);
	buffer_dump_path(sim,in->output_path,NULL,&buf);
	buffer_free(&buf);
}

if (in->ncontacts==2)
{
	if (buffer_set_file_name(sim,in,&buf,"jv.dat")==0)
	{
		buffer_malloc(&buf);
		buf.y_mul=1.0;
		buf.data_mul=1.0;
		sprintf(buf.title,"%s - %s",_("Current density"),_("Applied voltage"));
		strcpy(buf.type,"xy");
		strcpy(buf.y_label,_("Applied Voltage"));
		strcpy(buf.data_label,_("Current density"));
		strcpy(buf.y_units,"Volts");
		strcpy(buf.data_units,"A m^{-2}");
		buf.logscale_x=0;
		buf.logscale_y=0;
		dat_file_add_xy_data(sim,&buf,jvexternal.x, jvexternal.data, jvexternal.len);
		buffer_dump_path(sim,in->output_path,NULL,&buf);
		buffer_free(&buf);
	}

	if (buffer_set_file_name(sim,in,&buf,"iv.dat")==0)
	{
		math_xy_mul_long_double(&jvexternal,in->area);
		buffer_malloc(&buf);
		buf.y_mul=1.0;
		buf.data_mul=1.0;
		sprintf(buf.title,"%s - %s",_("Current "),_("Applied voltage"));
		strcpy(buf.type,"xy");
		strcpy(buf.y_label,_("Applied Voltage"));
		strcpy(buf.data_label,_("Current"));
		strcpy(buf.y_units,_("Volts"));
		strcpy(buf.data_units,"A");
		buf.logscale_x=0;
		buf.logscale_y=0;
		dat_file_add_xy_data(sim,&buf,jvexternal.x, jvexternal.data, jvexternal.len);
		buffer_dump_path(sim,in->output_path,NULL,&buf);
		buffer_free(&buf);
	}
}

if (thermal->newton_enable_external_thermal==TRUE)
{
	if (buffer_set_file_name(sim,in,&buf,"v_Tl.csv")==0)
	{
		buffer_malloc(&buf);
		buf.y_mul=1.0;
		buf.data_mul=1.0;
		sprintf(buf.title,"%s - %s",_("Voltage"),_("Lattice temperature"));
		strcpy(buf.type,"xy");
		strcpy(buf.y_label,_("Voltage"));
		strcpy(buf.data_label,_("Lattice temperature"));
		strcpy(buf.y_units,_("Volts"));
		strcpy(buf.data_units,"K");
		buf.logscale_x=0;
		buf.logscale_y=0;
		dat_file_add_xy_data(sim,&buf,Tl.x, Tl.data, Tl.len);
		buffer_dump_path(sim,in->output_path,NULL,&buf);
		buffer_free(&buf);
	}
}

if (in->emission_enabled==TRUE)
{
	if (buffer_set_file_name(sim,in,&buf,"vl.csv")==0)
	{
		buffer_malloc(&buf);
		buf.y_mul=1.0;
		buf.data_mul=1.0;
		buf.data_mul=1;
		sprintf(buf.title,"%s - %s",_("Voltage"),_("Light flux"));
		strcpy(buf.type,"xy");
		strcpy(buf.y_label,("Applied Voltage"));
		strcpy(buf.data_label,("Light flux"));
		strcpy(buf.y_units,"Volts");
		strcpy(buf.data_units,"W m^{-2}");
		buf.logscale_x=0;
		buf.logscale_y=0;
		dat_file_add_xy_data(sim,&buf,lv.x, lv.data, lv.len);
		buffer_dump_path(sim,in->output_path,NULL,&buf);
		buffer_free(&buf);
	}

	if (buffer_set_file_name(sim,in,&buf,"jl.csv")==0)
	{
		buffer_malloc(&buf);
		buf.y_mul=1.0;
		buf.data_mul=1.0;
		sprintf(buf.title,"%s - %s",_("Current density"),_("Light flux"));
		strcpy(buf.type,"xy");
		strcpy(buf.y_label,("Current density"));
		strcpy(buf.data_label,_("Light flux"));
		strcpy(buf.y_units,"A m^{-2}");
		strcpy(buf.data_units,"W m^{-2}");
		buf.logscale_x=0;
		buf.logscale_y=0;
		dat_file_add_xy_data(sim,&buf,lj.x, lj.data, lj.len);
		buffer_dump_path(sim,in->output_path,NULL,&buf);
		buffer_free(&buf);
	}

	if (buffer_set_file_name(sim,in,&buf,"v_optical_efficiency.csv")==0)
	{
		buffer_malloc(&buf);
		buf.y_mul=1.0;
		buf.data_mul=1.0;
		sprintf(buf.title,"%s - %s",_("Voltage"),_("Power in Optical emission/Electrical power in"));
		strcpy(buf.type,"xy");
		strcpy(buf.y_label,("Voltage"));
		strcpy(buf.data_label,_("Efficiency"));
		strcpy(buf.y_units,"V");
		strcpy(buf.data_units,"%");
		buf.logscale_x=0;
		buf.logscale_y=0;
		dat_file_add_xy_data(sim,&buf,v_optical_efficiency.x, v_optical_efficiency.data, v_optical_efficiency.len);
		buffer_dump_path(sim,in->output_path,NULL,&buf);
		buffer_free(&buf);
	}


	if (buffer_set_file_name(sim,in,&buf,"sigma.csv")==0)
	{
		buffer_malloc(&buf);
		buf.y_mul=1.0;
		buf.data_mul=1.0;
		sprintf(buf.title,"%s - %s",_("Voltage"),_("Conductivity"));
		strcpy(buf.type,"xy");
		strcpy(buf.y_label,("Voltage"));
		strcpy(buf.data_label,_("Conductivity"));
		strcpy(buf.y_units,"V");
		strcpy(buf.data_units,"S/m");
		buf.logscale_x=0;
		buf.logscale_y=0;
		dat_file_add_xy_data(sim,&buf,sigma.x, sigma.data, sigma.len);
		buffer_dump_path(sim,in->output_path,NULL,&buf);
		buffer_free(&buf);
	}
}

inter_free(&jvexternal);
inter_free(&vjexternal);
inter_free(&jv_internal);
inter_free(&power_den);
inter_free(&charge);
inter_free(&charge_with_surface_charge);
inter_free(&ivexternal);
inter_free(&lv);
inter_free(&lj);
inter_free(&klist);
inter_free(&R_list);
inter_free(&n_list);
inter_free(&tau_list);
inter_free(&tau_all_list);
inter_free(&Tl);
inter_free(&v_optical_efficiency);
inter_free(&sigma);
inter_free(&charge_tot);

dump_dynamic_save(sim,in,in->output_path,&store);
dump_dynamic_free(sim,in,&store);


dump_contacts_save(sim,in,&contact_store);
dump_contacts_free(sim,in,&contact_store);

light_set_sun(&(in->mylight),sun_orig);
}




void jv_load_config(struct simulation *sim,struct jv* in, struct json_obj *json_jv)
{
	struct json_obj *json_config;
	json_config=json_obj_find(json_jv, "config");
	if (json_config==NULL)
	{
		ewe(sim,"No config section found\n");
	}

	json_get_long_double(sim,json_config, &(in->Vstart),"Vstart");
	json_get_long_double(sim,json_config, &(in->Vstop),"Vstop");
	json_get_long_double(sim,json_config, &(in->Vstep),"Vstep");

	in->Vstep=fabs(in->Vstep);

	if (in->Vstop<in->Vstart)
	{
		in->Vstep*=-1.0;
	}

	json_get_long_double(sim,json_config, &(in->jv_step_mul),"jv_step_mul");
	if (in->jv_step_mul<1.0)
	{
		in->jv_step_mul=1.0;
	}
	json_get_long_double(sim,json_config, &(in->jv_light_efficiency),"jv_light_efficiency");
	in->jv_light_efficiency=gfabs(in->jv_light_efficiency);

	json_get_long_double(sim,json_config, &(in->jv_max_j),"jv_max_j");


	json_get_long_double(sim,json_config, &(in->jv_Rshunt),"jv_Rshunt");
	json_get_long_double(sim,json_config, &(in->jv_Rcontact),"jv_Rcontact");
	json_get_english(sim,json_config, &(in->jv_single_point),"jv_single_point");

	json_get_int(sim,json_config, &(in->dump_verbosity),"dump_verbosity");


	json_get_english(sim,json_config, &(in->dump_energy_space),"dump_energy_space");
	json_get_int(sim,json_config, &(in->dump_x),"dump_x");
	json_get_int(sim,json_config, &(in->dump_y),"dump_y");
	json_get_int(sim,json_config, &(in->dump_z),"dump_z");


	json_get_english(sim,json_config, &(in->jv_use_external_voltage_as_stop),"jv_use_external_voltage_as_stop");

}
