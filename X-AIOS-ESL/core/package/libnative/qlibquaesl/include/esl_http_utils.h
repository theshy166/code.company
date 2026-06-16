/**
 * @file esl_http_utils.h
 * @brief ESL (Electronic Shelf Label) HTTP utilities for network communication
 */
#ifndef _HTTP_ESL_UTILS_H
#define _HTTP_ESL_UTILS_H

/**
 * @brief Download an image in segments/chunks
 * @param url URL of the image to download
 * @param filePath Local file path to save the downloaded image
 * @param chunk_size Size of each download chunk in bytes
 * @param type Image type identifier
 * @param index Image index identifier
 * @return 0 on success, non-zero error code on failure
 */
int download_image_segment(const char *url, const char *filePath, long chunk_size, int type, int index);

/**
 * @brief Send a POST request to a remote URL with ESL-specific parameters
 * @param url Target URL for the POST request
 * @param post_data Data to send in the POST request body
 * @param wifi_ssid WiFi SSID information
 * @param local_ip Local IP address of the ESL device
 * @param version_no Firmware version number
 * @param max_retries Maximum number of retry attempts
 * @param retry_interval_ms Interval between retry attempts in milliseconds
 * @param platform Platform information
 * @return Pointer to the response data on success, NULL on failure
 */
char * esl_post_remote_url(const char *url, const char *post_data, const char *wifi_ssid, const char *local_ip, const char *version_no, int max_retries, int retry_interval_ms, char *platform);

/**
 * @brief Send a GET request to a remote URL with ESL-specific parameters
 * @param url Target URL for the GET request
 * @param wifi_ssid WiFi SSID information
 * @param local_ip Local IP address of the ESL device
 * @param version_no Firmware version number
 * @param max_retries Maximum number of retry attempts
 * @param retry_interval_ms Interval between retry attempts in milliseconds
 * @param platform Platform information
 * @return Pointer to the response data on success, NULL on failure
 */
char * esl_get_remote_url(const char *url, const char *wifi_ssid, const char *local_ip, const char *version_no, int max_retries, int retry_interval_ms, char *platform);

/**
 * @brief Send a POST request to a remote URL with enhanced ESL-specific parameters
 * @param url Target URL for the POST request
 * @param post_data Data to send in the POST request body
 * @param wifi_ssid WiFi SSID information
 * @param wifi_psw WiFi password
 * @param local_ip Local IP address of the ESL device
 * @param version_no Firmware version number
 * @param width Display width in pixels
 * @param height Display height in pixels
 * @param screens Number of screens
 * @param token Authentication token
 * @param platform Platform information
 * @return Pointer to the response data on success, NULL on failure
 */
char * esl_post_remote_url2(char *url, char *post_data, char *wifi_ssid, char *wifi_psw, char *local_ip, char *version_no, int width, int height, int screens, char *token, char *platform);

/**
 * @brief Send a GET request to a remote URL with enhanced ESL-specific parameters
 * @param url Target URL for the GET request
 * @param wifi_ssid WiFi SSID information
 * @param wifi_psw WiFi password
 * @param local_ip Local IP address of the ESL device
 * @param version_no Firmware version number
 * @param width Display width in pixels
 * @param height Display height in pixels
 * @param screens Number of screens
 * @param token Authentication token
 * @param platform Platform information
 * @return Pointer to the response data on success, NULL on failure
 */
char * esl_get_remote_url2(char *url, char *wifi_ssid, char *wifi_psw, char *local_ip, char *version_no, int width, int height, int screens, char *token, char *platform);

#endif //HTTP_UTILS_H