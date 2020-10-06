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

/** @file pulse.c
	@brief Time domain solver.
*/

#include <sim.h>
#include <dump.h>
#include <dynamic_store.h>
#include <i.h>
#include <exp.h>
#include "pulse.h"
#include <inp.h>
#include <dat_file.h>
#include <gui_hooks.h>
#include <lang.h>
#include <log.h>
#include <plot.h>
#include <cal_path.h>
#include <probe.h>
#include <newton_voc.h>
#include <newton_tricks.h>
#include <contacts.h>
#include <perovskite.h>
#include <contacts_vti_store.h>
#include <light_fun.h>

static int unused __attribute__((unused));

struct pulse pulse_config;

void sim_pulse(struct simulation *sim,struct device *in)
{
light_solve_and_update(sim,in,&(in->mylight),0.0);

char tunits[100];
int i=0;
char temp[200];

struct dat_file buf;
buffer_init(&buf);

struct dynamic_store store;
dump_dynamic_init(sim,&store,in);

struct math_xy out_j_int;
inter_init(sim,&out_j_int);

struct math_xy out_i;
inter_init(sim,&out_i);

struct math_xy out_v;
inter_init(sim,&out_v);

struct math_xy out_G;
inter_init(sim,&out_G);

struct math_xy out_jv;
inter_init(sim,&out_jv);

struct math_xy out_flip;
inter_init(sim,&out_flip);

struct math_xy out_j;
inter_init(sim,&out_j);

struct contacts_vti_store contact_store;
dump_contacts_init(sim,in,&contact_store);

struct newton_state *ns=&(in->ns);

char config_file_name[200];
struct dimensions *dim=&in->ns.dim;

if (find_config_file(sim,config_file_name,get_input_path(sim),in->simmode,"pulse")!=0)
{
	ewe(sim,"%s %s %s\n",_("no pulse config file found"),get_input_path(sim),in->simmode);
}

printf_log(sim,"%s\n",config_file_name);

pulse_load_config(sim,&pulse_config,in,config_file_name);
int number=strextract_int(config_file_name);

in->go_time=FALSE;

time_init(sim,in);
//time_load_mesh(in,number);

time_load_mesh(sim,in,number);
//time_mesh_save();
//getchar();
//struct math_xy pulseout;
//inter_init(&pulseout);

int ittr=0;

int step=0;
int run_perovskite=FALSE;

if (run_perovskite==FALSE)
{
	light_set_sun(&(in->mylight),time_get_sun(in)*pulse_config.pulse_light_efficiency);
	light_solve_and_update(sim,in,&(in->mylight), time_get_laser(in)*pulse_config.pulse_light_efficiency);
}else
{
	in->mylight.print_wavlengths=FALSE;
	in->mylight.dump_level=0;
	long double S=0;
	long double sun=time_get_sun(in)*pulse_config.pulse_light_efficiency;
	while(S<1.0)
	{
		printf("Solve light %Lf\n",S);
		light_set_sun(&(in->mylight),sun*S);
		light_solve_and_update(sim,in,&(in->mylight), time_get_laser(in)*pulse_config.pulse_light_efficiency);
		newton_externv_perovskite(sim,in,0.0,FALSE);
		S=S+0.1;
	}
}

gdouble V=0.0;
if (get_dump_status(sim,dump_optical_probe)==TRUE)
{
	probe_init(sim,in);
}

if (pulse_config.pulse_sim_mode==LOAD)
{

	//printf("%Le\n",time_get_voltage(in)+pulse_config.pulse_bias);
	//getchar();
	ramp_externalv(sim,in,V,time_get_voltage(in)+pulse_config.pulse_bias);
	sim_externalv(sim,in,time_get_voltage(in)+pulse_config.pulse_bias);
	//newton_externv(sim,in,time_get_voltage(in)+pulse_config.pulse_bias);



	if (run_perovskite==FALSE)
	{
		newton_externv(sim,in,time_get_voltage(in)+pulse_config.pulse_bias);
	}else
	{
		newton_externv_perovskite(sim,in,time_get_voltage(in)+pulse_config.pulse_bias,FALSE);
		//printf("%d\n",in->go_time);
		//getchar();
	}

	//sim_externalv(sim,in,time_get_voltage(in)+pulse_config.pulse_bias);

}else
if (pulse_config.pulse_sim_mode==IDEAL_DIODE_IDEAL_LOAD)
{
	//getchar();
	if ((dim->zlen>1) || (dim->xlen>1))
	{
		contact_set_wanted_active_contact_voltage(sim,in,time_get_voltage(in)+pulse_config.pulse_bias);
		//contact_set_active_contact_voltage(sim,in,config.Vstart);
		ntricks_auto_ramp_contacts(sim,in);
	}else
	{
		//if (gfabs(config.Vstart-Vapplied)>0.2)
		//{
			//ramp_externalv(sim,in,Vapplied,config.Vstart);
		ramp_externalv(sim,in,V,time_get_voltage(in)+pulse_config.pulse_bias);
		//}
	}

	newton_externalv_simple(sim,in,time_get_voltage(in)+pulse_config.pulse_bias);
}
else
if (pulse_config.pulse_sim_mode==OPEN_CIRCUIT)
{
	contact_set_active_contact_voltage(sim,in,in->Vbi);
	in->Rload=1e6;
	newton_sim_voc(sim,in);
	newton_sim_voc_fast(sim,in,FALSE);

}else
{
	ewe(sim,_("pulse mode not known\n"));
}

//device_timestep(sim,in);
time_store(sim,in);
in->go_time=TRUE;

//gdouble i0=0;
carrier_count_reset(in);
reset_np_save(in);
gdouble Vapplied=0.0;
Vapplied=contact_get_active_contact_voltage(sim,in);//contact_get_voltage(sim,in,0);
printf_log(sim,"Vapplied=%Le\n",Vapplied);
do
{

	if (get_dump_status(sim,dump_optical_probe)==TRUE)
	{
	probe_record_step(sim,in);
	}

	light_set_sun(&(in->mylight),time_get_sun(in)*pulse_config.pulse_light_efficiency);
	light_solve_and_update(sim,in,&(in->mylight), (time_get_laser(in)+time_get_fs_laser(in))*pulse_config.pulse_light_efficiency);
	//printf("%Le %Le %Le %Le\n",in->Gn[0][0][0],time_get_laser(in),time_get_fs_laser(in),pulse_config.pulse_light_efficiency);
	//int i;
	//FILE *t=fopen("t.dat","w");
	//for (i=0;i<in->ylen;i++)
	//{
	//	fprintf(t,"%Le %Le\n",in->ymesh[i],in->Gn[i]);
	//	printf_log(sim,"%Le %Le\n",in->ymesh[i],in->Gn[i]);
	//}
	//fclose(t);
	dump_dynamic_add_data(sim,&store,in,in->time);
	dump_contacts_add_data(sim,in,&contact_store);
	dump_write_to_disk(sim,in);

	if (pulse_config.pulse_sim_mode==LOAD)
	{
		V=time_get_voltage(in)+pulse_config.pulse_bias;
		if (run_perovskite==FALSE)
		{
			newton_externv(sim,in,V);
		}else
		{
			newton_externv_perovskite(sim,in,V,TRUE);
		}

	}else
	if (pulse_config.pulse_sim_mode==IDEAL_DIODE_IDEAL_LOAD)
	{
		V=time_get_voltage(in)+pulse_config.pulse_bias;
		newton_externalv_simple(sim,in,V);
	}else
	if (pulse_config.pulse_sim_mode==OPEN_CIRCUIT)
	{
		V=contact_get_active_contact_voltage(sim,in);
		newton_sim_voc_fast(sim,in,TRUE);
	}else
	{
		ewe(sim,_("pulse mode not known\n"));
	}

	contacts_cal_J_and_i(sim,in);

	if (get_dump_status(sim,dump_print_text)==TRUE)
	{
		time_with_units(tunits,in->time);
		printf_log(sim,"%s=%Le (%s) %s=%d %.1Le ",_("pulse time"),in->time,tunits,_("step"),step,ns->last_error);
		printf_log(sim,"Vtot=%Lf %s = %Le mA (%Le A/m^2)\n",V,_("current"),get_I(in)/1e-3,get_J(in));
	}

	ittr++;
	gui_send_data(sim,gui_sub,"pulse");

	plot_now(sim,in,"pulse.plot");
	inter_append(&out_i,in->time,get_I(in));
	inter_append(&out_j_int,in->time,get_avg_J(in));
	inter_append(&out_v,in->time,V);
	inter_append(&out_G,in->time,in->Gn[0][0][0]);
	inter_append(&out_jv,V,get_avg_J(in));	//get_J(in)

	//printf_log(sim,"%Le %d %Le\n",in->time,time_test_last_point(in),in->dt);
	if (time_test_last_point(in)==TRUE) break;
	device_timestep(sim,in);
	step++;
	//getchar();

	poll_gui(sim);

}while(1);


dump_dynamic_save(sim,in,get_output_path(sim),&store);
dump_dynamic_free(sim,in,&store);

if (pulse_config.pulse_subtract_dc==TRUE)
{
	inter_sub_long_double(&out_i,out_i.data[0]);
	inter_sub_long_double(&out_j_int,out_j_int.data[0]);
}

buffer_malloc(&buf);
buf.y_mul=1e3;
buf.x_mul=1e6;
sprintf(buf.title,"%s - %s",_("Time"),_("Current"));
strcpy(buf.type,"xy");
strcpy(buf.y_label,_("Time"));
strcpy(buf.y_units,"\\ms");

strcpy(buf.data_label,_("Current"));
strcpy(buf.data_units,"Am^{-2}");

buf.logscale_x=0;
buf.logscale_y=0;
buf.x=1;
buf.y=out_j_int.len;
buf.z=1;
buffer_add_info(sim,&buf);
buffer_add_xy_data(sim,&buf,out_j_int.x, out_j_int.data, out_j_int.len);
buffer_dump_path(sim,get_output_path(sim),"time_j_int.dat",&buf);
buffer_free(&buf);

buffer_malloc(&buf);
buf.y_mul=1e3;
buf.x_mul=1e6;
sprintf(buf.title,"%s - %s",_("Time"),_("Current"));
strcpy(buf.type,"xy");
strcpy(buf.y_label,_("Time"));
strcpy(buf.y_units,"\\ms");

strcpy(buf.data_label,_("Current"));
strcpy(buf.data_units,"A");
buf.logscale_x=0;
buf.logscale_y=0;
buf.x=1;
buf.y=out_i.len;
buf.z=1;
buffer_add_info(sim,&buf);
buffer_add_xy_data(sim,&buf,out_i.x, out_i.data, out_i.len);
buffer_dump_path(sim,get_output_path(sim),"time_i.dat",&buf);
buffer_free(&buf);

inter_copy(&out_j,&out_i,TRUE);
math_xy_mul_long_double(&out_j,1.0/(in->xlen*in->zlen));

buffer_malloc(&buf);
buf.y_mul=1e3;
buf.x_mul=1e6;
sprintf(buf.title,"%s - %s",_("Time"),_("current density"));
strcpy(buf.type,"xy");
strcpy(buf.y_label,_("Time"));
strcpy(buf.y_units,"\\ms");

strcpy(buf.data_label,_("Current density"));
strcpy(buf.data_units,"Am^{-2}");
buf.logscale_x=0;
buf.logscale_y=0;
buf.x=1;
buf.y=out_j.len;
buf.z=1;
buffer_add_info(sim,&buf);
buffer_add_xy_data(sim,&buf,out_j.x, out_j.data, out_j.len);
buffer_dump_path(sim,get_output_path(sim),"time_j.dat",&buf);
buffer_free(&buf);


inter_copy(&out_flip,&out_j,TRUE);
math_xy_mul_long_double(&out_flip,-1.0);

buffer_malloc(&buf);
buf.y_mul=1e3;
buf.x_mul=1e6;
sprintf(buf.title,"%s - %s",_("Time"),_("Current density"));
strcpy(buf.type,"xy");
strcpy(buf.y_label,_("Time"));
strcpy(buf.data_label,_("Current density"));
strcpy(buf.y_units,"\\ms");
strcpy(buf.data_units,"Am^{-2}");
buf.logscale_x=0;
buf.logscale_y=0;
buf.x=1;
buf.y=out_flip.len;
buf.z=1;
buffer_add_info(sim,&buf);
buffer_add_xy_data(sim,&buf,out_flip.x, out_flip.data, out_flip.len);
buffer_dump_path(sim,get_output_path(sim),"time_j_abs.dat",&buf);
buffer_free(&buf);

inter_free(&out_j);
inter_free(&out_flip);

inter_copy(&out_flip,&out_i,TRUE);
math_xy_mul_long_double(&out_flip,-1.0);

buffer_malloc(&buf);
buf.y_mul=1e3;
buf.x_mul=1e6;
sprintf(buf.title,"%s - %s",_("Time"),_("current"));
strcpy(buf.type,"xy");
strcpy(buf.y_label,_("Time"));
strcpy(buf.data_label,_("Current"));
strcpy(buf.y_units,"\\ms");
strcpy(buf.data_units,"A");
buf.logscale_x=0;
buf.logscale_y=0;
buf.x=1;
buf.y=out_flip.len;
buf.z=1;
buffer_add_info(sim,&buf);
buffer_add_xy_data(sim,&buf,out_flip.x, out_flip.data, out_flip.len);
buffer_dump_path(sim,get_output_path(sim),"time_i_abs.dat",&buf);
buffer_free(&buf);

inter_free(&out_flip);



buffer_malloc(&buf);
buf.y_mul=1.0;
buf.x_mul=1e6;
sprintf(buf.title,"%s - %s",_("Time"),_("Voltage"));
strcpy(buf.type,"xy");
strcpy(buf.y_label,_("Time"));
strcpy(buf.data_label,_("Volts"));
strcpy(buf.y_units,"\\ms");
strcpy(buf.data_units,_("Voltage"));
buf.logscale_x=0;
buf.logscale_y=0;
buf.x=1;
buf.y=out_v.len;
buf.z=1;
buffer_add_info(sim,&buf);
buffer_add_xy_data(sim,&buf,out_v.x, out_v.data, out_v.len);
buffer_dump_path(sim,get_output_path(sim),"time_v.dat",&buf);
buffer_free(&buf);


buffer_malloc(&buf);
buf.y_mul=1.0;
buf.x_mul=1e6;
sprintf(buf.title,"%s - %s",_("Time"),_("Photogeneration rate"));
strcpy(buf.type,"xy");
strcpy(buf.y_label,_("Time"));
strcpy(buf.data_label,_("Generation rate"));
strcpy(buf.y_units,"s");
strcpy(buf.data_units,"m^{-3} s^{-1}");
buf.logscale_x=0;
buf.logscale_y=0;
buf.x=1;
buf.y=out_G.len;
buf.z=1;
buffer_add_info(sim,&buf);
buffer_add_xy_data(sim,&buf,out_G.x, out_G.data, out_G.len);
buffer_dump_path(sim,get_output_path(sim),"time_G.dat",&buf);
buffer_free(&buf);

buffer_malloc(&buf);
buf.y_mul=1.0;
buf.x_mul=1e6;
sprintf(buf.title,"%s - %s",_("Voltage"),_("current"));
strcpy(buf.type,"xy");
strcpy(buf.y_label,_("Voltage"));
strcpy(buf.data_label,_("current"));
strcpy(buf.y_units,"s");
strcpy(buf.data_units,"Am^{-2}");
buf.logscale_x=0;
buf.logscale_y=0;
buf.x=1;
buf.y=out_jv.len;
buf.z=1;
buffer_add_info(sim,&buf);

for (i=0;i<out_jv.len;i++)
{
	time_with_units(tunits,out_G.x[i]);
	sprintf(temp,"%Le %Le #%s\n",out_jv.x[i],out_jv.data[i],tunits);
	buffer_add_string(&buf,temp);
}
//buffer_add_xy_data_z_label(&buf,out_jv.x, out_jv.data, out_G.x, out_jv.len);

buffer_dump_path(sim,get_output_path(sim),"jv.dat",&buf);
buffer_free(&buf);

if (get_dump_status(sim,dump_optical_probe)==TRUE)
{
	probe_dump(sim,in);
}

in->go_time=FALSE;

if (get_dump_status(sim,dump_optical_probe)==TRUE)
{
	probe_free(sim,in);
}

dump_contacts_save(sim,in,&contact_store);
dump_contacts_free(sim,in,&contact_store);

inter_free(&out_G);
inter_free(&out_i);
inter_free(&out_j_int);
inter_free(&out_v);
inter_free(&out_jv);


time_memory_free(&(in->tm));



}

void pulse_load_config(struct simulation *sim,struct pulse *in,struct device *dev,char *config_file_name)
{

char name[200];
char laser_name[200];
struct inp_file inp;
inp_init(sim,&inp);
inp_load_from_path(sim,&inp,get_input_path(sim),config_file_name);
inp_check(sim,&inp,1.30);

inp_search_gdouble(sim,&inp,&(in->pulse_shift),"#pulse_shift");
inp_search_string(sim,&inp,name,"#pulse_sim_mode");
inp_search_gdouble(sim,&inp,&(dev->L),"#pulse_L");
inp_search_gdouble(sim,&inp,&(dev->Rload),"#Rload");
inp_search_gdouble(sim,&inp,&(in->pulse_bias),"#pulse_bias");
inp_search_string(sim,&inp,laser_name,"#pump_laser");
in->pulse_subtract_dc=inp_search_english(sim,&inp,"#pulse_subtract_dc");

inp_search_gdouble(sim,&inp,&(in->pulse_light_efficiency),"#pulse_light_efficiency");
in->pulse_light_efficiency=gfabs(in->pulse_light_efficiency);

light_load_laser(sim,(&dev->mylight),laser_name);
in->pulse_sim_mode=english_to_bin(sim,name);

inp_free(sim,&inp);

}
