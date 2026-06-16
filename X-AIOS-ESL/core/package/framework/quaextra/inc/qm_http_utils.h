#ifndef __QM_HTTP_UTILS_H__
#define __QM_HTTP_UTILS_H__

#include "curl/curl.h"

typedef size_t (*qm_http_response_cb_t)(void *contents, size_t size, size_t nmemb, void *userp);

//** attention: headers should be free by caller, it's NOT freed in this function */
int qm_http_post(const char *url, const char *post_data, int post_data_size, struct curl_slist *headers, qm_http_response_cb_t response_data_cb);

int qm_http_get(const char *url, struct curl_slist *headers, char ** ppData, size_t * pDataLen);

//** attention: headers should be free by caller, it's NOT freed in this function */
// protocol: NULL/"http"/"https"/"undefined"
// connect_timeout: 0 means no timeout, others means timeout in seconds, default is 0
// timeout: 0 means no timeout, others means timeout in seconds, default is 0
// if user_data is set, caller need to handle the *pResult/*pResultLen; or keep user_data just NULL
int qm_http_post_raw(const char* url, const char* protocol, 
            int connect_timeout, int timeout,
            struct curl_slist *headers, 
            const char *post_data, int post_data_size,
            char** pResult, int * pResultLen,  char ** ppContentType, 
            qm_http_response_cb_t response_data_cb, void * user_data, 
            curl_xferinfo_callback xferinfo_cb,  void * xferinfo_cb_data);
#endif
