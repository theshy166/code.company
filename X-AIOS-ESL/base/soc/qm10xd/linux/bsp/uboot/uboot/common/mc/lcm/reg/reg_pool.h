#ifndef _REG_POOL_H_
#define _REG_POOL_H_

#include "../../lcm_module/include/data_type.h"

#ifdef __cplusplus
extern "C" {
#endif
/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**----------------------------------------------------------------------------*/
#define COMMENT_CHAR_LEN 64

/**---------------------------------------------------------------------------*
**                              Data Structures                              **
**---------------------------------------------------------------------------*/
typedef struct {
    ADDR addr;
    DATA data;
}reg_t;

typedef struct {
    reg_t reg;
    uint32_t id;
    char comment[COMMENT_CHAR_LEN];
}reg_description_t;

typedef struct {
    uint32_t idx;
    uint32_t regNum;
    uint32_t addrLen;
    uint32_t dataLen;
    reg_description_t *reg_des;
}reg_list_t;

/**---------------------------------------------------------------------------*
**                              function                                     **
**---------------------------------------------------------------------------*/
int32_t regDefaultChecker(void *regPool);
int32_t regDefaultGet(ADDR base_addr, void *regPool);
void *getRegValuePtr(ADDR addr, void *regPool);
reg_t *getRegPtr(ADDR addr, reg_list_t *list);
int32_t showRegValuePtr(void *regPool, const char *fun);

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif

