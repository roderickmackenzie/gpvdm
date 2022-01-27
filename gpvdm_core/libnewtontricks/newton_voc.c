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
		if (fabs(Vapplied)>1e-2)
		{
			ewe(sim,"I have found a negative Voc %Le\n",Vapplied);
		}
	}

return ret;
}


