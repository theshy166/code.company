#ifndef __ESL_BASE_FUNCTION__
#define __ESL_BASE_FUNCTION__

/**
 * @file esl_base_function.h
 * @brief Basic ESL (Electronic Shelf Label) function declarations for download logging and progress display
 */

/**
 * @brief Show download log on the first display instance
 * @param arg Pointer to the file name or log message to display
 */
void show_download_log_1(char *arg);

/**
 * @brief Set the value of a progress bar
 * @param index Index of the progress bar to update
 * @param value Value to set (typically 0-100 for percentage)
 */
void set_bar_value(int index, int value);

/**
 * @brief Show download log on the second display instance
 * @param arg Pointer to the file name or log message to display
 */
void show_download_log_2(char *arg);

#endif //__ESL_BASE_FUNCTION__