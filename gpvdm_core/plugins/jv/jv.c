//
// General-purpose Photovoltaic Device Model gpvdm.com - a drift diffusion
// base/Shockley-Read-Hall model for 1st, 2nd and 3rd generation solarcells.
// The model can simulate OLEDs, Perovskite cells, and OFETs.
// 
// Copyright (C) 2008-2020 Roderick C. I. MacKenzie
// 
// https://www.gpvdm.com
// r.c.i.mackenzie at googlemail.com
// 
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the GPVDM nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL Roderick C. I. MacKenzie BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
				if (get_dump_status(sim,dump_print_converge)==TRUE)
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
long double Pdenlast;
long double Vexternal;
long double V=0.0;
struct dimensions *dim=&in->ns.dim;

light_solve_and_update(sim,in,&(in->mylight),0.0);


printf_log(sim,_("Running JV simulation\n"));
struct dat_file buf;
buffer_init(&buf);

struct dynamic_store store;

dump_dynamic_init(sim,&store,in);

struct contacts_vti_store contact_store;
dump_contacts_init(sim,in,&contact_store);



char config_file_name[200];

if (find_config_file(sim,config_file_name,get_input_path(sim),in->simmode,"jv")!=0)
{
	ewe(sim,"%s %s %s\n",_("no jv config file found"),get_input_path(sim),in->simmode);
}

printf_log(sim,"%s\n",config_file_name);

jv_load_config(sim,&config,in,config_file_name);

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

struct math_xy vjexternal;
inter_init(sim,&vjexternal);

struct math_xy power_den;
inter_init(sim,&power_den);

struct math_xy charge;
inter_init(sim,&charge);

struct math_xy charge_tot;
inter_init(sim,&charge_tot);

struct math_xy klist;
inter_init(sim,&klist);

struct math_xy lv;
inter_init(sim,&lv);

struct math_xy lj;
inter_init(sim,&lj);

//contact_set_active_contact_voltage(sim,in,Vapplied);

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

long double sun_orig=light_get_sun(&(in->mylight));
light_set_sun(&(in->mylight),sun_orig*config.jv_light_efficiency);
light_solve_and_update(sim,in,&(in->mylight),0.0);

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
long double n_voc=0.0;
long double r_voc=0.0;
long double nsc=0.0;
long double n_trap_voc=0.0;
long double p_trap_voc=0.0;
long double n_free_voc=0.0;
long double p_free_voc=0.0;
long double np_voc_tot=0.0;
long double r_pmax=0.0;
long double n_pmax=0.0;
long double mue_pmax=0.0;
long double muh_pmax=0.0;

//Device characterisation
long double Voc=0.0;
long double Jsc=0.0;
long double FF=0.0;
long double Pmax=0.0;
long double V_pmax=0.0;
long double J_pmax=0.0;


long double cal_step=0;

long double n_steps=0.0;
char send_data[200];
long double V_simple_last=-1.0;
int power_min_pos=0;
int dump_step=0;
long double optical_power_m2=0.0;

struct newton_state *ns=&(in->ns);

n_steps=get_step_n(config.Vstep,config.jv_step_mul,config.Vstart);
n_steps+=get_step_n(config.Vstep,config.jv_step_mul,config.Vstop);

in->stop=FALSE;

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

		}
		ittr++;
		inter_append(&charge,Vexternal,get_extracted_np(in));
		inter_append(&charge_tot,Vexternal,get_np_tot(in));

		Pden=gfabs(J*Vexternal);

		plot_now(sim,in,"jv.plot");
		stop_start(sim,in);
		dump_dynamic_add_data(sim,&store,in,Vexternal);


		dump_contacts_add_data(sim,in,&contact_store);

		V_simple_last=V;
		if (get_dump_status(sim,dump_print_converge)==TRUE)
		{
			char mytemp[200];
			contacts_state_to_string(sim,mytemp, in);
			printf_log(sim," %s f()=%Le %lf ms\n",mytemp,ns->last_error, ns->last_time);
		}

		if (first==FALSE)
		{

			//check if we have crossed 0V
			if ((Vlast<=0)&&(Vexternal>=0.0))
			{
				nsc=get_extracted_np(in);
				printf_log(sim,"nsc=%Le\n",nsc);
			}

			if ((Jlast<=0)&&(J>=0.0))
			{
				r_voc=get_avg_recom(in);
				n_voc=get_extracted_np(in);
				np_voc_tot=get_total_np(in);
				n_trap_voc=get_n_trapped_charge(in);
				n_free_voc=get_free_n_charge(in);
				p_trap_voc=get_p_trapped_charge(in);
				p_free_voc=get_free_p_charge(in);
			}

			if ((Pden>Pdenlast)&&(Vexternal>0.0)&&(J<0.0))
			{
				r_pmax=get_avg_recom(in);
				n_pmax=get_extracted_np(in);
				mue_pmax=get_avg_mue(in);
				muh_pmax=get_avg_muh(in);
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
		Pdenlast=Pden;
		first=FALSE;

		if (config.dump_verbocity!=0)
		{
			if ((dump_step>=config.dump_verbocity)||(config.dump_verbocity==1))
			{
				dump_write_to_disk(sim,in);
				dump_step=0;
			}
			dump_step++;
		}

		#ifdef libemission_enabled
			optical_power_m2=calculate_photon_power_m2(sim,in);
		#endif

		inter_append(&lv,Vexternal,optical_power_m2);

		inter_append(&lj,J,optical_power_m2);
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

if (power_den.len>0)
{
	Jsc=inter_get(&jvexternal,0.0);
	Voc=inter_get(&vjexternal,0.0);
	power_min_pos=inter_get_min_pos(&power_den);
	Pmax=-1.0*power_den.data[power_min_pos];
	V_pmax=power_den.x[power_min_pos];
	J_pmax=jvexternal.data[power_min_pos];
	FF=gfabs(Pmax/(Jsc*Voc));
}

if (get_dump_status(sim,dump_print_text)==TRUE)
{
	printf_log(sim,"Max possible Jsc = %Lf\n",get_max_Jsc(in));
	printf_log(sim,"Voc= %Lf (V)\n",Voc);
	printf_log(sim,"Jsc= %Lf (A/m^2)\n",Jsc);
	printf_log(sim,"Pmax= %Lf (W/m^2)\n",Pmax);
	printf_log(sim,"Pmax %s= %Lf (V)\n",_("Voltage"),V_pmax);
	printf_log(sim,"FF= %Lf\n",FF*100.0);
	printf_log(sim,"%s= %Lf percent\n",_("Efficiency"),gfabs(Pmax/light_get_sun(&(in->mylight))/1000)*100.0);
}

long double added=0.0;
added=get_tot_photons_abs(in);
printf("photon density= %Le\n", added);
printf("%Le %Le",Pmax,light_get_sun(&(in->mylight)));
//getchar();

if (dumpfiles_should_dump(sim,"sim_info.dat")==0)
{
	FILE *out;
	out=fopena(get_output_path(sim),"sim_info.dat","w");
	fprintf(out,"#ff\n%Lf\n",FF);
	fprintf(out,"#pce\n%Lf\n",gfabs(100.0*Pmax/(1000.0*light_get_sun(&(in->mylight)))));
	fprintf(out,"#Pmax\n%Lf\n",Pmax);
	fprintf(out,"#voc\n%Lf\n",Voc);
	fprintf(out,"#voc_tau\n%Le\n",n_voc/r_voc);
	fprintf(out,"#voc_R\n%Le\n",r_voc);
	fprintf(out,"#jv_voc_k\n%Le\n",r_voc/n_voc);
	fprintf(out,"#jv_pmax_n\n%Le\n",n_pmax);
	fprintf(out,"#jv_pmax_tau\n%Le\n",n_pmax/r_pmax);
	fprintf(out,"#jv_pmax_mue\n%Le\n",mue_pmax);
	fprintf(out,"#jv_pmax_muh\n%Le\n",muh_pmax);
	fprintf(out,"#voc_nt\n%Le\n",n_trap_voc);
	fprintf(out,"#voc_pt\n%Le\n",p_trap_voc);
	fprintf(out,"#voc_nf\n%Le\n",n_free_voc);
	fprintf(out,"#voc_pf\n%Le\n",p_free_voc);
	fprintf(out,"#jsc\n%Le\n",Jsc);
	fprintf(out,"#jv_jsc_n\n%Le\n",nsc);
	fprintf(out,"#jv_vbi\n%Le\n",in->vbi);
	fprintf(out,"#jv_gen\n%Le\n",get_avg_gen(in));
	fprintf(out,"#voc_np_tot\n%Le\n",np_voc_tot);
	fprintf(out,"#j_pmax\n%Le\n",J_pmax);
	fprintf(out,"#v_pmax\n%Le\n",V_pmax);
	fprintf(out,"#end");
	fclose(out);
}

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
buf.x=1;
buf.y=charge.len;
buf.z=1;
buffer_add_info(sim,&buf);
buffer_add_xy_data(sim,&buf,klist.x, klist.data, klist.len);
buffer_dump_path(sim,get_output_path(sim),"k.dat",&buf);
buffer_free(&buf);

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
buf.x=1;
buf.y=charge.len;
buf.z=1;
buffer_add_info(sim,&buf);
buffer_add_xy_data(sim,&buf,charge.x, charge.data, charge.len);
buffer_dump_path(sim,get_output_path(sim),"charge.dat",&buf);
buffer_free(&buf);

buffer_malloc(&buf);
buf.y_mul=1.0;
buf.data_mul=1.0;
sprintf(buf.title,"%s - %s",_("Total charge density"),_("App.ed voltage"));
strcpy(buf.type,"xy");
strcpy(buf.y_label,_("Applied Voltage"));
strcpy(buf.data_label,_("Total charge density"));
strcpy(buf.y_units,"Volts");
strcpy(buf.data_units,"m^{-3}");
buf.logscale_x=0;
buf.logscale_y=0;
buf.logscale_z=0;
buf.x=1;
buf.y=charge.len;
buf.z=1;
buffer_add_info(sim,&buf);
buffer_add_xy_data(sim,&buf,charge_tot.x, charge_tot.data, charge.len);
buffer_dump_path(sim,get_output_path(sim),"charge_tot.dat",&buf);
buffer_free(&buf);

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
buf.x=1;
buf.y=jvexternal.len;
buf.z=1;
buffer_add_info(sim,&buf);
buffer_add_xy_data(sim,&buf,jvexternal.x, jvexternal.data, jvexternal.len);
buffer_dump_path(sim,get_output_path(sim),"jv.dat",&buf);
buffer_free(&buf);


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
buf.x=1;
buf.y=jvexternal.len;
buf.z=1;
buffer_add_info(sim,&buf);
buffer_add_xy_data(sim,&buf,jvexternal.x, jvexternal.data, jvexternal.len);
buffer_dump_path(sim,get_output_path(sim),"iv.dat",&buf);
buffer_free(&buf);

if (in->emission_enabled==TRUE)
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
	buf.x=1;
	buf.y=lv.len;
	buf.z=1;
	buffer_add_info(sim,&buf);
	buffer_add_xy_data(sim,&buf,lv.x, lv.data, lv.len);
	buffer_dump_path(sim,get_output_path(sim),"lv.dat",&buf);
	buffer_free(&buf);



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
	buf.x=1;
	buf.y=lj.len;
	buf.z=1;
	buffer_add_info(sim,&buf);
	buffer_add_xy_data(sim,&buf,lj.x, lj.data, lj.len);
	buffer_dump_path(sim,get_output_path(sim),"lj.dat",&buf);
	buffer_free(&buf);
}

inter_free(&jvexternal);
inter_free(&vjexternal);
inter_free(&power_den);
inter_free(&charge);
inter_free(&ivexternal);
inter_free(&lv);
inter_free(&lj);
inter_free(&klist);
inter_free(&charge_tot);

dump_dynamic_save(sim,in,get_output_path(sim),&store);
dump_dynamic_free(sim,in,&store);


dump_contacts_save(sim,in,&contact_store);
dump_contacts_free(sim,in,&contact_store);

light_set_sun(&(in->mylight),sun_orig);
}




void jv_load_config(struct simulation *sim,struct jv* in,struct device *dev, char* config_file_name)
{
	struct inp_file inp;
	inp_init(sim,&inp);
	inp_load_from_path(sim,&inp,get_input_path(sim),config_file_name);
	inp_check(sim,&inp,1.22);
	inp_search_gdouble(sim,&inp,&(in->Vstart),"#Vstart");
	inp_search_gdouble(sim,&inp,&(in->Vstop),"#Vstop");
	inp_search_gdouble(sim,&inp,&(in->Vstep),"#Vstep");
	in->Vstep=fabs(in->Vstep);

	if (in->Vstop<in->Vstart)
	{
		in->Vstep*=-1.0;
	}

	inp_search_gdouble(sim,&inp,&(in->jv_step_mul),"#jv_step_mul");
	inp_search_gdouble(sim,&inp,&(in->jv_light_efficiency),"#jv_light_efficiency");
	inp_search_gdouble(sim,&inp,&(in->jv_max_j),"#jv_max_j");


	inp_search_gdouble(sim,&inp,&(in->jv_Rshunt),"#jv_Rshunt");
	inp_search_gdouble(sim,&inp,&(in->jv_Rcontact),"#jv_Rcontact");
	inp_search_gdouble(sim,&inp,&(in->jv_Rcontact),"#jv_Rcontact");
	in->jv_single_point=inp_search_english(sim,&inp,"#jv_single_point");
	in->jv_light_efficiency=gfabs(in->jv_light_efficiency);
	inp_search_int(sim,&inp,&(in->dump_verbocity),"#dump_verbosity");
	in->jv_use_external_voltage_as_stop=inp_search_english(sim,&inp,"#jv_use_external_voltage_as_stop");

	inp_free(sim,&inp);

}
