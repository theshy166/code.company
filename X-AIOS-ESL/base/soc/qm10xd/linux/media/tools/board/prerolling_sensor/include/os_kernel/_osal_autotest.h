#ifndef __AUTOTEST_KERNEL_OSAL_H__
#define __AUTOTEST_KERNEL_OSAL_H__
#include "fh_std_error.h"
#include "fh_std_osal.h"

#ifndef _FHK_OBJ_ZONE
#define _FHK_OBJ_ZONE FHK_STD_MODID2ZONE(FHK_STD_MOD_NONE)
#warning "Need to define _FHK_OBJ_ZONE when include fh_std_api.h"
#endif

#ifdef FPGA_ENV
#define FHK_DEF_TIMEOUT (FH_SYS_HZ * 5)
#else
#define FHK_DEF_TIMEOUT (FH_SYS_HZ)
#endif

#ifndef AUTOTEST

#define FHK_AT_TRACE_INIT()
#define FHK_AT_TRACE_EXIT()
#define FHK_AT_CHKBORDER_INIT()
#define FHK_AT_CHKBORDER_EXIT()
/*对内存统一赋随机值*/
#define FHK_AT_MEM_RAND(phybase,size)
/*内存边界检查*/
#define FHK_RECOMMEND_MEM_BORDER (0)
#define FHK_AT_MEM_CHK_BORDER(phybase,size)
/*内存相关*/
#define fh_mem_remap(addr,size) _fh_mem_remap(addr,size)
#define fh_mem_unmap(addr)      _fh_mem_unmap(addr)
#define fh_malloc(size)         _fh_malloc(size)
#define fh_realloc(addr,size)   _fh_realloc(addr,size)
#define fh_free(addr)           _fh_free(addr)
#define fh_vmalloc(size)        _fh_vmalloc(size)
#define fh_vfree(addr)          _fh_vfree(addr)

/*系统函数*/
#define fh_mutex_init(mutex,name) _fh_mutex_init(mutex,name)
#define fh_mutex_destroy(mutex)   _fh_mutex_destroy(mutex)

#define fh_spin_init(lock,name)   _fh_spin_init(lock,name)
#define fh_spin_destroy(lock)     _fh_spin_destroy(lock)

#define fh_waitqueue_init(wait,name) _fh_waitqueue_init(wait,name)
#define fh_waitqueue_destroy(wait)   _fh_waitqueue_destroy(wait)

#else
/*自动测试时重载部分函数*/
FH_VOID _fhk_at_src_trace_init(FH_VOID);
FH_VOID _fhk_at_src_trace_exit(FH_VOID);
FH_VOID _fhk_at_chk_border_init(FH_VOID);
FH_VOID _fhk_at_chk_border_exit(FH_VOID);
FH_VOID _fhk_at_set_random_val(FH_UINT32 phybase,FH_UINT32 size);
FH_VOID *_fhk_at_mem_remap(FH_UINT32 zone,FH_CHAR *func_name,FH_UINT32 line_no,FH_UINT32 addr,FH_UINT32 size);
FH_VOID _fhk_at_mem_unmap(FH_UINT32 zone,FH_CHAR *func_name,FH_UINT32 line_no,FH_VOID *addr);
FH_VOID *_fhk_at_malloc(FH_UINT32 zone,FH_CHAR *func_name,FH_UINT32 line_no,FH_UINT32 size);
FH_VOID *_fhk_at_realloc(FH_UINT32 zone,FH_CHAR *func_name,FH_UINT32 line_no,FH_VOID *addr,FH_UINT32 size);
FH_VOID _fhk_at_free(FH_UINT32 zone,FH_CHAR *func_name,FH_UINT32 line_no,FH_VOID *addr);
FH_VOID *_fhk_at_vmalloc(FH_UINT32 zone,FH_CHAR *func_name,FH_UINT32 line_no,FH_UINT32 size);
FH_VOID _fhk_at_vfree(FH_UINT32 zone,FH_CHAR *func_name,FH_UINT32 line_no,FH_VOID *addr);
FH_VOID _fhk_at_mutex_init(FH_UINT32 zone,FH_CHAR *func_name,FH_UINT32 line_no,fh_mutex_t *mutex,FH_CHAR *name);
FH_VOID _fhk_at_mutex_destroy(FH_UINT32 zone,FH_CHAR *func_name,FH_UINT32 line_no,fh_mutex_t *mutex);
FH_VOID _fhk_at_spin_init(FH_UINT32 zone,FH_CHAR *func_name,FH_UINT32 line_no,fh_spinlock_t *mutex,FH_CHAR *name);
FH_VOID _fhk_at_spin_destroy(FH_UINT32 zone,FH_CHAR *func_name,FH_UINT32 line_no,fh_spinlock_t *mutex);
FH_VOID _fhk_at_waitqueue_init(FH_UINT32 zone,FH_CHAR *func_name,FH_UINT32 line_no,fh_wait_queue_t *wait,FH_CHAR *name);
FH_VOID _fhk_at_waitqueue_destroy(FH_UINT32 zone,FH_CHAR *func_name,FH_UINT32 line_no,fh_wait_queue_t *wait);
FH_VOID _fhk_at_set_mem_border(FH_UINT32 zone,FH_CHAR *func_name,FH_UINT32 line_no,FH_UINT32 phybase,FH_UINT32 size);

#define FHK_AT_TRACE_INIT()     _fhk_at_src_trace_init()
#define FHK_AT_TRACE_EXIT()     _fhk_at_src_trace_exit()
#define FHK_AT_CHKBORDER_INIT() _fhk_at_chk_border_init()
#define FHK_AT_CHKBORDER_EXIT() _fhk_at_chk_border_exit()
/*对内存统一赋随机值*/
#define FHK_AT_MEM_RAND(phybase,size)       _fhk_at_set_random_val(phybase,size)
/*内存边界检查*/
#define FHK_RECOMMEND_MEM_BORDER (1024)
#define FHK_AT_MEM_CHK_BORDER(phybase,size) _fhk_at_set_mem_border(_FHK_OBJ_ZONE,(FH_CHAR *)__FUNCTION__,__LINE__,phybase,size)
/*内存相关*/
#define fh_mem_remap(addr,size) _fhk_at_mem_remap(_FHK_OBJ_ZONE,(FH_CHAR *)__FUNCTION__,__LINE__,addr,size)
#define fh_mem_unmap(addr)      _fhk_at_mem_unmap(_FHK_OBJ_ZONE,(FH_CHAR *)__FUNCTION__,__LINE__,addr)
#define fh_malloc(size)         _fhk_at_malloc(_FHK_OBJ_ZONE,(FH_CHAR *)__FUNCTION__,__LINE__,size)
#define fh_realloc(addr,size)   _fhk_at_realloc(_FHK_OBJ_ZONE,(FH_CHAR *)__FUNCTION__,__LINE__,addr,size)
#define fh_free(addr)           _fhk_at_free(_FHK_OBJ_ZONE,(FH_CHAR *)__FUNCTION__,__LINE__,addr)
#define fh_vmalloc(size)        _fhk_at_vmalloc(_FHK_OBJ_ZONE,(FH_CHAR *)__FUNCTION__,__LINE__,size)
#define fh_vfree(addr)          _fhk_at_vfree(_FHK_OBJ_ZONE,(FH_CHAR *)__FUNCTION__,__LINE__,addr)

/*系统函数*/
#define fh_mutex_init(mutex,name) _fhk_at_mutex_init(_FHK_OBJ_ZONE,(FH_CHAR *)__FUNCTION__,__LINE__,mutex,name)
#define fh_mutex_destroy(mutex)   _fhk_at_mutex_destroy(_FHK_OBJ_ZONE,(FH_CHAR *)__FUNCTION__,__LINE__,mutex)

#define fh_spin_init(lock,name)   _fhk_at_spin_init(_FHK_OBJ_ZONE,(FH_CHAR *)__FUNCTION__,__LINE__,lock,name)
#define fh_spin_destroy(lock)     _fhk_at_spin_destroy(_FHK_OBJ_ZONE,(FH_CHAR *)__FUNCTION__,__LINE__,lock)

#define fh_waitqueue_init(wait,name) _fhk_at_waitqueue_init(_FHK_OBJ_ZONE,(FH_CHAR *)__FUNCTION__,__LINE__,wait,name)
#define fh_waitqueue_destroy(wait)   _fhk_at_waitqueue_destroy(_FHK_OBJ_ZONE,(FH_CHAR *)__FUNCTION__,__LINE__,wait)

#endif

#define FHK_MEM_BORDER_MALLOC(buf_info) do{ \
	MEM_INFO _info; \
	FH_UINT32 _ret; \
	if(FHK_RECOMMEND_MEM_BORDER) { \
		_ret = media_malloc(buf_info,&_info,FHK_RECOMMEND_MEM_BORDER,1); \
		if(!_ret) { \
			FHK_AT_MEM_CHK_BORDER(_info.base,_info.size); \
		} \
	} \
}while(0)
#define FHK_MEM_BORDER_MALLOC_EX(buf_info_ex) do{ \
	RW_MEM_INFO _info; \
	FH_UINT32 _ret; \
	if(FHK_RECOMMEND_MEM_BORDER) { \
		_ret = media_malloc_ex(buf_info_ex,&_info,FHK_RECOMMEND_MEM_BORDER,1); \
		if(!_ret) { \
			FHK_AT_MEM_CHK_BORDER(_info.mem.base,_info.mem.size); \
		} \
	} \
}while(0)

#endif
