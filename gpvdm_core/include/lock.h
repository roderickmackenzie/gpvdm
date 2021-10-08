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

//<stripall>

/** @file key.h
	@brief Header file for key.c
*/
#ifndef key_h
#define key_h
#include <sim_struct.h>
#include <lock_struct.h>


void lock_gather_info(struct simulation *sim);
int lock_decrypt(char *buf,long *len, char *key);
void lock_ecrypt(char **out,char *in,int *len_out,int len_in,char *key);
int lock_is_trial(struct lock *l);
int lock_is_expired(struct lock *l);
int get_file_from_web(char **buffer, const char * const szUrl);
void lock_register(struct simulation *sim,struct lock *l);
void url_encode(char *enc, char * name,unsigned char *in);
int lock_get_mac(char *out_mac);
void get_win_id(char * out_winid);
void reg_get_data(char * data,char *key);
void lock_write_data_to_reg(struct simulation *sim,char *key,char *data);
void lock_get_li(struct simulation *sim,struct lock *l);
void lock_validate_key(struct simulation *sim,struct lock *l,char *key);
void lock_update_license(struct simulation *sim,struct lock *l);
int lock_update_token(struct simulation *sim,struct lock *l,char *token,char *value);
void lock_check(struct simulation *sim,struct lock *l);
int lock_is_item_locked(struct simulation *sim, struct lock *l,char *test);
void lock_dump(struct simulation *sim,struct lock *l);
void lock_tx_uid_to_ui(struct simulation *sim,struct lock *l);
int lock_feature_enabled(struct simulation *sim);
int lock_update_key(struct simulation *sim,struct lock *l);
void lock_user_key_gen(struct simulation *sim,struct lock *l);
void lock_cal_answer(struct simulation *sim,char *out, char *in);
void stop_if_not_registered_and_gpvdm_next(struct simulation *sim);
#endif
