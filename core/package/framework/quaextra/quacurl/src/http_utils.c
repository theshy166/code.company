#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <curl/curl.h>
#include <sys/stat.h>
#include <sys/vfs.h>
#include "http_utils.h"

#define RESOURCE_LIMIT_RATE 0.2f

int is_png(const unsigned char *data) {
    return data[0] == 0x89 && data[1] == 0x50 && data[2] == 0x4E && data[3] == 0x47 &&
           data[4] == 0x0D && data[5] == 0x0A && data[6] == 0x1A && data[7] == 0x0A;
}

int is_jpg(const unsigned char *data) {
    return data[0] == 0xFF && data[1] == 0xD8 && data[2] == 0xFF;
}

static size_t _write_memory_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    MemoryStruct *mem = (MemoryStruct *)userp;

    char *ptr = realloc(mem->data, mem->size + realsize + 1);
    if(ptr == NULL) {
        // 内存分配失败
        printf("内存分配失败\n");
        return 0;
    }

    mem->data = ptr;
    memcpy(&(mem->data[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->data[mem->size] = '\0';

    return realsize;
}

size_t write_data(void *contents, size_t size, size_t nmemb, void *userp) {
    struct response_data *data = (struct response_data *)userp;
    size_t realsize = size * nmemb;
    //printf("realsize %d. data->data=%p\n", realsize, data->data);

    char *ptr = realloc(data->data, data->size + realsize + 1);
    //char *ptr = malloc(data->size + realsize + 1);
    if(ptr == NULL) {
        // 内存分配失败
        printf("内存分配失败\n");
        if (data->data) {
            free(data->data);
            data->data = NULL;
        }
        data->size = 0;
        return 0;
    }
    //printf("ptr %p , %s\n", ptr, ptr);
    data->data = ptr;
    memcpy(&(data->data[data->size]), contents, realsize);
    data->size += realsize;
    data->data[data->size] = '\0';
    //printf("realsize %d. data->data=%p\n", realsize, data->data);

    return realsize;

}

char * get_remote_url(const char *url) 
{
	struct response_data data = {NULL, 0};
	get_data_from_url(url, &data.data, &data.size);
	return data.data;
}

int get_data_from_url(const char *url, char ** ppData, size_t * pDataLen)
{
    if (!url) {
        printf("Invalid URL\n");
        return -1;
    }
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
        curl_easy_cleanup(curl);
        return res;
    }else{
		if(ppData != NULL){
			*ppData = data.data;
		}
		if(pDataLen != NULL){
			*pDataLen = data.size;
		}
    }
    // 清理资源
    curl_easy_cleanup(curl);
    return res;
}

char * post_remote_url(const char *url, const char *post_data) 
{
    if (!url || !post_data) {
        fprintf(stderr, "无效的URL或POST数据\n");
        return NULL;
    }
    CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Curl initialization failed\n");
        return NULL;
    }
    struct response_data data = {NULL, 0};
    struct curl_slist *headers = NULL;
    CURLcode res;
    char *result = NULL;

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
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(post_data));
    // 设置回调函数，用于处理接收到的数据
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    // 将缓冲区指针传递给回调函数
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
    // 执行请求
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "POST请求失败: %s\n", curl_easy_strerror(res));
        goto cleanup;
    }

    result = data.data;

cleanup:
    if (headers) {
        curl_slist_free_all(headers);
    }
    if (curl) {
        curl_easy_cleanup(curl);
    }
    if (!result && data.data) {
        free(data.data);
    }
    return result;
}

int download_image(const char *url, const char *filePath){
    printf("download image recv params url :%s ,filepath :%s",url,filePath);
    CURL *curl;
    CURLcode res;
    MemoryStruct chunk = {.data = NULL,.size = 0};

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if(curl) {
        // 设置要获取的 URL
        curl_easy_setopt(curl, CURLOPT_URL, url);
        // 设置请求方式为 GET
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
        // 设置ssl证书
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        // curl_easy_setopt(curl, CURLOPT_CAINFO,K_pem_path);
        // curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
        // curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
    // 检查URL是否为HTTPS
    if(strncmp(url, "https://", 8) == 0) {
        // 设置SSL验证选项
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    }

        //printf("K_pem_path ：%s\n", K_pem_path);
        // 忽略SSL证书
        // curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, true);
        // 设置接收数据的回调函数
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _write_memory_callback);
        // 将 MemoryStruct 结构体传递给回调函数
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
        // 执行请求
        res = curl_easy_perform(curl);
        if(res!= CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            return 0;
        }
        // 保存数据到本地文件
        #define WRITE_BUFFER_SIZE 10 * 1024 // 10K 缓冲区大小
        FILE *fp = fopen(filePath, "wb");
        if(fp) {
            size_t remaining = chunk.size;
            size_t offset = 0;
            while (remaining > 0) {
                size_t write_size = (remaining > WRITE_BUFFER_SIZE) ? WRITE_BUFFER_SIZE : remaining;
                fwrite(chunk.data + offset, 1, write_size, fp);
                offset += write_size;
                remaining -= write_size;
            }
            fclose(fp);
        } else {
            printf("无法打开文件进行写入\n");
            return 0;
        }

        // 释放内存
        free(chunk.data);
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();

    FILE *file = fopen(filePath, "rb");
    if (!file) {
        perror("Failed to open downloaded image");
        return 0;
    }
    unsigned char header[8];
    fread(header, 1, 8, file);
    fclose(file);
    int actual_format = 0; // 1 for PNG, 2 for JPG
    if (is_png(header)) {
        actual_format = 1;
    } else if (is_jpg(header)) {
        actual_format = 2;
    }

    // Determine the expected format based on file extension
    const char *ext = strrchr(filePath, '.');
    int expected_format = 0; // 1 for PNG, 2 for JPG
    if (ext && (strcmp(ext, ".png") == 0 || strcmp(ext, ".PNG") == 0)) {
        expected_format = 1;
    } else if (ext && (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0 || strcmp(ext, ".JPG") == 0 || strcmp(ext, ".JPEG") == 0)) {
        expected_format = 2;
    }

    // If formats do not match, convert the image
    /* if (actual_format != expected_format) {
        char temp_path[256];
        snprintf(temp_path, sizeof(temp_path), "%s.temp", filePath);

        if (actual_format == 1 && expected_format == 2) {
            _convert_png_to_jpg(filePath, temp_path);
            printf("_convert_png_to_jpg \n");
        } else if (actual_format == 2 && expected_format == 1) {
            _convert_jpg_to_png(filePath, temp_path);
            printf("_convert_jpg_to_png \n");
        }

        // Replace the original file with the converted file
        remove(filePath);
        rename(temp_path, filePath);
    } */
    if (expected_format == 1 && actual_format == 2) {
        char jpg_path[256];
        strncpy(jpg_path, filePath, ext - filePath);
        jpg_path[ext - filePath] = '\0';
        strcat(jpg_path, ".jpg");
        FILE *src = fopen(filePath, "rb");
        FILE *dst = fopen(jpg_path, "wb");
        if (src && dst) {
            char buffer[1024];
            size_t bytes;
            while ((bytes = fread(buffer, 1, sizeof(buffer), src)) > 0) {
                fwrite(buffer, 1, bytes, dst);
            }
            fclose(src);
            fclose(dst);
            printf("Copied %s to %s\n", filePath, jpg_path);
        } else {
            perror("Failed to copy file");
        }
    }else if(expected_format == 2 && actual_format == 1){
        char png_path[256];
        strncpy(png_path, filePath, ext - filePath);
        png_path[ext - filePath] = '\0';
        strcat(png_path, ".png");
        FILE *src = fopen(filePath, "rb");
        FILE *dst = fopen(png_path, "wb");
        if (src && dst) {
            char buffer[1024];
            size_t bytes;
            while ((bytes = fread(buffer, 1, sizeof(buffer), src)) > 0) {
                fwrite(buffer, 1, bytes, dst);
            }
            fclose(src);
            fclose(dst);
            printf("Copied %s to %s\n", filePath, png_path);
        } else {
            perror("Failed to copy file");
        }
    }
    return 1;
}

static int file_name_index = 0;
// 回调函数，将数据写入文件
static size_t _write_memory_callback_v2(void *contents, size_t size, size_t nmemb, void *userp) {
    FILE *fp = (FILE *)userp;
    size_t written = fwrite(contents, size, nmemb, fp);
    if (written != size * nmemb) {
        fprintf(stderr, "文件写入错误，预期写入 %zu 字节，实际写入 %zu 字节\n", size * nmemb, written);
    }
    return written;
}

int download_image_by_segment(const char *url, const char *filePath) {
    CURL *curl_handle;
    CURLcode res;
    FILE *fp;
    double downloaded_size = 0.0;
    long start = 0;
    double total_size = 0.0;
    char temp_filePath[256];
    long chunk_size = 500*1024;
    // Create temporary file path with .download extension
    file_name_index ++;
    snprintf(temp_filePath, sizeof(temp_filePath), "%s_%d.download", filePath,file_name_index);

    curl_global_init(CURL_GLOBAL_ALL);
    printf("下载图片: %s\n", url);

    // 获取文件总大小
    curl_handle = curl_easy_init();
    if (curl_handle) {
        curl_easy_setopt(curl_handle, CURLOPT_URL, url);
        curl_easy_setopt(curl_handle, CURLOPT_NOBODY, 1L); // 只获取头部
        curl_easy_setopt(curl_handle, CURLOPT_HEADER, 1L);
        res = curl_easy_perform(curl_handle);
        if (res == CURLE_OK) {
            curl_easy_getinfo(curl_handle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &total_size);
            if (total_size <= 0) {
                fprintf(stderr, "无法获取文件大小\n");
                curl_easy_cleanup(curl_handle);
                curl_global_cleanup();
                return 0;
            }
        } else {
            fprintf(stderr, "curl_easy_perform() 失败: %s\n", curl_easy_strerror(res));
            curl_easy_cleanup(curl_handle);
            curl_global_cleanup();
            return 0;
        }
        curl_easy_cleanup(curl_handle);
    } else {
        curl_global_cleanup();
        return 0;
    }

    // Check if the file exists and clear it if it does
    if ((fp = fopen(temp_filePath, "r")) != NULL) {
        fclose(fp);
        fp = fopen(temp_filePath, "wb");
        if (fp) {
            fclose(fp);
        } else {
            fprintf(stderr, "无法打开文件进行清空: %s\n", temp_filePath);
            curl_global_cleanup();
            return 0;
        }
    }

    while (start < total_size) {
        curl_handle = curl_easy_init();

        if (curl_handle) {
            fp = fopen(temp_filePath, "ab+"); // 以追加模式打开文件
            if (!fp) {
                curl_easy_cleanup(curl_handle);
                curl_global_cleanup();
                return 0;
            }

            curl_easy_setopt(curl_handle, CURLOPT_URL, url);
            curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, _write_memory_callback_v2);
            curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)fp);

            // 设置分段下载的范围
            char range[64];
            if (start + chunk_size - 1 > total_size) {
                snprintf(range, sizeof(range), "%ld-", start);
            } else {
                snprintf(range, sizeof(range), "%ld-%ld", start, start + chunk_size - 1);
            }
            curl_easy_setopt(curl_handle, CURLOPT_RANGE, range);

            res = curl_easy_perform(curl_handle);

            if (res == CURLE_OK) {
                // 获取下载的字节数
                curl_easy_getinfo(curl_handle, CURLINFO_SIZE_DOWNLOAD, &downloaded_size);
                start += (long)downloaded_size;
                printf("已下载 %ld 字节\n", (long)downloaded_size);
            } else {
                fprintf(stderr, "curl_easy_perform() 失败: %s\n", curl_easy_strerror(res));
                fclose(fp);
                curl_easy_cleanup(curl_handle);
                curl_global_cleanup();
                return 0;
            }

            // 清理
            fclose(fp);
            curl_easy_cleanup(curl_handle);
        } else {
            curl_global_cleanup();
            return 0;
        }
    }

    // 检查文件大小是否与预期一致
    fp = fopen(temp_filePath, "rb");
    if (fp) {
        fseek(fp, 0, SEEK_END);
        long file_size = ftell(fp);
        fclose(fp);
        if (file_size != (long)total_size) {
            fprintf(stderr, "文件大小不一致，预期: %ld, 实际: %ld\n", (long)total_size, file_size);
            return 0;
        }
    } else {
        fprintf(stderr, "无法打开文件进行检查: %s\n", temp_filePath);
        return 0;
    }

    curl_global_cleanup();

    // Rename the temporary file to the final file name
    if (rename(temp_filePath, filePath) != 0) {
        perror("Failed to rename the downloaded file");
        return 0;
    }

    return 1;
}