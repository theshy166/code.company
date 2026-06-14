#ifndef __ESL_MESSAGE_MODEL_H__
#define __ESL_MESSAGE_MODEL_H__

typedef enum{
    EslMessageTypeUNKOWN = 0,//未知类型
    EslMessageTypeCONTENT = 1,//模版修改
    EslMessageTypeWIFI = 2,//修改wifi 
    EslMessageTypeUNBIND = 3,//解绑
    EslMessageTypeRESTRAT = 4,//重启
    EslMessageTypeGROUP = 5,//分组
    EslMessageTypeUPDATE = 6,//OTA升级
    EslMessageTypeREFRESH = 7,//刷新
    EslMessageTypeURL = 100,//刷新
    EslMessageTypeBRIGHTNESS = 101,//亮度
    EslMessageTypeSCREENSWITCH = 102,//屏幕开关
    EslMessageTypeLANGUAGE = 103,//语言
    EslMessageTypeCHECKLIFE = 999,//探活
} EslMessageType;

typedef struct esl_message_model{
    char * message_id; //消息ID
    char * device_no; //设备编码
    int type;//消息类型
    char * data;//消息内容
    int ignore;//备用字段 当前消息忽略
}esl_message_model_t;

#endif