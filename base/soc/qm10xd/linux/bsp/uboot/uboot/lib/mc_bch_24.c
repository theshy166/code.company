#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "mc_bch.h"
static int payload[10000];//MSB
static int payload_num;
uint8_t ecc_24[42];

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
	//printf("crc:");
	for(i= 0;i<len>>3;i++){
		ch=data[i*8]|data[i*8+1]<<1|data[i*8+2]<<2|data[i*8+3]<<3|data[i*8+4]<<4|data[i*8+5]<<5|data[i*8+6]<<6|data[i*8+7]<<7;
		ecc_24[(len>>3)-i-1] = ch;
		//printf("%x",ch);
	}
	//printf("\n\t");

}
void encode_bch_24(struct image_info *info,const uint8_t *data,
		unsigned int len,char *ecc,int no_flag)
{
	int i ,j;
	int crc[336];
	int feedback[336];

	//init payload  and ctrl byte
	byte_to_bit(info,data,len);

	payload_num = (len+info->ctrl_num)*8;

	for(i=0;i<336;i++)
		crc[i]=0;


	//printf("[%s] ---- payload_num=%d \n",__FUNCTION__, payload_num);
	//for(i=payload_num;i;i-=1)
	//	{
	//		payload[i]=1;
	//	}
	//payload[0]=0;
	for(i=payload_num;i;i-=4)
	{
		for(j=0;j<332;j++)
			feedback[j] = crc[j];
		for(j=0;j<4;j++)
			feedback[j+332] = crc[j+332]^payload[i+j-4]^no_flag;

		crc[0]=feedback[332]^feedback[333]^feedback[334]^feedback[335];
		crc[1]=feedback[333]^feedback[334]^feedback[335];
		crc[2]=feedback[332]^feedback[333];
		crc[3]=feedback[333]^feedback[334];
		crc[4]=feedback[332]^feedback[333]^feedback[0];
		crc[5]=feedback[332]^feedback[335]^feedback[1];
		crc[6]=feedback[333]^feedback[2];
		crc[7]=feedback[334]^feedback[3];
		crc[8]=feedback[332]^feedback[333]^feedback[334]^feedback[4];
		crc[9]=feedback[332]^feedback[5];
		crc[10]=feedback[333]^feedback[6];
		crc[11]=feedback[334]^feedback[7];
		crc[12]=feedback[332]^feedback[333]^feedback[334]^feedback[8];
		crc[13]=feedback[332]^feedback[9];
		crc[14]=feedback[332]^feedback[334]^feedback[335]^feedback[10];
		crc[15]=feedback[333]^feedback[335]^feedback[11];
		crc[16]=feedback[332]^feedback[333]^feedback[335]^feedback[12];
		crc[17]=feedback[332]^feedback[335]^feedback[13];
		crc[18]=feedback[332]^feedback[334]^feedback[335]^feedback[14];
		crc[19]=feedback[333]^feedback[335]^feedback[15];
		crc[20]=feedback[332]^feedback[333]^feedback[335]^feedback[16];
		crc[21]=feedback[332]^feedback[335]^feedback[17];
		crc[22]=feedback[333]^feedback[18];
		crc[23]=feedback[332]^feedback[333]^feedback[335]^feedback[19];
		crc[24]=feedback[332]^feedback[335]^feedback[20];
		crc[25]=feedback[333]^feedback[21];
		crc[26]=feedback[334]^feedback[22];
		crc[27]=feedback[332]^feedback[333]^feedback[334]^feedback[23];
		crc[28]=feedback[333]^feedback[334]^feedback[335]^feedback[24];
		crc[29]=feedback[334]^feedback[335]^feedback[25];
		crc[30]=feedback[332]^feedback[333]^feedback[334]^feedback[26];
		crc[31]=feedback[332]^feedback[27];
		crc[32]=feedback[332]^feedback[334]^feedback[335]^feedback[28];
		crc[33]=feedback[332]^feedback[334]^feedback[29];
		crc[34]=feedback[332]^feedback[334]^feedback[30];
		crc[35]=feedback[333]^feedback[335]^feedback[31];
		crc[36]=feedback[334]^feedback[32];
		crc[37]=feedback[332]^feedback[333]^feedback[334]^feedback[33];
		crc[38]=feedback[333]^feedback[334]^feedback[335]^feedback[34];
		crc[39]=feedback[332]^feedback[333]^feedback[35];
		crc[40]=feedback[333]^feedback[334]^feedback[36];
		crc[41]=feedback[332]^feedback[333]^feedback[37];
		crc[42]=feedback[332]^feedback[335]^feedback[38];
		crc[43]=feedback[333]^feedback[39];
		crc[44]=feedback[334]^feedback[40];
		crc[45]=feedback[335]^feedback[41];
		crc[46]=feedback[332]^feedback[333]^feedback[334]^feedback[335]^feedback[42];
		crc[47]=feedback[332]^feedback[43];
		crc[48]=feedback[332]^feedback[334]^feedback[335]^feedback[44];
		crc[49]=feedback[332]^feedback[334]^feedback[45];
		crc[50]=feedback[333]^feedback[335]^feedback[46];
		crc[51]=feedback[332]^feedback[333]^feedback[335]^feedback[47];
		crc[52]=feedback[332]^feedback[335]^feedback[48];
		crc[53]=feedback[333]^feedback[49];
		crc[54]=feedback[332]^feedback[333]^feedback[335]^feedback[50];
		crc[55]=feedback[333]^feedback[334]^feedback[51];
		crc[56]=feedback[332]^feedback[333]^feedback[52];
		crc[57]=feedback[333]^feedback[334]^feedback[53];
		crc[58]=feedback[334]^feedback[335]^feedback[54];
		crc[59]=feedback[335]^feedback[55];
		crc[60]=feedback[332]^feedback[333]^feedback[334]^feedback[335]^feedback[56];
		crc[61]=feedback[333]^feedback[334]^feedback[335]^feedback[57];
		crc[62]=feedback[332]^feedback[333]^feedback[58];
		crc[63]=feedback[332]^feedback[335]^feedback[59];
		crc[64]=feedback[333]^feedback[60];
		crc[65]=feedback[332]^feedback[333]^feedback[335]^feedback[61];
		crc[66]=feedback[332]^feedback[335]^feedback[62];
		crc[67]=feedback[332]^feedback[334]^feedback[335]^feedback[63];
		crc[68]=feedback[333]^feedback[335]^feedback[64];
		crc[69]=feedback[332]^feedback[333]^feedback[335]^feedback[65];
		crc[70]=feedback[332]^feedback[335]^feedback[66];
		crc[71]=feedback[333]^feedback[67];
		crc[72]=feedback[332]^feedback[333]^feedback[335]^feedback[68];
		crc[73]=feedback[333]^feedback[334]^feedback[69];
		crc[74]=feedback[334]^feedback[335]^feedback[70];
		crc[75]=feedback[332]^feedback[333]^feedback[334]^feedback[71];
		crc[76]=feedback[332]^feedback[72];
		crc[77]=feedback[333]^feedback[73];
		crc[78]=feedback[332]^feedback[333]^feedback[335]^feedback[74];
		crc[79]=feedback[332]^feedback[335]^feedback[75];
		crc[80]=feedback[332]^feedback[334]^feedback[335]^feedback[76];
		crc[81]=feedback[333]^feedback[335]^feedback[77];
		crc[82]=feedback[332]^feedback[333]^feedback[335]^feedback[78];
		crc[83]=feedback[333]^feedback[334]^feedback[79];
		crc[84]=feedback[332]^feedback[333]^feedback[80];
		crc[85]=feedback[333]^feedback[334]^feedback[81];
		crc[86]=feedback[332]^feedback[333]^feedback[82];
		crc[87]=feedback[333]^feedback[334]^feedback[83];
		crc[88]=feedback[332]^feedback[333]^feedback[84];
		crc[89]=feedback[333]^feedback[334]^feedback[85];
		crc[90]=feedback[332]^feedback[333]^feedback[86];
		crc[91]=feedback[332]^feedback[335]^feedback[87];
		crc[92]=feedback[333]^feedback[88];
		crc[93]=feedback[332]^feedback[333]^feedback[335]^feedback[89];
		crc[94]=feedback[333]^feedback[334]^feedback[90];
		crc[95]=feedback[334]^feedback[335]^feedback[91];
		crc[96]=feedback[332]^feedback[333]^feedback[334]^feedback[92];
		crc[97]=feedback[332]^feedback[93];
		crc[98]=feedback[332]^feedback[334]^feedback[335]^feedback[94];
		crc[99]=feedback[332]^feedback[334]^feedback[95];
		crc[100]=feedback[333]^feedback[335]^feedback[96];
		crc[101]=feedback[332]^feedback[333]^feedback[335]^feedback[97];
		crc[102]=feedback[333]^feedback[334]^feedback[98];
		crc[103]=feedback[334]^feedback[335]^feedback[99];
		crc[104]=feedback[335]^feedback[100];
		crc[105]=feedback[101];
		crc[106]=feedback[102];
		crc[107]=feedback[103];
		crc[108]=feedback[332]^feedback[333]^feedback[334]^feedback[335]^feedback[104];
		crc[109]=feedback[332]^feedback[105];
		crc[110]=feedback[333]^feedback[106];
		crc[111]=feedback[332]^feedback[333]^feedback[335]^feedback[107];
		crc[112]=feedback[332]^feedback[335]^feedback[108];
		crc[113]=feedback[332]^feedback[334]^feedback[335]^feedback[109];
		crc[114]=feedback[333]^feedback[335]^feedback[110];
		crc[115]=feedback[334]^feedback[111];
		crc[116]=feedback[332]^feedback[333]^feedback[334]^feedback[112];
		crc[117]=feedback[333]^feedback[334]^feedback[335]^feedback[113];
		crc[118]=feedback[334]^feedback[335]^feedback[114];
		crc[119]=feedback[332]^feedback[333]^feedback[334]^feedback[115];
		crc[120]=feedback[333]^feedback[334]^feedback[335]^feedback[116];
		crc[121]=feedback[334]^feedback[335]^feedback[117];
		crc[122]=feedback[335]^feedback[118];
		crc[123]=feedback[119];
		crc[124]=feedback[120];
		crc[125]=feedback[332]^feedback[333]^feedback[334]^feedback[335]^feedback[121];
		crc[126]=feedback[333]^feedback[334]^feedback[335]^feedback[122];
		crc[127]=feedback[332]^feedback[333]^feedback[123];
		crc[128]=feedback[332]^feedback[335]^feedback[124];
		crc[129]=feedback[333]^feedback[125];
		crc[130]=feedback[334]^feedback[126];
		crc[131]=feedback[335]^feedback[127];
		crc[132]=feedback[332]^feedback[333]^feedback[334]^feedback[335]^feedback[128];
		crc[133]=feedback[332]^feedback[129];
		crc[134]=feedback[332]^feedback[334]^feedback[335]^feedback[130];
		crc[135]=feedback[333]^feedback[335]^feedback[131];
		crc[136]=feedback[334]^feedback[132];
		crc[137]=feedback[332]^feedback[333]^feedback[334]^feedback[133];
		crc[138]=feedback[333]^feedback[334]^feedback[335]^feedback[134];
		crc[139]=feedback[334]^feedback[335]^feedback[135];
		crc[140]=feedback[332]^feedback[333]^feedback[334]^feedback[136];
		crc[141]=feedback[333]^feedback[334]^feedback[335]^feedback[137];
		crc[142]=feedback[332]^feedback[333]^feedback[138];
		crc[143]=feedback[333]^feedback[334]^feedback[139];
		crc[144]=feedback[334]^feedback[335]^feedback[140];
		crc[145]=feedback[335]^feedback[141];
		crc[146]=feedback[332]^feedback[333]^feedback[334]^feedback[335]^feedback[142];
		crc[147]=feedback[333]^feedback[334]^feedback[335]^feedback[143];
		crc[148]=feedback[334]^feedback[335]^feedback[144];
		crc[149]=feedback[335]^feedback[145];
		crc[150]=feedback[332]^feedback[333]^feedback[334]^feedback[335]^feedback[146];
		crc[151]=feedback[333]^feedback[334]^feedback[335]^feedback[147];
		crc[152]=feedback[332]^feedback[333]^feedback[148];
		crc[153]=feedback[332]^feedback[335]^feedback[149];
		crc[154]=feedback[332]^feedback[334]^feedback[335]^feedback[150];
		crc[155]=feedback[333]^feedback[335]^feedback[151];
		crc[156]=feedback[332]^feedback[333]^feedback[335]^feedback[152];
		crc[157]=feedback[333]^feedback[334]^feedback[153];
		crc[158]=feedback[332]^feedback[333]^feedback[154];
		crc[159]=feedback[332]^feedback[335]^feedback[155];
		crc[160]=feedback[333]^feedback[156];
		crc[161]=feedback[332]^feedback[333]^feedback[335]^feedback[157];
		crc[162]=feedback[333]^feedback[334]^feedback[158];
		crc[163]=feedback[332]^feedback[333]^feedback[159];
		crc[164]=feedback[333]^feedback[334]^feedback[160];
		crc[165]=feedback[332]^feedback[333]^feedback[161];
		crc[166]=feedback[333]^feedback[334]^feedback[162];
		crc[167]=feedback[334]^feedback[335]^feedback[163];
		crc[168]=feedback[335]^feedback[164];
		crc[169]=feedback[332]^feedback[333]^feedback[334]^feedback[335]^feedback[165];
		crc[170]=feedback[333]^feedback[334]^feedback[335]^feedback[166];
		crc[171]=feedback[334]^feedback[335]^feedback[167];
		crc[172]=feedback[335]^feedback[168];
		crc[173]=feedback[169];
		crc[174]=feedback[170];
		crc[175]=feedback[171];
		crc[176]=feedback[332]^feedback[333]^feedback[334]^feedback[335]^feedback[172];
		crc[177]=feedback[332]^feedback[173];
		crc[178]=feedback[332]^feedback[334]^feedback[335]^feedback[174];
		crc[179]=feedback[333]^feedback[335]^feedback[175];
		crc[180]=feedback[332]^feedback[333]^feedback[335]^feedback[176];
		crc[181]=feedback[332]^feedback[335]^feedback[177];
		crc[182]=feedback[332]^feedback[334]^feedback[335]^feedback[178];
		crc[183]=feedback[333]^feedback[335]^feedback[179];
		crc[184]=feedback[332]^feedback[333]^feedback[335]^feedback[180];
		crc[185]=feedback[333]^feedback[334]^feedback[181];
		crc[186]=feedback[332]^feedback[333]^feedback[182];
		crc[187]=feedback[333]^feedback[334]^feedback[183];
		crc[188]=feedback[332]^feedback[333]^feedback[184];
		crc[189]=feedback[333]^feedback[334]^feedback[185];
		crc[190]=feedback[332]^feedback[333]^feedback[186];
		crc[191]=feedback[332]^feedback[335]^feedback[187];
		crc[192]=feedback[333]^feedback[188];
		crc[193]=feedback[332]^feedback[333]^feedback[335]^feedback[189];
		crc[194]=feedback[332]^feedback[335]^feedback[190];
		crc[195]=feedback[333]^feedback[191];
		crc[196]=feedback[334]^feedback[192];
		crc[197]=feedback[335]^feedback[193];
		crc[198]=feedback[332]^feedback[333]^feedback[334]^feedback[335]^feedback[194];
		crc[199]=feedback[332]^feedback[195];
		crc[200]=feedback[332]^feedback[334]^feedback[335]^feedback[196];
		crc[201]=feedback[333]^feedback[335]^feedback[197];
		crc[202]=feedback[332]^feedback[333]^feedback[335]^feedback[198];
		crc[203]=feedback[333]^feedback[334]^feedback[199];
		crc[204]=feedback[334]^feedback[335]^feedback[200];
		crc[205]=feedback[335]^feedback[201];
		crc[206]=feedback[332]^feedback[333]^feedback[334]^feedback[335]^feedback[202];
		crc[207]=feedback[332]^feedback[203];
		crc[208]=feedback[333]^feedback[204];
		crc[209]=feedback[332]^feedback[333]^feedback[335]^feedback[205];
		crc[210]=feedback[332]^feedback[335]^feedback[206];
		crc[211]=feedback[333]^feedback[207];
		crc[212]=feedback[334]^feedback[208];
		crc[213]=feedback[332]^feedback[333]^feedback[334]^feedback[209];
		crc[214]=feedback[332]^feedback[210];
		crc[215]=feedback[332]^feedback[334]^feedback[335]^feedback[211];
		crc[216]=feedback[333]^feedback[335]^feedback[212];
		crc[217]=feedback[334]^feedback[213];
		crc[218]=feedback[335]^feedback[214];
		crc[219]=feedback[215];
		crc[220]=feedback[216];
		crc[221]=feedback[217];
		crc[222]=feedback[218];
		crc[223]=feedback[332]^feedback[333]^feedback[334]^feedback[335]^feedback[219];
		crc[224]=feedback[333]^feedback[334]^feedback[335]^feedback[220];
		crc[225]=feedback[332]^feedback[333]^feedback[221];
		crc[226]=feedback[333]^feedback[334]^feedback[222];
		crc[227]=feedback[332]^feedback[333]^feedback[223];
		crc[228]=feedback[332]^feedback[335]^feedback[224];
		crc[229]=feedback[333]^feedback[225];
		crc[230]=feedback[334]^feedback[226];
		crc[231]=feedback[332]^feedback[333]^feedback[334]^feedback[227];
		crc[232]=feedback[333]^feedback[334]^feedback[335]^feedback[228];
		crc[233]=feedback[334]^feedback[335]^feedback[229];
		crc[234]=feedback[335]^feedback[230];
		crc[235]=feedback[332]^feedback[333]^feedback[334]^feedback[335]^feedback[231];
		crc[236]=feedback[332]^feedback[232];
		crc[237]=feedback[333]^feedback[233];
		crc[238]=feedback[332]^feedback[333]^feedback[335]^feedback[234];
		crc[239]=feedback[333]^feedback[334]^feedback[235];
		crc[240]=feedback[332]^feedback[333]^feedback[236];
		crc[241]=feedback[332]^feedback[335]^feedback[237];
		crc[242]=feedback[333]^feedback[238];
		crc[243]=feedback[332]^feedback[333]^feedback[335]^feedback[239];
		crc[244]=feedback[332]^feedback[335]^feedback[240];
		crc[245]=feedback[332]^feedback[334]^feedback[335]^feedback[241];
		crc[246]=feedback[333]^feedback[335]^feedback[242];
		crc[247]=feedback[334]^feedback[243];
		crc[248]=feedback[335]^feedback[244];
		crc[249]=feedback[332]^feedback[333]^feedback[334]^feedback[335]^feedback[245];
		crc[250]=feedback[333]^feedback[334]^feedback[335]^feedback[246];
		crc[251]=feedback[334]^feedback[335]^feedback[247];
		crc[252]=feedback[335]^feedback[248];
		crc[253]=feedback[332]^feedback[333]^feedback[334]^feedback[335]^feedback[249];
		crc[254]=feedback[333]^feedback[334]^feedback[335]^feedback[250];
		crc[255]=feedback[334]^feedback[335]^feedback[251];
		crc[256]=feedback[332]^feedback[333]^feedback[334]^feedback[252];
		crc[257]=feedback[332]^feedback[253];
		crc[258]=feedback[332]^feedback[334]^feedback[335]^feedback[254];
		crc[259]=feedback[332]^feedback[334]^feedback[255];
		crc[260]=feedback[333]^feedback[335]^feedback[256];
		crc[261]=feedback[334]^feedback[257];
		crc[262]=feedback[332]^feedback[333]^feedback[334]^feedback[258];
		crc[263]=feedback[332]^feedback[259];
		crc[264]=feedback[333]^feedback[260];
		crc[265]=feedback[332]^feedback[333]^feedback[335]^feedback[261];
		crc[266]=feedback[333]^feedback[334]^feedback[262];
		crc[267]=feedback[332]^feedback[333]^feedback[263];
		crc[268]=feedback[332]^feedback[335]^feedback[264];
		crc[269]=feedback[332]^feedback[334]^feedback[335]^feedback[265];
		crc[270]=feedback[332]^feedback[334]^feedback[266];
		crc[271]=feedback[333]^feedback[335]^feedback[267];
		crc[272]=feedback[334]^feedback[268];
		crc[273]=feedback[332]^feedback[333]^feedback[334]^feedback[269];
		crc[274]=feedback[332]^feedback[270];
		crc[275]=feedback[333]^feedback[271];
		crc[276]=feedback[332]^feedback[333]^feedback[335]^feedback[272];
		crc[277]=feedback[332]^feedback[335]^feedback[273];
		crc[278]=feedback[332]^feedback[334]^feedback[335]^feedback[274];
		crc[279]=feedback[333]^feedback[335]^feedback[275];
		crc[280]=feedback[332]^feedback[333]^feedback[335]^feedback[276];
		crc[281]=feedback[332]^feedback[335]^feedback[277];
		crc[282]=feedback[333]^feedback[278];
		crc[283]=feedback[334]^feedback[279];
		crc[284]=feedback[332]^feedback[333]^feedback[334]^feedback[280];
		crc[285]=feedback[332]^feedback[281];
		crc[286]=feedback[333]^feedback[282];
		crc[287]=feedback[332]^feedback[333]^feedback[335]^feedback[283];
		crc[288]=feedback[332]^feedback[335]^feedback[284];
		crc[289]=feedback[332]^feedback[334]^feedback[335]^feedback[285];
		crc[290]=feedback[332]^feedback[334]^feedback[286];
		crc[291]=feedback[332]^feedback[334]^feedback[287];
		crc[292]=feedback[333]^feedback[335]^feedback[288];
		crc[293]=feedback[334]^feedback[289];
		crc[294]=feedback[332]^feedback[333]^feedback[334]^feedback[290];
		crc[295]=feedback[333]^feedback[334]^feedback[335]^feedback[291];
		crc[296]=feedback[332]^feedback[333]^feedback[292];
		crc[297]=feedback[333]^feedback[334]^feedback[293];
		crc[298]=feedback[332]^feedback[333]^feedback[294];
		crc[299]=feedback[332]^feedback[335]^feedback[295];
		crc[300]=feedback[332]^feedback[334]^feedback[335]^feedback[296];
		crc[301]=feedback[332]^feedback[334]^feedback[297];
		crc[302]=feedback[332]^feedback[334]^feedback[298];
		crc[303]=feedback[333]^feedback[335]^feedback[299];
		crc[304]=feedback[332]^feedback[333]^feedback[335]^feedback[300];
		crc[305]=feedback[333]^feedback[334]^feedback[301];
		crc[306]=feedback[334]^feedback[335]^feedback[302];
		crc[307]=feedback[332]^feedback[333]^feedback[334]^feedback[303];
		crc[308]=feedback[332]^feedback[304];
		crc[309]=feedback[332]^feedback[334]^feedback[335]^feedback[305];
		crc[310]=feedback[333]^feedback[335]^feedback[306];
		crc[311]=feedback[332]^feedback[333]^feedback[335]^feedback[307];
		crc[312]=feedback[333]^feedback[334]^feedback[308];
		crc[313]=feedback[334]^feedback[335]^feedback[309];
		crc[314]=feedback[332]^feedback[333]^feedback[334]^feedback[310];
		crc[315]=feedback[332]^feedback[311];
		crc[316]=feedback[333]^feedback[312];
		crc[317]=feedback[332]^feedback[333]^feedback[335]^feedback[313];
		crc[318]=feedback[333]^feedback[334]^feedback[314];
		crc[319]=feedback[334]^feedback[335]^feedback[315];
		crc[320]=feedback[332]^feedback[333]^feedback[334]^feedback[316];
		crc[321]=feedback[332]^feedback[317];
		crc[322]=feedback[333]^feedback[318];
		crc[323]=feedback[334]^feedback[319];
		crc[324]=feedback[332]^feedback[333]^feedback[334]^feedback[320];
		crc[325]=feedback[333]^feedback[334]^feedback[335]^feedback[321];
		crc[326]=feedback[334]^feedback[335]^feedback[322];
		crc[327]=feedback[335]^feedback[323];
		crc[328]=feedback[324];
		crc[329]=feedback[332]^feedback[333]^feedback[334]^feedback[335]^feedback[325];
		crc[330]=feedback[333]^feedback[334]^feedback[335]^feedback[326];
		crc[331]=feedback[334]^feedback[335]^feedback[327];
		crc[332]=feedback[335]^feedback[328];
		crc[333]=feedback[329];
		crc[334]=feedback[330];
		crc[335]=feedback[332]^feedback[333]^feedback[334]^feedback[335]^feedback[331];
	}
	for(i=0;i<336;i++)crc[i]=crc[i]^no_flag;
	bit_to_byte(crc,336);
	memcpy(ecc,&ecc_24,336>>3);
//	for(i=0;i<42;i++)
//		printf("%02x ",(unsigned char)*(ecc+i));
}
