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

/** @file pulse.c
	@brief Time domain solver.
*/

#include <sim.h>
#include <dump.h>
#include <dynamic_store.h>
#include <i.h>
#include <exp.h>
#include "pulse.h"
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
#include <contacts_vti_store.h>
#include <light_fun.h>
#include <contacts.h>
#include <contacts_vti_store.h>
#include <pl.h>

static int unused __attribute__((unused));

struct pulse config;

void sim_pulse(struct simulation *sim,struct device *in)
{
find_n0(sim,in);
light_solve_and_update(sim,in,&(in->mylight),0.0);

char tunits[100];
int i=0;
char temp[200];
long double J_for_jv;

struct json_obj *json_pulse;
struct dat_file buf;
dat_file_init(&buf);

struct dynamic_store store;
dump_dynamic_init(sim,&store,in);

struct contacts_vti_store contact_store;
dump_contacts_init(sim,in,&contact_store);

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

struct math_xy time_emission;
inter_init(sim,&time_emission);

struct newton_state *ns=&(in->ns);

struct dimensions *dim=&in->ns.dim;


json_pulse=json_find_sim_struct(sim, &(in->config),in->simmode);
pulse_load_config(sim,&config,in,json_pulse);

in->go_time=FALSE;

time_init(sim,in);

time_load_mesh(sim,in,json_pulse);
//time_mesh_save();
//getchar();
//struct math_xy pulseout;
//inter_init(&pulseout);

int ittr=0;
int dump_step=0;
int step=0;

light_set_sun(&(in->mylight),time_get_sun(in)*config.pulse_light_efficiency);
light_solve_and_update(sim,in,&(in->mylight), time_get_laser(in)*config.pulse_light_efficiency);

gdouble V=0.0;
if (get_dump_status(sim,dump_optical_probe)==TRUE)
{
	probe_init(sim,in);
}

if (config.pulse_sim_mode==LOAD)
{

	//printf("%Le\n",time_get_voltage(in)+config.pulse_bias);
	//getchar();
	ramp_externalv(sim,in,V,time_get_voltage(in)+config.pulse_bias);
	sim_externalv(sim,in,time_get_voltage(in)+config.pulse_bias);
	//newton_externv(sim,in,time_get_voltage(in)+config.pulse_bias);


		newton_externv(sim,in,time_get_voltage(in)+config.pulse_bias);
	//sim_externalv(sim,in,time_get_voltage(in)+config.pulse_bias);

}else
if (config.pulse_sim_mode==IDEAL_DIODE_IDEAL_LOAD)
{
	//getchar();
	if ((dim->zlen>1) || (dim->xlen>1))
	{
		contact_set_wanted_active_contact_voltage(sim,in,time_get_voltage(in)+config.pulse_bias);
		//contact_set_active_contact_voltage(sim,in,config.Vstart);
		ntricks_auto_ramp_contacts(sim,in);
	}else
	{
		//if (gfabs(config.Vstart-Vapplied)>0.2)
		//{
			//ramp_externalv(sim,in,Vapplied,config.Vstart);
		//double s=1.0;
		//double ds=1.0;
		ramp_externalv(sim,in,V,time_get_voltage(in)+config.pulse_bias);
		newton_externalv_simple(sim,in,time_get_voltage(in)+config.pulse_bias);
		/*while(s<time_get_sun(in))
		{
			light_set_sun(&(in->mylight),s*config.pulse_light_efficiency);
			light_solve_and_update(sim,in,&(in->mylight), time_get_laser(in)*config.pulse_light_efficiency);


			if (ns->last_ittr<16)
			{
				ds=ds*(1.0+0.005);
			}

			if (ns->last_ittr>18)
			{
				ds*=(1.0-0.001);
			}

			if (ds<0.1)
			{
				ds=0.1;
			}

			printf("%le %d \n",s,ns->last_ittr);
			//getchar();
			//ds=ds*1.01;
			//if (ds>10.0)
			//{
			//	ds=3.0;
			//}
			s+=ds;
		}*/

		//}
	}

}
else
if (config.pulse_sim_mode==OPEN_CIRCUIT)
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
reset_np_save(in);
gdouble Vapplied=0.0;
Vapplied=contact_get_active_contact_voltage(sim,in);//contact_get_voltage(sim,in,0);
printf_log(sim,"Vapplied=%Le\n",Vapplied);
//FILE *out;
//out=fopen("r_phi.dat","w");
long double n_tot=0.0;
long double J_tot=0.0;
long double n_count_last=0.0;
long double solver_i=0.0;
long double optical_power_m2=0.0;

//newton_externalv_simple(sim,in,time_get_voltage(in)+config.pulse_bias);
//int ii;
//struct time_mesh *tm=&(in->tm);
/*for (ii=0;ii<tm->tm_mesh_len-1;ii++)
{
	//printf("%Le %Le %Le\n",tm->tm_time_mesh[ii],in->time,tm->tm_fs_laser[ii]);
	printf("%Le %Le \n",tm->tm_time_mesh[ii+1]-tm->tm_time_mesh[ii],tm->tm_dt[ii]);

	device_timestep(sim,in);
}

getchar();*/


do
{

	if (get_dump_status(sim,dump_optical_probe)==TRUE)
	{
	probe_record_step(sim,in);
	}

	light_set_sun(&(in->mylight),time_get_sun(in)*config.pulse_light_efficiency);
	light_solve_and_update(sim,in,&(in->mylight), (time_get_laser(in)+time_get_fs_laser(in))*config.pulse_light_efficiency);

	/*printf("%Le %Le\n",time_get_fs_laser(in),in->time);
	int r;
	if (time_get_fs_laser(in)>0.0)
	{
		for (r=0;r<dim->ylen;r++)
		{
			printf("%Le\n", in->Gn[0][0][0]);
		}
	}*/
	//getchar();

	//printf("%Le %Le %Le %Le\n",in->Gn[0][0][0],time_get_laser(in),time_get_fs_laser(in),config.pulse_light_efficiency);
	//int i;
	//FILE *t=fopen("t.dat","w");
	//for (i=0;i<in->ylen;i++)
	//{
	//	fprintf(t,"%Le %Le\n",in->y[i],in->Gn[i]);
	//	printf_log(sim,"%Le %Le\n",in->y[i],in->Gn[i]);
	//}
	//fclose(t);
	dump_dynamic_add_data(sim,&store,in,in->time);
	//fprintf(out,"%Le %Le\n",in->time,ns->phi[0][7][1]);
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

	int y;
	long double sum=0.0;
	long double n_count=0.0;



	for (y = 0; y < dim->ylen; y++)
	{
		//sum+=(in->Rn[0][0][y]+in->Rp[0][0][y])/2.0;
		sum+=in->Rn[0][0][y];
		n_count+=in->n[0][0][y];
	}

	sum/=dim->ylen;
	n_count/=dim->ylen;

	if (ittr==0)
	{
		n_count_last=n_count;
	}
	//*dim->dy[y]

	//long double dJn=(get_J_right(in)+get_J_left(in))/dim->y[dim->ylen-1]/Qe/2.0;
	long double dJn=(in->Jn_y1[0][0]-in->Jn_y0[0][0])/dim->y[dim->ylen-1]/Qe;
	long double error=(n_count-n_count_last)/in->dt-dJn+sum;
	n_tot+=error*in->dt;
	J_tot+=dJn*in->dt;

	n_count_last=n_count;

	if (config.pulse_sim_mode==LOAD)
	{
		V=time_get_voltage(in)+config.pulse_bias;
		solver_i=newton_externv(sim,in,V);
		J_for_jv=solver_i/in->area;
	}else
	if (config.pulse_sim_mode==IDEAL_DIODE_IDEAL_LOAD)
	{
		V=time_get_voltage(in)+config.pulse_bias;
		newton_externalv_simple(sim,in,V);
		solver_i=get_I(in);
		J_for_jv=get_avg_J(in);

	}else
	if (config.pulse_sim_mode==OPEN_CIRCUIT)
	{
		V=contact_get_active_contact_voltage(sim,in);
		newton_sim_voc_fast(sim,in,TRUE);
		solver_i=get_I(in);
		J_for_jv=get_avg_J(in);
	}else
	{
		ewe(sim,_("pulse mode not known\n"));
	}

	contacts_cal_J_and_i(sim,in);

	dump_contacts_add_data(sim,in,&contact_store);

	if (in->emission_enabled==TRUE)
	{
		#ifdef libemission_enabled
			optical_power_m2=calculate_photon_power_m2(sim,in);
		#endif

		inter_append(&time_emission,in->time,optical_power_m2);

	}

	if (in->solver_verbosity>=SOLVER_VERBOSITY_AT_END)
	{
		char mytemp[200];
		contacts_state_to_string(sim,mytemp, in);
		time_with_units(tunits,in->time);
		printf_log(sim,"%s=%Le (%s) %s=%d %.1Le ",_("pulse time"),in->time,tunits,_("step"),step,ns->last_error);
		printf_log(sim," %s\n",mytemp);
	}

	ittr++;
	gui_send_data(sim,gui_sub,"pulse");

	plot_now(sim,in,"pulse.plot");
	inter_append(&out_i,in->time,solver_i);
	inter_append(&out_j_int,in->time,get_avg_J(in));
	inter_append(&out_v,in->time,V);
	inter_append(&out_G,in->time,in->Gn[0][0][0]);
	inter_append(&out_jv,V,J_for_jv);	//get_J(in)

	//printf_log(sim,"%Le %d %Le\n",in->time,time_test_last_point(in),in->dt);
	if (time_test_last_point(in)==TRUE) break;
	device_timestep(sim,in);
	step++;
	//getchar();

	poll_gui(sim);

}while(1);

dump_dynamic_save(sim,in,in->output_path,&store);
dump_dynamic_free(sim,in,&store);

if (config.pulse_subtract_dc==TRUE)
{
	inter_sub_long_double(&out_i,out_i.data[0]);
	inter_sub_long_double(&out_v,out_v.data[0]);
	inter_sub_long_double(&out_j_int,out_j_int.data[0]);
}


if (in->ncontacts==2)
{
	inter_copy(&out_j,&out_i,TRUE);
	math_xy_mul_long_double(&out_j,1.0/(in->xlen*in->zlen));

	if (buffer_set_file_name(sim,in,&buf,"time_j_int.dat")==0)
	{
		buffer_malloc(&buf);
		buf.y_mul=1.0;
		buf.data_mul=1.0;

		sprintf(buf.title,"%s - %s",_("Time"),_("Current"));
		strcpy(buf.type,"xy");
		strcpy(buf.y_label,_("Time"));
		strcpy(buf.y_units,"s");

		strcpy(buf.data_label,_("Current"));
		strcpy(buf.data_units,"Am^{-2}");

		buf.logscale_x=0;
		buf.logscale_y=0;
		dat_file_add_xy_data(sim,&buf,out_j_int.x, out_j_int.data, out_j_int.len);
		buffer_dump_path(sim,in->output_path,NULL,&buf);
		buffer_free(&buf);
	}

	if (buffer_set_file_name(sim,in,&buf,"time_i.dat")==0)
	{
		buffer_malloc(&buf);
		buf.y_mul=1.0;
		buf.data_mul=1.0;
		sprintf(buf.title,"%s - %s",_("Time"),_("Current"));
		strcpy(buf.type,"xy");
		strcpy(buf.y_label,_("Time"));
		strcpy(buf.y_units,"s");

		strcpy(buf.data_label,_("Current"));
		strcpy(buf.data_units,"A");
		buf.logscale_x=0;
		buf.logscale_y=0;
		dat_file_add_xy_data(sim,&buf,out_i.x, out_i.data, out_i.len);
		buffer_dump_path(sim,in->output_path,NULL,&buf);
		buffer_free(&buf);
	}

	if (buffer_set_file_name(sim,in,&buf,"time_j.dat")==0)
	{

		buffer_malloc(&buf);
		buf.y_mul=1.0;
		buf.data_mul=1.0;
		sprintf(buf.title,"%s - %s",_("Time"),_("current density"));
		strcpy(buf.type,"xy");
		strcpy(buf.y_label,_("Time"));
		strcpy(buf.y_units,"s");

		strcpy(buf.data_label,_("Current density"));
		strcpy(buf.data_units,"Am^{-2}");
		buf.logscale_x=0;
		buf.logscale_y=0;
		dat_file_add_xy_data(sim,&buf,out_j.x, out_j.data, out_j.len);
		buffer_dump_path(sim,in->output_path,NULL,&buf);
		buffer_free(&buf);
	}

	if (buffer_set_file_name(sim,in,&buf,"time_j_abs.dat")==0)
	{
		inter_copy(&out_flip,&out_j,TRUE);
		math_xy_mul_long_double(&out_flip,-1.0);

		buffer_malloc(&buf);
		buf.y_mul=1.0;
		buf.data_mul=1.0;
		sprintf(buf.title,"%s - %s",_("Time"),_("Current density"));
		strcpy(buf.type,"xy");
		strcpy(buf.y_label,_("Time"));
		strcpy(buf.data_label,_("Current density"));
		strcpy(buf.y_units,"s");
		strcpy(buf.data_units,"Am^{-2}");
		buf.logscale_x=0;
		buf.logscale_y=0;
		dat_file_add_xy_data(sim,&buf,out_flip.x, out_flip.data, out_flip.len);
		buffer_dump_path(sim,in->output_path,NULL,&buf);
		buffer_free(&buf);

		inter_free(&out_flip);
	}


	if (buffer_set_file_name(sim,in,&buf,"time_i_abs.dat")==0)
	{
		inter_copy(&out_flip,&out_i,TRUE);
		math_xy_mul_long_double(&out_flip,-1.0);

		buffer_malloc(&buf);
		buf.y_mul=1.0;
		buf.data_mul=1.0;
		sprintf(buf.title,"%s - %s",_("Time"),_("current"));
		strcpy(buf.type,"xy");
		strcpy(buf.y_label,_("Time"));
		strcpy(buf.data_label,_("Current"));
		strcpy(buf.y_units,"s");
		strcpy(buf.data_units,"A");
		buf.logscale_x=0;
		buf.logscale_y=0;
		dat_file_add_xy_data(sim,&buf,out_flip.x, out_flip.data, out_flip.len);
		buffer_dump_path(sim,in->output_path,NULL,&buf);
		buffer_free(&buf);

		inter_free(&out_flip);
	}


	if (buffer_set_file_name(sim,in,&buf,"time_v.dat")==0)
	{
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
		dat_file_add_xy_data(sim,&buf,out_v.x, out_v.data, out_v.len);
		buffer_dump_path(sim,in->output_path,NULL,&buf);
		buffer_free(&buf);
	}

	inter_free(&out_j);
}

if (buffer_set_file_name(sim,in,&buf,"time_G.csv")==0)
{
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
	dat_file_add_xy_data(sim,&buf,out_G.x, out_G.data, out_G.len);
	buffer_dump_path(sim,in->output_path,NULL,&buf);
	buffer_free(&buf);
}

if (in->emission_enabled==TRUE)
{
	if (buffer_set_file_name(sim,in,&buf,"time_emission.csv")==0)
	{
		buffer_malloc(&buf);
		buf.y_mul=1.0;
		buf.x_mul=1e6;
		sprintf(buf.title,"%s - %s",_("Time"),_("Emission"));
		strcpy(buf.type,"xy");
		strcpy(buf.y_label,_("Time"));
		strcpy(buf.data_label,_("Emission"));
		strcpy(buf.y_units,"s");
		strcpy(buf.data_units,"W");
		buf.logscale_x=0;
		buf.logscale_y=0;
		dat_file_add_xy_data(sim,&buf,time_emission.x, time_emission.data, time_emission.len);
		buffer_dump_path(sim,in->output_path,NULL,&buf);
		buffer_free(&buf);
	}

	if (buffer_set_file_name(sim,in,&buf,"time_emission_norm.csv")==0)
	{
		buffer_malloc(&buf);
		buf.y_mul=1.0;
		buf.x_mul=1e6;
		sprintf(buf.title,"%s - %s",_("Time"),_("Emission"));
		strcpy(buf.type,"xy");
		strcpy(buf.y_label,_("Time"));
		strcpy(buf.data_label,_("Emission"));
		strcpy(buf.y_units,"s");
		strcpy(buf.data_units,"W");
		buf.logscale_x=0;
		buf.logscale_y=0;
		inter_norm(&(time_emission),1.0);
		dat_file_add_xy_data(sim,&buf,time_emission.x, time_emission.data, time_emission.len);
		buffer_dump_path(sim,in->output_path,NULL,&buf);
		buffer_free(&buf);
	}
}

if (in->ncontacts==2)
{
	if (buffer_set_file_name(sim,in,&buf,"jv.dat")==0)
	{
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

		buffer_dump_path(sim,in->output_path,NULL,&buf);
		buffer_free(&buf);
	}
}

if (get_dump_status(sim,dump_optical_probe)==TRUE)
{
	probe_dump(sim,in);
}


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
inter_free(&time_emission);


time_memory_free(&(in->tm));


in->go_time=FALSE;

}

void pulse_load_config(struct simulation *sim,struct pulse *in,struct device *dev,struct json_obj *json_pulse)
{
	char laser_name[200];
	struct json_obj *json_config;
	json_config=json_obj_find(json_pulse, "config");

	json_get_long_double(sim,json_config,&(in->pulse_shift),"pulse_shift");
	json_get_english(sim,json_config,&(in->pulse_sim_mode),"load_type");
	json_get_long_double(sim,json_config,&(dev->L),"pulse_L");
	json_get_long_double(sim,json_config,&(dev->Rload),"Rload");
	json_get_long_double(sim,json_config,&(in->pulse_bias),"pulse_bias");

	json_get_string(sim,json_config,laser_name,"pump_laser");
	light_load_laser(sim,(&dev->mylight),laser_name,&(dev->config.obj));

	json_get_english(sim,json_config,&(in->pulse_subtract_dc),"pulse_subtract_dc");
	json_get_int(sim,json_config,&(in->dump_verbosity),"dump_verbosity");

	json_get_english(sim,json_config, &(in->dump_energy_space),"dump_energy_space");
	json_get_int(sim,json_config, &(in->dump_x),"dump_x");
	json_get_int(sim,json_config, &(in->dump_y),"dump_y");
	json_get_int(sim,json_config, &(in->dump_z),"dump_z");

	json_get_long_double(sim,json_config,&(in->pulse_light_efficiency),"pulse_light_efficiency");
	in->pulse_light_efficiency=gfabs(in->pulse_light_efficiency);



}
