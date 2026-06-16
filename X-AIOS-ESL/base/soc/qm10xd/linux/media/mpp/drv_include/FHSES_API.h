/************************************************************************/
/*      Inc            Author           Date                            */
/*----------------------------------------------------------------------*/
/*    fullhan        benbiao.luo      2022.05.20                        */
/************************************************************************/


#ifndef FHSES_API_
#define FHSES_API_


/*
* 功能: 获取音频算法当前参数.
* 参数:
*       id : 音频算法索引，方便一个音频通路中进行调用多次音频算法，可取0，1，2，3，取值时需与FHSES_Process函数的id值相匹配
*       p : 音频算法参数指针，将当前算法使用的参数写入到该指针指向的结构体，获得当前算法参数
*       len :音频算法参数结构体 FH_AC_SesParam 所占字节数（len=sizeof(FH_AC_SesParam)）
* 返回值: 0表示成功,其他表示错误
*/
int FHSES_Get_Current_Param(int id, void *p, int len);


/*
* 功能: 设置音频算法参数
* 参数:
*       id : 音频算法索引，方便一个音频通路中进行调用多次音频算法，可取0，1，2，3，取值时需与FHSES_Process函数的id值相匹配
*       p : 音频算法输入参数指针。如果该指针为空，则使用算法内置默认参数
*       len :音频算法参数结构体 FH_AC_SesParam 所占字节数（len=sizeof(FH_AC_SesParam)）
* 返回值: 0表示成功,其他表示错误
*/
int FHSES_Set_Current_Param(int id, void *p, int len);


/*
* 功能: 音频算法处理.（每帧都需要调用）
* 参数:
*       id : 音频算法索引，方便一个音频通路中进行调用多次音频算法，可取0，1，2，3，取值时需与FHSES_Set_Current_Param函数的id值相匹配
*       sameple_rate:输入音频采样率
*       mic_1：近端麦克风1信号输入buffer
*       mic_2：近端麦克风2信号输入buffer，小于2个麦克风信号处理此处填NULL
*       mic_3：近端麦克风3信号输入buffer，小于3个麦克风信号处理此处填NULL
*       mic_4：近端麦克风4信号输入buffer，小于4个麦克风信号处理此处填NULL
*       ref_：远端参考信号输入buffer
*       out：音频算法输出buffer
*       degree：定位算法输出角度
*       samples : 每个buffer输入长度，16k采样每帧buffer长度为160，8k采样每帧buffer长度为80。samples的值应为处理长度的整数倍，如果处理的采样率不定建议取值160，320等
* 返回值: 0表示成功,其他表示错误
*/
int FHSES_Process(int id, int sameple_rate, short* mic_1, short* mic_2, short* mic_3,
	short* mic_4, short* ref_,short* out, int* degree, int samples);


/*
* 功能: 打印音频算法状态
* 参数: id : 音频算法索引
* 返回值: 0
*/
int FHSES_print(int id);


/*
* 功能: 音频算法版本（算法修改日期）
* 参数: 无
* 返回值: 当前算法版本号
*/
int FHSES_version(void);


/*
* 功能: 音频算法版本（打包版本）
* 参数: 无
* 返回值: 当前算法版本号（格式为a.b.c。a = (bb & 0xFF000000)>>24;b = (bb & 0x00FF0000)>>16;c = (bb & 0x0000FF00)>>8。其中bb为返回值）
*/
int FHSES_version2(void);

#endif