/**
 * Copyright (c) 2015-2019 Shanghai Fullhan Microelectronics Co., Ltd.
 *
 * 定义标准化模块的对外接口
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-01-17     chenjn       the first version
 *
 */
#ifndef __FH_STD_API_H__
#define __FH_STD_API_H__
#include "fh_std_typedef.h"
#include "fh_std_error.h"

// GCC 本身不支持#pragma region,此处作用为屏蔽掉相关报错
#ifdef __GNUC__
	#if ((__GNUC__ * 100) + __GNUC_MINOR__) < 406
	#pragma GCC diagnostic ignored "-Wunknown-pragmas"
	#else
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wunknown-pragmas"
	#endif
#endif

/* 追踪模式
- FH_TRACE_DEBUG:支持内存追踪,DEBUG时使用
- FH_TRACE_MINI:最简模式,不增加c代码,只有头文件应用。打印ERR以上错误。适用于快速启动等在意代码大小模式
- 默认模式:不支持内存追踪,包含ZONE和LEVEL的打印控制
*/
// #define FH_TRACE_MINI
// #define FH_TRACE_DEBUG

// 使用此模块需要定义一些宏,请确保此宏只在本模块生效避免引发重定义
// #define _FHK_OBJ_ZONE FHK_STD_ZONE_OS
// #define _FHK_OBJ_ZONE FHK_STD_MODID2ZONE(FHK_STD_MOD_NONE)
#ifndef _FHK_OBJ_ZONE
#define _FHK_OBJ_ZONE FHK_STD_MODID2ZONE(FHK_STD_MOD_NONE)
#warning "Need to define _FHK_OBJ_ZONE when include fh_std_api.h"
#endif

#if defined(_FHK_API_LAYER)
	#include "fh_std_osal_usr.h"
	#include "fh_std_osal_at.h"

	#ifdef OS_LINUX
	#define fhstd_printf    printf
	#define fhstd_snprintf  snprintf
	#else
	#define fhstd_printf    rt_kprintf
	#define fhstd_snprintf  rt_snprintf
	#endif

	#if defined(FH_TRACE_DEBUG)
		#ifdef OS_LINUX
		#define fh_api_mtrace(lev,zone,...)  do{ \
				FH_CHAR _TMP_MSG[250]; \
				DEV_FD _tmp_fd; \
				FH_UINT32 _tmp_size_ = 0; \
				_tmp_fd = dev_open("/proc/driver/trace",O_RDWR,0); \
				if(dev_islegal(_tmp_fd)) { \
					_tmp_size_ += fhstd_snprintf(&_TMP_MSG[_tmp_size_],249 - _tmp_size_,"msg:%d:%d:[API]",lev,zone); \
					_tmp_size_ += fhstd_snprintf(&_TMP_MSG[_tmp_size_],249 - _tmp_size_,__VA_ARGS__); \
					dev_write(_tmp_fd,_TMP_MSG,_tmp_size_); \
					dev_close(_tmp_fd); \
				} \
			}while(0)
		#define fh_api_timing(name,tag) do{ \
				FH_CHAR _TMP_MSG[250]; \
				DEV_FD _tmp_fd; \
				FH_UINT32 _tmp_size_ = 0; \
				_tmp_fd = dev_open("/proc/driver/trace",O_RDWR,0); \
				if(dev_islegal(_tmp_fd)) { \
					_tmp_size_ += fhstd_snprintf(&_TMP_MSG[_tmp_size_],249 - _tmp_size_,"timing_%s_%s",name,tag); \
					dev_write(_tmp_fd,_TMP_MSG,_tmp_size_); \
					dev_close(_tmp_fd); \
				} \
			}while(0)
		#else
		extern int media_trace_write_proc(char *s);
		#define fh_api_mtrace(lev,zone,...)  do{ \
				FH_CHAR _TMP_MSG[250]; \
				FH_UINT32 _tmp_size_ = 0; \
				_tmp_size_ += fhstd_snprintf(&_TMP_MSG[_tmp_size_],249 - _tmp_size_,"msg:%d:%d:[API]",lev,zone); \
				_tmp_size_ += fhstd_snprintf(&_TMP_MSG[_tmp_size_],249 - _tmp_size_,__VA_ARGS__); \
			}while(0)
		#define fh_api_timing(name,tag) do{ \
				FH_CHAR _TMP_MSG[250]; \
				FH_UINT32 _tmp_size_ = 0; \
				_tmp_size_ += fhstd_snprintf(&_TMP_MSG[_tmp_size_],249 - _tmp_size_,"timing_%s_%s",name,tag); \
			}while(0)
		#endif
	#else
		#define fh_api_mtrace(lev,zone,...)
		#define fh_api_timing(name,tag)
	#endif

	#define FHK_MTRACE_CREATE(base,size)
	#define FHK_MTRACE_DESTROY()
	#define FHK_MTRACE_DISPLAY(sfile)
	#define FHK_MTRACE(lev,zone,...)      fh_api_mtrace(lev,zone,__VA_ARGS__)
	#define FHK_API_TIMING(name,tag)      fh_api_timing(name,tag)
	#define FHK_PRT(lev,zone,...) \
	do{ \
		if(lev >= FHK_STD_LEV_WARN){ \
			fhstd_printf(__VA_ARGS__); \
		} \
	}while(0)


	#define FHK_STD_ERR(mod, lev, id) FHK_STD_API_ERR(mod, lev, id)
#else //_FHK_DRV_LAYER
	#include "fh_std_osal.h"
	#include "fh_std_osal_at.h"
	#if defined(FH_TRACE_DEBUG)
		extern FH_SINT32 fhk_mtrace_create(FH_UINT32 base,FH_UINT32 size);
		extern FH_SINT32 fhk_mtrace_destroy(void);
		extern FH_SINT32 fhk_trace(FH_UINT32 lev,FH_UINT32 zone,const char *fmt,...);
		extern FH_SINT32 fhk_mtrace(FH_UINT32 lev,FH_UINT32 zone,const FH_CHAR *fmt,...);
		extern FH_SINT32 fhk_mtrace_display(fh_proc *sfile);
		extern FH_SINT32 fhk_mtrace_display_clear_en(FH_UINT32 enable);
		extern FH_SINT32 fhk_mtrace_limit_display_size(FH_UINT32 maxsize);
		#define FHK_MTRACE_CREATE(base,size)         fhk_mtrace_create(base,size)
		#define FHK_MTRACE_DESTROY()                 fhk_mtrace_destroy()
		#define FHK_MTRACE_DISPLAY(sfile)            fhk_mtrace_display(sfile)
		#define FHK_MTRACE(lev,zone,...)             fhk_mtrace(lev,zone,__VA_ARGS__)
		#define FHK_DISP_CLEAR(enable)               fhk_mtrace_display_clear_en(enable)
		#define FHK_DISP_LIMIT(size)                 fhk_mtrace_limit_display_size(size)
		#define FHK_PRT(lev,zone,...)                fhk_trace(lev,zone,__VA_ARGS__)
	#elif defined(FH_TRACE_MINI)
		#define FHK_MTRACE_CREATE(base,size)
		#define FHK_MTRACE_DESTROY()
		#define FHK_MTRACE_DISPLAY(sfile)
		#define FHK_MTRACE(lev,zone,...)
		#define FHK_DISP_CLEAR(enable)
		#define FHK_DISP_LIMIT(size)
		#define FHK_PRT(lev,zone,...) \
		do{ \
			if(lev >= FHK_STD_LEV_WARN){ \
				fh_printk(__VA_ARGS__); \
			} \
		}while(0)
	#else
		extern FH_SINT32 fhk_trace(FH_UINT32 lev,FH_UINT32 zone,
			const char *fmt,...);
		#define FHK_MTRACE_CREATE(base,size)
		#define FHK_MTRACE_DESTROY()
		#define FHK_MTRACE_DISPLAY(sfile)
		#define FHK_MTRACE(lev,zone,...)
		#define FHK_DISP_CLEAR(enable)
		#define FHK_DISP_LIMIT(size)
		#define FHK_PRT(lev,zone,...)                fhk_trace(lev,zone,__VA_ARGS__)
	#endif

	#define FHK_STD_ERR(mod, lev, id) FHK_STD_DRV_ERR(mod, lev, id)
#endif

/* PROC调试控制接口　*/
#if !defined(FH_TRACE_MINI) && !defined(_FHK_API_LAYER)
extern FH_SINT32 fhk_tracecoloren(FH_UINT32 enable);
extern FH_UINT32 fhk_trace_zone_mask(FH_UINT32 mask);
extern FH_UINT32 fhk_trace_mod(FH_UINT32 mod_id,FH_UINT8 prtlev,FH_UINT8 mprtlev);
extern FH_UINT32 fhk_trace_mod_all(FH_UINT8 prtlev,FH_UINT8 mprtlev);
extern FH_UINT32 fhk_trace_zone(FH_UINT8 prtlev,FH_UINT8 mprtlev);
extern FH_SINT32 fhk_trace_info(FH_VOID);
#define FHK_COLOR(enable)                  fhk_tracecoloren(enable)
#define FHK_ZONE_MASK(mask)                fhk_trace_zone_mask(mask)
#define FHK_MOD_LEV(mod_id,lev,mlev)       fhk_trace_mod(mod_id,lev,mlev)
#define FHK_MOD_LEV_ALL(lev,mlev)          fhk_trace_mod_all(lev,mlev)
#define FHK_ZONE_LEV(lev,mlev)             fhk_trace_zone(lev,mlev)
#define FHK_STD_INFO()                     fhk_trace_info()
#else
#define FHK_COLOR(enable)
#define FHK_ZONE_MASK(mask)
#define FHK_MOD_LEV(mod_id,lev,mlev)
#define FHK_MOD_LEV_ALL(lev,mlev)
#define FHK_ZONE_LEV(lev,mlev)
#define FHK_STD_INFO()
#endif

// 常用宏定义
#pragma region FHK_STD_COMMON_DEFINE

/* 对齐,align非2的N次方　*/
#define FHK_ALIGN_NP2(n,align)   (((n) + (align) - 1) - (((n) + (align) - 1) % (align)))
/* 对齐,align为2的N次方　*/
#define FHK_ALIGN(n,align)       (((n) + ((align) - 1)) & ~((align) - 1))
/* 去掉非对齐部分,align为2的N次方　*/
#define FHK_ALIGN_BASE(n,align)  ((n) & ~((align) - 1))
/* 转换为以unit为单位，不足部分会补齐 */
#define FHK_TRAN_UNIT(size,unit) (((size) + (unit) - 1) / (unit))
/* 最大值　*/
#define FHK_MAX(a,b)             ((a) > (b) ? (a) : (b))
/* 最小值　*/
#define FHK_MIN(a,b)             ((a) < (b) ? (a) : (b))
/* 绝对值　*/
#define FHK_ABS(x)               ((x) > 0 ? (x) : -(x))
/* 判断是否在范围内　*/
#define FHK_JUDGE(data,min,max)  ((data) >= (min) ? ((data) <= (max) ? 1 : 0) : 0)
/* 限制变量到指定范围　*/
#define FHK_CLIP(data,min,max)   ((data) >= (min) ? ((data) <= (max) ? (data) : (max)) : (min))
/* 减后绝对值　*/
#define FHK_MINUS_ABS(a,b)       ((a) > (b) ? (a) - (b) : (b) - (a))
/* 计算对应的偏移地址　*/
#define FHK_CALC_OFFSET(base,vbase,curbase) \
	(curbase - base + vbase)
/* 32位PTS的差,考虑U32溢出的情况*/
#define FHK_U32PTS_INTERVAL(_new,_old) \
	({ \
		FH_UINT32 _new_ = (FH_UINT32)(_new); \
		FH_UINT32 _old_ = (FH_UINT32)(_old); \
		FH_UINT32 _diff_= ((_new_) >= (_old_) ? ((_new_) - (_old_)) : (((FH_UINT32)0xffffffff) - (_old_) + (_new_))); \
		_diff_; \
	})
/* 对需要翻转变量加减1 */
#define FHK_FORWARD(_idx_,_len_)     (((_idx_)+1)%(_len_))
#define FHK_BACKWARD(_idx_,_len_)    (((_idx_)+(_len_)-1)%(_len_))
#define FHK_GO_FORWARD(_idx_,_len_)  _idx_=FHK_FORWARD(_idx_,_len_)
#define FHK_GO_BACKWARD(_idx_,_len_) _idx_=FHK_BACKWARD(_idx_,_len_)

/* 获取帧率中帧数和帧率 */
#define FHK_FPS_FRMTIME(fps)         (((fps) >> 16) & 0xffff)
#define FHK_FPS_FRMCOUNT(fps)        ((fps) & 0xffff)

#pragma endregion

// 追踪模块
#pragma region FHK_STD_TRACE

#define FHK_PRT_LF                            "\n"
#define FHK_MTRACE_N(lev,zone,...)            FHK_MTRACE(lev,zone,__VA_ARGS__)
#define FHK_PRT_N(lev,zone,...)               FHK_PRT(lev,zone,__VA_ARGS__)
#define FHK_STD_MPRT(lev,zone,msg,...)        FHK_MTRACE(lev,zone,"%s(%d): " msg,__FUNCTION__,__LINE__,##__VA_ARGS__)
#define FHK_STD_PRT(lev,zone,msg,...)         FHK_PRT(lev,zone,"%s(%d): " msg,__FUNCTION__,__LINE__,##__VA_ARGS__)
#define FHK_STD_LOG(lev,zone,msg,...) \
do{ \
	FHK_STD_MPRT(lev,zone,msg,##__VA_ARGS__); \
	FHK_STD_PRT(lev,zone,msg,##__VA_ARGS__); \
}while(0)

#define FHK_STD_LOG_DEBG(msg,...) FHK_STD_LOG(FHK_STD_LEV_DEBG,_FHK_OBJ_ZONE,msg,##__VA_ARGS__)
#define FHK_STD_LOG_INFO(msg,...) FHK_STD_LOG(FHK_STD_LEV_INFO,_FHK_OBJ_ZONE,msg,##__VA_ARGS__)
#define FHK_STD_LOG_NOTC(msg,...) FHK_STD_LOG(FHK_STD_LEV_NOTC,_FHK_OBJ_ZONE,msg,##__VA_ARGS__)
#define FHK_STD_LOG_WARN(msg,...) FHK_STD_LOG(FHK_STD_LEV_WARN,_FHK_OBJ_ZONE,msg,##__VA_ARGS__)
#define FHK_STD_LOG_ERRR(msg,...) FHK_STD_LOG(FHK_STD_LEV_ERRR,_FHK_OBJ_ZONE,msg,##__VA_ARGS__)

/* 函数进入退出追踪　*/
#define FHK_STD_HEADER() \
	FHK_STD_LOG(FHK_STD_LEV_DEBG,FHK_STD_ZONE_CODETRC,"++\n")
#define FHK_STD_HEADER_MSG(msg,...) \
	FHK_STD_LOG(FHK_STD_LEV_DEBG,FHK_STD_ZONE_CODETRC,"++" msg,##__VA_ARGS__)
#define FHK_STD_FOOTER() \
	FHK_STD_LOG(FHK_STD_LEV_DEBG,FHK_STD_ZONE_CODETRC,"--\n")
#define FHK_STD_FOOTER_MSG(msg,...) \
	FHK_STD_MPRT(FHK_STD_LEV_DEBG,FHK_STD_ZONE_CODETRC,"--" msg,##__VA_ARGS__)

/* DEBUG追踪打印 */
#define FHK_DBG_TRACE()                       FHK_STD_PRT(FHK_STD_LEV_WARN,_FHK_OBJ_ZONE,"Trace\n")
#define FHK_DBG_TRACE_NP()                    FHK_STD_PRT(FHK_STD_LEV_DEBG,_FHK_OBJ_ZONE,"Trace\n")

/* 各模块完成等时序追踪 */
extern FH_UINT64 get_pts64(void);
#define FHK_TIMING_TRACE(name,tag)            FHK_MTRACE_N(FHK_STD_LEV_DEBG,FHK_STD_ZONE_TIMING,"[vcd]%llu,%s,%s\n",get_pts64(),name,tag)
#define FHK_TIMING_TRACE_USR(name,tag)        FHK_MTRACE_N(FHK_STD_LEV_DEBG,FHK_STD_ZONE_TIMING,"[vcd]%llu,USR_%s,%s\n",get_pts64(),name,tag)
#define FHK_TIMING_TRACE_IDX(name,tag,idx)    FHK_MTRACE_N(FHK_STD_LEV_DEBG,FHK_STD_ZONE_TIMING,"[vcd]%llu,%s_BLK%d,%s\n",get_pts64(),name,idx,tag)

/* 各模块内部内存细分追踪 */
#define FHK_MEM_TRACE(name,start,end)         FHK_STD_LOG(FHK_STD_LEV_DEBG,FHK_STD_ZONE_MEMORY,"[MEM_LOG] 0x%08x-0x%08x %s(%d KB)\n",start,end,name,(end - start)/1024)
#define FHK_MEM_TRACE_EX(name,mem)            FHK_MEM_TRACE(name,mem.base,mem.base + mem.size)
#pragma endregion

// 参数检查
#pragma region FHK_STD_CHECK

/* 参数检查宏定义
 * - FHK_CHK_XXXX         自定义信息级别和ZONE,同时打印内存可以附加自定义内容。
 * - FHK_STD_CHK_XXXX     使用模块定义_FHK_OBJ_ZONE,错误基本为FHK_STD_LEV_DEBG
 * - FHK_STD_CHK_XXXX_API 使用模块定义_FHK_OBJ_ZONE,错误基本为FHK_STD_LEV_ERRR
 * - FHK_STD_CHK_XXXX_MSG 使用模块定义_FHK_OBJ_ZONE,错误基本为FHK_STD_LEV_ERRR,同时打印内存可以附加自定义内容。
 */

/* 检测参数值的范围,S32 */
#define FHK_CHK_RANGE(level,zone,name,param,min,max,msg,...) \
	do{ \
		FH_SINT32 _TParam = (param); \
		if(!(FHK_JUDGE(param,min,max))) \
		{ \
			FHK_STD_LOG(level,zone,"[INVALID_PARAM]%s:%d,need:[%d~%d]." msg,name,_TParam,min,max,##__VA_ARGS__); \
			return FHK_STD_ERR(FHK_STD_ZONE2MODID(zone),level,FHER_STD_ILLEGAL_PARAM); \
		} \
	}while(0)
#define FHK_STD_CHK_RANGE(name,param,min,max)                 FHK_CHK_RANGE(FHK_STD_LEV_NOTC,_FHK_OBJ_ZONE,name,param,min,max,FHK_PRT_LF)
#define FHK_STD_CHK_RANGE_NP(name,param,min,max)              FHK_CHK_RANGE(FHK_STD_LEV_DEBG,_FHK_OBJ_ZONE,name,param,min,max,FHK_PRT_LF)
#define FHK_STD_CHK_RANGE_API(name,param,min,max)             FHK_CHK_RANGE(FHK_STD_LEV_ERRR,_FHK_OBJ_ZONE,name,param,min,max,FHK_PRT_LF)
#define FHK_STD_CHK_RANGE_MSG(name,param,min,max,msg,...)     FHK_CHK_RANGE(FHK_STD_LEV_ERRR,_FHK_OBJ_ZONE,name,param,min,max,msg,##__VA_ARGS__)

/* 检测参数值的范围,U32 */
#define FHK_CHK_RANGE_U32(level,zone,name,param,min,max,msg,...) \
	do{ \
		FH_UINT32 _TParam = (param); \
		if(!(FHK_JUDGE(_TParam,min,max))) \
		{ \
			FHK_STD_LOG(level,zone,"[INVALID_PARAM]%s:%u,need:[%u~%u]." msg,name,_TParam,min,max,##__VA_ARGS__); \
			return FHK_STD_ERR(FHK_STD_ZONE2MODID(zone),level,FHER_STD_ILLEGAL_PARAM); \
		} \
	}while(0)
#define FHK_STD_CHK_RANGE_U32(name,param,min,max)             FHK_CHK_RANGE_U32(FHK_STD_LEV_NOTC,_FHK_OBJ_ZONE,name,param,min,max,FHK_PRT_LF)
#define FHK_STD_CHK_RANGE_U32_NP(name,param,min,max)          FHK_CHK_RANGE_U32(FHK_STD_LEV_DEBG,_FHK_OBJ_ZONE,name,param,min,max,FHK_PRT_LF)
#define FHK_STD_CHK_RANGE_U32_API(name,param,min,max)         FHK_CHK_RANGE_U32(FHK_STD_LEV_ERRR,_FHK_OBJ_ZONE,name,param,min,max,FHK_PRT_LF)
#define FHK_STD_CHK_RANGE_U32_MSG(name,param,min,max,msg,...) FHK_CHK_RANGE_U32(FHK_STD_LEV_ERRR,_FHK_OBJ_ZONE,name,param,min,max,msg,##__VA_ARGS__)

/* 检测参数是否为0,为0将返回错误 */
#define FHK_CHK_ZERO(level,zone,name,param,msg,...) \
	do{ \
		FH_SINT32 _TParam = (param); \
		if((_TParam) == 0) \
		{ \
			FHK_STD_LOG(level,zone,"[INVALID_PARAM]%s=0." msg,name,##__VA_ARGS__); \
			return FHK_STD_ERR(FHK_STD_ZONE2MODID(zone),level,FHER_STD_ILLEGAL_PARAM); \
		} \
	}while(0)
#define FHK_STD_CHK_ZERO(name,param)                           FHK_CHK_ZERO(FHK_STD_LEV_NOTC,_FHK_OBJ_ZONE,name,param,FHK_PRT_LF)
#define FHK_STD_CHK_ZERO_NP(name,param)                        FHK_CHK_ZERO(FHK_STD_LEV_DEBG,_FHK_OBJ_ZONE,name,param,FHK_PRT_LF)
#define FHK_STD_CHK_ZERO_API(name,param)                       FHK_CHK_ZERO(FHK_STD_LEV_ERRR,_FHK_OBJ_ZONE,name,param,FHK_PRT_LF)
#define FHK_STD_CHK_ZERO_MSG(name,param,msg,...)               FHK_CHK_ZERO(FHK_STD_LEV_ERRR,_FHK_OBJ_ZONE,name,param,msg,##__VA_ARGS__)

/* 检测指针是否为空,为空将返回错误 */
#define FHK_CHK_PTR_VALID(level,zone,name,param,msg,...) \
	do{ \
		FH_ADDR _TParam = (FH_ADDR)(param); \
		if(_TParam == FH_NULL) \
		{ \
			FHK_STD_LOG(level,zone,"[INVALID_PARAM]%s=null." msg,name,##__VA_ARGS__); \
			return FHK_STD_ERR(FHK_STD_ZONE2MODID(zone),level,FHER_STD_NULL_POINTER); \
		} \
	}while(0)
#define FHK_STD_CHK_PTR_VALID(name,param)                     FHK_CHK_PTR_VALID(FHK_STD_LEV_NOTC,_FHK_OBJ_ZONE,name,param,FHK_PRT_LF)
#define FHK_STD_CHK_PTR_VALID_NP(name,param)                  FHK_CHK_PTR_VALID(FHK_STD_LEV_DEBG,_FHK_OBJ_ZONE,name,param,FHK_PRT_LF)
#define FHK_STD_CHK_PTR_VALID_API(name,param)                 FHK_CHK_PTR_VALID(FHK_STD_LEV_ERRR,_FHK_OBJ_ZONE,name,param,FHK_PRT_LF)
#define FHK_STD_CHK_PTR_VALID_MSG(name,param,msg,...)         FHK_CHK_PTR_VALID(FHK_STD_LEV_ERRR,_FHK_OBJ_ZONE,name,param,msg,##__VA_ARGS__)

/* 检测指针是否满足对齐要求,不满足将返回错误 */
#define FHK_CHK_ALIGN(level,zone,name,param,align,msg,...) \
	do{ \
		FH_UINT32 _TParam = (param); \
		if((_TParam % (align)) != 0) \
		{ \
			FHK_STD_LOG(level,zone,"[INVALID_PARAM]%s(%u) not align-%d." msg,name,_TParam,align,##__VA_ARGS__); \
			return FHK_STD_ERR(FHK_STD_ZONE2MODID(zone),level,FHER_STD_ILLEGAL_PARAM); \
		} \
	}while(0)
#define FHK_STD_CHK_ALIGN(name,param,align)                   FHK_CHK_ALIGN(FHK_STD_LEV_NOTC,_FHK_OBJ_ZONE,name,param,align,FHK_PRT_LF)
#define FHK_STD_CHK_ALIGN_NP(name,param,align)                FHK_CHK_ALIGN(FHK_STD_LEV_DEBG,_FHK_OBJ_ZONE,name,param,align,FHK_PRT_LF)
#define FHK_STD_CHK_ALIGN_API(name,param,align)               FHK_CHK_ALIGN(FHK_STD_LEV_ERRR,_FHK_OBJ_ZONE,name,param,align,FHK_PRT_LF)
#define FHK_STD_CHK_ALIGN_MSG(name,param,align,msg,...)       FHK_CHK_ALIGN(FHK_STD_LEV_ERRR,_FHK_OBJ_ZONE,name,param,align,msg,##__VA_ARGS__)

/* 检测模块是否存在,参数为0为不存在,将返回错误 */
#define FHK_CHK_EXIST(level,zone,name,param,msg,...) \
	do{ \
		FH_UINT32 _TParam = (FH_UINT32)(param); \
		if((_TParam) == 0) \
		{ \
			FHK_STD_LOG(level,zone,"[NOT EXIST]%s(%u) not exist." msg,name,_TParam,##__VA_ARGS__); \
			return FHK_STD_ERR(FHK_STD_ZONE2MODID(zone),level,FHER_STD_NOT_EXIST); \
		} \
	}while(0)
#define FHK_STD_CHK_EXIST(name,param)                         FHK_CHK_EXIST(FHK_STD_LEV_NOTC,_FHK_OBJ_ZONE,name,param,FHK_PRT_LF)
#define FHK_STD_CHK_EXIST_NP(name,param)                      FHK_CHK_EXIST(FHK_STD_LEV_DEBG,_FHK_OBJ_ZONE,name,param,FHK_PRT_LF)
#define FHK_STD_CHK_EXIST_API(name,param)                     FHK_CHK_EXIST(FHK_STD_LEV_ERRR,_FHK_OBJ_ZONE,name,param,FHK_PRT_LF)
#define FHK_STD_CHK_EXIST_MSG(name,param,msg,...)             FHK_CHK_EXIST(FHK_STD_LEV_ERRR,_FHK_OBJ_ZONE,name,param,msg,##__VA_ARGS__)

/* 检测模块是否存在,参数不为0为存在,将返回错误 */
#define FHK_CHK_NOT_EXIST(level,zone,name,param,msg,...) \
	do{ \
		FH_UINT32 _TParam = (FH_UINT32)(param); \
		if((_TParam) != 0) \
		{ \
			FHK_STD_LOG(level,zone,"[NOT EXIST]%s(%u) has exist." msg,name,_TParam,##__VA_ARGS__); \
			return FHK_STD_ERR(FHK_STD_ZONE2MODID(zone),level,FHER_STD_EXIST); \
		} \
	}while(0)
#define FHK_STD_CHK_NOT_EXIST(name,param)                     FHK_CHK_NOT_EXIST(FHK_STD_LEV_NOTC,_FHK_OBJ_ZONE,name,param,FHK_PRT_LF)
#define FHK_STD_CHK_NOT_EXIST_NP(name,param)                  FHK_CHK_NOT_EXIST(FHK_STD_LEV_DEBG,_FHK_OBJ_ZONE,name,param,FHK_PRT_LF)
#define FHK_STD_CHK_NOT_EXIST_API(name,param)                 FHK_CHK_NOT_EXIST(FHK_STD_LEV_ERRR,_FHK_OBJ_ZONE,name,param,FHK_PRT_LF)
#define FHK_STD_CHK_NOT_EXIST_MSG(name,param,msg,...)         FHK_CHK_NOT_EXIST(FHK_STD_LEV_ERRR,_FHK_OBJ_ZONE,name,param,msg,##__VA_ARGS__)

#pragma endregion

// 错误处理
#pragma region FHK_STD_ERROR_HANDLE

#if !defined(FH_TRACE_MINI) && !defined(_FHK_API_LAYER)
extern FH_CHAR *fhk_zone2name(FH_UINT32 zone);
extern FH_CHAR *fhk_mod2name(FH_UINT32 mod);
extern FH_CHAR *fhk_errid2name(FH_UINT32 errid);

//获取ZONE对应模块名
#define FHK_ZONE2NAME(zone)        fhk_zone2name(zone)
#define FHK_MOD2NAME(mod)          fhk_mod2name(mod)
//获取错误号对应模块
#define FHK_ERR2MOD(_ret)          FHK_STD_ERR2MOD(_ret)
#define FHK_ERR2MODNAME(_ret)      fhk_mod2name(FHK_ERR2MOD(_ret))
//获取错误号对应错误信息
#define FHK_ERR2NAME(_ret)         fhk_errid2name(FHK_STD_ERR2ID(_ret))
#else
#define FHK_ZONE2NAME(zone)        ""
#define FHK_MOD2NAME(mod)          ""
#define FHK_ERR2MOD(_ret)          FHK_STD_ERR2MOD(_ret)
#define FHK_ERR2MODNAME(_ret)      ""
#define FHK_ERR2NAME(_ret)         ""
#endif
//简化错误号的调用参数
#define FHK_ERRNO(_ret)            FHK_STD_ERR(FHK_STD_ZONE2MODID(_FHK_OBJ_ZONE),FHK_STD_LEV_ERRR,_ret)
#define FHK_ERRNO_DEBG(_ret)       FHK_STD_ERR(FHK_STD_ZONE2MODID(_FHK_OBJ_ZONE),FHK_STD_LEV_DEBG,_ret)
#define FHK_ERRNO_INFO(_ret)       FHK_STD_ERR(FHK_STD_ZONE2MODID(_FHK_OBJ_ZONE),FHK_STD_LEV_INFO,_ret)
#define FHK_ERRNO_NOTC(_ret)       FHK_STD_ERR(FHK_STD_ZONE2MODID(_FHK_OBJ_ZONE),FHK_STD_LEV_NOTC,_ret)
#define FHK_ERRNO_WARN(_ret)       FHK_STD_ERR(FHK_STD_ZONE2MODID(_FHK_OBJ_ZONE),FHK_STD_LEV_WARN,_ret)
#define FHK_ERRNO_ERRR(_ret)       FHK_STD_ERR(FHK_STD_ZONE2MODID(_FHK_OBJ_ZONE),FHK_STD_LEV_ERRR,_ret)
#define FHK_ERRNO_CRIT(_ret)       FHK_STD_ERR(FHK_STD_ZONE2MODID(_FHK_OBJ_ZONE),FHK_STD_LEV_CRIT,_ret)
#define FHK_ERRNO_ALRT(_ret)       FHK_STD_ERR(FHK_STD_ZONE2MODID(_FHK_OBJ_ZONE),FHK_STD_LEV_ALRT,_ret)
#define FHK_ERRNO_FATL(_ret)       FHK_STD_ERR(FHK_STD_ZONE2MODID(_FHK_OBJ_ZONE),FHK_STD_LEV_FATL,_ret)
//错误判段
#define FHK_IS_ERROR(_ret)         ((_ret) < 0)
#define FHK_IS_NULL(ptr)           ((ptr) == FH_NULL)

#define FHK_STD_ASSERT(exp) \
	do{ \
		if (!(exp)) \
		{ \
			FHK_STD_LOG(FHK_STD_LEV_WARN,_FHK_OBJ_ZONE,"[ASSERT]ASSERT at (%s)\n", \
				#exp); \
		} \
	}while(0)
#define FHK_STD_ERR_RET(func) \
	do{ \
		FH_SINT32 _ret = func;\
		if(FHK_IS_ERROR(_ret)) \
		{ \
			FHK_STD_LOG(FHK_STD_LEV_INFO,_FHK_OBJ_ZONE,"[ERROR RETURN]function returned 0x%x(%s:%s)\n", \
				_ret,FHK_ERR2MODNAME(_ret),FHK_ERR2NAME(_ret)); \
			return _ret; \
		} \
	}while(0)
#define FHK_STD_ERR_BREAK(func) \
	{ \
		FH_SINT32 _ret = func;\
		if(FHK_IS_ERROR(_ret)) \
		{ \
			FHK_STD_LOG(FHK_STD_LEV_INFO,_FHK_OBJ_ZONE,"[ERROR BREAK]function break 0x%x(%s:%s)\n", _ret, \
				FHK_ERR2MODNAME(_ret),FHK_ERR2NAME(_ret)); \
			break; \
		} \
	} \
	do { } while(0)
#define FHK_STD_ONERROR(func) \
	do{ \
		_status_ = func; \
		if (FHK_IS_ERROR(_status_)) \
		{ \
			FHK_STD_LOG(FHK_STD_LEV_INFO,_FHK_OBJ_ZONE, "[ONERROR]status=0x%x(%s:%s)\n",_status_, \
				FHK_ERR2MODNAME(_status_),FHK_ERR2NAME(_status_)); \
			goto OnError; \
		} \
	} while (0)

#define FHK_STD_VERIFY_OK(func) \
	do{ \
		FH_SINT32 _ret = func; \
		if ((_ret) == FHER_STD_RET_OK) \
		{ \
			FHK_STD_LOG(FHK_STD_LEV_DEBG,_FHK_OBJ_ZONE,"[VERIFY_OK] function returned 0x%x(%s:%s)\n",_ret, \
				FHK_ERR2MODNAME(_status_),FHK_ERR2NAME(_status_)); \
		} \
		FHK_STD_ASSERT(_ret == FHER_STD_RET_OK); \
	} while (0)
#define FHK_STD_REMAP_RET(mapbase) \
	do{ \
		FH_ADDR _map = (FH_ADDR)mapbase; \
		if ((_map) == FH_NULL) \
		{ \
			FHK_STD_LOG(FHK_STD_LEV_INFO,_FHK_OBJ_ZONE,"[MEMORY] remap address failed\n"); \
			return FHK_ERRNO_ERRR(FHER_STD_REMAP_FAIL); \
		} \
	} while (0)
#define FHK_STD_REMAP_ONERROR(mapbase) \
	do{ \
		FH_ADDR _map = (FH_ADDR)mapbase; \
		if ((_map) == FH_NULL) \
		{ \
			FHK_STD_LOG(FHK_STD_LEV_INFO,_FHK_OBJ_ZONE,"[MEMORY] remap address failed\n"); \
			_status_ = FHK_ERRNO_ERRR(FHER_STD_REMAP_FAIL); \
			goto OnError; \
		} \
	} while (0)
#define FHK_STD_REMAP2_RET(base,size) \
	({ \
		FH_ADDR _map = (FH_ADDR)fh_mem_remap(base,size); \
		if ((_map) == FH_NULL) \
		{ \
			FHK_STD_LOG(FHK_STD_LEV_INFO,_FHK_OBJ_ZONE,"[MEMORY] remap address failed\n"); \
			return FHK_ERRNO_ERRR(FHER_STD_REMAP_FAIL); \
		} \
		_map; \
	})
#define FHK_STD_REMAP2_ONERROR(base,size) \
	({ \
		FH_ADDR _map = (FH_ADDR)fh_mem_remap(base,size); \
		if ((_map) == FH_NULL) \
		{ \
			FHK_STD_LOG(FHK_STD_LEV_INFO,_FHK_OBJ_ZONE,"[MEMORY] remap address failed\n"); \
			_status_ = FHK_ERRNO_ERRR(FHER_STD_REMAP_FAIL); \
			goto OnError; \
		} \
		_map; \
	})
#define FHK_STD_UNMAP(ptr) \
	do{ \
		if(ptr != FH_NULL) { \
			fh_mem_unmap(ptr); \
			ptr = FH_NULL; \
		} \
	} while (0)
#define FHK_STD_MALLOC_RET(size) \
	({ \
		FH_ADDR _ptr = (FH_ADDR)fh_malloc(size); \
		if ((_ptr) == FH_NULL) \
		{ \
			FHK_STD_LOG(FHK_STD_LEV_INFO,_FHK_OBJ_ZONE,"[MEMORY] malloc buffer(%#x) failed\n",size); \
			return FHK_ERRNO_ERRR(FHER_STD_NOMEM); \
		} \
		_ptr; \
	})
#define FHK_STD_MALLOC_ONERROR(size) \
	({ \
		FH_ADDR _ptr = (FH_ADDR)fh_malloc(size); \
		if ((_ptr) == FH_NULL) \
		{ \
			FHK_STD_LOG(FHK_STD_LEV_INFO,_FHK_OBJ_ZONE,"[MEMORY] malloc buffer(%#x) failed\n",size); \
			_status_ = FHK_ERRNO_ERRR(FHER_STD_NOMEM); \
			goto OnError; \
		} \
		_ptr; \
	})
#define FHK_STD_FREE(ptr) \
	do{ \
		if(ptr != FH_NULL) { \
			fh_free(ptr); \
			ptr = FH_NULL; \
		} \
	} while (0)
#define FHK_STD_VMALLOC_RET(size) \
	({ \
		FH_ADDR _ptr = (FH_ADDR)fh_vmalloc(size); \
		if ((_ptr) == FH_NULL) \
		{ \
			FHK_STD_LOG(FHK_STD_LEV_INFO,_FHK_OBJ_ZONE,"[MEMORY] malloc buffer(%#x) failed\n",size); \
			return FHK_ERRNO_ERRR(FHER_STD_NOMEM); \
		} \
		_ptr; \
	})
#define FHK_STD_VMALLOC_ONERROR(size) \
	({ \
		FH_ADDR _ptr = (FH_ADDR)fh_vmalloc(size); \
		if ((_ptr) == FH_NULL) \
		{ \
			FHK_STD_LOG(FHK_STD_LEV_INFO,_FHK_OBJ_ZONE,"[MEMORY] malloc buffer(%#x) failed\n",size); \
			_status_ = FHK_ERRNO_ERRR(FHER_STD_NOMEM); \
			goto OnError; \
		} \
		_ptr; \
	})
#define FHK_STD_VFREE(ptr) \
	do{ \
		if(ptr != FH_NULL) { \
			fh_vfree(ptr); \
			ptr = FH_NULL; \
		} \
	} while (0)
#define FHK_STD_GET_CLOCK_RET(name) \
	({ \
		fh_clock *_clk = fh_clk_get(name); \
		if (fh_clk_is_err(_clk)) \
		{ \
			FHK_STD_LOG(FHK_STD_LEV_ERRR,_FHK_OBJ_ZONE,"[CLOCK]Fail to get clock,%s!\n",name); \
			return FHK_ERRNO_ERRR(FHER_STD_NOT_MATCH); \
		} \
		_clk; \
	})
#define FHK_STD_GET_CLOCK_ONERROR(name) \
	({ \
		fh_clock *_clk = fh_clk_get(name); \
		if (fh_clk_is_err(_clk)) \
		{ \
			FHK_STD_LOG(FHK_STD_LEV_ERRR,_FHK_OBJ_ZONE,"[CLOCK]Fail to get clock,%s!\n",name); \
			_status_ = FHK_ERRNO_ERRR(FHER_STD_NOT_MATCH); \
			goto OnError; \
		} \
		_clk; \
	})
#define FHK_STD_DRV_OPEN(drvref) \
	do{ \
		if((++ drvref) > 1){ \
			return FHER_STD_RET_OK; \
		} \
	} while (0)
#define FHK_STD_DRV_CLOSE(drvref) \
	do{ \
		fh_drvclose_ref_cnt_handle(drvref); \
		if((-- drvref) > 0){ \
			return FHER_STD_RET_OK; \
		} \
	} while (0)
#pragma endregion

#pragma region FHK_STD_DEBUG_CFG

#pragma endregion

#ifdef __GNUC__
	#if ((__GNUC__ * 100) + __GNUC_MINOR__) < 406
	#pragma GCC diagnostic warning "-Wunknown-pragmas"
	#else
	#pragma GCC diagnostic pop
	#endif
#endif

#endif