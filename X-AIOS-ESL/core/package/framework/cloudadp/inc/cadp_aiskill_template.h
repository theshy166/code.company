#ifdef CONFIG_XOS_FWK_CADP_SAMPLE
#ifndef LOS_SAMPLE_ENTRY__H
#define LOS_SAMPLE_ENTRY__H


typedef enum xxx_state
{	
    INIT,
    RUNNING,
    SLEEP,
}xxx_state;

/**
 *   val : 定义消息类型
 * *
 *   data: 返回具体数据
*/
typedef struct xxx_msg_param
{
    int val;       
    void *data;
} xxx_msg_param;


typedef void (*reqCallback)(const xxx_msg_param* params); 

typedef void (*connectCallback)(const xxx_msg_param* params); 



/**
**函数管理器 - AI 初始化必须要实现的逻辑
**/

typedef struct xxx_ai_ops {
	//必须实现
    void* (*xxx_on_init)(xxx_dev_info_ptr data);
    void  (*xxx_on_deinit)();
    void  (*xxx_register_respose_message)(reqCallback callback); /*返回结果，也可以支持返回过程信息，通过过msg做调整*/
	void  (*xxx_register_connect_respose)(connectCallback callback);   /**如果此AI没有回调，直接写空函数**/


    //根据各个AI的需求进行补充
	int (*xxx_sen_cmd)((char* comand));	/**如果此AI没有回调，直接写空函数**/
	int (*xxx_input_content)((char* path,char* text));
} xxx_ai_ops;


#endif/*LOS_SAMPLE_ENTRY__H*/
#endif/*CONFIG_XOS_FWK_CADP_SAMPLE*/