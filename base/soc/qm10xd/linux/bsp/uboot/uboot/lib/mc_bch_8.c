#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "mc_bch.h"
static int payload[10000];//MSB
static int payload_num;
uint8_t ecc_8[14];

static void byte_to_bit(struct image_info *info,const uint8_t * data,unsigned int len)
{
	uint8_t ch;
	int i,j;
	//printf("payload(%d-%d):",len,info->ctrl_num);
	for(i = 0;i<len;i++)
	{
		ch = *data;
		//printf("%02x",ch);
		payload[(len + info->ctrl_num-i-1)*8+7]=(ch>>7)%2;
		payload[(len + info->ctrl_num-i-1)*8+6]=(ch>>6)%2;
		payload[(len + info->ctrl_num-i-1)*8+5]=(ch>>5)%2;
		payload[(len + info->ctrl_num-i-1)*8+4]=(ch>>4)%2;
		payload[(len + info->ctrl_num-i-1)*8+3]=(ch>>3)%2;
		payload[(len + info->ctrl_num-i-1)*8+2]=(ch>>2)%2;
		payload[(len + info->ctrl_num-i-1)*8+1]=(ch>>1)%2;
		payload[(len + info->ctrl_num-i-1)*8+0]=(ch>>0)%2;
		data++;
	}
//	printf("ctrl_num=%d\r\n",info->ctrl_num);
	for(j=0;j<info->ctrl_num;j++){
		
		//printf("ff");
		payload[(info->ctrl_num -j-1)*8+7] = 1;
		payload[(info->ctrl_num -j-1)*8+6] = 1;
		payload[(info->ctrl_num -j-1)*8+5] = 1;
		payload[(info->ctrl_num -j-1)*8+4] = 1;
		payload[(info->ctrl_num -j-1)*8+3] = 1;
		payload[(info->ctrl_num -j-1)*8+2] = 1;
		payload[(info->ctrl_num -j-1)*8+1] = 1;
		payload[(info->ctrl_num -j-1)*8+0] = 1;
	}
	//printf("\n\t");
}
static void bit_to_byte(int *data,unsigned int len){
	uint8_t ch;
	int i;
	//printf("crc(%d):",(len>>3));
	for(i= 0;i<len>>3;i++){
		ch=data[i*8]|data[i*8+1]<<1|data[i*8+2]<<2|data[i*8+3]<<3|data[i*8+4]<<4|data[i*8+5]<<5|data[i*8+6]<<6|data[i*8+7]<<7;
		//printf("%02x",ch);
		ecc_8[(len>>3)-i-1] = ch;
//		printf("%d=%x ",(len>>3)-i-1,ecc_8[(len>>3)-i-1]);
	}
	//printf("\n\t");

}

void encode_bch_8(struct image_info *info,const uint8_t *data,
		unsigned int len,  char *ecc, int no_flag)
{
	int i ,j;
	int crc[112];
	int feedback[112];
//	printf("encode_bch_8\r\n");
#if 0
	if(debug_mode==1)
	{
		char ch1;
		int ch;
		for(i=payload_num;i;)
		{
			i-=4;
			data_file>>ch1;
			if(ch1>60)
				ch=ch1-87;
			else 
				ch=ch1-48;
			payload[i+3]=(ch/8)%2;
			payload[i+2]=(ch/4)%2;
			payload[i+1]=(ch/2)%2;
			payload[i+0]=(ch/1)%2;
		}
	}
#endif
	byte_to_bit(info,data,len);
	payload_num = (len+info->ctrl_num)*8;
	for(i=0;i<112;i++)
		crc[i]=0;

	for(i=payload_num;i;i-=4)
	{
		for(j=0;j<108;j++)
			feedback[j] = crc[j];
		for(j=0;j<4;j++)
			feedback[j+108] = crc[j+108]^payload[i+j-4]^no_flag;
			//feedback[j+108] = crc[j+108]^0^no_flag;

		crc[0]=feedback[108]^feedback[109]^feedback[110]^feedback[111];
		crc[1]=feedback[109]^feedback[110]^feedback[111];
		crc[2]=feedback[110]^feedback[111];
		crc[3]=feedback[108]^feedback[109]^feedback[110];
		crc[4]=feedback[109]^feedback[110]^feedback[111]^feedback[0];
		crc[5]=feedback[110]^feedback[111]^feedback[1];
		crc[6]=feedback[111]^feedback[2];
		crc[7]=feedback[3];
		crc[8]=feedback[4];
		crc[9]=feedback[108]^feedback[109]^feedback[110]^feedback[111]^feedback[5];
		crc[10]=feedback[108]^feedback[6];
		crc[11]=feedback[109]^feedback[7];
		crc[12]=feedback[110]^feedback[8];
		crc[13]=feedback[111]^feedback[9];
		crc[14]=feedback[108]^feedback[109]^feedback[110]^feedback[111]^feedback[10];
		crc[15]=feedback[109]^feedback[110]^feedback[111]^feedback[11];
		crc[16]=feedback[110]^feedback[111]^feedback[12];
		crc[17]=feedback[111]^feedback[13];
		crc[18]=feedback[14];
		crc[19]=feedback[108]^feedback[109]^feedback[110]^feedback[111]^feedback[15];
		crc[20]=feedback[108]^feedback[16];
		crc[21]=feedback[108]^feedback[110]^feedback[111]^feedback[17];
		crc[22]=feedback[108]^feedback[110]^feedback[18];
		crc[23]=feedback[108]^feedback[110]^feedback[19];
		crc[24]=feedback[108]^feedback[110]^feedback[20];
		crc[25]=feedback[109]^feedback[111]^feedback[21];
		crc[26]=feedback[108]^feedback[109]^feedback[111]^feedback[22];
		crc[27]=feedback[108]^feedback[111]^feedback[23];
		crc[28]=feedback[109]^feedback[24];
		crc[29]=feedback[108]^feedback[109]^feedback[111]^feedback[25];
		crc[30]=feedback[109]^feedback[110]^feedback[26];
		crc[31]=feedback[110]^feedback[111]^feedback[27];
		crc[32]=feedback[108]^feedback[109]^feedback[110]^feedback[28];
		crc[33]=feedback[109]^feedback[110]^feedback[111]^feedback[29];
		crc[34]=feedback[108]^feedback[109]^feedback[30];
		crc[35]=feedback[109]^feedback[110]^feedback[31];
		crc[36]=feedback[108]^feedback[109]^feedback[32];
		crc[37]=feedback[109]^feedback[110]^feedback[33];
		crc[38]=feedback[108]^feedback[109]^feedback[34];
		crc[39]=feedback[109]^feedback[110]^feedback[35];
		crc[40]=feedback[108]^feedback[109]^feedback[36];
		crc[41]=feedback[108]^feedback[111]^feedback[37];
		crc[42]=feedback[109]^feedback[38];
		crc[43]=feedback[110]^feedback[39];
		crc[44]=feedback[111]^feedback[40];
		crc[45]=feedback[108]^feedback[109]^feedback[110]^feedback[111]^feedback[41];
		crc[46]=feedback[108]^feedback[42];
		crc[47]=feedback[109]^feedback[43];
		crc[48]=feedback[110]^feedback[44];
		crc[49]=feedback[111]^feedback[45];
		crc[50]=feedback[108]^feedback[109]^feedback[110]^feedback[111]^feedback[46];
		crc[51]=feedback[109]^feedback[110]^feedback[111]^feedback[47];
		crc[52]=feedback[110]^feedback[111]^feedback[48];
		crc[53]=feedback[108]^feedback[109]^feedback[110]^feedback[49];
		crc[54]=feedback[109]^feedback[110]^feedback[111]^feedback[50];
		crc[55]=feedback[108]^feedback[109]^feedback[51];
		crc[56]=feedback[108]^feedback[111]^feedback[52];
		crc[57]=feedback[109]^feedback[53];
		crc[58]=feedback[108]^feedback[109]^feedback[111]^feedback[54];
		crc[59]=feedback[108]^feedback[111]^feedback[55];
		crc[60]=feedback[109]^feedback[56];
		crc[61]=feedback[108]^feedback[109]^feedback[111]^feedback[57];
		crc[62]=feedback[108]^feedback[111]^feedback[58];
		crc[63]=feedback[108]^feedback[110]^feedback[111]^feedback[59];
		crc[64]=feedback[108]^feedback[110]^feedback[60];
		crc[65]=feedback[109]^feedback[111]^feedback[61];
		crc[66]=feedback[108]^feedback[109]^feedback[111]^feedback[62];
		crc[67]=feedback[109]^feedback[110]^feedback[63];
		crc[68]=feedback[108]^feedback[109]^feedback[64];
		crc[69]=feedback[108]^feedback[111]^feedback[65];
		crc[70]=feedback[108]^feedback[110]^feedback[111]^feedback[66];
		crc[71]=feedback[109]^feedback[111]^feedback[67];
		crc[72]=feedback[110]^feedback[68];
		crc[73]=feedback[108]^feedback[109]^feedback[110]^feedback[69];
		crc[74]=feedback[108]^feedback[70];
		crc[75]=feedback[109]^feedback[71];
		crc[76]=feedback[108]^feedback[109]^feedback[111]^feedback[72];
		crc[77]=feedback[108]^feedback[111]^feedback[73];
		crc[78]=feedback[109]^feedback[74];
		crc[79]=feedback[110]^feedback[75];
		crc[80]=feedback[111]^feedback[76];
		crc[81]=feedback[108]^feedback[109]^feedback[110]^feedback[111]^feedback[77];
		crc[82]=feedback[109]^feedback[110]^feedback[111]^feedback[78];
		crc[83]=feedback[110]^feedback[111]^feedback[79];
		crc[84]=feedback[111]^feedback[80];
		crc[85]=feedback[108]^feedback[109]^feedback[110]^feedback[111]^feedback[81];
		crc[86]=feedback[108]^feedback[82];
		crc[87]=feedback[108]^feedback[110]^feedback[111]^feedback[83];
		crc[88]=feedback[109]^feedback[111]^feedback[84];
		crc[89]=feedback[108]^feedback[109]^feedback[111]^feedback[85];
		crc[90]=feedback[109]^feedback[110]^feedback[86];
		crc[91]=feedback[110]^feedback[111]^feedback[87];
		crc[92]=feedback[108]^feedback[109]^feedback[110]^feedback[88];
		crc[93]=feedback[109]^feedback[110]^feedback[111]^feedback[89];
		crc[94]=feedback[110]^feedback[111]^feedback[90];
		crc[95]=feedback[111]^feedback[91];
		crc[96]=feedback[92];
		crc[97]=feedback[108]^feedback[109]^feedback[110]^feedback[111]^feedback[93];
		crc[98]=feedback[108]^feedback[94];
		crc[99]=feedback[109]^feedback[95];
		crc[100]=feedback[108]^feedback[109]^feedback[111]^feedback[96];
		crc[101]=feedback[109]^feedback[110]^feedback[97];
		crc[102]=feedback[108]^feedback[109]^feedback[98];
		crc[103]=feedback[108]^feedback[111]^feedback[99];
		crc[104]=feedback[109]^feedback[100];
		crc[105]=feedback[108]^feedback[109]^feedback[111]^feedback[101];
		crc[106]=feedback[109]^feedback[110]^feedback[102];
		crc[107]=feedback[110]^feedback[111]^feedback[103];
		crc[108]=feedback[108]^feedback[109]^feedback[110]^feedback[104];
		crc[109]=feedback[109]^feedback[110]^feedback[111]^feedback[105];
		crc[110]=feedback[110]^feedback[111]^feedback[106];
		crc[111]=feedback[108]^feedback[109]^feedback[110]^feedback[107];
	}		
	for(i=0;i<112;i++)crc[i]=crc[i]^no_flag;
	bit_to_byte(crc,112);
	memcpy(ecc,&ecc_8,112>>3);
//	for(i=0;i<14;i++)
//		printf("%02x ",(unsigned char)*(ecc+i));
}
