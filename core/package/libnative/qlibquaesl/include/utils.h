/**
 * @file utils.h
 * @brief General utility functions for ESL (Electronic Shelf Label)
 */
#ifndef __UTILS_H__
#define __UTILS_H__

/**
 * @brief Create a directory at the specified path
 * @param path Path of the directory to create
 * @return 0 on success, non-zero error code on failure
 */
int create_directory(const char *path);

/**
 * @brief Transform/normalize a file path
 * @param input_path Original file path
 * @return Transformed file path (caller is responsible for freeing memory)
 */
char* transform_path(const char* input_path);

/**
 * @brief Check if a string is empty
 * @param str String to check
 * @return 1 if the string is empty, 0 otherwise
 */
int is_string_empty(const char *str);

/**
 * @brief Check if a string ends with a specific suffix
 * @param str String to check
 * @param suffix Suffix to compare
 * @return 1 if the string ends with the suffix, 0 otherwise
 */
int ends_with(const char *str, const char *suffix);

/**
 * @brief Check if a target string ends with any string in an array
 * @param arr Array of suffix strings
 * @param target Target string to check
 * @param size Size of the array
 * @return 1 if the target ends with any suffix in the array, 0 otherwise
 */
int is_string_ends_with(const char **arr, const char *target, int size);

/**
 * @brief Check if a string starts with a specific prefix
 * @param str String to check
 * @param prefix Prefix to compare
 * @return 1 if the string starts with the prefix, 0 otherwise
 */
int starts_with(const char *str, const char *prefix);

/**
 * @brief Convert a string to an array of strings (tokenization)
 * @param str String to convert
 * @param count Pointer to store the number of elements in the resulting array
 * @return Array of strings (caller is responsible for freeing memory)
 */
char** convert_string_to_char_array(const char *str, int *count);

/**
 * @brief Boot to Linux operating system
 * @return 0 on success, non-zero error code on failure
 */
int boot2linux() ;

/**
 * @brief Get the current time in milliseconds
 * @return Current time in milliseconds since epoch
 */
long long get_milliseconds();

/**
 * @brief Check if resources need to be deleted
 * @return 1 if resources need to be deleted, 0 otherwise
 */
int is_need_delete_resource();

/**
 * @brief Remove HTML entities from a string
 * @param str String to process (will be modified in place)
 */
void remove_html_entities(char *str);

#endif //__UTILS_H__

