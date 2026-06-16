#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <curl/curl.h>
#include <sys/stat.h>
#include <sys/vfs.h>
#include "esl_http_utils.h"
#include "http_utils.h"
#include "../view/page_esl.h"

#define RESOURCE_LIMIT_RATE 0.2f
static int file_name_index = 0;

static int is_enough_space(double size){
    struct statfs diskInfo;
    statfs("/data", &diskInfo);
    unsigned long long totalBlocks = diskInfo.f_bsize;
    unsigned long long totalSize = totalBlocks * diskInfo.f_blocks;
    unsigned long long freeDisk = diskInfo.f_bfree*totalBlocks;
    printf ("/data ===============is_enough_space================  total=%d, free=%d\n", totalSize, freeDisk);
    unsigned long long mbFreedisk_base = totalSize * RESOURCE_LIMIT_RATE;
    if ((freeDisk - mbFreedisk_base)< size){
        return 0;
    }else{
        return 1;
    }
}

// 回调函数，将数据写入文件
static size_t _write_memory_callback_v2(void *contents, size_t size, size_t nmemb, void *userp) {
    FILE *fp = (FILE *)userp;
    size_t written = fwrite(contents, size, nmemb, fp);
    if (written != size * nmemb) {
        fprintf(stderr, "文件写入错误，预期写入 %zu 字节，实际写入 %zu 字节\n", size * nmemb, written);
    }
    return written;
}

int download_image_segment(const char *url, const char *filePath, long chunk_size,int type,int index) {
    CURL *curl_handle;
    CURLcode res;
    FILE *fp;
    double downloaded_size = 0.0;
    long start = 0;
    double total_size = 0.0;
    char temp_filePath[256];

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

    // 判断当前磁盘空间是否足够
    printf ("===============filePath:%s================  \n", filePath);
    if(type == 2){
        if (!is_enough_space(total_size)) {
            fprintf(stderr, "磁盘空间不足\n");
            return 0;
        }
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
                set_bar_value(index,100);
            } else {
                snprintf(range, sizeof(range), "%ld-%ld", start, start + chunk_size - 1);
                set_bar_value(index,(int)((start + chunk_size) * 100 / total_size));
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
        usleep(10 * 1000); // 休眠10毫秒
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
    } else if (expected_format == 2 && actual_format == 1) {
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