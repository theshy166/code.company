#ifndef _HTTP_UTILS_H
#define _HTTP_UTILS_H
struct response_data {
    char* data;
    size_t size;
};

typedef struct {
    char *data;
    size_t size;
} MemoryStruct;

size_t write_data(void *contents, size_t size, size_t nmemb, void *userp); //default write_back

//get请求远程服务
char * get_remote_url(const char *url);
int get_data_from_url(const char *url, char ** ppData, size_t * pDataLen);
//post请求远程服务
char * post_remote_url(const char *url, const char *post_data);
//下载图片并且保存在本地
int download_image(const char *url, const char *filePath);

int download_image_by_segment(const char *url, const char *filePath);
//判读字节是否是png
int is_png(const unsigned char *data);
//判断字节是否是jpeg
int is_jpg(const unsigned char *data);
#endif //HTTP_UTILS_H