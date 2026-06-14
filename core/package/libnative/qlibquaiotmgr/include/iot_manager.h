
///IoT Manager: internet of things manager, it is a singleton, you can get it by iot_manager_get_instance()
#ifndef IOT_MANAGER_H_
#define IOT_MANAGER_H_


typedef struct iot_manager_t iot_manager_t;
typedef struct iot_thing_t iot_thing_t;

// 单例获取接口
iot_manager_t* iot_manager_get_instance(void);

// 操作方法
void iot_manager_add_thing(iot_manager_t* manager, iot_thing_t * thing); //thing will be freed when manager releasing;
char* iot_manager_get_descriptors(iot_manager_t* manager); //need to free result outside
//iot_manager_get_states: out_json need to free outside(no matter the return value true or false)
//  delta: 0 means get all states, 1 means get states changed since last time, when delta is 1, return value is true if states changed, false if not
int iot_manager_get_states(iot_manager_t* manager, char** out_json, int delta); 
void iot_manager_invoke(iot_manager_t* manager, const char * command); //command is json format

// 内存管理
void iot_manager_release_instance(void);

#endif // IOT_MANAGER_H_