#ifndef LOS_AISKILL_ENTRY__H
#define LOS_AISKILL_ENTRY__H

typedef enum {  
    MODULE_DEMO,  
 #if defined (CONFIG_XOS_FWK_CADP_QUA) || defined(CONFIG_XOS_USE_APP_SCANNER)
    MODULE_QUA,  
 #endif
 #if defined (CONFIG_XOS_FWK_CADP_KUQI)  && !defined(BUILD_SIMULATOR)
    MODULE_KUQI,  
 #endif
    // 可以添加更多模块  
    MODULE_COUNT  
} AiModuleType; 


typedef enum {  
    IDLE = 0,  
    CONNECTING, 
    CONNECTED,
    DISCONNECTED,  
    NETMAX  
} AiNeTType; 
/***
 * 
 * dev_info  AI 初始化需要的参数，根据需要添加参数
 * 
 */

typedef struct AiDevInfo
{
    int  server_type;
    char* hosturl;       
    int  port;
    char* ac;
    char* pw;
    int product_id;
    char* devsn;  
    char* clientid ;    // 设备分配的clientid ,这个值需要看AI需求是否是拼接而成
    char* public_key; //是否需要传入KEY，这个字段也可以放在AI自己的模块里面，这么做是为了在界面上切换服务器环境
    char* securet_key; 
    char appPhone[64];
    char* subscriber_topic; //订阅的主题
    char* publish_topic; //发布的主题
} AiDevInfo;



/**
 *   val : 定义消息类型
 * *
 *   data: 返回具体数据
*/
typedef struct AiMessage
{
    int val;       //定义消息类型，有各个AI模块自由发挥
    char* data1;   //传递的内容基本上都是一些字符串
    char* data2;   // 自已根据需要多加些消息类型
    char* topic;   // 一般是用来做消息的主题
    
} AiMessage;

typedef void (*respose)(const AiMessage* params); 
typedef void (*connectCallback)(const AiMessage* params); 

typedef struct {  
    void (*init)(AiDevInfo* dev );  
    void (*deinit)(void);  
    void (*register_respose)(respose callback); /*返回结果，也可以支持返回过程信息，通过过msg做调整*/
    void (*register_conn_respose)(connectCallback callback);   /**如果此AI没有回调，直接写空函数**/

    int  (*send_cmd)(AiMessage* comand);	/**如果此AI没有回调，直接写空函数**/
    int  (*input_content)(AiMessage* content);
} AiModule;


#endif/*LOS_AISKILL_ENTRY__H*/