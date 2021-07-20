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

/** @file newton_voc.c
	@brief Run the newton solver to calculate Voc.
*/
//<stripall>

#include <dump.h>
#include <sim.h>
#include <exp.h>
#include <log.h>
#include <contacts.h>
#include <newton_voc.h>
#include <light_fun.h>



gdouble newton_sim_voc_fast(struct simulation *sim,struct device *in,int do_LC)
{
	gdouble Vapplied=0.0;
	gdouble Vapplied_last=0.0;
	//long double photon_density=0.0;

	//photon_density=three_d_avg(in, in->Gn);
	//if (photon_density<1.0)
	//{
	//	return 0.0;
	//}

	Vapplied=contact_get_active_contact_voltage(sim,in);
	Vapplied_last=contact_get_active_contact_voltage_last(sim,in);

		newton_sim_voc(sim, in);
	
return get_I(in)+in->C*(Vapplied-Vapplied_last)+Vapplied/in->Rshunt;
}

long double newton_sim_voc(struct simulation *sim, struct device *in)
{
	printf_log(sim,"Looking for Voc (1)\n");
	//long double photon_density=0.0;
	//photon_density=three_d_avg(in, in->Gn);
	//if (photon_density<1.0)
	//{
	//	return 0.0;
	//}

	long double C=in->C;
	long double clamp=0.05;
	long double step=0.01;
	long double e0;
	long double e1;
	long double i0;
	long double i1;
	long double deriv;

	long double Vapplied=0.0;
	long double Vapplied_last=0.0;
	int count=0;
	int max=200;
	long double error_diff=0.0;
	long double ret=0.0;

	Vapplied=contact_get_active_contact_voltage(sim,in);
	//printf("%Lf %Le %Lf %Le\n",Vapplied,in->Gn[0][0][0],in->Rload,in->Rcontact);
	//getchar();
	Vapplied_last=contact_get_active_contact_voltage_last(sim,in);
	//printf("Generation rate: %Le m^-3 s^-1\n",in->Gn[0][0][0]);
	//getchar();
	//printf("%p\n",sim->dll_matrix_handle);
	//getchar();
	solve_all(sim,in);
	//getchar();
	i0=get_I(in);
	if (in->go_time==FALSE)
	{
		e0=fabs(i0+Vapplied*(1.0/in->Rshunt));
	}else
	{
		e0=fabs(i0+Vapplied*(1.0/in->Rshunt)+C*(Vapplied-Vapplied_last)/in->dt);
	}
	Vapplied+=step;
	contact_set_active_contact_voltage(sim,in,Vapplied);
	solve_all(sim,in);
	i1=get_I(in);
	if (in->go_time==FALSE)
	{
		e1=fabs(i1+Vapplied*(1.0/in->Rshunt));
	}else
	{
		e1=fabs(i1+Vapplied*(1.0/in->Rshunt)+C*(Vapplied-Vapplied_last)/in->dt);
	}

	deriv=(e1-e0)/step;
	step=-e1/deriv;

	step=step/(1.0+fabs(step/clamp));
	Vapplied+=step;
	contact_set_active_contact_voltage(sim,in,Vapplied);

	do
	{
		e0=e1;
		solve_all(sim,in);
		i1=get_I(in);
		if (in->go_time==FALSE)
		{
			e1=fabs(i1+Vapplied*(1.0/in->Rshunt));
		}else
		{
			e1=fabs(i1+Vapplied*(1.0/in->Rshunt)+C*(Vapplied-Vapplied_last)/in->dt);
		}
		printf("%Le\n",get_equiv_J(sim,in));
		deriv=(e1-e0)/step;
		step=-e1/deriv;
		error_diff=e1-e0;

		step=step/(1.0+fabs(step/clamp));
		Vapplied+=step;
		contact_set_active_contact_voltage(sim,in,Vapplied);

		//if (get_dump_status(sim,dump_print_text)==TRUE)
		{
			printf_log(sim,"%d Vapplied=%Lf step=%Le f()=%Le\n",count,Vapplied,step,e1);
		}
		if (count>max) break;
		count++;

		
		if (error_diff>0)
		{
			clamp/=1.1;
			printf_log(sim,"*");
		}
			
	}while(e1>1e-12);

	if (Vapplied<0.0)
	{
		ewe(sim,"I have found a negative Voc");
	}

return ret;
}


