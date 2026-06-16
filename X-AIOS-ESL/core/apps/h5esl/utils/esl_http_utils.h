#ifndef _HTTP_ESL_UTILS_H
#define _HTTP_ESL_UTILS_H

//分段下载图片
int download_image_segment(const char *url, const char *filePath,long chunk_size,int type,int index);
#endif //HTTP_UTILS_H