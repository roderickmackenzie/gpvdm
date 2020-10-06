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

/** @file near_to_far.c
	@brief Fear to far filed generator, not currently linked to.
*/

void near_to_far(double *x, double *E,int len,double *y, double *E_right,int len_right)
{
int i;
double dx=x[1]-x[0];
double dy=y[1]-y[0];
double mid=x[len/2];
struct vec start;
struct vec stop;
double left=-0.1;
double right=0.1;
double dist=0.01;
double pos=left;
double *far=NULL;
far=malloc(sizeof(double)*len_far);
double dfar=(right-left)/((double)len_far);
int j;

if (far_avg==NULL)
{
far_avg=malloc(sizeof(double)*len_far);
near_top_avg=malloc(sizeof(double)*len);
near_right_avg=malloc(sizeof(double)*len_right);
xfar=malloc(sizeof(double)*len_far);
	for (j=0;j<len_far;j++)
	{
		far_avg[j]=0.0;
	}

	for (j=0;j<len;j++)
	{
		near_top_avg[j]=0.0;
	}

	for (j=0;j<len_right;j++)
	{
		near_right_avg[j]=0.0;
	}

}

pos=left;
double k=(2.0*pi/lambda);
double modr;
double complex cresult;
double result;
int n;

//FILE *out=fopen("./near_top.dat","w");
for (j=0;j<len;j++)
{
	near_top_avg[j]+=E[j];//pow(E[j],2.0);
	//	near_right_avg[j]=0.0;fprintf_log(sim,out,"%le %le\n",x[j], );
}
//fclose(out);

//out=fopen("./near_right.dat","w");
for (j=0;j<len_right;j++)
{
	near_right_avg[j]+=pow(E_right[j],2.0);
	//fprintf_log(sim,out,"%le %le\n",y[j], pow(E_right[j],2.0));
}
//fclose(out);

double theta;

struct vec dr;

pos=-80;
double dtheta=160/(double)len_far;
for (j=0;j<len_far;j++)
{
xfar[j]=pos;
pos+=dtheta;
}

for (j=0;j<len_far;j++)
{
result=0.0;
	//for (n=0;n<8;n++)
	{
		pos=dist*tan((2*pi/360.0)*xfar[j]);
		complex cresult=0+0*I;
		for (i=0;i<len;i++)
		{
			vec_set(&start,0.0,0.0,x[i]-mid);
			vec_set(&stop,0.0,pos,dist);
			//vec_set(&start,0.0,x[i]-mid-((double)n)*x[len-1],0.0);
			//vec_set(&stop,0.0,pos,dist);
			vec_cpy(&dr,&stop);
			vec_sub(&dr,&start);
			modr=vec_mod(&dr);
			cresult+=dx*E[i]*cexp(I*k*modr*-1.0)/modr;
		}

		for (i=len_right;i<len_right;i++)
		{
			vec_set(&start,0.0,y[i]-y[len_right-1],x[len-1]-mid);
			vec_set(&stop,0.0,pos,dist);
			//vec_set(&start,0.0,x[i]-mid-((double)n)*x[len-1],0.0);
			//vec_set(&stop,0.0,pos,dist);
			vec_cpy(&dr,&stop);
			vec_sub(&dr,&start);
			modr=vec_mod(&dr);
			cresult+=dy*E_right[i]*cexp(I*k*modr*-1.0)/modr;
		}

		result=cabs(cresult);

	}
theta=(360.0/(2.0*pi))*atan(pos/dist);
far[j]=fabs(result);

}

pos=left;


//out=fopen("./far.dat","w");
for (j=0;j<len_far;j++)
{
	far_avg[j]+=far[j];
//	fprintf_log(sim,out,"%lf %le\n",xfar[j], far_avg[j]/((double)far_steps));
}
//fclose(out);



far_steps+=1.0;
free(far);

}
