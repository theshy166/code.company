#ifdef CONFIG_XOS_APP_ESL2  
// ============================================================
// ★ mqtt_control.c — MQTT 连接控制层
// 作用：封装 MQTT 客户端的 init / 注册回调 / 发消息 / 销毁
// 不直接处理 MQTT 协议，全部委托给底层的 QuaModule（AiModule）
//
// 调用链：
//   page_esl.c → init_mqtt() → create_module(MODULE_QUA) → QuaModule.init(dev)
//                         ↓                           ↓
//             register_connect_cb()              AiModule 内部完成
//             register_repose_cb()               TCP/TLS 连接 →
//                                                MQTT CONNECT →
//                                                MQTT SUBSCRIBE
//                         ↓
//             当服务器推送消息 → QuaModule 调用 register_repose_cb 注册的回调
//                         ↓
//             mqtt_subscriber.c 中的 mqtt_subscriber() 被触发
// ============================================================

#include "lvgl/lvgl.h"
#include "appcommon.h"
#include "appmanager.h"
#include "cadp_aiskill.h"
#include "mqtt_control.h"
#include "../conf/conf.h"

// ★ AiModule 工厂函数：通过类型创建对应的 MQTT 通信模块
// MODULE_QUA 是当前项目使用的模块类型（底层封装了 MQTT 协议栈）
extern  AiModule* create_module(AiModuleType type);

// ★ 全局唯一的 MQTT 模块实例，整个程序生命周期内只有一个
AiModule* t_QuaModule = NULL;

/**
 * @brief ★ 初始化 MQTT 连接 — 连接到云平台 MQTT Broker
 *
 * 1. 如果之前已经连接过，先 deinit 断开旧连接
 * 2. 打印连接参数日志（便于调试）
 * 3. 创建 QuaModule 实例 → init(dev) 建立 TCP/TLS 连接 → 发送 MQTT CONNECT
 *
 * @param dev MQTT 连接信息（host、port、用户名、密码、client_id、订阅topic等）
 * @return 0 成功
 *
 * dev 结构体关键字段（来自 query_push_server2 的返回值）：
 *   dev->hosturl           = 云平台 MQTT Broker 地址
 *   dev->port              = MQTT 端口（通常是 1883 或 8883）
 *   dev->ac / dev->pw      = MQTT 认证用户名/密码
 *   dev->clientid          = MQTT Client ID（通常是设备SN）
 *   dev->subscriber_topic  = MQTT 订阅主题（设备专属 topic）
 *   dev->devsn             = 设备序列号
 */
int init_mqtt(AiDevInfo* dev) {
   // 如果已存在旧连接，先断开（避免重复连接）
   if( t_QuaModule != NULL && t_QuaModule->deinit != NULL ){
       t_QuaModule->deinit();    // 断开旧 MQTT 连接，释放资源
    }
    // 打印连接参数到日志，方便调试
    QM_ESL2_LOG("hosturl:%s,port:%d,ac:%s,pw:%s,devsn:%s,clientid:%s,subscriber_topic:%s",dev->hosturl,dev->port,dev->ac,dev->pw,dev->devsn,dev->clientid,dev->subscriber_topic);
    // ★ 创建 QuaModule 实例（底层封装了 TCP/TLS + MQTT 协议）
    t_QuaModule = create_module( MODULE_QUA );
    // ★ 初始化连接：TCP 握手 → TLS（如需要）→ MQTT CONNECT → MQTT SUBSCRIBE
    t_QuaModule->init(dev);
    return 0;
}

/**
 * @brief 注册「MQTT 连接状态变化」回调
 *
 * 当 MQTT 连接成功/断开/重连时，QuaModule 会调用此回调通知上层
 * 当前项目注册的是 mqtt_connect_response()（在 mqtt_subscriber.c 底部）
 *
 * @param callback 连接状态回调函数指针
 */
void register_connect_cb(connectCallback callback){
    // ★ 将回调注册到 QuaModule，由其管理调用时机
    t_QuaModule->register_conn_respose(callback);
}

/**
 * @brief ★ 注册「MQTT 消息接收」回调 — 这是云→设备 的核心入口
 *
 * 当 QuaModule 从 MQTT Broker 收到消息时，会调用此回调
 * 当前项目注册的是 mqtt_subscriber()（mqtt_subscriber.c 中的核心函数）
 * 该函数解析 JSON 消息 → 判断类型(模板更新/重启/WiFi切换等) → 投递到 LVGL 线程更新 UI
 *
 * @param callback 消息接收回调函数指针
 */
void register_repose_cb(respose callback){
    // ★ 将消息回调注册到 QuaModule
    t_QuaModule->register_respose(callback);
}

/**
 * @brief ★ 销毁 MQTT 连接 — 断开与 Broker 的连接并释放资源
 *
 * 通常在程序退出或需要重连时调用
 */
void destroy_mqtt(void){
    if( t_QuaModule != NULL && t_QuaModule->deinit != NULL ){
      t_QuaModule->deinit();   // 发送 MQTT DISCONNECT → 关闭 TCP → 释放内存
    }
}

/**
 * @brief ★ 向 MQTT Broker 发送消息（设备 → 云平台）
 *
 * 用于设备主动上报状态，例如：
 *   - 上报屏幕已收到模板（msg_type=1/2/3 对应收到/转发/确认）
 *   - 上报设备在线状态
 *   - 上报其他业务数据
 *
 * 数据流：msg0,msg1 → 组装 AiMessage → QuaModule.input_content() →
 *         QuaModule.send_cmd() → MQTT PUBLISH → Broker
 *
 * @param msg_type 消息类型编号
 * @param msg0     消息数据字段1
 * @param msg1     消息数据字段2
 */
void send_message_to_server(int msg_type,char* msg0 ,char* msg1){
    AiMessage msg = {0};         // AiMessage 是 MQTT 模块内部的消息结构体
    msg.val = msg_type;          // val：消息类型
    msg.data1 =msg0;             // data1：数据字段1
    msg.data2 =msg1;             // data2：数据字段2
    t_QuaModule->input_content(&msg);  // 将消息放入发送队列
    t_QuaModule->send_cmd(&msg);       // 触发实际发送 → MQTT PUBLISH
}
#endif