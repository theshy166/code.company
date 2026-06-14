
#include <common.h>
#include <command.h>
#include <malloc.h>
#include <linux/string.h>
#include <stdlib.h>

extern int   env_set(const char *varname, const char *varvalue);
extern char *env_get(const char *name);

// 假设这里有一个函数来设置 lcm 参数
void qua_set_lcm_bootargs(int value) {
    char lcm_str[10] = {0};
    char *bootargs = env_get("bootargs");
    char *new_bootargs = NULL;
    size_t new_bootargs_len = 0;

    // 构建 lcm 参数的字符串
    snprintf(lcm_str, sizeof(lcm_str), "lcm=%d", value);

    if (bootargs) {
        // 检查原 bootargs 中是否已经有 lcm 参数
        char *pos = strstr(bootargs, "lcm=");
        if (pos) {
            // 找到 lcm 参数，修改其值
            char *end = strchr(pos, ' ');
            if (end) {
                // lcm 参数后面还有其他参数
                new_bootargs_len = strlen(bootargs) - (end - pos) + strlen(lcm_str);
                new_bootargs = (char *)malloc(new_bootargs_len + 1);

                if (new_bootargs) {
                    memset(new_bootargs, 0, new_bootargs_len + 1);
                    strncpy(new_bootargs, bootargs, pos - bootargs);
                    new_bootargs[pos - bootargs] = '\0';
                    strcat(new_bootargs, lcm_str);
                    strcat(new_bootargs, end);
                }
            } else {
                // lcm 参数是最后一个参数
                new_bootargs_len = strlen(bootargs) - strlen(strstr(bootargs, "lcm=")) + strlen(lcm_str);
                new_bootargs = (char *)malloc(new_bootargs_len + 1);

                if (new_bootargs) {
                    memset(new_bootargs, 0, new_bootargs_len + 1);
                    strncpy(new_bootargs, bootargs, pos - bootargs);
                    new_bootargs[pos - bootargs] = '\0';
                    strcat(new_bootargs, lcm_str);
                }
            }
        } else {
            // 原 bootargs 中没有 lcm 参数，追加该参数
            new_bootargs_len = strlen(bootargs) + strlen(lcm_str) + 1 + 1;
            new_bootargs = (char *)malloc(new_bootargs_len);

            if (new_bootargs) {
                memset(new_bootargs, 0, new_bootargs_len);
                snprintf(new_bootargs, new_bootargs_len, "%s %s", bootargs, lcm_str);
                new_bootargs[new_bootargs_len - 1] = '\0';
            }
        }
    } else {
        // 原 bootargs 为空，直接设置为 lcm 参数
        new_bootargs_len = strlen(lcm_str);
        new_bootargs = (char *)malloc(new_bootargs_len + 1);

        if (new_bootargs) {
            memset(new_bootargs, 0, new_bootargs_len + 1);
            strcpy(new_bootargs, lcm_str);
        }
    }

    if (new_bootargs) {
        // 设置新的 bootargs 环境变量
        // setenv("bootargs", new_bootargs);
        env_set("bootargs", new_bootargs);
        // 释放动态分配的内存
        printf("%s bootargs=%s.\n", __func__, new_bootargs);
        free(new_bootargs);
    }
}
