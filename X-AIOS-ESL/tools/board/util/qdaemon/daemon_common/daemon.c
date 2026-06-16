#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <arpa/inet.h>
#undef _BSD_SOURCE    // 防止与 <net/if.h> 冲突
#include <net/if.h>    // 包含 if_indextoname 等函数
#include <linux/if.h>  // 包含内核接口定义
#include <stdbool.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/watchdog.h>
#include <dlfcn.h>
#include <pthread.h> // 新增线程头文件
#include <dirent.h>
#include <ctype.h>
#include <sys/stat.h>

int get_interface_flags(const char *ifname);
int get_default_gateway(char *gateway, size_t len);
int get_interface_ip(const char *ifname, char *ip_buf, size_t len);

#define QXOSNAME "qxosui"
#define XOS_WATCHDOG_ENABLE 1
#define XOS_ZRAM_WRITEBACK  1

#ifndef RTNLGRP_LINK
#define RTNLGRP_LINK RTMGRP_LINK
#endif
#define BUFFER_SIZE 8192
#define INTERFACE_NAME "wlan0"
#define SOCKET_RCV_TIMEOUT 2
int g_last_status = 0;
// 状态标志位（使用枚举提高可读性）
typedef enum {
    WLAN_STATUS_NONE      = 0x00,
    WLAN_UP              = 0x01,    // 接口已启用
    WLAN_HAS_CARRIER     = 0x02,    // 物理层连接
    WLAN_HAS_IP          = 0x04     // 有 IPv4 地址
} WifiStatusFlags;

#ifndef RTNLGRP_LINK
#define RTNLGRP_LINK RTMGRP_LINK
#endif

#define PATH_BUFF_SIZE 32    // 路径缓冲区（足够存储 /proc/99999/cmdline）
#define CMDLINE_BUFF_SIZE 256 // 命令行缓冲区（匹配短进程名场景）

// #define TRIGGER_COMPACTION  "echo 1 > /proc/sys/vm/compact_memory"
// #define DROP_CACHE      "sync && echo 3 > /proc/sys/vm/drop_caches"
#define MDEV_TRIGGER        "echo /sbin/mdev > /proc/sys/kernel/hotplug"
#define DAEMON_CALLBACK  "/data/.daemon_callback"
#define DAEMON_SDCARD_CALLBACK  "/sdcard/.sdcard_callback"
#define DAEMON_MMC_CALLBACK  "/udisk/.udisk_callback"

#define TIME_VALID_STAMP        1740485110

#if XOS_ZRAM_WRITEBACK
static int bdev_config_max_pages = -1;
#define CONFIG_FILE "/config/param.ini"
#define WRITEBACK_DEBUG_CLOSE "/data/.nowriteback"
#define BD_MAX_PAGES  2800
#define BD_TRIGGER_THRESHOLD  (BD_MAX_PAGES * 3 /4)
#define MEM_TRIGGER_THRESHOLD 6000 /* avialiable mem < 6M */
#endif

int is_process_running(const char *process_name) {
    DIR *dir;
    struct dirent *ent;
    char path[PATH_BUFF_SIZE];      // 缩小至 32 字节
    char cmdline[CMDLINE_BUFF_SIZE]; // 缩小至 256 字节
    FILE *fp;

    dir = opendir("/proc");
    if (dir == NULL) {
        perror("opendir /proc failed");
        return 0;
    }

    while ((ent = readdir(dir)) != NULL) {
        if (!isdigit(ent->d_name[0])) {
            continue;
        }

        // 确保路径拼接安全（使用 snprintf 限制长度）
        snprintf(path, sizeof(path), "/proc/%s/cmdline", ent->d_name);
        fp = fopen(path, "r");
        if (fp == NULL) {
            continue;
        }

        // 读取命令行参数（限制读取长度，避免缓冲区溢出）
        if (fgets(cmdline, sizeof(cmdline), fp) != NULL) {
            cmdline[strcspn(cmdline, "\n")] = '\0'; // 处理换行符

            if (strstr(cmdline, process_name) != NULL) {
		        //printf("is_process_running cmdline: %s\n",cmdline);
                fclose(fp);
                closedir(dir);
                return 1;
            }
        }
        fclose(fp);
    }

    closedir(dir);
    return 0;
}

#if XOS_WATCHDOG_ENABLE
#define WATCHDOG_DEV "/dev/watchdog"
static int wd_fd = -1;
static unsigned int wd_timeout = 30;  // 默认超时30秒
static pid_t qxosui_pid = -1;

int is_process_alive(pid_t pid) {
    if (kill(pid, 0) == 0) {
        return 1;  // 进程存在且有权限访问
    } else {
        // 检查错误码
        if (errno == ESRCH) {
            return 0;  // 进程不存在
        } else if (errno == EPERM) {
               return -1;  // 权限不足，无法确定进程状态
        } else {
            return -1;  // 其他错误
        }
    }
}

pid_t find_qxosui_pid() {
    DIR* dir;
    struct dirent* entry;
    char path[1024];
    FILE* fp;
    char buffer[512];

    dir = opendir("/proc");
    if (!dir) return -1;

    while ((entry = readdir(dir)) != NULL) {
        // 检查条目是否为数字（进程ID）
        if (atoi(entry->d_name) <= 0) continue;

        snprintf(path, sizeof(path), "/proc/%s/comm", entry->d_name);
        fp = fopen(path, "r");
        if (fp) {
            if (fgets(buffer, sizeof(buffer), fp) != NULL) {
                // 移除换行符
                buffer[strcspn(buffer, "\n")] = 0;
                if (strcmp(buffer, QXOSNAME) == 0) {
                    pid_t pid = atoi(entry->d_name);
                    fclose(fp);
                    closedir(dir);
                    return pid;
                }
            }
            fclose(fp);
        }
    }

    closedir(dir);
    return -1;  // 未找到
}

bool is_qxosui_exist(void)
{
    if (qxosui_pid > 0) {
        // 已有缓存的PID，直接检查是否存活
        if (is_process_alive(qxosui_pid)) {
            return true;
        } else {
            // 进程已退出，重置PID
            qxosui_pid = -1;
        }
    } else {
        // 无缓存或进程不存在，重新查找
        qxosui_pid = find_qxosui_pid();
        if (qxosui_pid > 0) {
            return true;
        }
    }
    return false;
}

// 新增喂狗线程函数
static void* watchdog_feeder(void *arg) {
    (void)arg; // 避免未使用参数警告
    while(wd_fd != -1) { // 当看门狗打开时持续运行
	#ifdef BIND_WDT2QXOSUI
	if(is_qxosui_exist() || (access("/tmp/.NO_BIND_WDT2QXOSUI", F_OK) == 0))
	{
        feed_watchdog();
	}
	else
	{
        perror("qxosui is not exist, don't feed watchdog\n");
	}
	#else
	feed_watchdog();
	#endif
        sleep(5); // 每5秒喂一次狗
    }
    return NULL;
}
int init_watchdog()
{
    // 打开看门狗设备
    wd_fd = open(WATCHDOG_DEV, O_WRONLY);
    if (wd_fd == -1) {
        perror("open watchdog\n");
        return -1;
    }

    // 设置超时时间
    if (ioctl(wd_fd, WDIOC_SETTIMEOUT, &wd_timeout)) {
        perror("set timeout\n");
        close(wd_fd);
        return -1;
    }

    // // 验证超时设置
    // int actual_timeout;
    // if (ioctl(wd_fd, WDIOC_GETTIMEOUT, &actual_timeout)) {
    //     perror("get timeout\n");
    //     close(wd_fd);
    //     return -1;
    // }
    printf("Watchdog timeout set to %d seconds\n", wd_timeout);
    pthread_t feeder_tid;
    if(pthread_create(&feeder_tid, NULL, watchdog_feeder, NULL) != 0) {
        fprintf(stderr, "Failed to create watchdog feeder thread\n");
        close_watchdog();
        return -1;
    }
    pthread_detach(feeder_tid); // 分离线程
    return 0;
}

void feed_watchdog()
{
    //struct timeval tv = { 0 };
    //if (gettimeofday(&tv, NULL) == -1) {
    //     printf("gettimeofday fail to get time");
    //     return;
    //}

    if (wd_fd != -1) {
        ioctl(wd_fd, WDIOC_KEEPALIVE, 0);
        //printf("Watchdog feed timestamp: %lds %ldus\n",
        //    tv.tv_sec, tv.tv_usec);  // 新增时间戳打印
    }
}

void close_watchdog()
{
    if (wd_fd != -1) {
        write(wd_fd, "V", 1); // 发送关闭命令
        close(wd_fd);
        wd_fd = -1;
    }
}

#endif

// 执行系统命令并返回输出结果
char *execute_command(const char *command)
{
    FILE *pipe = popen(command, "r");
    if (!pipe) {
        perror("popen");
        return NULL;
    }

    char buffer[1024];
    char *output = NULL;
    size_t output_size = 0;

    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        size_t len = strlen(buffer);
        char *temp = realloc(output, output_size + len + 1);
        if (temp == NULL) {
            printf("realloc failed, current output size: %zu, trying to add %zu bytes\n", output_size, len);
            pclose(pipe);
            free(output);
            return NULL;
        }
        output = temp;
        if (output_size == 0) {
            output[0] = '\0';  // 初始化字符串
        }
        strncat(output, buffer, len);
        output_size += len;
    }

    pclose(pipe);
    return output;
}

// 杀死进程
void kill_process(const char *process_name)
{
    char command[256];
    snprintf(command, sizeof(command), "pkill -9  %s", process_name);
    if (system(command) != 0) {
        perror("pkill");
    }
    printf("pkill %s\n", process_name);
}

// 启动进程
void start_process(const char *command)
{
    if (system(command) == 0) {
        printf("%s\n", command);
    } else {
        perror("system");
    }
}

int check_wlan0_status()
{
    int status = WLAN_STATUS_NONE;
    int flags = get_interface_flags(INTERFACE_NAME);

    if (flags == -1) {
        if (g_last_status != status) {
            g_last_status = status;
            printf("%s not found\n", INTERFACE_NAME);
        }
        return 0;
    }
    // 判断接口状态
    int is_up = (flags & IFF_UP) != 0;
    int has_carrier = (flags & IFF_LOWER_UP) != 0;  // 推荐标志
    // printf("%s status: UP=%d, CARRIER=%d\n",
    //        INTERFACE_NAME, is_up, has_carrier);

    if (is_up) {
        status |= WLAN_UP;
    }

    if (has_carrier) {
        status |= WLAN_HAS_CARRIER;
    }

    char ip[INET_ADDRSTRLEN] = {0};
    if (get_interface_ip(INTERFACE_NAME, ip, sizeof(ip))) {
        status |= WLAN_HAS_IP;
    }

    if (g_last_status != status) {
        printf("%s IP: %s, status: %d\n", INTERFACE_NAME, ip, status);
        g_last_status = status;
    }
    return status;
}

int trigger_memory_compaction() {
    int fd = open("/proc/sys/vm/compact_memory", O_WRONLY);
    if (fd < 0) {
        perror("Failed to open compact_memory");
        return -1;
    }
    int ret = write(fd, "1", 1);
    close(fd);
    return (ret == 1) ? 0 : -1;
}

int set_oom_score_adj(int value) {
    char path[64];
    char score[16];
    FILE *fp;

    snprintf(path, sizeof(path), "/proc/%d/oom_score_adj", getpid());
    snprintf(score, sizeof(score), "%d", value);

    fp = fopen(path, "w");
    if (!fp) {
        perror("Failed to open oom_score_adj");
        return -1;
    }

    if (fputs(score, fp) == EOF) {
        perror("Failed to write oom_score_adj");
        fclose(fp);
        return -1;
    }

    fclose(fp);
    return 0;
}

int set_mdev_hotplug() {
    int fd = open("/proc/sys/kernel/hotplug", O_WRONLY);
    if (fd < 0) {
        perror("Failed to open hotplug");
        return -1;
    }
    int ret = write(fd, "/sbin/mdev", 9);
    close(fd);
    return (ret == 9) ? 0 : -1;
}

int drop_system_caches() {
    // 先执行sync
    sync();

    int fd = open("/proc/sys/vm/drop_caches", O_WRONLY);
    if (fd < 0) {
        perror("Failed to open drop_caches");
        return -1;
    }
    int ret = write(fd, "3", 1);
    close(fd);
    return (ret == 1) ? 0 : -1;
}

#if XOS_ZRAM_WRITEBACK
static int get_mem_available_kb() {
    FILE *fp = fopen("/proc/meminfo", "r");
    if (!fp) {
        perror("Failed to open /proc/meminfo");
        return -1;
    }

    char line[256];
    long mem_available = -1;

    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, "MemAvailable:")) {
            sscanf(line, "MemAvailable: %ld kB", &mem_available);
            break;
        }
    }
    fclose(fp);

    return mem_available;  // Returns value in kB, or -1 if not found
}

static int get_writeback_pages() {
    FILE *fp = fopen(CONFIG_FILE, "r");
    if (!fp) {
        return BD_MAX_PAGES;
    }

    char line[CMDLINE_BUFF_SIZE];
    int in_sys_section = 0;
    int writeback_pages = BD_MAX_PAGES;

    while (fgets(line, sizeof(line), fp)) {

        // Remove newline and trailing whitespace
        line[strcspn(line, "\n")] = '\0';

        // Skip empty lines and comments
        if (line[0] == '\0' || line[0] == '#') {
            continue;
        }

        // Check for section headers
        if (line[0] == '[') {
            in_sys_section = (strstr(line, "[sys]") != NULL);
            continue;
        }

        // Only process if we're in the [sys] section
        if (in_sys_section) {
            // Find the equals sign
            char *equals = strchr(line, '=');
            if (!equals) {
                continue;
            }

            // Split into key and value
            *equals = '\0';
            char *key = line;
            char *value = equals + 1;

            // Trim whitespace from key
            while (*key == ' ' || *key == '\t') key++;
            char *key_end = key + strlen(key) - 1;
            while (key_end > key && (*key_end == ' ' || *key_end == '\t')) {
                *key_end = '\0';
                key_end--;
            }

            // Trim whitespace from value
            while (*value == ' ' || *value == '\t') value++;
            char *value_end = value + strlen(value) - 1;
            while (value_end > value && (*value_end == ' ' || *value_end == '\t')) {
                *value_end = '\0';
                value_end--;
            }

            if (strcmp(key, "writeback_pages") == 0) {
                writeback_pages = atoi(value);
                printf("set writeback_pages=%d\n", writeback_pages);
                break;
            }
        }
    }

    fclose(fp);
    return writeback_pages;
}

static int get_zram_bd_stat_first_value() {
    FILE *fp = fopen("/sys/block/zram0/bd_stat", "r");
    if (!fp) {
        return -1;
    }

    int first_value;
    if (fscanf(fp, "%d", &first_value) != 1) {
        fclose(fp);
        return -1;
    }

    fclose(fp);
    return first_value;
}

static int get_swap_used_pages( int * free_pages) {
    FILE *fp = fopen("/proc/meminfo", "r");
    if (!fp) {
        perror("Failed to open /proc/meminfo");
        return -1;
    }

    long swap_total = -1;
    long swap_free = -1;
    char line[256];

    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, "SwapTotal:")) {
            sscanf(line, "SwapTotal: %ld kB", &swap_total);
        } else if (strstr(line, "SwapFree:")) {
            sscanf(line, "SwapFree: %ld kB", &swap_free);
        }

        if (swap_total != -1 && swap_free != -1) {
            break;
        }
    }
    fclose(fp);

    if (swap_total == -1 || swap_free == -1) {
        return -1;
    }

    long page_size_kb = sysconf(_SC_PAGESIZE) / 1024;
    if (page_size_kb <= 0) {
        page_size_kb = 4;  // fallback to 4KB if sysconf fails
    }

    long swap_used_kb = swap_total - swap_free;
    int swap_used_pages = swap_used_kb / page_size_kb;
     if(free_pages != NULL)
            *free_pages = swap_free / page_size_kb;

    return swap_used_pages;
}

static bool is_zram_backing_dev_none(void) {
    FILE *fp = fopen("/sys/block/zram0/backing_dev", "r");
    if (!fp) {
       return false; // Error case
    }

    char buffer[16] = {0};
    if (fgets(buffer, sizeof(buffer), fp) == NULL) {
        fclose(fp);
        return false; // Read error
    }
    fclose(fp);

    // Remove newline if present
    buffer[strcspn(buffer, "\n")] = '\0';

    return strcmp(buffer, "none") == 0;
}

int trigger_zram_writeback_needed(void) {
    int bd_stat_first = get_zram_bd_stat_first_value();
    int swap_free_pages = 0xFFFF;
    int swap_used_pages = get_swap_used_pages(&swap_free_pages);

    if(bd_stat_first == -1 || is_zram_backing_dev_none()) {
        return 0;
    }

    if((swap_used_pages > bdev_config_max_pages || swap_free_pages < 256) &&
        (bd_stat_first <  BD_TRIGGER_THRESHOLD) &&
        get_mem_available_kb() < MEM_TRIGGER_THRESHOLD )
      {
         printf("trigger writeback,swap_used_pages: %d swap_free_pages: %d, bd_stat: %d\n",
                swap_used_pages, swap_free_pages, bd_stat_first);
         return 1;
      }
    return 0;
}

int trigger_zram_writeback_idle(void) {
    const char *path = "/sys/block/zram0/writeback";
    const char *cmd = "idle";
    const char *limte = "256";//1024K
    FILE *fp;
    int ret = 0;

    if(access(WRITEBACK_DEBUG_CLOSE, F_OK) == 0) {
        return 0;
    }

    if (access(path, F_OK) == -1 && is_zram_backing_dev_none()) {
	    return 0;
    }

    if (access("/sys/block/zram0/idle", F_OK) == -1) {
        return 0;
    }

    fp = fopen("/sys/block/zram0/idle", "w");
    if (!fp) {
        return -1;
    }

    if (fputs("all", fp) == EOF) {
        ret = -1;
    }

    fclose(fp);

    fp = fopen("/sys/block/zram0/writeback_limit", "w");
    if (!fp) {
        return -1;
    }

    if (fputs(limte, fp) == EOF) {
        ret = -1;
    }

    fclose(fp);

    fp = fopen(path, "w");
    if (!fp) {
        //fprintf(stderr, "Failed to open %s: %s\n", path, strerror(errno));
        return -1;
    }

    if (fputs(cmd, fp) == EOF) {
        //fprintf(stderr, "Write error to %s: %s\n", path, strerror(errno));
        ret = -1;
    }

    if (fflush(fp) != 0) {
        //fprintf(stderr, "Flush error: %s\n", strerror(errno));
        ret = -1;
    }

    if (fclose(fp) != 0) {
        //fprintf(stderr, "Close error: %s\n", strerror(errno));
        ret = -1;
    }

    return ret;
}
#endif
int main()
{
    int count = 0;
    bool time_sync_flag = false;
    char env_str[] =
        "PATH=/sbin:/usr/sbin:/bin:/usr/bin:/vendor/qua/ko:/vendor/qua/BT:/vendor/qua/wifi/bin";
    struct timeval tv = { 0 };
    int time_stamp = 0;
    bool daemon_callback = false;
    int network_ready_count = 0;
    // 使用 putenv 函数添加环境变量
    if (putenv(env_str) != 0) {
        perror("putenv");
        return 1;
    }

    // 验证环境变量是否添加成功
    char *path = getenv("PATH");
    if (path == NULL) {
        return 1;
    }

#if XOS_WATCHDOG_ENABLE
    if (init_watchdog() < 0) {
        fprintf(stderr, "Failed to initialize watchdog\n");
        return 1;
    }
#endif

    atexit(close_watchdog);
    signal(SIGTERM, close_watchdog);
    signal(SIGINT, close_watchdog);

    system(MDEV_TRIGGER);
    sleep(5);
    drop_system_caches();
    trigger_memory_compaction();

#if XOS_ZRAM_WRITEBACK
    bdev_config_max_pages = get_writeback_pages();
#endif

    set_oom_score_adj(-100);

    while (1) {

        count += 5;

        int wlan0_exists_and_has_ip = check_wlan0_status();

        if ((WLAN_UP | WLAN_HAS_CARRIER | WLAN_HAS_IP) == wlan0_exists_and_has_ip) {
            if (count % 28800 == 0) {
                start_process("udhcpc -q -iwlan0");
            }
            if (!time_sync_flag)  {
                if (!is_process_running("ntpd")) {
		    #ifdef ESL
                      start_process("ntpd -nq &");
		    #else
                      start_process("ntpd -nq && hwclock -wu &");
		    #endif
                    sleep(5);
                }

                if (gettimeofday(&tv, NULL) == -1) {
                    printf("gettimeofday fail to get time");
                    continue;
                }
                //Tue Feb 25 20:05:10 CST 2025 date +%s 1740485110
                // for make sure time_stamp is int , tv.tv_sec length cannot make sure
                time_stamp  = tv.tv_sec;
                if (time_stamp >  TIME_VALID_STAMP) {
                    time_sync_flag = true;
                    printf("date sync successfully linux stamp :%d\n", time_stamp);
                }
            }
        }

        sleep(5);
        if (!daemon_callback) {
            if(access(DAEMON_CALLBACK, F_OK) == 0) {
                daemon_callback = true;
                system(DAEMON_CALLBACK" &");
            }
            else if(access(DAEMON_SDCARD_CALLBACK, F_OK) == 0) {
                daemon_callback = true;
                system(DAEMON_SDCARD_CALLBACK" &");
            }
            else if(access(DAEMON_MMC_CALLBACK, F_OK) == 0) {
                daemon_callback = true;
                system(DAEMON_MMC_CALLBACK" &");
            }
        }

        if (count % 60 == 0) {
            //drop_system_caches();
            trigger_memory_compaction();
        }
        #if XOS_ZRAM_WRITEBACK
        if (((WLAN_UP | WLAN_HAS_CARRIER | WLAN_HAS_IP) == wlan0_exists_and_has_ip)) {
            if( network_ready_count <= 30 && (network_ready_count % 4 == 0)) {
                trigger_zram_writeback_idle();
            }
            else if((network_ready_count % 4 == 0) && trigger_zram_writeback_needed()) {
                trigger_zram_writeback_idle();
            }
            network_ready_count++;
        }
        else {
              if(network_ready_count || count < 20){
                trigger_zram_writeback_idle();
              }
             network_ready_count = 0;
        }
        #endif
    }

    return 0;
}
