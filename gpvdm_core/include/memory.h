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

/** @file memory.h
@brief allocate 3D memory
*/
#ifndef memory_h
#define memory_h

#include <device.h>

//zxy long double
void three_d_copy_gdouble(struct dimensions *dim, long double ***dst, long double ***out);
void zxy_mul_gdouble(struct dimensions *dim, gdouble ***src, gdouble val);
void zxy_div_gdouble(struct dimensions *dim, gdouble ***src, gdouble val);
void zxy_long_double_mul_by_zxy_long_double(struct dimensions *dim, gdouble ***a, gdouble ***b);
void zxy_long_double_div_by_zxy_long_double(struct dimensions *dim, gdouble ***a, gdouble ***b);
void cpy_zxy_long_double(struct dimensions *dim, long double * (***out), long double * (***in));

//2d zx long double
void malloc_zx_gdouble(struct dimensions *dim, gdouble * (**var));
void free_zx_gdouble(struct dimensions *dim, gdouble * (**var));
void cpy_zx_long_double(struct dimensions *dim, long double * (**out),long double * (**in));
void mem_set_zx_long_double(struct dimensions *dim, long double **data, long double val);
void mem_mul_zx_area(struct dimensions *dim, long double **data);
void mem_mul_zx_long_double(struct dimensions *dim, long double **data,long double val);
void mem_div_zx_long_double(struct dimensions *dim, long double **data,long double val);
void mem_zx_invert_long_double(struct dimensions *dim, long double **data);

//2d zy long double
void malloc_zy_long_double(struct dimensions *dim, long double * (**var));
void free_zy_long_double(struct dimensions *dim, long double * (**var));
void cpy_zy_long_double(struct dimensions *dim, long double * (**out), long double * (**in));

//2d zy int
void malloc_zy_int(struct dimensions *dim, int * (**var));
void free_zy_int(struct dimensions *dim, int *(**in_var));
void cpy_zy_int(struct dimensions *dim, int *(**out), int *(**in));

//srh bands
void free_srh_bands(struct dimensions *dim, gdouble * (**** in_var));
void malloc_srh_bands(struct dimensions *dim, gdouble * (****var));


//zxy_int
void malloc_zxy_int(struct dimensions *dim, int * (***var));
void free_zxy_int(struct dimensions *dim, int * (***var));
void dump_zxy_int(struct dimensions *dim, int ***var);
void cpy_zxy_int(struct dimensions *dim, int * (***out),int * (***in));

//zxy_p_object
void malloc_zxy_p_object(struct dimensions *dim, struct object * (****var));
void free_zxy_p_object(struct dimensions *dim, struct object * (****var));
void cpy_zxy_p_object(struct dimensions *dim, struct object * (****out), struct object * (****in));

//zx_layer_p_object
void malloc_zx_layer_p_object(struct dim_zx_epitaxy *dim, struct object * (****var));
void free_zx_layer_p_object(struct dim_zx_epitaxy *dim, struct object * (****var));
void cpy_zx_layer_p_object(struct dim_zx_epitaxy *dim, struct object * (****out), struct object * (****in));
//zx_int
void malloc_zx_int(struct dimensions *dim, int * (**var));
void free_zx_int(struct dimensions *dim, int *(**in_var));
void cpy_zx_int(struct dimensions *dim, int * (**out), int * (**in));

//3d opps
long double three_d_avg(struct device *in, gdouble ***src);
long double three_d_avg_raw(struct device *in, long double ***src);
long double three_d_integrate(struct dimensions *dim, long double ***src);
long double three_d_avg_fabsl(struct device *in, long double ***src);
void three_d_printf(struct dimensions *dim, long double ***src);
void three_d_sub_gdouble(struct dimensions *dim, gdouble ***var, gdouble ***sub);
void three_d_add_gdouble(struct dimensions *dim, gdouble ***var, gdouble ***add);
void three_d_interpolate_gdouble(long double ***out, long double ***in, struct dimensions *dim_out, struct dimensions *dim_in);
void three_d_quick_dump(char *file_name, long double ***in, struct dimensions *dim);
void three_d_interpolate_srh(long double ****out, long double ****in, struct dimensions *dim_out, struct dimensions *dim_in,int band);
void srh_quick_dump(char *file_name, long double ****in, struct dimensions *dim,int band);
void three_d_interpolate_srh2(long double ****out, long double ****in, struct dimensions *dim_out, struct dimensions *dim_in,int band);
long double zxy_min_gdouble(struct dimensions *dim, gdouble ***var);
long double zxy_max_gdouble(struct dimensions *dim, gdouble ***var);
long double zxy_sum_gdouble(struct dimensions *dim, long double ***src);
long double zx_y_max_gdouble(struct dimensions *dim, gdouble ***var,int y);



//zxy_long_double
void malloc_zxy_long_double(struct dimensions *dim, gdouble * (***var));
void free_zxy_long_double(struct dimensions *dim, gdouble * (***in_var));
void zxy_load_long_double(struct simulation *sim, struct dimensions *dim,long double * *** data,char *file_name);
void zx_y_quick_dump(char *file_name, long double ***in, struct dimensions *dim);
void zxy_set_gdouble(struct dimensions *dim, gdouble ***var, gdouble val);
void flip_zxy_long_double_y(struct simulation *sim, struct dimensions *dim,long double *** data);
long double interpolate_zxy_long_double(struct dimensions *dim, long double ***data,int z, int x, long double y_in);

//zx_epitaxy_int
void malloc_zx_epitaxy_int(struct dim_zx_epitaxy *dim, int * (***var));
void free_zx_epitaxy_int(struct dim_zx_epitaxy *dim, int *(***var));
void cpy_zx_epitaxy_int(struct dim_zx_epitaxy *dim, int * (***out),int * (***in));
void dump_zx_epitaxy_int(struct dim_zx_epitaxy *dim, int ***var);

//zxy_long_double_complex
void malloc_zxy_long_double_complex(struct dimensions *dim, long double complex * (***var));
void free_zxy_long_double_complex(struct dimensions *dim, long double complex * (***in_var));
void cpy_light_zxyl_long_double_complex(struct dimensions *dim, long double complex * (****out),long double complex * (****in));

//light_zxyl_long_double
void malloc_light_zxyl_long_double(struct dimensions *dim, long double * (****var));
void free_light_zxyl_long_double(struct dimensions *dim, long double * (****in_var));
void flip_light_zxyl_long_double_y(struct simulation *sim, struct dimensions *dim,long double **** data);
void div_light_zxyl_long_double(struct dimensions *dim, long double ****data,long double val);
void memset_light_zxyl_long_double(struct dimensions *dim, long double ****data,int val);
void memset_light_zxyl_long_double_y(struct dimensions *dim, long double ****data,int z, int x, int l,long double val);
void cpy_light_zxyl_long_double(struct dimensions *dim, long double * (****out), long double * (****in));

//light_zxyl_float
void malloc_light_zxyl_float(struct dimensions *dim, float * (****var));
void free_light_zxyl_float(struct dimensions *dim, float * (****in_var));
void cpy_light_zxyl_float(struct dimensions *dim, float * (****out), float * (****in));
void flip_light_zxyl_float_y(struct simulation *sim, struct dimensions *dim,float **** data);
void div_light_zxyl_float(struct dimensions *dim, float ****data,float val);
void memset_light_zxyl_float(struct dimensions *dim, float ****data,int val);
void memset_light_zxyl_float_y(struct dimensions *dim, float ****data,int z, int x, int l,float val);

//light_zxy_long_double
void malloc_light_zxy_long_double(struct dimensions *dim, long double * (***var));
void free_light_zxy_long_double(struct dimensions *dim, long double * (***in_var));
void cpy_light_zxy_long_double(struct dimensions *dim, long double * (***out), long double * (***in));
void flip_light_zxy_long_double_y(struct simulation *sim, struct dimensions *dim,long double *** data);
void memset_light_zxy_long_double(struct dimensions *dim, long double ***data,int val);
void div_light_zxy_long_double(struct dimensions *dim, long double ***data,long double val);
long double interpolate_light_zxy_long_double(struct dimensions *dim, long double ***data,int z, int x, long double y_in);
long double interpolate_light_zxy_long_double_intergral(struct dimensions *dim, long double ***data,int z, int x, long double y_start,long double y_stop);
void light_zxy_mul_long_double(struct simulation *sim, struct dimensions *dim,long double *** data,long double mul);

//light_l_long_double
void malloc_light_l_long_double(struct dimensions *dim, long double * (*var));
void free_light_l_long_double(struct dimensions *dim, long double * (*in_var));
long double intergrate_light_l_long_double(struct dimensions *dim, long double *var);
void cpy_light_l_long_double(struct dimensions *dim, long double * (*out), long double * (*in));

//light_zxyl_long_double_complex
void malloc_light_zxyl_long_double_complex(struct dimensions *dim, long double complex * (****var));
void free_light_zxyl_long_double_complex(struct dimensions *dim, long double complex * (****in_var));

//light_zxyl_float_complex
void malloc_light_zxyl_float_complex(struct dimensions *dim, float complex * (****var));
void free_light_zxyl_float_complex(struct dimensions *dim, float complex * (****in_var));
void cpy_light_zxyl_float_complex(struct dimensions *dim, float complex * (****out),float complex * (****in));

//light_zxy_p_object
void malloc_light_zxy_p_object(struct dimensions *dim, struct object * (****var));
void free_light_zxy_p_object(struct dimensions *dim, struct object * (****in_var));
void cpy_light_zxy_p_object(struct dimensions *dim, struct object * (****out),struct object * (****in));

// heat_zxy_long_double
void flip_heat_zxy_long_double_y(struct simulation *sim, struct dimensions *dim,long double *** data);
void memset_heat_zxy_long_double(struct dimensions *dim, long double ***data,int val);
void div_heat_zxy_long_double(struct dimensions *dim, long double ***data,long double val);
long double interpolate_heat_zxy_long_double(struct dimensions *dim, long double ***data,int z, int x, long double y_in);
long double intergrate_heat_zxy_long_double(struct dimensions *dim, long double ***data);
long double avg_heat_zxy_long_double(struct dimensions *dim, long double ***data);

//2d opps
void mem_set_zx_gdouble_from_zx_gdouble(struct dimensions *dim, gdouble **data_out, gdouble **data_in);
void mem_add_zx_gdouble_from_zx_gdouble(struct dimensions *dim, gdouble **data_out, gdouble **data_in);
void zx_copy_gdouble(struct dimensions *dim, gdouble **dst, gdouble **src);

void memory_flip_1d_long_double(long double *var,int len);
void memory_flip_1d_int(int *var,int len);

//srh opps
void malloc_srh_bands(struct dimensions *dim, gdouble * (****var));
void cpy_srh_long_double(struct dimensions *dim, long double *(****dst), long double *(****src));

//matrix
void matrix_init(struct matrix *mx);
void matrix_dump(struct simulation *sim,struct matrix *mx);
void matrix_dump_to_file(struct simulation *sim,struct matrix *mx,char *file_name);
void matrix_malloc(struct simulation *sim,struct matrix *mx);
void matrix_free(struct simulation *sim,struct matrix *mx);
void matrix_cpy(struct simulation *sim,struct matrix *out,struct matrix *in);
void matrix_realloc(struct simulation *sim,struct matrix *mx);
int matrix_solve(struct simulation *sim,struct matrix_solver_memory *msm,struct matrix *mx);
void matrix_cache_reset(struct simulation *sim,struct matrix *mx);
void matrix_save(struct simulation *sim,struct matrix *mx);
int matrix_load(struct simulation *sim,struct matrix *mx);
long double matrix_cal_error(struct simulation *sim,struct matrix *mx);
void matrix_zero_b(struct simulation *sim,struct matrix *mx);
void matrix_dump_b(struct simulation *sim,struct matrix *mx);
void matrix_dump_J(struct simulation *sim,struct matrix *mx);
void matrix_add_nz_item(struct simulation *sim,struct matrix *mx,int x,int y,long double val);
void matrix_convert_J_to_sparse(struct simulation *sim,struct matrix *mx);
void matrix_stats(struct simulation *sim,struct matrix *mx);
int matrix_cmp_to_file(struct simulation *sim,struct matrix *mx,char *file_name);
void matrix_load_from_file(struct simulation *sim,struct matrix *mx,char *file_name);

//raw memory opps
int search(long double *x,int N,long double find);

//Basic memory opps

//2d long double
void malloc_2d_long_double(int zlen, int xlen, long double * (**var));
void free_2d_long_double(int zlen, int xlen, long double * (**in_var));
void cpy_2d_long_double(int zlen, int xlen, long double * (**out), long double * (**in));

//4d long double
void free_4d_long_double(int zlen, int xlen, int ylen,int bands, long double *(**** in_var));
void cpy_4d_long_double(int zlen, int xlen, int ylen,int bands, long double *(****dst), long double *(****src));
void malloc_4d_long_double(int zlen, int xlen, int ylen,int bands, long double * (****var));

//3d int
void malloc_3d_int(int zlen, int xlen, int ylen, int * (***var));
void free_3d_int(int zlen, int xlen, int ylen, int * (***in_var));
void cpy_3d_int(int zlen, int xlen, int ylen, int * (***out), int * (***in));

//matrix solver memory
void matrix_solver_memory_init(struct matrix_solver_memory *msm);

//generic
//3d
void malloc_3d( void * (***var),int zlen, int xlen, int ylen,int item_size);
void free_3d(void * (***in_var),int zlen, int xlen, int ylen,int item_size);
void cpy_3d(void * (***out), void * (***in),int zlen, int xlen, int ylen,int item_size);
void cpy_3d_alloc(void * (***out), void * (***in),int zlen, int xlen, int ylen,int item_size);

//4d
void free_4d( void *(**** in_var), int zlen, int xlen, int ylen,int bands,int item_size);
void cpy_4d( void *(****dst), void *(****src),int zlen, int xlen, int ylen,int bands,int item_size);
void malloc_4d( void * (****var), int zlen, int xlen, int ylen,int bands,int item_size);

//2d
void malloc_2d(void * (**var), int zlen, int xlen, int item_size);
void free_2d(void *(**in_var), int zlen, int xlen, int item_size);
void cpy_2d(void * (**out), void * (**in),int zlen, int xlen,  int item_size);
void cpy_2d_alloc(void * (**out), void * (**in),int zlen, int xlen, int item_size);

//1d
void malloc_1d(void * (*var),int zlen, int item_size);
void free_1d( void * (*in_var), int item_size);
void cpy_1d(void * (*out), void * (*in), int zlen, int item_size);
void cpy_1d_alloc(void * (*out), void * (*in), int zlen, int item_size);


// light_zxyl_double
void malloc_light_zxyl_double(struct dimensions *dim, double * (****var));
void free_light_zxyl_double(struct dimensions *dim, double * (****in_var));
void cpy_light_zxyl_double(struct dimensions *dim, double * (****out), double * (****in));
void cpy_light_zxyl_wavelength_double(struct dimensions *dim, double ****out, double ****in,int l);
void flip_light_zxyl_double_y(struct simulation *sim, struct dimensions *dim,double **** data);
void div_light_zxyl_double(struct dimensions *dim, double ****data,double val);
void memset_light_zxyl_double(struct dimensions *dim, double ****data,int val);
void memset_light_zxyl_double_y(struct dimensions *dim, double ****data,int z, int x, int l,double val);

//light_zxy_float
void malloc_light_zxy_float(struct dimensions *dim, float * (***var));
void free_light_zxy_float(struct dimensions *dim, float * (***var));
void cpy_light_zxy_float(struct dimensions *dim, float * (***out), float * (***in));
void cpy_light_zxy_float_no_alloc(struct dimensions *dim, float * (***out), float * (***in));
void light_zxy_mul_float(struct simulation *sim, struct dimensions *dim,float *** data,float mul);
void memset_light_zxy_float(struct dimensions *dim, float ***data,int val);
void div_light_zxy_float(struct dimensions *dim, float ***data,float val);


#endif
