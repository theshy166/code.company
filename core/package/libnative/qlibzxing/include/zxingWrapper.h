#ifndef ZXING_WRAPPER_H
#define ZXING_WRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif

// 定义解码结果类型
typedef struct {
    char text[256];       // 解码的文本
    int success;      // 是否成功解码（1: 成功, 0: 失败）
} ZXingResult,*pZXingResult;

// 解码二维码文件
int zxing_decode_file(unsigned char* data, int len, pZXingResult result);

int genarate_128code(char* content, unsigned int height, char* path);


#ifdef __cplusplus
}
#endif

#endif // ZXING_WRAPPER_H

