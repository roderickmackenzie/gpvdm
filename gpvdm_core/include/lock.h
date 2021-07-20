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
