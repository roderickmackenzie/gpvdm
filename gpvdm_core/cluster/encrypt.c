#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>

#include <pwd.h>
#include <openssl/md5.h>
#include <openssl/aes.h>
#include <openssl/sha.h>

#include "util.h"
#include "inp.h"

char key_text[100];
char iv_text[100];

void encrypt_load()
{
	struct inp_file inp;
	inp_init(&inp);
	if (inp_load(&inp,"crypto.inp")!=0)
	{
		printf("can't find file crypto.inp");
		exit(0);
	}

	inp_check(&inp,1.0);
	inp_search_string(&inp,iv_text,"#iv");
	inp_search_string(&inp,key_text,"#key");
	inp_free(&inp);
}

void encrypt(char *data,int round_len)
{
	return;
	char *out=malloc(sizeof(char)*round_len);

	int key_len=16;

	memset(out, 0, round_len);
	//memset(buf, 0, round_len);
	//memcpy (buf, in, len );

	unsigned char IV[key_len];
	unsigned char key[key_len];


	MD5((const unsigned char*)key_text, strlen(key_text),(unsigned char *)key);
	MD5((const unsigned char*)iv_text, strlen(iv_text),(unsigned char *)IV);

	AES_KEY aeskey;

	AES_set_encrypt_key(key, key_len*8, &aeskey);
	AES_cbc_encrypt( (unsigned char *)data,(unsigned char *)out, round_len, &aeskey, IV, AES_ENCRYPT);

	memcpy (data, out, round_len );
	//printf("decrypted: '%s'\n",data);
	free(out);


}


void decrypt(char *data,int round_len)
{
	return;
	char *out=malloc(sizeof(char)*round_len);

	int key_len=16;

	memset(out, 0, round_len);
	//memset(buf, 0, round_len);
	//memcpy (buf, in, len );

	unsigned char IV[key_len];
	unsigned char key[key_len];

	MD5((const unsigned char*)key_text, strlen(key_text),(unsigned char *)key);
	MD5((const unsigned char*)iv_text, strlen(iv_text),(unsigned char *)IV);

	AES_KEY aeskey;

	AES_set_decrypt_key(key, key_len*8, &aeskey);
	AES_cbc_encrypt((unsigned char*)data, (unsigned char*)out, round_len, &aeskey, IV, AES_DECRYPT);

	memcpy (data, out, round_len );
	//printf("decrypted: '%s'\n",data);
	free(out);


}

