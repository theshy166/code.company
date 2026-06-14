#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <curl/curl.h>
#include "qm_http_utils.h"
#include "http_utils.h" //write_data, response_data

int qm_http_post(const char *url, const char *post_data, int post_data_size, struct curl_slist *headers, qm_http_response_cb_t response_data_cb)
{
    if (!url || !post_data) {
        fprintf(stderr, "无效的URL或POST数据\n");
        return -1;
    }
    CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Curl initialization failed\n");
        return -1;
    }
    // struct response_data data = {NULL, 0};
    // struct curl_slist *headers = NULL;
    CURLcode res;
    // char *result = NULL;

    headers = curl_slist_append(headers, "Content-Type: application/json");
    if (!headers) {
        fprintf(stderr, "创建请求头失败\n");
        goto cleanup;
    }
    // 设置要请求的 URL
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    // 设置请求头
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    // 设置 POST 数据
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, post_data_size);
    // 设置 SSL 选项
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L); // 验证对等方证书
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L); // 验证主机名
    // 设置回调函数，用于处理接收到的数据
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, response_data_cb);
    // 将缓冲区指针传递给回调函数
    // curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
    // 执行请求
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "POST请求失败: %s\n", curl_easy_strerror(res));
        goto cleanup;
    }

    // result = data.data;

cleanup:
    // headers not free here, it's caller's responsibility
    // if (headers) {
    //     curl_slist_free_all(headers);
    // }
    if (curl) {
        curl_easy_cleanup(curl);
    }
    // if (!result && data.data) {
    //     free(data.data);
    // }
    return 0;
}

int qm_http_get(const char *url, struct curl_slist *headers, char ** ppData, size_t * pDataLen)
{
    CURL *curl = curl_easy_init();
    if (!curl) {
        printf("Curl initialization failed\n");
        return -1;
    }

    struct response_data data = {NULL, 0};

    // 设置要获取的 URL
    curl_easy_setopt(curl, CURLOPT_URL, url);
    // 设置回调函数，用于处理接收到的数据
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    // 设置请求头
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    // 将缓冲区指针传递给回调函数
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);

    // 检查URL是否为HTTPS
    if(strncmp(url, "https://", 8) == 0) {
        // 设置SSL验证选项
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    }
    
    // 执行请求
    CURLcode res = curl_easy_perform(curl);
    if (res!= CURLE_OK) {
        fprintf(stderr, "get_remote_url curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        if (data.data) {
            free(data.data);
            data.data = NULL;
        }
        goto cleanup;
    }else{
		if(ppData != NULL){
			*ppData = data.data;
		}
		if(pDataLen != NULL){
			*pDataLen = data.size;
		}
    }
    // 清理资源
cleanup:
    if (curl) {
        curl_easy_cleanup(curl);
    }
    return res;
}

int qm_http_post_raw(const char* url, const char* protocol, 
            int connect_timeout, int timeout,
            struct curl_slist *headers, 
            const char *post_data, int post_data_size,
            char** pResult, int * pResultLen,  char ** ppContentType, 
            qm_http_response_cb_t response_data_cb, void * user_data, 
            curl_xferinfo_callback xferinfo_cb,  void * xferinfo_cb_data)

{
    CURL* curl;
    CURLcode res = CURLE_OK;
    curl = curl_easy_init();
    if (curl) {
        //set connect timeout
        if(connect_timeout > 0){
            curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, connect_timeout); 
        }
        if(timeout > 0){
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout); 
        }
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        if(protocol != NULL){
            curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, protocol);
        }
        // 检查URL是否为HTTPS
        if(strncmp(url, "https://", 8) == 0) {
            // 设置SSL验证选项
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);  // 不验证证书
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);  // 不验证主机名
        }

        struct response_data res_data = {0, NULL};

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, post_data_size);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, user_data != NULL ? user_data : &res_data);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, response_data_cb != NULL ? response_data_cb : write_data);

        if(xferinfo_cb!= NULL){
            // Enable the progress callback
            curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
            // Set the progress callback function
            curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, xferinfo_cb);
            curl_easy_setopt(curl, CURLOPT_XFERINFODATA, xferinfo_cb_data);
        }

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            printf("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            if(res_data.data!= NULL){
                free(res_data.data);
                res_data.data = NULL;
            }
        }else{
            if(pResult!= NULL && user_data == NULL){
                *pResult = res_data.data;
            }
            if(pResultLen!= NULL && user_data == NULL){
                *pResultLen = res_data.size; 
            }
            if(ppContentType != NULL){
                char * contentType = NULL;
                res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &contentType); 
                if (res == CURLE_OK) {
                    *ppContentType = strdup(contentType); 
                }else{
                    printf("curl_easy_getinfo() failed: %s\n", curl_easy_strerror(res)); 
                }
            }
        }
    }

    if(curl){
        curl_easy_cleanup(curl);
    }
    return res;
}