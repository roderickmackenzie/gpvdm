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

/** @file newton_tricks.c
	@brief A collection of helper functions for the newton solver.
*/
//<stripall>

#include <stdio.h>
#include <exp.h>
#include "sim.h"
#include "newton_tricks.h"
#include "gui_hooks.h"
#include <plot.h>
#include <cal_path.h>
#include <heat.h>
#include <heat_fun.h>
#include <contacts.h>
#include <dump.h>
#include <log.h>
#include <enabled_libs.h>
#include <heat_fun.h>

static int unused __attribute__((unused));


struct newton_math_state math_save_state;

int solve_cur(struct simulation *sim,struct device *dev,int z,int x)
{
	//printf("pointer=%p\n",dev->dll_solve_cur);
	int ret=(*dev->dll_solve_cur)(sim,dev,z,x);
	if (is_errors(sim)==0)
	{
		errors_dump(sim);
		ewe(sim,"error");
	}
return ret;
}

void newton_push_state(struct device *in)
{
	math_save_state.min_cur_error=in->min_cur_error;
	math_save_state.max_electrical_itt=in->max_electrical_itt;
	math_save_state.newton_min_itt=in->newton_min_itt;
	math_save_state.electrical_clamp=in->electrical_clamp;
	math_save_state.newton_clever_exit=in->newton_clever_exit;
}

void newton_pop_state(struct device *in)
{
	in->min_cur_error=math_save_state.min_cur_error;
	in->max_electrical_itt=math_save_state.max_electrical_itt;
	in->newton_min_itt=math_save_state.newton_min_itt;
	in->electrical_clamp=math_save_state.electrical_clamp;
	in->newton_clever_exit=math_save_state.newton_clever_exit;
}

void ramp_externalv(struct simulation *sim,struct device *in,gdouble from,gdouble to)
{
gdouble V=from;
gdouble dV=0.12;
if ((to-from)<0.0) dV*= -1.0;
printf_log(sim,"dV=%Le\n",dV);
printf_log(sim,"Ramping: from=%Le to=%Le\n",from,to);


if (fabs(to-from)<=fabs(dV)) return;

do
{

	V+=dV;
	if (get_dump_status(sim,dump_print_text)==TRUE) printf_log(sim,"ramp: %Lf %Lf %d\n",V,to,in->kl_in_newton);


	sim_externalv(sim,in,V);

	//plot_now(sim,in,"jv.plot");
	gui_send_data(sim,gui_sub,"pulse");

	if (fabs(V-to)<fabs(dV))
	{
		break;
	}

}while(1);

if (V!=to)
{
	V=to;
	sim_externalv(sim,in,V);
}

return;
}

void ntricks_auto_ramp_contacts(struct simulation *sim,struct device *in)
{
printf_log(sim,"Multidimentional autoramp\n");
int i;
int ret=0;
int changed=TRUE;
char send_data[200];
char temp[200];

gdouble Vapplied=0.0;
in->kl_in_newton=FALSE;
solver_realloc(sim,in);

newton_push_state(in);

in->min_cur_error=1e-8;
in->max_electrical_itt=100;
in->newton_min_itt=3;
in->electrical_clamp=1.0;
//in->newton_clever_exit=FALSE;


while (changed==TRUE)
{

	changed=contacts_itterate_to_desired_voltage(sim,in,temp);
	if (changed==TRUE)
	{
		solve_all(sim,in);
		ret=snprintf(send_data,200,"text:%s",temp);
		if (ret<0)
		{
			ewe(sim,"snprintf error\n");
		}
		gui_send_data(sim,gui_sub,send_data);

		gui_send_data(sim,gui_sub,"pulse");
		poll_gui(sim);
	}
}

newton_pop_state(in);



printf_log(sim,"Finished with multidimentional autoramp\n");
return;

}






void solve_all(struct simulation *sim,struct device *in)
{
int z=0;
int x=0;
int ittr=0;
int cont=TRUE;
struct dimensions *dim=&in->ns.dim;
struct heat *thermal=&(in->thermal);



for (z=0;z<dim->zlen;z++)
{
//	for (x=0;x<in->xlen;x++)
//	{

		if (thermal->newton_enable_external_thermal==FALSE)
		{
			solve_cur(sim,in,z,x);
		}else
		{
			#ifdef libheat_enabled
				do
				{

					solve_cur(sim,in,z,x);

					//plot_now(sim,"thermal.plot");
					//getchar();
					heat_solve(sim,&(in->thermal),in,z,x);
					if (in->thermal.thermal_couple_to_electrical_solver==TRUE)
					{
						heat_transfer_temperatures_to_device(in,&(in->thermal));
					}
					//plot_now(sim,"thermal.plot");
					//getchar();

					//plot_now(in);
					///getchar();
					//printf("%d %d\n",thermal->thermal_conv,in->dd_conv);
					if (((thermal->thermal_conv==TRUE)&&(in->dd_conv==TRUE))||(ittr>10)) cont=FALSE;
					//getchar();
					ittr++;
				}while(cont==TRUE);
			#endif
		}
//	}
	if (in->circuit_simulation==TRUE)
	{
		break;
	}
}


}

void newton_sim_simple(struct simulation  *sim,struct device *in)
{
in->kl_in_newton=FALSE;
solver_realloc(sim,in);

solve_all(sim,in);
}

