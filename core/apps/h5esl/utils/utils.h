#ifndef __UTILS_H__
#define __UTILS_H__

int create_directory(const char *path);

char* transform_path(const char* input_path);

int is_string_empty(const char *str);

int ends_with(const char *str, const char *suffix);

int is_string_ends_with(const char **arr, const char *target, int size);

int starts_with(const char *str, const char *prefix);

char** convert_string_to_char_array(const char *str, int *count);

int boot2linux() ;

long long get_milliseconds();

int is_need_delete_resource();
#endif //__UTILS_H__

