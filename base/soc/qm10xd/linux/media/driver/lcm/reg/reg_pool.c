#include "reg.h"
#include "reg_pool.h"
#include "dsi_phy_v01_reg_def.h"
/**---------------------------------------------------------------------------*
**                Compiler Flag                    **
**---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif
/**---------------------------------------------------------------------------*
*                Micro Define                    **
**----------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
**                Data Structures                    **
**---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 extend Variables and function            *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 Local Variables                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 Constant Variables                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                 Local Function Prototypes                *
 **---------------------------------------------------------------------------*/

/*
    reg_t table reg value compare with read from chip register
*/
int32_t regDefaultChecker(void *regPool)
{
    int32_t rtn = SUCCESS;
    reg_t *reg_pool_ptr = (reg_t *)regPool;
    uint32_t reg_counter = 0x00;
    uint32_t reg_addr = 0x00;
    uint32_t reg_default_value = 0x00;
    uint32_t reg_value = 0x00;

    CHECK_PTR(reg_pool_ptr, NULL, "regPool is NULL ~_~!");

    while(0xffffffff != reg_pool_ptr[reg_counter].addr) {
        reg_addr = reg_pool_ptr[reg_counter].addr;
        reg_default_value = reg_pool_ptr[reg_counter].data;

        reg_value = regRead32(reg_addr);
        if (reg_default_value != reg_value) {
            printf("addr:0x%x, default:0x%x, read:0x%x\n",reg_addr, reg_default_value, reg_value);
            rtn = FAIL_NO_EQUAL;
        }

        reg_counter++;
    };

    EXIT:

    return rtn;
}

int32_t regDefaultGet(ADDR base_addr, void *regPool)
{
    int32_t rtn = SUCCESS;
    reg_t *reg_pool_ptr = (reg_t *)regPool;
    uint32_t reg_counter = 0x00;
    uint32_t reg_addr = 0x00;
    uint32_t reg_value = 0x00;

    CHECK_PTR(reg_pool_ptr, NULL, "regPool is NULL ~_~!");

    printf("---reg default get-----\n");

    while(0xffffffff != reg_pool_ptr[reg_counter].addr) {
        reg_addr = reg_pool_ptr[reg_counter].addr;
        reg_value = regRead32(base_addr + reg_addr);
        reg_pool_ptr[reg_counter].data = reg_value;
        reg_counter++;
    };

    printf("---reg default end-----\n");

    EXIT:

    return rtn;
}

/*
    get reg value from reg_t table
*/
void *getRegValuePtr(ADDR addr, void *regPool)
{
    int32_t rtn = SUCCESS;
    reg_t *reg_pool_ptr = (reg_t *)regPool;
    void *reg_value_ptr = NULL;
    uint32_t reg_counter = 0x00;

    CHECK_PTR(reg_pool_ptr, NULL, "regPool is NULL ~_~!");

    while(0xffffffff != reg_pool_ptr[reg_counter].addr) {
        if (addr == reg_pool_ptr[reg_counter].addr) {
            reg_value_ptr = (void *)&reg_pool_ptr[reg_counter].data;
            break;
        }

        reg_counter++;

        if (0xffffffff == reg_pool_ptr[reg_counter].addr) {
            printf("reg 0x%08x is not in reg pool ~_~!\n", addr);
        }
    };

    if (rtn) {
    }

    EXIT:

    return reg_value_ptr;
}

/*
    get reg_t pointer from reg_list_t table by reg addr
*/
reg_t *getRegPtr(ADDR addr, reg_list_t *list)
{
    int32_t rtn = SUCCESS;
    reg_list_t *regList = NULL;
    reg_description_t *reg_des = NULL;
    reg_t *reg = NULL;
    uint32_t maxRegNum = 0;
    uint32_t i = 0;

    CHECK_PTR(list, NULL, "reg_list_t is NULL ~_~!");
    CHECK_PTR(list->reg_des, NULL, "reg_description_t is NULL ~_~!");

    regList = list;
    reg_des = regList->reg_des;
    maxRegNum = regList->regNum;

    for (i = 0; i < maxRegNum; i++) {
        if (addr == reg_des->reg.addr) {
            reg = &reg_des->reg;
            break;
        }
        reg_des++;
    }

    if (rtn) {
    }

    EXIT:

    return reg;
}

int32_t showRegValuePtr(void *regPool, const char *fun)
{
    int32_t rtn = SUCCESS;
    reg_t *reg_pool_ptr = (reg_t *)regPool;
    uint32_t reg_counter = 0x00;

    CHECK_PTR(reg_pool_ptr, NULL, "regPool is NULL ~_~!");

    printf("---reg show start-----\n");

    while(0xffffffff != reg_pool_ptr[reg_counter].addr) {

        printf("%s, 0x%08x : 0x%08x\n", fun, reg_pool_ptr[reg_counter].addr, reg_pool_ptr[reg_counter].data);
        reg_counter++;

        if (0xffffffff == reg_pool_ptr[reg_counter].addr) {
            break;
        }
    };

    printf("---reg show end-----\n");

    EXIT:

    return rtn;
}

#if 0
int parserRegToList(FILE *fp, reg_list_t *list)
{
    int32_t rtn = 0;
    reg_list_t *regList = NULL;
    reg_t reg = {0x00};
    reg_t *regPtr = NULL;
    uint32_t endFlag = 0;
    uint32_t maxRegNum = 0;
    uint32_t i = 0;

    if ((NULL == fp)
        || (NULL == list)
        || (NULL == list->reg)) {
        goto EXIT;
    }



    regList = list;
    maxRegNum = list->regNum - 1;
    regList->idx = 0;

    do {
        endFlag = _parserReg(fp, &reg);

        if (0 == endFlag) {
            regPtr = getRegPtr(reg.addr, regList);
            if (NULL != regPtr) {
                regPtr->data = reg.data;
                regList->idx++;
                if (maxRegNum == regList->idx) {
                    goto EXIT;
                }
            }
        } else {
            goto EXIT;
        }
    } while(1);

    EXIT:

    //rtn = back_fileHeader(NULL, fp);

    return rtn;
}

int saveRegToFile(FILE *fp, reg_list_t *list)
{
    int32_t rtn = 0;
    reg_list_t *regList = NULL;
    reg_t *reg = NULL;
    uint32_t maxRegNum = 0;
    uint32_t addrLen = 0;
    uint32_t dataLen = 0;
    uint32_t i = 0;

    if ((NULL == fp)
        || (NULL == list)
        || (NULL == list->reg)) {
        goto EXIT;
    }
    regList = list;
    reg = regList->reg;
    maxRegNum = regList->regNum - 1;
    addrLen = regList->addrLen;
    dataLen = regList->dataLen;

    if (NULL != fp) {
        for (i = 0; i < maxRegNum; i++) {
            if (64 == addrLen) {
                fprintf(fp, "%016x ", reg[i].addr);
            } else {
                fprintf(fp, "%08x ", reg[i].addr);
            }

            if (64 == dataLen) {
                fprintf(fp, "%016x ", reg[i].data);
            } else {
                fprintf(fp, "%08x ", reg[i].data);
            }

            fprintf(fp, "//%s\n", reg[i].comment);
        }
    }

    EXIT:

    return rtn;
}

int parserRegFromFile(char *filePath, reg_list_t *list)
{
    int32_t rtn = 0;
    FILE *fp = NULL;

    if ((NULL == filePath)
        || (NULL == list)
        || (NULL == list->reg)) {
        goto EXIT;
    }

    fp = fopen(filePath, "rb");

    rtn = parserRegToList(fp, list);

    EXIT:

    if (NULL != fp) fclose(fp);

    return rtn;
}

int saveRegAsFile(char *filePath, reg_list_t *list)
{
    int32_t rtn = 0;
    FILE *fp = NULL;

    if ((NULL == filePath)
        || (NULL == list)
        || (NULL == list->reg)) {
        goto EXIT;
    }

    fp = fopen(filePath, "wb");

    rtn = saveRegToFile(fp, list);

    EXIT:

    if (NULL != fp) fclose(fp);

    return rtn;
}
#endif


/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

