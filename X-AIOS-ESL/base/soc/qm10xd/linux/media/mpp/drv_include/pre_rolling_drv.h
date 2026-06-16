#ifndef __PRE_ROLLING_DRV_H__
#define __PRE_ROLLING_DRV_H__

typedef struct {
    unsigned char portType;
    unsigned char cropEnable;
    unsigned char rawecCompressBypass;
    unsigned char rawecOutputBypass;
    unsigned char aeEnable;
    unsigned char blcEnable;
    unsigned char awbEnable;
    unsigned char axiPulseEnable;//axi pulse默认走回滚机制
    unsigned char shmmEnable;
} Camif_Stream_On_S;

int prg_set_camif_info(Camif_Stream_On_S *params);


#endif
