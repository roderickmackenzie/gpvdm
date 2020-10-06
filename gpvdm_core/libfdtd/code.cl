
__kernel void cal_E ( __global float* Ex,__global float* Ey,__global float* Ez ,__global float* Hx,__global float* Hy,__global float* Hz,__global float* Ex_last,__global float* Ey_last,__global float* Ez_last,__global float* Hx_last,__global float* Hy_last,__global float* Hz_last, __global float* epsilon_r, __global float* y,__global float* C)
{
float Cx=C[0];
float Cy=C[1];
float Cz=C[2];
int ylen=(int)C[3];
int zlen=(int)C[4];
float time=C[5];
float omega=C[6];
float lambda=C[7];
float dx=C[8];
float dy=C[9];
float dz=C[10];
float dt2=C[11];
float Cmx=C[12];
float Cmy=C[13];
float Cmz=C[14];
float src_start=C[15];
int excitation_mesh_point=(int)C[16];

const float clight=(float)3e8;

const uint gid = get_global_id(0);//*2+get_local_id(0);
if (gid < ylen*zlen)
{
int i=(gid/ylen);
int j=gid-i*ylen;
//Ex[gid]=0;


//
if ((i>0)&&(i<zlen-1))
{
	if ((j>=0)&&(j<ylen-1))
	{


		float Hzd=Hz[i*ylen+j-1];
		float Hxl=Hx[(i-1)*ylen+j];
		float Hxd=Hx[i*ylen+j-1];
		float Hyl=Hy[(i-1)*ylen+j];

		Ex[gid]=Ex_last[gid]+(Hz[gid]-Hzd)*epsilon_r[gid]*Cy-(Hy[gid]-Hyl)*epsilon_r[gid]*Cz;
		Ey[gid]=Ey_last[gid]+(Hx[gid]-Hxl)*epsilon_r[gid]*Cz;
		Ez[gid]=Ez_last[gid]-(Hx[gid]-Hxd)*epsilon_r[gid]*Cy;
		//printf("%e\n",Cmz);
		if ((j==excitation_mesh_point)&&(i==excitation_mesh_point))
		{
			//if ((y[j]>src_start)&&(y[j]<src_stop))
			{
				float start=y[1];
				float a=0.0;
				float b=0.0;
				float c=0.0;
				float phi=0.0;
				float theta=0.0;


				float dot=0.0;
				phi=0.0;
				theta=90.0;

				float shift=-2.0;
				a=sin(theta*(2.0*M_PI_F/360.0))*cos(phi*(2.0*M_PI_F/360.0));
				b=sin(theta*(2.0*M_PI_F/360.0))*sin(phi*(2.0*M_PI_F/360.0));
				c=cos(theta*(2.0*M_PI_F/360.0));

				dot=tan(2.0*M_PI_F*(shift)/360.0)*(y[j]-start)*2.0*M_PI_F/lambda;
				float mod=1.0;
				//dot=0.0;
				Ex[gid]=mod*a*sin(dot-time*omega);
				Ey[gid]=mod*b*sin(dot-time*omega);
				Ez[gid]=mod*c*sin(dot-time*omega);
			}
		}

		if (i==1)
		{
			float Ex_last_l=Ex_last[(i-1)*ylen+j];
			float Ey_last_l=Ey_last[(i-1)*ylen+j];
			float Ez_last_l=Ez_last[(i-1)*ylen+j];
			Ex[(i-1)*ylen+j]=Ex_last[gid]+((clight*dt2-dz)/(clight*dt2+dz))*(Ex[gid]-Ex_last_l);
			Ey[(i-1)*ylen+j]=Ey_last[gid]+((clight*dt2-dz)/(clight*dt2+dz))*(Ey[gid]-Ey_last_l);
			Ez[(i-1)*ylen+j]=Ez_last[gid]+((clight*dt2-dz)/(clight*dt2+dz))*(Ez[gid]-Ez_last_l);
		}

		if (i==zlen-2)
		{
			float Ex_last_r=Ex_last[(i+1)*ylen+j];
			float Ey_last_r=Ey_last[(i+1)*ylen+j];
			float Ez_last_r=Ez_last[(i+1)*ylen+j];
			Ex[(i+1)*ylen+j]=Ex_last[gid]+((clight*dt2-dz)/(clight*dt2+dz))*(Ex[gid]-Ex_last_r);
			Ey[(i+1)*ylen+j]=Ey_last[gid]+((clight*dt2-dz)/(clight*dt2+dz))*(Ey[gid]-Ey_last_r);
			Ez[(i+1)*ylen+j]=Ez_last[gid]+((clight*dt2-dz)/(clight*dt2+dz))*(Ez[gid]-Ez_last_r);
		}


	}

}


if ((i>=0)&&(i<(zlen-1)))
{
	if (j==1)
	{
		float Ex_last_d=Ex_last[(i)*ylen+j-1];
		float Ey_last_d=Ey_last[(i)*ylen+j-1];
		float Ez_last_d=Ez_last[(i)*ylen+j-1];
		Ex[(i)*ylen+j-1]=Ex_last[gid]+((clight*dt2-dy)/(clight*dt2+dy))*(Ex[gid]-Ex_last_d);
		Ey[(i)*ylen+j-1]=Ey_last[gid]+((clight*dt2-dy)/(clight*dt2+dy))*(Ey[gid]-Ey_last_d);
		Ez[(i)*ylen+j-1]=Ez_last[gid]+((clight*dt2-dy)/(clight*dt2+dy))*(Ez[gid]-Ez_last_d);
	}

	if (j==(ylen-2))
	{
		float Ex_last_u=Ex_last[(i)*ylen+j+1];
		float Ey_last_u=Ey_last[(i)*ylen+j+1];
		float Ez_last_u=Ez_last[(i)*ylen+j+1];
		//printf("%ld %le %le\n",dt2,dx,clight);
		
		Ex[(i)*ylen+j+1]=Ex_last[gid]+((clight*dt2-dy)/(clight*dt2+dy))*(Ex[gid]-Ex_last_u);
		Ey[(i)*ylen+j+1]=Ey_last[gid]+((clight*dt2-dy)/(clight*dt2+dy))*(Ey[gid]-Ey_last_u);
		Ez[(i)*ylen+j+1]=Ez_last[gid]+((clight*dt2-dy)/(clight*dt2+dy))*(Ez[gid]-Ez_last_u);

	}
}

//Ex_last[gid]=Ex[gid];
//Ey_last[gid]=Ey[gid];
//Ez_last[gid]=Ez[gid];
}
}

__kernel void cal_H ( __global float* Ex,__global float* Ey,__global float* Ez ,__global float* Hx,__global float* Hy,__global float* Hz,__global float* Ex_last,__global float* Ey_last,__global float* Ez_last,__global float* Hx_last,__global float* Hy_last,__global float* Hz_last, __global float* epsilon_r, __global float* y,__global float* C)
{
float Cx=C[0];
float Cy=C[1];
float Cz=C[2];
int ylen=(int)C[3];
int zlen=(int)C[4];
float time=C[5];
float omega=C[6];
float lambda=C[7];
float dx=C[8];
float dy=C[9];
float dz=C[10];
float dt2=C[11];
float Cmx=C[12];
float Cmy=C[13];
float Cmz=C[14];
const float clight=(float)3e8;
//printf("%d %d\n",get_global_id(0),get_local_id(0));
const uint gid = get_global_id(0);
if (gid < ylen*zlen)
{
int i=(gid/ylen);
int j=gid-i*ylen;

if ((i>=0)&&(i<(zlen-1)))
{
	if ((j>=0)&&(j<ylen-1))
	{
		float Ezu=Ez[i*ylen+j+1];
		float Exr=Ex[(i+1)*ylen+j];
		float Exu=Ex[i*ylen+j+1];
		float Eyr=Ey[(i+1)*ylen+j];

		Hx[gid]=Hx_last[gid]-(Ezu-Ez[gid])*Cmy+(Eyr-Ey[gid])*Cmz;
		Hy[gid]=Hy_last[gid]-(Exr-Ex[gid])*Cmz;
		Hz[gid]=Hz_last[gid]+(Exu-Ex[gid])*Cmy;	
	}
}

//Hx_last[gid]=Hx[gid];
//Hy_last[gid]=Hy[gid];
//Hz_last[gid]=Hz[gid];
}
}

__kernel void update_E ( __global float* Ex,__global float* Ey,__global float* Ez ,__global float* Hx,__global float* Hy,__global float* Hz,__global float* Ex_last,__global float* Ey_last,__global float* Ez_last,__global float* Hx_last,__global float* Hy_last,__global float* Hz_last, __global float* epsilon_r, __global float* y,__global float* C)
{
float Cx=C[0];
float Cy=C[1];
float Cz=C[2];
int ylen=(int)C[3];
int zlen=(int)C[4];
float time=C[5];
float omega=C[6];
float lambda=C[7];
float dx=C[8];
float dy=C[9];
float dz=C[10];
float dt2=C[11];
float Cmx=C[12];
float Cmy=C[13];
float Cmz=C[14];
const uint gid = get_global_id(0);//*2+get_local_id(0);
if (gid==0) C[5]+=C[11];

if (gid < ylen*zlen)
{
	Ex_last[gid]=Ex[gid];
	Ey_last[gid]=Ey[gid];
	Ez_last[gid]=Ez[gid];
}

}

__kernel void update_H ( __global float* Ex,__global float* Ey,__global float* Ez ,__global float* Hx,__global float* Hy,__global float* Hz,__global float* Ex_last,__global float* Ey_last,__global float* Ez_last,__global float* Hx_last,__global float* Hy_last,__global float* Hz_last, __global float* epsilon_r, __global float* y,__global float* C)
{
float Cx=C[0];
float Cy=C[1];
float Cz=C[2];
int ylen=(int)C[3];
int zlen=(int)C[4];
float time=C[5];
float omega=C[6];
float lambda=C[7];
float dx=C[8];
float dy=C[9];
float dz=C[10];
float dt2=C[11];
float Cmx=C[12];
float Cmy=C[13];
float Cmz=C[14];
const uint gid = get_global_id(0);//*2+get_local_id(0);
if (gid==0)
{
C[5]+=C[11];
}
if (gid < ylen*zlen)
{
Hx_last[gid]=Hx[gid];
Hy_last[gid]=Hy[gid];
Hz_last[gid]=Hz[gid];
}

}
