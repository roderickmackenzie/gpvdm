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

/** @file sim_find_n0.c
@brief calculate 0V in the dark using the newton solver.
*/

#include <stdio.h>
#include <exp.h>
#include "util.h"
#include "sim.h"
#include "dos.h"
#include "dump.h"
#include "log.h"
#include <cal_path.h>
#include <contacts.h>
#include <lang.h>
#include <light_fun.h>
#include <memory.h>

void find_n0(struct simulation *sim,struct device *in)
{
struct newton_state *ns=&in->ns;
long double ***Bfree_bk=NULL;
long double ***Binterface_bk=NULL;
long double ***Bt_interface_bk=NULL;

struct dimensions *dim=&in->ns.dim;
printf_log(sim,"%s\n",_("Finding equilibrium conditions"));
gdouble oldsun=light_get_sun(&(in->mylight));

contacts_force_to_zero(sim,in);



light_set_sun(&(in->mylight),0);

light_solve_and_update(sim,in,&(in->mylight),0.0);

	if (in->drift_diffision_simulations_enabled==TRUE)
	{
		cpy_zxy_long_double(dim, &(Bfree_bk), &(in->B));
		cpy_zxy_long_double(dim, &(Binterface_bk), &(in->interface_B));
		cpy_zxy_long_double(dim, &(Bt_interface_bk), &(in->interface_Bt));

		zxy_set_gdouble(dim, in->B, 0.0);
		zxy_set_gdouble(dim, in->interface_B, 0.0);
		zxy_set_gdouble(dim, in->interface_Bt, 0.0);

		gdouble save_clamp=in->electrical_clamp;
		int save_ittr=in->max_electrical_itt;
		gdouble save_electricalerror=in->min_cur_error;

		in->electrical_clamp=in->electrical_clamp0;
		in->max_electrical_itt=in->max_electrical_itt0;
		in->min_cur_error=in->electrical_error0;

		//int Nion_enabled=dim->Nion_enabled;
		//dim->Nion_enabled=FALSE;
		//solve_all(sim,in);
		//dim->Nion_enabled=Nion_enabled;
		//getchar();
		//struct shape *s;
		if (ns->Nion_enabled==TRUE)
		{
			ns->Nion_enabled=FALSE;
			solve_all(sim,in);
			//getchar();
			ns->Nion_enabled=TRUE;
			in->mobileion.Nion_mul=1e-10;
			in->mobileion.full_newton=TRUE;
			solve_all(sim,in);

			/*int y=0;
			for (y=0;y<dim->ylen;y++)
			{
				in->Nad[0][0][y]-=in->Nion[0][0][y];
			}*/
			/*int y=0;
			long double orig=in->Nion[0][0][10];
			long double mul=1e-5;
			while(mul<1.0)
			{
				for (y=0;y<dim->ylen;y++)
				{
					s=in->obj_zxy[0][0][y]->s;
					in->Nion[0][0][y]=get_dos_ion_density(s)*mul;
					printf("%Le\n",in->Nion[0][0][y]);
					long double top=get_top_from_ion(in->Nion[0][0][y]);
					ns->x_Nion[0][0][y]=-(ns->phi[0][0][y]+in->Ev[0][0][y]-top);
				}
				solve_all(sim,in);
				FILE *out=fopen("out.dat","w");
				for (y=0;y<dim->ylen;y++)
				{
					fprintf(out,"%Le %Le\n",dim->ymesh[y],in->Nion[0][0][y]);
				}
				fclose(out);

				out=fopen("phi.dat","w");
				for (y=0;y<dim->ylen;y++)
				{
					fprintf(out,"%Le %Le\n",dim->ymesh[y],ns->phi[0][0][y]);
				}
				fclose(out);
				printf("mul=%Le\n",mul);
				mul=mul*1.5;
				//getchar();
			}

			getchar();*/
			
		}
		//getchar();
		//exit(0);
		in->max_electrical_itt=save_ittr;
		in->electrical_clamp=save_clamp;
		in->min_cur_error=save_electricalerror;
	}

	solve_all(sim,in);

	if (in->drift_diffision_simulations_enabled==TRUE)
	{
		cpy_zxy_long_double(dim, &(in->B), &(Bfree_bk));
		cpy_zxy_long_double(dim, &(in->interface_B), &(Binterface_bk));
		cpy_zxy_long_double(dim, &(in->interface_Bt), &(Bt_interface_bk));

		free_zxy_gdouble(dim, &(Bfree_bk));
		free_zxy_gdouble(dim, &(Binterface_bk));
		free_zxy_gdouble(dim, &(Bt_interface_bk));

		reset_np_save(in);
		reset_npequlib(in);
	}

in->contact_charge=cal_contact_charge(in);

light_set_sun(&(in->mylight),oldsun);

contacts_update(sim,in);		//This should restore the value of the contacts

printf_log(sim,"%s\n",_("Finished finding equilibrium conditions"));
}
