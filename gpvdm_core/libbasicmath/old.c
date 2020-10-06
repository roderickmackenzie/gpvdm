
struct triangle *triangles_find_zx_connected_vector(struct triangles *in,struct triangle *tri0,double z0,double x0,double z1,double x1, double *ret_z0,double *ret_x0,double *ret_z1,double *ret_x1)
{
	struct triangle *ret=NULL;
	int i;
	double dx0=0.0;
	double dz0=0.0;
	double dx1=0.0;
	double dz1=0.0;
	
	dx0=x1-x0;
	dz0=z1-z0;

	int connected0=-1;
	int connected1=-1;

	double dist=1e6;
	double dist_temp=0.0;

	struct triangle *tri;
	for (i=0;i<in->len;i++)
	{
		connected0=-1;
		connected1=-1;
		tri=&(in->data[i]);
		if (eq(tri->xy0.z,z1)&&eq(tri->xy0.x,x1))
		{
			connected0=0;
		}else
		if (eq(tri->xy1.z,z1)&&eq(tri->xy1.x,x1))
		{
			connected0=1;
		}else
		if (eq(tri->xy2.z,z1)&&eq(tri->xy2.x,x1))
		{
			connected0=2;
		}

		if (connected0>=0)
		{
			if (connected0==0)
			{
				dx1=tri->xy1.x-x1;
				dz1=tri->xy1.z-z1;
				//printf("%e %e %e %e %d %d %d\n",dx0,dz0,dx1,dz1, eq(dx0,dx1), eq(dz0,dz1),(eq(dx0,dx1) && eq(dz0,dz1)));
				//getchar();
				if (eq(dx0,dx1) && eq(dz0,dz1))
				{
					dist_temp=vec_dist(&tri0->cog,&tri->cog);
					if (dist_temp<dist)
					{
						dist=dist_temp;
						*ret_z0=tri->xy1.z;
						*ret_x0=tri->xy1.x;
						*ret_z1=tri->xy2.z;
						*ret_x1=tri->xy2.x;

						ret=tri;
						connected1=1;
					}
				}

				dx1=tri->xy2.x-x1;
				dz1=tri->xy2.z-z1;
				if (eq(dx0,dx1) && eq(dz0,dz1))
				{
					dist_temp=vec_dist(&tri0->cog,&tri->cog);
					if (dist_temp<dist)
					{
						dist=dist_temp;
						*ret_z0=tri->xy2.z;
						*ret_x0=tri->xy2.x;
						*ret_z1=tri->xy1.z;
						*ret_x1=tri->xy1.x;

						ret=tri;
						connected1=2;
					}
				}
			}else
			if (connected0==1)
			{
				dx1=tri->xy0.x-x1;
				dz1=tri->xy0.z-z1;
				if (eq(dx0,dx1) && eq(dz0,dz1))
				{
					dist_temp=vec_dist(&tri0->cog,&tri->cog);
					if (dist_temp<dist)
					{
						dist=dist_temp;
						*ret_z0=tri->xy0.z;
						*ret_x0=tri->xy0.x;
						*ret_z1=tri->xy2.z;
						*ret_x1=tri->xy2.x;

						ret=tri;
						connected1=0;
					}
				}

				dx1=tri->xy2.x-x1;
				dz1=tri->xy2.z-z1;
				if (eq(dx0,dx1) && eq(dz0,dz1))
				{
					dist_temp=vec_dist(&tri0->cog,&tri->cog);
					if (dist_temp<dist)
					{
						dist=dist_temp;
						*ret_z0=tri->xy2.z;
						*ret_x0=tri->xy2.x;
						*ret_z1=tri->xy0.z;
						*ret_x1=tri->xy0.x;

						ret=tri;
						connected1=2;
					}
				}
			}else
			if (connected0==2)
			{
				dx1=tri->xy0.x-x1;
				dz1=tri->xy0.z-z1;
				if (eq(dx0,dx1) && eq(dz0,dz1))
				{
					dist_temp=vec_dist(&tri0->cog,&tri->cog);
					if (dist_temp<dist)
					{
						dist=dist_temp;
						*ret_z0=tri->xy0.z;
						*ret_x0=tri->xy0.x;
						*ret_z1=tri->xy1.z;
						*ret_x1=tri->xy1.x;

						ret=tri;
						connected1=0;
					}
				}

				dx1=tri->xy1.x-x1;
				dz1=tri->xy1.z-z1;
				if (eq(dx0,dx1) && eq(dz0,dz1))
				{
					dist_temp=vec_dist(&tri0->cog,&tri->cog);
					if (dist_temp<dist)
					{
						dist=dist_temp;
						*ret_z0=tri->xy1.z;
						*ret_x0=tri->xy1.x;
						*ret_z1=tri->xy0.z;
						*ret_x1=tri->xy0.x;

						ret=tri;
						connected1=1;
					}
				}
			}

		}

	}


	return ret;
}
