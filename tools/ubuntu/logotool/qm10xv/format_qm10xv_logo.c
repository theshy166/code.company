#include <stdio.h>
#include <stdlib.h>
#include "logo_rgb565_240x320.h"

// 交换两个字节的顺序（辅助函数）
void swapTwoBytes(char *a, char *b)
{
    char temp = *a;
    *a = *b;
    *b = temp;
}

int main()
{
    int numElements = sizeof(res_rgb565_240x320_map) / sizeof(res_rgb565_240x320_map[0]);

    for (int i = 0; i < numElements - 1; i += 2)
    {
        swapTwoBytes(&res_rgb565_240x320_map[i], &res_rgb565_240x320_map[i + 1]);
    }

    FILE *outputFile = fopen("logo_rgb565_240x320.c", "w");
    if (outputFile == NULL)
    {
        perror("无法打开输出文件");
        return 1;
    }

    fprintf(outputFile, "const unsigned char res_rgb565_240x320_map[] = {\n");
    int count = 0;
    for (int i = 0; i < numElements; i++)
    {
        if (count % 480 == 0 && count != 0)
        {
            fprintf(outputFile, "\n");
        }
        fprintf(outputFile, " 0x%02x,", (unsigned char)res_rgb565_240x320_map[i]);
        count++;
    }
    fprintf(outputFile, "\n};\n");

    fclose(outputFile);

    return 0;
}