
#include <stdio.h>
#include "../conf/conf.h"

char * str_find_next(char *str, char *find, char end, char *temp_string) {
    // QM_ESL2_LOG("str_find_next: str: %s, find: %s, end: %c\n", str, find, end);
    char *ptr = strstr(str, find);
    if (ptr == NULL) {
        QM_ESL2_LOG("str_find_next: ptr is NULL\n");
        return NULL;
    }
    char *data_ptr = ptr + strlen(find);
    char *end_ptr = strchr(data_ptr, end);
    if (end_ptr == NULL) {
        QM_ESL2_LOG("str_find_next: end_ptr is NULL\n");
        return NULL;
    }
    memcpy(temp_string, data_ptr, end_ptr - data_ptr);
    temp_string[end_ptr - data_ptr] = '\0';
    // QM_ESL2_LOG("str_find_next: %s\n", temp_string);
    return temp_string;
}

// {"date_type":998,"master_group_id":274,"master_ip":"172.16.58.96"}
char * str_find_ip(char *str, char *temp_string) {
    return str_find_next(str, "\"master_ip\":\"", '\"', temp_string);
}
