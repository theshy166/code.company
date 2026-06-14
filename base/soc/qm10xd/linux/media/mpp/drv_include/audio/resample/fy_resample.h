#ifndef __FY_RESAMPLE_H
#define __FY_RESAMPLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "aio_drv_ioc.h"
#include "fy_ai_mpi.h"
#include "fh_debug.h"

struct resample_config {
	int chan_num;	/*Number of channels*/
	int rate_in;	/*Input sampling frequency*/
	int rate_out;	/*Output sampling frequency*/
	int level;		/*Resampling level, you can take [0,10], generally take 3 or 4, the higher the level, the better the effect, the greater the resource consumption*/
};


/* Function: Audio resampling initialization function */
void resample_init(struct resample_config * config);


/* Function: Audio resampling processing function */
/*
* src: address of the source data to be sample
* dst: address of the data after sample
* inlen: Length of source data before sample
* outlen: inlen times the ratio of the input and output sampling rates(example : in_rate=8k, out_rate=32k, outlen = inlen*(32/8k) = inlen*4)
*/
void resample(AUDIO_DEV AiDevId,int* src, int* dst, int inlen, int outlen);

#if 0
int demo()
{
#define IN_NUM 80
	FILE* fp_in = fopen("./8k.pcm", "rb");
	FILE* fp_out = fopen("./32k.pcm", "wb");
	struct resample_config config;
	unsigned short* in_buff = (unsigned short* )malloc(sizeof(unsigned short)*IN_NUM);
	/*src: 8k Single track, 16bit -> dst: 32k Dual track 32bit*/
	/*dts_data_volume = 16 * src_data_volume*/
	unsigned short* out_buff = (unsigned short* )malloc(sizeof(unsigned short)*IN_NUM*16);

	config.chan_num = 2;
	config.rate_in = 8000;
	config.rate_out = 32000;
	config.level = 4;
	int in_len=IN_NUM;
	int out_len = in_len*(config.rate_out/config.rate_in);
	resample_init(&config);

	while(in_len == fread(in_buff, 2, in_len, fp_in))
	{
		resample((int* )in_buff, (int* )out_buff, in_len, out_len);
		fwrite(out_buff, 2, 1280, fp_out);
	}
	fclose(fp_in);
	fclose(fp_out);
}
#endif

#endif
