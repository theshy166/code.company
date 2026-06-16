#ifndef __ESL_PUSH_SERVER_H__
#define __ESL_PUSH_SERVER_H__

typedef struct {
    char *update_version;
    char *update_url;
    char *update_file_md5;
    char *boot_logo_url;
    char *boot_logo_md5;
    char *bind_logo_url;
    char *bind_logo_md5;
    char *host;
    int port;
    char *username;
    char *password;
    char *protocol;
    char *client_id;
    char *topic;
    int master;
    int group_id;
    int query_success;// 0:失败 1:成功 -1:未绑定
} esl_push_server_data;


typedef struct {
    int id;
    char *name;
} group_info;

typedef struct {
    char *host;
    int port;
    char *username;
    char *password;
    char *client_id;
    char *topic;
} mqtt_info;

typedef struct {
    char *host;
    int port;
    char *username;
    char *password;
} amqp_info;

typedef struct {
    char *url;
    char *md5;
} logo_info;

typedef struct {
    char *name;
    char *password;
} wifi_info;

typedef struct {
    char *version;
    char *url;
    int size;
    char *md5;
} version_info;

typedef struct {
    int master;
    group_info group;
    mqtt_info mqtt;
    amqp_info amqp;
    logo_info bootLogo;
    wifi_info wifi;
    version_info versionInfo;
    int query_success;// 0:失败 1:成功 -1:未绑定
} esl_push_server_data2;

#endif // __ESL_PUSH_SERVER_H__